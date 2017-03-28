use std::collections::HashMap;
use std::fmt::Debug;

pub mod base;
use self::base::literals;

use lexer::{Token, TokenType};

#[derive(Debug)]
pub enum ASTNode {
    NumberLiteral(literals::NumberLiteral),
    StringLiteral(literals::StringLiteral),
}

pub trait ParserNode: Debug {
    fn parse(&self, parser: &Parser) -> ASTNode;
}

#[derive(Debug)]
pub struct Parser {
    signatures: HashMap<Vec<TokenType>, Box<ParserNode>>,
    tokens:     Vec<Token>,
    ast:        Vec<Box<ParserNode>>,
    pos:        usize,
}

impl Parser {
    pub fn new(tokens: Vec<Token>) -> Parser {
        Parser {
            signatures: HashMap::new(),
            tokens:     tokens,
            ast:        Vec::new(),
            pos:        0,
        }
    }

    pub fn introduce_node(&mut self, signature: Vec<TokenType>, node: Box<ParserNode>) {
        self.signatures.insert(signature, node);
    }

    pub fn parse(&mut self) -> Option<ASTNode> {
        if self.tokens.len() > 0 {
            loop {
                let k = match self.signatures.keys().find(|sig| self.match_sequence(sig, 0)) {
                    Some(v) => v.clone(),
                    None    => return None,
                };

                let b = self.signatures.get(&k).unwrap();

                self.pos += 1;

                return Some(b.parse(&self))
            }
        }

        None
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
        if self.pos + offset > self.tokens.len() {
            return Token::new(TokenType::EOF, 0, 0)
        }

        self.tokens[self.pos + offset].clone()
    }

    pub fn get_backward(&self, offset: usize) -> Token {
        self.tokens[self.pos - offset].clone()
    }
}