use parser::lexer;

use lexer::{
    Lexer,
    TokenType,
    Token,
    BinaryOp,
};

#[derive(Debug, Clone)]
pub enum Expression {
    Double(f64),
    StringLiteral(String),
    Bool(bool),
    Call(Box<Expression>, Box<Vec<Expression>>),
}

#[derive(Debug, Clone)]
pub enum Statement {

}

#[derive(Debug, Clone)]
pub enum Type {
    Str,
    Int,
    Float,
    Double,
    Bool,
    Dynamic,
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

    fn guard_look(source: &Vec<Token>, pos: usize) -> &Token {
        if pos > source.len() - 1 {
            return &source[source.len() - 1]
        }

        &source[pos]
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

    fn parse_word(&mut self) -> Result<Expression, String> {
        match &self.look(0).token_type {
            &TokenType::NumberLiteral(ref n) => {
                match n.parse::<f64>() {
                    Ok(i)  => Ok(Expression::Double(i)),
                    Err(_) => Err(String::from("failed to parse number literal")),
                }
            },

            &TokenType::Bool(b) => Ok(Expression::Bool(b)),

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
            let a = Self::guard_look(&self.source, self.pos.clone());

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

                if prec > oper_list.last().unwrap().1 {
                    let left  = oper_list.pop().unwrap();
                    let right = oper_list.pop().unwrap()
                }
            }
        }

        Err(String::from("heyo")).unwrap()
    }
}