use std::fmt::Debug;

pub mod base;
use self::base::literals;
use self::base::assignment;

use lexer::{
    Token,
    TokenType,
};

use parser::block_tree::{
    Branch,
    ChunkContent,
};

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
pub enum SignatureKey {
    Only(TokenType),
    Any(Vec<TokenType>),
}

#[derive(Debug)]
pub struct Signature {
    key:  Vec<SignatureKey>,
    node: Box<ParserNode>,
}

impl Signature {
    pub fn new(key: Vec<SignatureKey>, node: Box<ParserNode>) -> Signature {
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

pub fn flatten_branch<'a>(branch: &Branch<'a>) -> Vec<Token> {
    let mut flat: Vec<Token> = Vec::new();

    for chunk in branch.content.iter() {
        match chunk.content {
            ChunkContent::Tokens(ref t)    => flat.append(&mut t.clone()),
            ChunkContent::Block(ref b) => flat.push(
                    Token::new(TokenType::Block(flatten_branch(b)), 0, 0)
                ),
            _                          => (),
        }
    }

    flat
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
                let len = sig.key.len();

                if self.match_signature(&sig.key, 0) {
                    self.pos += len;

                    return (Some(sig.node.parse(&mut self)), self)
                }
            }
        }

        (None, self)
    }

    // TODO: work in progress
    pub fn expression(&mut self, offset: usize) -> Option<ASTNode> {
        let mut expr_stack: Vec<ASTNode> = Vec::new();

        match self.atom(offset) {
            Some(v) => expr_stack.push(v),
            None    => (),
        }

        Some(expr_stack[0].clone())
    }

    // TODO: work in progress
    pub fn atom(&mut self, offset: usize) -> Option<ASTNode> {

        // number
        if self.match_signature(&vec![
            SignatureKey::Only(TokenType::NumberLiteral(String::from(""))),
        ], offset) {
            let v = match self.get(offset).token_type {
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

    pub fn match_signature(&mut self, sequence: &Vec<SignatureKey>, offset: usize) -> bool {
        'seq: for e in sequence {
            let b = self.get(offset).token_type;

            match e {
                &SignatureKey::Only(ref a) => if !match_token(a.clone(), b.clone()) {
                    return false
                },

                &SignatureKey::Any(ref va) => {

                    for a in va.iter() {
                        if match_token(a.clone(), b.clone()) {
                            break 'seq
                        }
                    }

                    return false
                },
            }
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

fn match_token(a: TokenType, b: TokenType) -> bool {
    match a {
        TokenType::Keyword(k) => match b {
            TokenType::Keyword(k1) => {
                if k != k1 {
                    return false
                }
            },

            _ => return false,
        },

        TokenType::Identifier(_) => match b {
            TokenType::Identifier(_) => (),
            _                        => return false,
        },

        TokenType::NumberLiteral(_) => match b {
            TokenType::NumberLiteral(_) => (),
            _                           => return false,
        },

        TokenType::StringLiteral(_) => match b {
            TokenType::StringLiteral(_) => (),
            _                           => return false,
        },

        TokenType::BinaryOp(_) => match b {
            TokenType::BinaryOp(_) => (),
            _                      => return false,
        },

        TokenType::Block(_) => match b {
            TokenType::Block(_) => (),
            _                   => return false,
        },

        s => if s != b {
            return false
        }
    }

    true
}