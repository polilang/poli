use super::super::{
    Parser, ParserNode, ASTNode, TokenType,
};

#[derive(Debug, Clone)]
pub struct NumberLiteral {
    value: f64,
}

impl NumberLiteral {
    pub fn new(value: f64) -> NumberLiteral {
        NumberLiteral {
            value: value,
        }
    }
}

impl ParserNode for NumberLiteral {
    fn parse(&self, p: &Parser) -> ASTNode {
        match p.get_backward(1).token_type {
            TokenType::NumberLiteral(v) => ASTNode::NumberLiteral(NumberLiteral::new(v.parse::<f64>().unwrap())),
            _                           => panic!("todo: make nice errors - number parser")
        }
    }
}

#[derive(Debug, Clone)]
pub struct StringLiteral {
    value: String,
}

impl StringLiteral {
    pub fn new(value: String) -> StringLiteral {
        StringLiteral {
            value: value,
        }
    }
}

impl ParserNode for StringLiteral {
    fn parse(&self, p: &Parser) -> ASTNode {
        match p.get_backward(1).token_type {
            TokenType::StringLiteral(v) => ASTNode::StringLiteral(StringLiteral::new(v)),
            _                           => panic!("todo: make nice errors - number parser")
        }
    }
}