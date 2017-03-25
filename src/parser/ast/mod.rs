use std::collections::HashMap;
use std::fmt::Debug;

use lexer::{Token, TokenType};

pub trait ParserNode: Debug {
    fn parse(&self, tokens: &mut Vec<Token>);
}

#[derive(Debug)]
pub struct Parser {
    signatures: HashMap<TokenType, Box<ParserNode>>,
    tokens:     Vec<Token>,
}

impl Parser {
    pub fn new(tokens: Vec<Token>) -> Parser {
        Parser {
            tokens: tokens,
            signatures: HashMap::new(),
        }
    }

    pub fn introduce_node(&mut self, signature: TokenType, node: Box<ParserNode>) {
        self.signatures.insert(signature, node);
    }

    pub fn parse(&mut self) {
        loop {
            match self.tokens.pop() {
                Some(t) => {
                    if let Some(n) = self.signatures.get(&t.token_type) {
                        println!("{:?}", n);
                        n.parse(&mut self.tokens)
                    }
                },
                None    => break
            }
        }
    }
}