use super::tokenizer::Tokenizer;
use super::token::{
        TokenType, Operator,
    };

#[derive(Debug, Clone)]
pub enum Expression {
    Integer(i64),
    Float(f64),
    Text(String),
    Boolean(bool),

    Ident(String),

    Return(Box<Expression>),

    Operation(
            Box<Expression>,
            Operator,
            Box<Expression>,
        ),
    
    Call(
            Box<Expression>,
            Box<Vec<Expression>>,
        ),
}

#[derive(Debug, Clone)]
pub enum Statement {
    Assignment(
            String,
            Box<Expression>,
        ),
    
    Block(
            Box<Vec<Statement>>,
        ),

    If(
            Box<Expression>,
            Box<Statement>,
        ),

    IfElse(
            Box<Expression>,
            Box<Statement>,
            Box<Statement>,
        ),

    Expression(Box<Expression>),
}

#[derive(Debug, Clone)]
pub struct Parser {
    tokenizer: Tokenizer,
}

impl Parser {
    pub fn new() -> Parser {
        Parser {
            tokenizer: Tokenizer::new(),
        }
    }

    pub fn from(tokenizer: Tokenizer) -> Parser {
        Parser {
            tokenizer: tokenizer,
        }
    }

    pub fn parse(&mut self) -> Result<Vec<Statement>, String> {
        let mut stack = Vec::new();

        loop {
            if self.tokenizer.remaining() < 1 {
                break
            }

            stack.push(
                    try!(self.statement()),
                );
            
            self.tokenizer.next_token();
        }

        Ok(stack)
    }
    
    fn operation(
        &mut self,
        expression: Expression
    ) -> Result<Expression, String> {

        let mut ex_stack = vec!(expression);
        let mut op_stack: Vec<(Operator, u8)> = Vec::new();

        op_stack.push(
                super::tokenizer::operator(
                        &self.tokenizer.current_content()
                    ).unwrap()
            );

        self.tokenizer.next_token();

        ex_stack.push(
                try!(self.term())
            );
        
        let mut done = false;
        while ex_stack.len() > 1 {
            
            if !done && self.tokenizer.next_token() {
                if self.tokenizer.current().get_type() != TokenType::Operator {
                    self.tokenizer.prev_token();
                    
                    done = true;

                    continue
                }

                let (op, prec) = super::tokenizer::operator(
                        &self.tokenizer.current_content()
                    ).unwrap();
                
                if prec > op_stack.last().unwrap().1 {
                    let left  = ex_stack.pop().unwrap();
                    let right = ex_stack.pop().unwrap();

                    ex_stack.push(
                            Expression::Operation(
                                    Box::new(left),
                                    op_stack.pop().unwrap().0,
                                    Box::new(right),
                                )
                        );

                    self.tokenizer.next_token();

                    ex_stack.push(try!(self.term()));
                    op_stack.push((op, prec));

                    continue
                }
                
                self.tokenizer.next_token();

                ex_stack.push(try!(self.term()));
                op_stack.push((op, prec));
            }

            let left  = ex_stack.pop().unwrap();
            let right = ex_stack.pop().unwrap();

            ex_stack.push(
                    Expression::Operation(
                            Box::new(left),
                            op_stack.pop().unwrap().0,
                            Box::new(right),
                        )
                );
        }

        Ok(
            ex_stack.pop().unwrap()
        )
    }

    fn call(&mut self, caller: Expression) -> Result<Expression, String> {
        let mut stack = Vec::new();

        self.tokenizer.next_token();

        while self.tokenizer.current().get_type() != TokenType::RParen {            
            stack.push(
                    try!(self.expression())
                );

            self.tokenizer.next_token();
            
            if self.tokenizer.current().get_type() == TokenType::Comma {                
                self.tokenizer.next_token();
            }
        }

        Ok(
            Expression::Call(
                    Box::new(caller),
                    Box::new(stack),
                )
        )
    }

    fn term(&mut self) -> Result<Expression, String> {
        let token_type = self.tokenizer.current().get_type();
        
        match token_type {
            TokenType::Integer => Ok(
                    Expression::Integer(
                            self.tokenizer.current_content().parse::<i64>().unwrap(),
                        )
                ),

            TokenType::Float => Ok(
                    Expression::Float(
                            self.tokenizer.current_content().parse::<f64>().unwrap(),
                        )
                ),

            TokenType::Boolean => Ok(
                    Expression::Boolean(
                            self.tokenizer.current_content() == "true",
                        )
                ),

            TokenType::Text => Ok(
                    Expression::Text(
                            self.tokenizer.current_content(),
                        )
                ),
            
            TokenType::Ident => {
                    let ident = Expression::Ident(
                            self.tokenizer.current_content(),
                        );
                    
                    if self.tokenizer.next_token() {
                        return match self.tokenizer.current().get_type() {
                            TokenType::Operator  => self.operation(ident),
                            TokenType::LParen    => self.call(ident),
                            _                    => {
                                    self.tokenizer.prev_token();
                                    Ok(ident)
                                },
                        }
                    }

                    Ok(ident)
                },

            TokenType::LParen => {

                    self.tokenizer.next_token();

                    let expression = try!(self.expression());

                    self.tokenizer.next_token();

                    try!(self.tokenizer.match_current(TokenType::RParen));

                    self.tokenizer.next_token();

                    if self.tokenizer.current().get_type() == TokenType::LParen {
                        return self.call(expression)
                    }

                    self.tokenizer.prev_token();

                    Ok(expression)
                },

            TokenType::Return => {
                    self.tokenizer.next_token();

                    let expression = try!(self.expression());

                    Ok(Expression::Return(Box::new(expression)))
                },
            
            _ => Err(
                    format!("unexpected term: {:#?}", token_type)
                ),
        }
    }

    fn block(&mut self) -> Result<Vec<Statement>, String> {
        match self.tokenizer.current().get_type() {
            TokenType::Block(v) => {
                    let mut p = Parser::from(
                            Tokenizer::from(v)
                        );
                    
                    p.parse()
                },
            _ => Err(
                    format!(
                            "expected block, found: {:?}",
                            self.tokenizer.current().get_type(),
                        )
                ),
        }
    }

    fn statement(&mut self) -> Result<Statement, String> {
        match self.tokenizer.current().get_type() {
            TokenType::Ident => {
                let ident = self.tokenizer.current_content();

                self.tokenizer.next_token();

                if self.tokenizer.current().get_type() != TokenType::Assign {
                    self.tokenizer.prev_token();

                    let expression = try!(self.expression());

                    return Ok(
                        Statement::Expression(
                                Box::new(expression),
                            )
                    )
                }

                self.tokenizer.next_token();

                let expression = try!(self.expression());

                Ok(
                    Statement::Assignment(
                            ident,
                            Box::new(expression),
                        )
                )
            },

            TokenType::If => {
                    self.tokenizer.next_token();

                    let condition = try!(self.expression());

                    self.tokenizer.next_token();

                    let body = try!(self.block());  

                    self.tokenizer.next_token();

                    if self.tokenizer.current().get_type() == TokenType::Else {
                        self.tokenizer.next_token();

                        let else_body = try!(self.block());

                        return Ok(
                                Statement::IfElse(
                                        Box::new(condition),
                                        Box::new(
                                                Statement::Block(
                                                        Box::new(body),
                                                    )
                                            ),

                                        Box::new(
                                                Statement::Block(
                                                        Box::new(else_body),
                                                    )
                                            ),
                                    )
                            )
                    }

                    self.tokenizer.next_token();

                    Ok(
                        Statement::If(
                                Box::new(condition),
                                Box::new(
                                        Statement::Block(
                                                Box::new(body)
                                            )
                                    )
                            )
                    )
                },

            _ => {
                let expression = try!(self.expression());

                Ok(
                    Statement::Expression(
                            Box::new(expression)
                        )
                )
            }
        }
    }

    fn expression(&mut self) -> Result<Expression, String> {
        let expr = try!(self.term());

        self.tokenizer.next_token();

        if self.tokenizer.remaining() > 0 {
            if self.tokenizer.current().get_type() == TokenType::Operator {
                return self.operation(expr)
            }

            self.tokenizer.prev_token();
        }

        Ok(expr)
    }
}