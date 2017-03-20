use std::ops::Range;

#[derive(Debug, Clone, PartialEq, Eq)]
pub enum TokenType {
    Identifier(String),
    
    StringLiteral(String),
    NumberLiteral(String),
    
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

    Assign,

    If,
    Else,
    For,
    This,
    Return,
    Bool(bool),

    BinaryOp((BinaryOp, u8)),

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
    LtEquals,
    GtEquals,
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

            self.move_forward(1);
        }

        &self.source[self.token_pos .. self.pos]
    }

    fn lex_number(&mut self) -> &str {
        while let Some(c) = self.current {
            if !c.is_digit(10) && c != '.' {
                break
            }

            self.move_forward(1);
        }

        &self.source[self.token_pos .. self.pos]
    }

    fn lex_bin_op(&mut self) -> String {
        let mut accum  = 2; // max operand length
        let mut buffer = String::new();

        while let Some(c) = self.current {
            if accum == 0 {
                break
            }

            buffer.push(c);
            accum -= 1;

            match bin_op(&buffer) {
                Some(_) => {
                    self.move_forward(2 - accum);
                    break
                },
                None    => (),
            }

            self.move_forward(1);
        }

        buffer
    }

    fn lex_string(&mut self) -> String {
        let mut buffer = String::new();

        loop {
            let c = match self.current {
                Some(c) => c,
                None    => panic!("opened string is unterminated"),
            };

            self.move_forward(1);

            match c {
                '\\' => {
                    let escaped = match self.current {
                        Some(e) => e,
                        None    => continue,
                    };

                    self.move_forward(1);

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

            self.move_forward(1)
        }
    }

    fn move_forward(&mut self, delta: usize) {
        for _ in 0 .. delta {
            if let Some(c) = self.current {
                self.pos += c.len_utf8();
                self.current = self.char_at(self.pos);
            } else {
                panic!("lexer lexing too far")
            }
        }
    }

    fn move_backward(&mut self, delta: usize) {
        for _ in 0 .. delta {
            if let Some(c) = self.current {
                self.pos -= c.len_utf8();
                self.current = self.char_at(self.pos);
            } else {
                panic!("lexer lexing too far")
            }
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
                "else"   => TokenType::Else,
                "for"    => TokenType::For,
                "true"   => TokenType::Bool(true),
                "false"  => TokenType::Bool(false),
                "return" => TokenType::Return,
                "this"   => TokenType::This,
                _        => TokenType::Identifier(
                                String::from(self.lex_term())
                            ),
            }
        } else if c.is_digit(10) {
            TokenType::NumberLiteral(
                String::from(self.lex_number())
            )
        } else {
            match bin_op(&self.lex_bin_op()) {
                Some(o) => TokenType::BinaryOp(o),
                None    => {
                    self.move_backward(1);

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
                        ','  => TokenType::Comma,
                        ':'  => TokenType::Colon,
                        ';'  => TokenType::Semicolon,
                        '.'  => TokenType::Period,
                        '='  => TokenType::Assign,
                        _    => TokenType::Invalid
                    }
                },
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

fn bin_op(v: &str) -> Option<(BinaryOp, u8)> {
    match v {
        "*"  => Some((BinaryOp::Mul, 1)),
        "/"  => Some((BinaryOp::Div, 1)),
        "+"  => Some((BinaryOp::Plus, 2)),
        "-"  => Some((BinaryOp::Minus, 2)),
        "==" => Some((BinaryOp::Equals, 4)),
        "~=" => Some((BinaryOp::NotEquals, 4)),
        "<"  => Some((BinaryOp::Lt, 4)),
        ">"  => Some((BinaryOp::Gt, 4)),
        "<=" => Some((BinaryOp::GtEquals, 4)),
        ">=" => Some((BinaryOp::LtEquals, 4)),
        _    => None,
    }
}