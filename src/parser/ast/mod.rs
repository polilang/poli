use std::fmt::Debug;

pub mod base;
use self::base::literals;
use self::base::assignment;

use lexer::{Token, TokenType};

#[derive(Debug, Clone)]
pub enum ASTNode {
    NumberLiteral(literals::NumberLiteral),
    StringLiteral(literals::StringLiteral),
    Assignment(assignment::Assignment),
}

pub trait ParserNode: Debug {
    fn parse(&self, parser: &mut Parser) -> ASTNode;
}

#[derive(Debug)]
pub struct Parser {
    tokens:     Vec<Token>,
    ast:        Vec<Box<ParserNode>>,
    pub pos:    usize,
}

#[derive(Debug)]
pub struct Signature {
    key:  Vec<TokenType>,
    node: Box<ParserNode>,
}

impl Signature {
    pub fn new(key: Vec<TokenType>, node: Box<ParserNode>) -> Signature {
        Signature {
            key:  key,
            node: node,
        }
    }
}

#[derive(Debug)]
pub struct SignaturePool {
    signatures: Vec<Signature>,
    longest:    usize,
}

pub fn create_pool(signatures: Vec<Signature>) -> SignaturePool {
    // TODO: automate sorting
    SignaturePool {
        signatures: signatures,
        longest:    0,
    }
}

impl Parser {
    pub fn new(tokens: Vec<Token>) -> Parser {
        Parser {
            tokens: tokens,
            ast:    Vec::new(),
            pos:    0,
        }
    }

    pub fn parse(mut self, pool: &SignaturePool) -> (Option<ASTNode>, Parser) {
        if self.tokens.len() > 0 {
            for sig in &pool.signatures {
                if self.match_sequence(&sig.key, 0) {
                    self.pos += 1;

                    return (Some(sig.node.parse(&mut self)), self)
                }
            }
        }

        (None, self)
    }

    // TODO: work in progress
    pub fn expression(&self, offset: usize) -> Option<ASTNode> {
        let mut expr_stack: Vec<ASTNode> = Vec::new();

        match self.atom(offset) {
            Some(v) => expr_stack.push(v),
            None    => (),
        }

        Some(expr_stack[0].clone())
    }

    // TODO: work in progress
    pub fn atom(&self, offset: usize) -> Option<ASTNode> {

        // number
        if self.match_sequence(&vec![
            TokenType::NumberLiteral(String::from("")),
        ], offset) {
            let v = match self.get(0).token_type {
                TokenType::NumberLiteral(v) => v,
                _                           => panic!("todo: make nice errors - number atom")
            };

            return Some(
                ASTNode::NumberLiteral(
                    literals::NumberLiteral::new(
                        v.parse::<f64>().unwrap()
                    )
                )
            )
        }

        None
    }

    pub fn match_sequence(&self, sequence: &Vec<TokenType>, offset: usize) -> bool {
        let mut off = 0 + offset;

        for e in sequence {
            let t = self.get(off).token_type;

            match e.clone() {
                TokenType::Keyword(k) => match t {
                    TokenType::Keyword(k1) => {
                        if k != k1 {
                            return false
                        }
                    },

                    _ => return false,
                },

                TokenType::Identifier(_) => match t {
                    TokenType::Identifier(_) => (),
                    _                        => return false,
                },

                TokenType::NumberLiteral(_) => match t {
                    TokenType::NumberLiteral(_) => (),
                    _                           => return false,
                },

                TokenType::StringLiteral(_) => match t {
                    TokenType::StringLiteral(_) => (),
                    _                           => return false,
                },

                TokenType::BinaryOp(_) => match t {
                    TokenType::BinaryOp(_) => (),
                    _                      => return false,
                },

                a => if a != t {
                    return false
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