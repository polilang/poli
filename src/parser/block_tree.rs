#[derive(Debug, Clone)]
enum ChunkContent<'a> {
    Text(&'a str),
    Block(Box<Branch<'a>>),
}

#[derive(Debug, Clone)]
struct Chunk<'a> {
    content: ChunkContent<'a>,
}

#[derive(Debug, Clone)]
struct Branch<'a> {
    content: Vec<Chunk<'a>>,
}

#[derive(Debug, Clone)]
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
            if l.trim().len() > 0 {
                let indent = get_indent(&l);
                indent_list.push((indent, &l[indent..]));
            }
        }
        
        indent_list
    }

    pub fn make_tree(&mut self, indent_list: &Vec<(usize, &'a str)>) -> Branch<'a> {

        let base_line               = indent_list.get(self.line_number);
        let &(base_indent, _) = match base_line {
            Some(l) => l,
            None    => panic!("parsing non-existing branch"),
        };

        let mut branch: Branch = Branch {
            content: Vec::new(),
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
                            Box::new(self.make_tree(&indent_list))
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

    while line[pos..].chars().next() == Some(' ') {
        pos += 1
    }

    pos
}