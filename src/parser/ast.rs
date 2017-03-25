use std::collections::HashMap;

use lexer::{
    Token,
    TokenType,
};

#[derive(Debug, Clone)]
struct Node {

}

trait ParserNode: Sized {
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
            signatures: HashMap::new(),
        }
    }

    fn parse(&mut self) {
        loop {
            match self.tokens.pop() {
                Some(t) => {
                    match self.signatures[&t.token_type] {
                        ref n => println!("found signature: {:#?}", n), 
                    }
                },

                None    => break
            }
        }
    }
}
