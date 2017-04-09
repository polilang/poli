use super::token::{
        Token,
        TokenType,
        Operator,
    };

use super::block_tree;

#[derive(Debug, Clone)]
pub struct Tokenizer {
    tokens: Vec<Token>,
    lines:  u32,
    start:  usize,
    pos:    usize,
    top:    usize,
}

impl<'a> Tokenizer {
    pub fn new() -> Tokenizer {
        Tokenizer {
            tokens: Vec::new(),
            lines:  0,
            start:  0,
            pos:    0,
            top:    0,
        }
    }

    pub fn from(
        tokens: Vec<Token>
    ) -> Tokenizer {
        Tokenizer {
            tokens: tokens,
            lines:  0,
            start:  0,
            pos:    0,
            top:    0,
        }
    }

    pub fn get_tokens(&self) -> &Vec<Token> {
        &self.tokens
    }

    pub fn clear(&mut self) {
        self.tokens = Vec::new();
        self.lines  = 0;
        self.start  = 0;
        self.pos    = 0;
        self.top    = 0;
    }

    fn push(&mut self, token_type: TokenType, line: &str) {
        self.tokens.push(
            Token::new(
                token_type,
                line[self.start .. self.pos].to_string(),
                self.lines,
                self.pos as u32,
            )
        );

        self.start = self.pos
    }

    fn peek(&self, line: &str, offset: usize) -> char {
        match line.chars().nth(self.pos + offset) {
            Some(c) => c,
            None    => ' ',
        }
    }

    fn look(&self, line: &str) -> char {
        self.peek(line, 0)
    }

    fn skip_white(&mut self, line: &str) {
        loop {
            if self.pos >= line.len() - 1 {
                break
            }

            match self.look(line) {
                ' ' => {
                    self.pos   += 1;
                    self.start += 1
                },

                _ => break,
            }
        }
    }

    fn is_operator(&mut self, line: &str) -> bool {
        let mut is_op = false;

        let mut offset = 2; // dirty: longest operator length
        while self.pos + offset >= line.len() {
            offset -= 1
        }

        while offset > 0 && !is_op {
            match operator(&line[self.start .. self.pos + offset]) {
                Some(_) => is_op = true,
                None    => (),
            }

            offset -= 1
        }

        self.pos += offset;

        is_op
    }

    pub fn next_token(&mut self) -> bool {
        if self.top < self.tokens.len() {
            self.top += 1;
            
            return true
        }

        false
    }

    pub fn prev_token(&mut self) -> bool {
        if self.top < self.tokens.len() {
            self.top -= 1;
            
            return true
        }

        false
    }

    pub fn remaining(&self) -> usize {
        self.tokens.len() - self.top
    }

    pub fn current(&self) -> &Token {
        if self.top > self.tokens.len() - 1 {
            return &self.tokens[self.tokens.len() - 1]
        }

        &self.tokens[self.top]
    }

    pub fn current_content(&self) -> String {
        self.current().get_content().clone()
    }

    pub fn match_current(&self, token: TokenType) -> Result<&Token, String> {
        match self.current().get_type() == token {
            true  => Ok(self.current()),
            false => Err(
                    format!(
                        "expected {:?} but found {:#?}", token, self.current()
                    )
                )
        }
    }

    fn push_move(&mut self, token: TokenType, line: &str) {
        self.pos += 1;
        self.push(token, line)
    }

    pub fn tokenize(&mut self, source: String) -> Result<(), String> {
        for line in source.lines() {
            self.lines += 1;
            self.start  = 0;
            self.pos    = 0;

            while self.pos < line.len() {
                self.skip_white(line);

                let c = self.look(line);

                if c == '"' || c == '\'' {
                    let del = c;

                    self.pos   += 1;
                    self.start += 1;
                    
                    while self.look(line) != del {
                        self.pos += 1
                    }

                    self.push(TokenType::Text, line);

                    self.pos   += 1;
                    self.start += 1;

                    continue
                }

                if identifier(self.look(line)) {
                    while identifier(self.look(line)) || self.look(line).is_digit(10) {
                        self.pos += 1
                    }

                    match keyword(&line[self.start .. self.pos]) {
                        Some(t) => self.push(t, line),
                        None    => self.push(TokenType::Ident, line),
                    }

                    continue
                }

                let peek = self.peek(line, 1);

                if c.is_digit(10) || c == '.' && peek.is_digit(10)
                                  || c == '-' && peek.is_digit(10) {
                    if c == '-' {
                        self.pos += 1
                    }

                    while self.look(line).is_digit(10) {
                        self.pos += 1
                    }

                    if self.look(line) == '.' && self.peek(line, 1).is_digit(10) {
                        self.pos += 1;

                        while self.look(line).is_digit(10) {
                            self.pos += 1
                        }

                        self.push(TokenType::Float, line);

                        continue
                    }

                    self.push(TokenType::Integer, line);

                    continue
                }

                // TODO: make less hacky
                if c == '-' && self.look(line) == '>' {
                    self.pos += 2;
                    self.push(TokenType::Arrow, line);
                    
                    continue
                }

                if self.is_operator(line) {
                    self.pos += 1;
                    self.push(TokenType::Operator, line);

                    continue
                }

                match c {
                    ' '  => break,
                    '\n' => break,
                    '\0' => break,

                    _ => (),
                }

                match symbol(c) {
                    Some(t) => self.push_move(t, line),
                    None    => panic!(
                                "unexpected symbol: {}, ln {} col {}",
                                &line[self.start .. line.len()],
                                self.lines,
                                self.start,
                            )
                }
            }
        }

        Ok(())
    }

    #[allow(unused_mut, unused_must_use)]
    pub fn tokenize_branch(branch: &block_tree::Branch<'a>) -> block_tree::Branch<'a> {
        let mut product = block_tree::Branch::new(Vec::new());

        for chunk in branch.content.iter() {
            match chunk.get_value() {
                block_tree::ChunkValue::Text(t) => {
                    let mut tokenizer = Tokenizer::new();

                    tokenizer.tokenize(t.to_string());

                    product.content.push(
                        block_tree::Chunk::new(
                            block_tree::ChunkValue::Tokens(
                                    tokenizer.get_tokens().clone(),
                                )
                        )
                    )
                },

                block_tree::ChunkValue::Block(ref b) => product.content.push(
                        block_tree::Chunk::new(
                            block_tree::ChunkValue::Block(Self::tokenize_branch(b)),
                        )
                    ),

                _ => continue,
            }
        }

        product
    }
}

pub fn flatten_tree<'b>(branch: &block_tree::Branch<'b>) -> Vec<Token> {
    let mut flattened: Vec<Token> = Vec::new();

    for chunk in branch.content.iter() {
        match chunk.get_value() {
            block_tree::ChunkValue::Tokens(ref t) => flattened.append(&mut t.clone()),
            block_tree::ChunkValue::Block(ref b)  => flattened.push(
                    Token::new(
                            TokenType::Block(flatten_tree(b)), "".to_string(), 0, 0,
                        )
                ),
            _ => continue,
        }
    }

    flattened
}

fn identifier(c: char) -> bool {
    c.is_alphabetic() || c == '_'
                      || c == '?'
                      || c == '!'
}

fn keyword(v: &str) -> Option<TokenType> {
    match v {
        "true" |
        "false"  => Some(TokenType::Boolean),
        "else"   => Some(TokenType::Else),
        "if"     => Some(TokenType::If),
        "return" => Some(TokenType::Return),
        _ => None,
    }
}

fn symbol(c: char) -> Option<TokenType> {
    match c {
        '('  => Some(TokenType::LParen),
        ')'  => Some(TokenType::RParen),
        '['  => Some(TokenType::LBracket),
        ']'  => Some(TokenType::RBracket),
        '{'  => Some(TokenType::LBrace),
        '}'  => Some(TokenType::RBrace),
        ':'  => Some(TokenType::Colon),
        ','  => Some(TokenType::Comma),
        '.'  => Some(TokenType::Period),
        ';'  => Some(TokenType::Semicolon),
        '!'  => Some(TokenType::Bang),
        '='  => Some(TokenType::Assign),
        _    => None,
    }
}

pub fn operator(v: &str) -> Option<(Operator, u8)> {
    match v {
        "*"  => Some((Operator::Mul,     1)),
        "/"  => Some((Operator::Div,     1)),
        "%"  => Some((Operator::Mod,     1)),
        "+"  => Some((Operator::Plus,    2)),
        "-"  => Some((Operator::Minus,   2)),
        "==" => Some((Operator::Equal,   3)),
        "!=" => Some((Operator::NEqual,  3)),
        "<"  => Some((Operator::Lt,      4)),
        ">"  => Some((Operator::Gt,      4)),
        "<=" => Some((Operator::LtEqual, 4)),
        ">=" => Some((Operator::GtEqual, 4)),
        _    => None,
    }
}