use std::ops::Range;
use parser::block_tree::{
    Branch,
    ChunkContent,
    Chunk,
};

#[derive(Debug, Clone, PartialEq, Eq, Hash)]
pub enum TokenType {
    Identifier(String),
    StringLiteral(String),
    NumberLiteral(String),
    Keyword(String),
    BinaryOp(String),
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
    Invalid,

    EOF,
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

#[derive(Debug, Clone)]
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

    pub fn tokenize(source: &str) -> Vec<Token> {
        Self::new(source).collect()
    }

    /// Tokenize block tree
    pub fn tokenize_branch(branch: &Branch<'a>) -> Branch<'a> {
        let mut branch_ret = Branch {
            content: Vec::new(),
        };

        for chunk in branch.content.iter() {
            match chunk.content {
                ChunkContent::Text(t) => {
                    branch_ret.content.push(
                        Chunk {
                            content: ChunkContent::Tokens(
                                Self::tokenize(t),
                            ),
                        },
                    );
                },

                ChunkContent::Block(ref b) => {
                    branch_ret.content.push(
                        Chunk {
                            content: ChunkContent::Block(
                                Self::tokenize_branch(b),
                            ),
                        },
                    );
                },

                _ => continue,
            }
        }

        branch_ret
    }

    /// Lex identifier term at current position `self.pos`
    fn lex_term(&mut self) -> &str {
        while let Some(c) = self.current {
            if !(identifier_worthy(c) || c.is_digit(10)) {
                break
            }

            self.move_forward(1);
        }

        &self.source[self.token_pos .. self.pos]
    }

    /// Lex number literal at current position `self.pos`
    fn lex_number(&mut self) -> &str {
        while let Some(c) = self.current {
            if !c.is_digit(10) && c != '.' {
                break
            }

            self.move_forward(1);
        }

        &self.source[self.token_pos .. self.pos]
    }

    /// Lex binary operand at current position `self.pos`
    fn lex_bin_op(&mut self) -> String {
        let mut accum  = 2;
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
                None    => {
                    ()
                },
            }

            self.move_forward(1);
        }

        buffer
    }

    /// Lex string literal by given delimeter `del`
    fn lex_string(&mut self, del: char) -> String {
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

                c if c == del  => break,
                c    => buffer.push(c),
            }
        }

        buffer
    }

    /// Moves past whitespace in `self.source` string
    fn move_whitespace(&mut self) {
        while let Some(c) = self.current {
            if !c.is_whitespace() {
                break
            }

            self.move_forward(1)
        }
    }

    /// Moves position pointer `delta` steps forward
    fn move_forward(&mut self, delta: usize) {
        for _ in 0 .. delta {
            if let Some(c) = self.current {
                self.pos += c.len_utf8();
                self.current = self.char_at(self.pos);
            } else {
                panic!("lexer lexing too far forward")
            }
        }
    }

    /// Moves position pointer `delta` steps backward
    fn move_backward(&mut self, delta: usize) {
        for _ in 0 .. delta {
            if let Some(c) = self.current {
                self.pos     -= c.len_utf8();
                self.current = self.char_at(self.pos);
            }
        }
    }

    /// Returns char at position `pos`
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
            match keyword(self.lex_term()) {
                Some(t) => t,
                None    => TokenType::Identifier(String::from(self.lex_term())),
            }
        } else if c.is_digit(10) {
            TokenType::NumberLiteral(
                String::from(self.lex_number())
            )
        } else {
            let a = self.lex_bin_op();
            match bin_op(&a) {
                Some(_) => TokenType::BinaryOp(a),
                None    => {
                    match c {
                        '"' | '\'' =>  {
                            self.move_backward(1);
                            TokenType::StringLiteral(
                                String::from(self.lex_string(c))
                            )
                        },
                        _    => {
                            match symbol(&c) {
                                Some(s) => {
                                    self.move_backward(1);
                                    s
                                },
                                None    => TokenType::Invalid,
                            }
                        }
                    }
                },
            }
        };

        Some(Token::new(token_type, self.token_pos, self.pos))
    }
}

fn identifier_worthy(c: char) -> bool {
    c.is_alphabetic() || c == '_'
                      || c == '?'
}

fn keyword(k: &str) -> Option<TokenType> {
    match k {
          "if"
        | "for"
        | "else"
        | "return"
        | "true"
        | "false"
        | "elif"
        | "not"
        | "or"
        | "with"
        | "from"
        | "this" => Some(TokenType::Keyword(String::from(k))),
        _        => None,
    }
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

fn symbol(v: &char) -> Option<TokenType> {
    match *v {
        '['  => Some(TokenType::LBracket),
        ']'  => Some(TokenType::RBracket),
        '('  => Some(TokenType::LParen),
        ')'  => Some(TokenType::RParen),
        '{'  => Some(TokenType::LBrace),
        '}'  => Some(TokenType::RBrace),
        ','  => Some(TokenType::Comma),
        ':'  => Some(TokenType::Colon),
        ';'  => Some(TokenType::Semicolon),
        '.'  => Some(TokenType::Period),
        '='  => Some(TokenType::Assign),
        _    => None,
    }
}