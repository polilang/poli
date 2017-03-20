use std::ops::Range;

#[derive(Debug, Clone, PartialEq, Eq)]
pub enum TokenType {
    Identifier(String),
    
    StringLiteral(String),
    IntegerLiteral(String),
    FloatLiteral(String),
    
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

    If,
    Else,
    For,
    This,
    Return,
    Bool(bool),

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

    fn lex_term(&mut self) -> &str {
        while let Some(c) = self.current {
            if !(identifier_worthy(c) || c.is_digit(10)) {
                break
            }

            self.move_forward();
        }

        &self.source[self.token_pos .. self.pos]
    }

    fn lex_string(&mut self) -> String {
        let mut buffer = String::new();

        loop {
            let c = match self.current {
                Some(c) => c,
                None    => panic!("opened string is unterminated"),
            };

            self.move_forward();

            match c {
                '\\' => {
                    let escaped = match self.current {
                        Some(e) => e,
                        None    => continue,
                    };

                    self.move_forward();

                    buffer.push(match escaped {
                        '"' | '\\' => escaped,
                        't'         => '\t',
                        'n'         => '\n',
                        'r'         => '\r',
                        c           => {
                            panic!("invalid escaped character: {:?}", c)
                        },
                    })
                },

                '"'  => break,
                c    => buffer.push(c),
            }
        }

        buffer
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
            match self.lex_term() {
                "if"     => TokenType::If,
                "for"    => TokenType::For,
                "true"   => TokenType::Bool(true),
                "false"  => TokenType::Bool(false),
                "return" => TokenType::Return,
                _        => TokenType::Identifier(
                                String::from(self.lex_term())
                            ),
            }
        } else {
            self.move_forward();

            match c {
                '"' => TokenType::StringLiteral(
                           String::from(self.lex_string())
                       ),
                '['  => TokenType::LBracket,
                ']'  => TokenType::RBracket,
                '('  => TokenType::LParen,
                ')'  => TokenType::RParen,
                '{'  => TokenType::LBrace,
                '}'  => TokenType::RBrace,
                _    => TokenType::Invalid,
            }
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