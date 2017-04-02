use super::super::{
    Parser, ParserNode, ASTNode
};

#[derive(Debug, Clone)]
pub struct Assignment {
    left:  Box<ASTNode>,
    right: Box<ASTNode>,
}

impl Assignment {
    pub fn new(left: Box<ASTNode>, right: Box<ASTNode>) -> Assignment {
        Assignment {
            left:  left,
            right: right,
        }
    }
}

impl ParserNode for Assignment {
    fn parse(&self, p: &mut Parser) -> ASTNode {
        p.pos -= 3;

        let content = match (p.expression(0), p.expression(2)) {
            (Some(left), Some(right)) => Assignment::new(Box::new(left), Box::new(right)),
            _                         => panic!("todo: make nice errors - assignment parser"),
        };

        p.pos += 3;

        ASTNode::Assignment(content)
    }
}