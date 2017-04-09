extern crate docopt;

use docopt::Docopt;

use std::io;
use std::io::prelude::*;

use std::fs::File;
use std::env;

mod parser;

mod compiler;
use compiler::Module;

const USAGE: &'static str = "
the poli language

usage:
    poli build <source> <destination>
    poli run <source>
    poli trees <source>
    poli repl
    poli (-h | --help)
    poli --version

options:
    -h --help   show this message
    --version   show compiler version
    --optimize  optimize compiled LLVM IR
";

fn build(ast: Vec<parser::ast::Statement>, destination: &str) {

    let module = Module::new("poli");

    module.write_object(destination);

    println!("=> writing to: {}\n", destination);
    
    module.dump();

    println!("")
}

fn repl() {
    loop {
        print!(">>> ");
        io::stdout().flush().unwrap();

        let mut input_line = String::new();

        match io::stdin().read_line(&mut input_line) {
            Ok(_)  => {
                if input_line.trim() == String::from(":quit") ||
                   input_line.trim() == String::from(":q") 
                {
                    println!("=> bye bb <3");

                    std::process::exit(0)
                }
                
                println!("=>\n{:#?}", parse(&input_line));
            },

            Err(e) => panic!(e),
        }
    }
}

fn parse(source: &str) -> Vec<parser::ast::Statement> {
    use parser::block_tree::BlockTree;

    let mut tree = BlockTree::new(source, 0);
    let indents  = tree.collect_indents();

    let root = tree.make_tree(&indents);

    let mut parser = parser::ast::Parser::from(
             parser::tokenizer::Tokenizer::from(
                    parser::tokenizer::flatten_tree(
                            &parser::tokenizer::Tokenizer::tokenize_branch(&root)
                        ),
                ),
        );

    match parser.parse() {
        Ok(c)  => c,
        Err(e) => panic!(e),
    }
}

#[allow(unused_must_use)]
fn file<'a>(source: &str) -> String {
     let mut source_file = match File::open(source) {
        Ok(f)  => f,
        Err(_) => panic!("failed to open path: {}", source),
    };

    let mut source_buffer = String::new();
    source_file.read_to_string(&mut source_buffer).unwrap();

    source_buffer
}

fn main() {
    let argv: Vec<String> = env::args().collect();

    let args = Docopt::new(USAGE)
                       .and_then(|d| d.argv(argv.into_iter()).parse())
                       .unwrap_or_else(|e| e.exit());

    if args.get_bool("repl") {
        repl()
    } else if args.get_bool("run") {
         println!("abstract syntax tree =>\n{:#?}", parse(&file(args.get_str("<source>"))));
    } else if args.get_bool("build") {
        build(parse(&file(args.get_str("<source>"))), args.get_str("<destination>"));
    }
}