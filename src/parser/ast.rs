use lexer::{
    TokenType,
    Token,
    BinaryOp,
};

#[derive(Debug, Clone)]
pub enum Expression {
    DoubleLiteral(f64),
    StringLiteral(String),
    Bool(bool),
    Call(Box<Expression>, Box<Vec<Expression>>),
    Operation(Box<Expression>, BinaryOp, Box<Expression>),
}

#[derive(Debug, Clone)]
pub enum Statement {

}

#[derive(Debug, Clone, PartialEq, Eq)]
pub struct Parser {
    source: Vec<Token>,
    pos:    usize,
}

impl Parser {
    pub fn new(source: Vec<Token>) -> Parser {
        let parser = Parser {
            source: source,
            pos:    0,
        };

        parser
    }

    fn guard_look(source: Vec<Token>, pos: usize) -> Token {
        if pos > source.len() - 1 {
            return source[source.len() - 1].clone()
        }

        source[pos].clone()
    }

    fn look(&self, offset: usize) -> &Token {
        if self.pos + offset > self.source.len() - 1 {
            return &self.source[self.source.len() - 1]
        }

        &self.source[self.pos + offset]
    }

    fn has_next(&self) -> bool {
        self.pos < self.source.len() - 1
    }

    fn match_current_token(&self, t: TokenType) -> Result<Token, String> {
        let a = Self::guard_look(self.source.clone(), self.pos.clone());

        match a.token_type == t {
            true  => Ok(a.clone()),
            false => Err(format!(
                "expected {:?} but found {:?}", t, a,
            )),
        }
    }

    fn parse_word(&mut self) -> Result<Expression, String> {
        match Self::guard_look(self.source.clone(), self.pos.clone()).token_type {
            TokenType::NumberLiteral(n) => {
                match n.parse::<f64>() {
                    Ok(i)  => Ok(Expression::DoubleLiteral(i)),
                    Err(_) => Err(String::from("failed to parse number literal")),
                }
            },

            TokenType::Bool(b) => Ok(Expression::Bool(b)),
            TokenType::StringLiteral(s) => Ok(Expression::StringLiteral(s)),

            TokenType::LParen => {
                self.pos += 1;

                let expr = try!(self.parse_expression());
                
                try!(self.match_current_token(TokenType::RParen));

                self.pos += 1;

                if Self::guard_look(self.source.clone(), self.pos.clone()).token_type == TokenType::LParen {
                    return self.parse_call(expr)
                }

                self.pos -= 1;

                Ok(expr)
            },

            _ => {
                Err(String::from("v bad expression, didn't parse correctly"))
            }
        }
    }

    fn parse_bin_op(&mut self, expr: Expression) -> Result<Expression, String> {
        let mut expr_list = vec!(expr);
        let mut oper_list: Vec<(BinaryOp, u8)> = Vec::new();

        match &self.look(0).token_type {
            &TokenType::BinaryOp((ref b, ref prec)) => {
                oper_list.push((b.clone(), prec.clone()));
            },

            _ => {
                Err(String::from("failed to parse binary operation")).unwrap()
            },
        }
        
        self.pos += 1;

        expr_list.push(try!(self.parse_word()));

        let mut done = false;

        while expr_list.len() > 1 {
            let a = Self::guard_look(self.source.clone(), self.pos.clone());

            if !done && self.has_next() {
                let (op, prec) = match &a.token_type {
                    &TokenType::BinaryOp((ref b, ref prec)) => {
                        (b, prec)
                    },

                    _ => {
                        self.pos -= 1;
                        done = true;

                        continue
                    },
                };

                if prec > &oper_list.last().unwrap().1 {

                    let left = expr_list.pop().unwrap();
                    let right = expr_list.pop().unwrap();

                    expr_list.push(Expression::Operation(
                        Box::new(left),
                        oper_list.pop().unwrap().0,
                        Box::new(right),
                    ));

                    self.pos += 1;

                    expr_list.push(try!(self.parse_word()));
                    oper_list.push((op.clone(), prec.clone()));

                    continue
                }

                let left  = expr_list.pop().unwrap();
                let right = expr_list.pop().unwrap();

                expr_list.push(Expression::Operation(
                    Box::new(left), oper_list.pop().unwrap().0, Box::new(right),
                ));
            }
        }

        Ok(expr_list.pop().unwrap())
    }

    fn parse_expression(&mut self) -> Result<Expression, String> {
        let expr = try!(self.parse_word());

        self.pos += 1;

        if self.source[self.pos..].len() > 0 {

            let is_bin_op = match Self::guard_look(self.source.clone(), self.pos.clone()).token_type {
                TokenType::BinaryOp((_, _)) => true,
                _                => false,
            };

            if is_bin_op {
                return self.parse_bin_op(expr)
            }

            self.pos -= 1;
        }

        Ok(expr)
    }

    fn parse_call(&mut self, callee: Expression) -> Result<Expression, String> {
        let mut stack = Vec::new();

        self.pos += 1;

        while Self::guard_look(self.source.clone(), self.pos.clone()).token_type != TokenType::RParen {
            stack.push(try!(self.parse_expression()));

            self.pos += 1;

            if Self::guard_look(self.source.clone(), self.pos.clone()).token_type == TokenType::Comma {
                self.pos += 1;
            }
        }

        Ok(Expression::Call(Box::new(callee), Box::new(stack)))
    }
}