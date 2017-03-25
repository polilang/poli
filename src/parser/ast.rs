use std::collections::HashMap;

use lexer::{
    Token,
    TokenType,
};

trait Node: Sized {
    fn parse(&mut self, parser: Parser);
}

#[derive(Debug, Clone)]
pub struct Parser {
    signatures: HashMap<TokenType, Node>,
    tokens:     Vec<Token>,
}

impl Parser {
    pub fn new(tokens: Vec<Token>) -> Parser {
        Parser {
            tokens: tokens,
        }
    }

    fn parse(&mut self) {
        loop {
            match self.tokens.pop() {
                Some(t) => {
                    match self.signatures[&t] {
                        Some(&n) => n.parse(), 
                        None     => continue
                    }
                },

                None    => break
            }
        }
    }
}
