use std::collections::HashMap;
use std::fmt::Debug;

use parser::block_tree::{
    Branch,
    ChunkContent,
};


use lexer::{Token, TokenType};

pub trait ParserNode: Debug {
    fn parse(&self, parser: &Parser) -> Box<ParserNode>;
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

    pub fn parse(mut self) -> Box<ParserNode> {
        loop {
            let b = self.signatures.get(&self.signatures.keys().find(
                |sig| self.match_sequence(sig)
            ).unwrap().clone()).unwrap();

            self.pos += 1;

            return b.parse(&self)
        }
    }

    fn match_sequence(&self, sequence: &Vec<TokenType>) -> bool {
        let mut off = 0;

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

    fn get(&self, offset: usize) -> Token {
        if self.pos + offset > self.tokens.len() {
            return Token::new(TokenType::EOF, 0, 0)
        }

        self.tokens[self.pos + offset].clone()
    }
}