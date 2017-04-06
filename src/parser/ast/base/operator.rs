use super::super::{
    ASTNode
};

#[derive(Debug, Clone)]
pub struct Operator {
    left:    Box<ASTNode>,
    right:   Box<ASTNode>,
    operand: Box<ASTNode>,
}

impl Operator {
    pub fn new(left: Box<ASTNode>, operand: Box<ASTNode>, right: Box<ASTNode>) -> Operator {
        match *operand {
            ASTNode::BinaryOp((_, _)) => (),
            _                         => panic!("todo: make nice errors - operator constructor"),
        }

        Operator {
            left:    left,
            right:   right,
            operand: operand,
        }
    }
}