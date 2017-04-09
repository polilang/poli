use super::token::Token;

#[derive(Debug, Clone)]
pub enum ChunkValue<'a> {
    Text(&'a str),
    Tokens(Vec<Token>),
    Block(Branch<'a>),
}

#[derive(Debug, Clone)]
pub struct Chunk<'a> {
    value: ChunkValue<'a>,
}

impl<'a> Chunk<'a> {
    pub fn new(
        value: ChunkValue<'a>
    ) -> Chunk<'a> {

        Chunk {
            value: value,
        }
    }

    pub fn get_value(&self) -> ChunkValue<'a> {
        self.value.clone()
    }
}

#[derive(Debug, Clone)]
pub struct Branch<'a> {
    pub content: Vec<Chunk<'a>>,
}

impl<'a> Branch<'a> {
    pub fn new(
        content: Vec<Chunk<'a>>
    ) -> Branch<'a> {

        Branch {
            content: content,
        }
    }
}

#[derive(Debug, Clone)]
pub struct BlockTree<'a> {
    source: &'a str,
    line:   usize,
    method: Option<char>,
}

impl<'a> BlockTree<'a> {
    pub fn new(
        source: &'a str,
        line:   usize,
    ) -> BlockTree {

        BlockTree {
            source: source,
            line:   line,
            method: None,
        }
    }

    pub fn collect_indents(&mut self) -> Vec<(usize, &'a str)> {
        let mut indents: Vec<(usize, &'a str)> = Vec::new();
        
        let mut lines = self.source.lines();
    
        while let Some(line) = lines.next() {

            let parts: Vec<&str> = line.split("~").collect();
            let ln = parts.get(0).unwrap().trim();

            if ln.len() > 0 {
                let indent = self.get_indent(&line);
                indents.push((indent, ln))
            }
        }

        indents
    }

    pub fn get_indent(&mut self, line: &str) -> usize {
        let mut pos: usize = 0;

        for c in line.chars() {
            match c {
                ' ' | '\t' => {
                    match self.method {
                        Some(m) => assert!(m == c, "use of inconsistent indentation"),
                        None    => self.method = Some(c),
                    }

                    pos += 1
                }

                _ => break,
            }
        }

        pos
    }

    pub fn make_tree(
        &mut self,
        indents: &Vec<(usize, &'a str)>
    ) -> Branch<'a> {
        let mut branch = Branch::new(Vec::new());
        let base_line  = indents.get(self.line);

        let &(base_indent, _) = match base_line {
                                    Some(i) => i,
                                    None    => return branch,
                                };

        while self.line < indents.len() {
            let &(indent, line) = match indents.get(self.line) {
                                      Some(i) => i,
                                      None    => panic!("branching nothing!?"),
                                  };
            
            if indent == base_indent {
                branch.content.push(
                    Chunk::new(ChunkValue::Text(line)),
                )
            } else if indent < base_indent {
                self.line -= 1;
                return branch
            } else if indent > base_indent {
                branch.content.push(
                    Chunk::new(
                            ChunkValue::Block(
                                self.make_tree(&indents)
                            ),
                        ),
                )
            }

            self.line += 1
        }

        branch
    }
}
