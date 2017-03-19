use std::ops::Range;

#[derive(Debug, Clone, PartialEq, Eq)]
pub enum TokenType {
    Identifier(String),
    
    StringLiteral,
    IntegerLiteral,
    FloatLiteral,
    
    Colon,
    Semicolon,
    Comma,
    Period,

    LParen,
    RParen,
    LBrace,
    RBrace,
    LBracket,
    RBracket,

    BinaryOp,

    Invalid,
}

#[derive(Debug, Clone, PartialEq, Eq)]
pub enum BinaryOp {
    Mul,
    Div,
    Plus,
    Minus,
    Equals,
    NotEquals,
    Lt,
    Gt,
    LtEqual,
    GtEqual,
}

#[derive(Debug, Clone, PartialEq, Eq)]
pub struct Token {
    pub token_type: TokenType,
    pub span:       Span,
}

impl Token {
    pub fn new(token_type: TokenType, start: usize, end: usize) -> Token {
        Token {
            token_type: token_type,
            span:       Span::new(start, end),
        }
    }
}

#[derive(Debug, Clone, PartialEq, Eq)]
pub struct Span {
    pub start: usize,
    pub end:   usize,
}

impl Span {
    pub fn new(start: usize, end: usize) -> Span {
        Span {
            start: start,
            end:   end,
        }
    }
}

impl From<usize> for Span {
    fn from(position: usize) -> Span {
        Span::new(position, position)
    }
}

impl From<Range<usize>> for Span {
    fn from(range: Range<usize>) -> Span {
        Span::new(range.start, range.end)
    }
}

#[derive(Debug, Clone, PartialEq, Eq)]
pub struct Lexer<'a> {
    source:    &'a str,

    pos:       usize,
    token_pos: usize,
    
    current:   Option<char>,
}

impl<'a> Lexer<'a> {
    pub fn new(source: &'a str) -> Lexer {
        let mut lexer = Lexer {
            source:    source,

            pos:       0,
            token_pos: 0,

            current:   None,
        };

        lexer.current = lexer.char_at(0);
        lexer
    }

    fn lex_identifier(&mut self) -> String {
        while let Some(c) = self.current {
            if !(identifier_worthy(c) || c.is_digit(10)) {
                break
            }

            self.move_forward();
        }

        String::from(&self.source[self.token_pos .. self.pos])
    }

    fn move_whitespace(&mut self) {
        while let Some(c) = self.current {
            if !c.is_whitespace() {
                break
            }

            self.move_forward()
        }
    }

    fn move_forward(&mut self) {
        if let Some(c) = self.current {
            self.pos += c.len_utf8();
            self.current    = self.char_at(self.pos);
        } else {
            panic!("lexer lexing too far")
        }
    }

    fn char_at(&self, pos: usize) -> Option<char> {
        self.source[pos ..].chars().next()
    }
}

impl<'a> Iterator for Lexer<'a> {
    type Item = Token;

    fn next(&mut self) -> Option<Token> {

        self.move_whitespace();
        self.token_pos = self.pos;

        let c = match self.current {
            Some(c) => c,
            None    => return None,
        };

        let token_type = if c.is_alphabetic() {
            TokenType::Identifier(self.lex_identifier())
        } else {
            self.move_forward();

            TokenType::Invalid
        };

        Some(Token::new(token_type, self.token_pos, self.pos))
    }
}

pub fn tokenize(source: &str) -> Vec<Token> {
    Lexer::new(source).collect()
}

fn identifier_worthy(c: char) -> bool {
    c.is_alphabetic() || c == '_'
                      || c == '?'
}