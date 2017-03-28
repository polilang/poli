use parser::lexer::Token;

#[derive(Debug, Clone)]
pub enum ChunkContent<'a> {
    Text(&'a str),
    Tokens(Vec<Token>),
    Block(Branch<'a>),
}

#[derive(Debug, Clone)]
pub struct Chunk<'a> {
    pub content: ChunkContent<'a>,
}

#[derive(Debug, Clone)]
pub struct Branch<'a> {
    pub content: Vec<Chunk<'a>>,
}

#[derive(Debug, Clone, PartialEq, Eq)]
pub struct BlockTree<'a> {
    source: &'a str,
    line_number: usize,
}

impl<'a> BlockTree<'a> {
    pub fn new(source: &'a str, line_number: usize) -> BlockTree {
        BlockTree {
            source: source,
            line_number: line_number,
        }
    }

    pub fn collect_indents(&mut self) -> Vec<(usize, &'a str)> {
        let mut indent_list: Vec<(usize, &'a str)> = Vec::new();

        let mut lines = self.source.lines();

        while let Some(l) = lines.next() {

            let vec_ln: Vec<&str> = l.split("~").collect();

            let ln = vec_ln.get(0).unwrap().trim();

            if ln.len() > 0 {
                let indent = get_indent(&l);
                indent_list.push((indent, ln));
            }
        }
        
        indent_list
    }

    pub fn make_tree(&mut self, indent_list: &Vec<(usize, &'a str)>) -> Branch<'a> {
        let mut branch: Branch = Branch {
            content: Vec::new(),
        };

        let base_line               = indent_list.get(self.line_number);
        let &(base_indent, _) = match base_line {
            Some(l) => l,
            None    => return branch,
        };
        
        while self.line_number < indent_list.len() {
            let &(indent, line) = match indent_list.get(self.line_number) {
                Some(l) => l,
                None    => panic!("parsing non-existing branch"),
            };

            if indent == base_indent {
                branch.content.push(
                    Chunk {
                        content: ChunkContent::Text(line),
                    },
                );
            } else if indent < base_indent {
                self.line_number -= 1;

                return branch
            } else if indent > base_indent {
                branch.content.push(
                    Chunk {
                        content: ChunkContent::Block(
                            self.make_tree(&indent_list)
                        ),
                    },
                );
            }

            self.line_number += 1;
        }

        branch
    }
}

fn get_indent(line: &str) -> usize {
    let mut pos: usize = 0;

    for c in line.chars() {
        if c == ' ' {
            pos += 1
        }
    }

    pos
}