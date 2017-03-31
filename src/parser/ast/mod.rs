use std::collections::HashMap;
use std::fmt::Debug;

pub mod base;
use self::base::literals;

use lexer::{Token, TokenType};

#[derive(Debug, Clone)]
pub enum ASTNode {
    NumberLiteral(literals::NumberLiteral),
    StringLiteral(literals::StringLiteral),
}

pub trait ParserNode: Debug {
    fn parse(&self, parser: &Parser) -> ASTNode;
}

#[derive(Debug)]
pub struct Parser {
    tokens:     Vec<Token>,
    ast:        Vec<Box<ParserNode>>,
    pub pos:        usize,
}

impl Parser {
    pub fn new(tokens: Vec<Token>) -> Parser {
        Parser {
            tokens: tokens,
            ast:    Vec::new(),
            pos:    0,
        }
    }

    pub fn parse(mut self, signatures: &HashMap<Vec<TokenType>, Box<ParserNode>>) -> (Option<ASTNode>, Parser) {
        if self.tokens.len() > 0 {
            let b = match signatures.keys().find(|sig| self.match_sequence(sig, 0)) {
                Some(v) => signatures.get(&v.clone()).unwrap(),
                None    => return (None, self),
            };

            self.pos += 1;

            return (Some(b.parse(&mut self)), self)
        }

        (None, self)
    }

    // TODO: work in progress
    pub fn expression(&self) -> ASTNode {
        let mut expr_stack: Vec<ASTNode> = Vec::new();

        expr_stack.push(self.atom(0));

        expr_stack[0].clone()
    }

    // TODO: work in progress
    pub fn atom(&self, offset: usize) -> ASTNode {

        // number
        if self.match_sequence(&vec![
            TokenType::NumberLiteral(String::from("")),
        ], offset) {
            let v = match self.get(0).token_type {
                TokenType::NumberLiteral(v) => v,
                _                           => panic!("todo: make nice errors - number atom")
            };

            return ASTNode::NumberLiteral(
                literals::NumberLiteral::new(
                    v.parse::<f64>().unwrap()
                )
            )
        } else {
            return self.expression()
        }
    }

    pub fn match_sequence(&self, sequence: &Vec<TokenType>, offset: usize) -> bool {
        let mut off = 0 + offset;

        for e in sequence {
            let t = self.get(off).token_type;

            match e.clone() {
                TokenType::Keyword(k) => {
                    match t {
                        TokenType::Keyword(k1) => {
                            if k != k1 {
                                return false
                            }
                        },

                        _ => return false,
                    }
                },

                TokenType::Identifier(_) => {
                    match t {
                        TokenType::Identifier(_) => (),
                        _                        => return false,
                    }
                },

                TokenType::NumberLiteral(_) => {
                    match t {
                        TokenType::NumberLiteral(_) => (),
                        _                           => return false,
                    }
                },

                TokenType::StringLiteral(_) => {
                    match t {
                        TokenType::StringLiteral(_) => (),
                        _                           => return false,
                    }
                },

                a => {
                    if a != t {
                        return false
                    }
                }
            }

            off += 1;
        }

        true
    }

    pub fn get(&self, offset: usize) -> Token {
        if self.pos + offset > self.tokens.len() - 1 {
            return Token::new(TokenType::EOF, 0, 0)
        }

        self.tokens[self.pos + offset].clone()
    }

    pub fn get_backward(&self, offset: usize) -> Token {
        self.tokens[self.pos - offset].clone()
    }
}