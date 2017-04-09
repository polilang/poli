#[derive(Debug, PartialEq, Clone)]
pub enum TokenType {
    Block(Vec<Token>),

    Integer,
    Float,

    Text,
    Ident,
    Assign,

    Return,

    LParen,
    RParen,
    LBrace,
    RBrace,
    LBracket,
    RBracket,

    Arrow,

    Colon,
    Comma,
    Period,
    Bang,
    Semicolon,

    If,
    Else,

    Boolean,
    Operator,
}

#[derive(Debug, Clone)]
pub enum Operator {
    Mul,
    Div,
    
    Plus,
    Minus,

    Equal,
    NEqual,

    Mod,

    Lt,
    LtEqual,
    Gt,
    GtEqual,    
}

#[derive(Debug, Clone, PartialEq)]
pub struct Token {
    token_type: TokenType,
    row:        u32,
    col:        u32,
    content:    String,    
}

impl Token {
    pub fn new(
        token_type: TokenType,
        content:    String,
        row:        u32,
        col:        u32,
    ) -> Token {
        Token {
            token_type: token_type,
            row:        row,
            col:        col,
            content:    content,
        }
    }

    pub fn get_content(&self) -> &String {
        &self.content
    }

    pub fn get_type(&self) -> TokenType {
        self.token_type.clone()
    }

    pub fn get_position(&self) -> (&u32, &u32) {
        (&self.row, &self.col)
    }
}