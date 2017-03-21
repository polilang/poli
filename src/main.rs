extern crate docopt;

use docopt::Docopt;

use std::io;
use std::io::prelude::*;

use std::fs::File;
use std::env;

mod parser;
use parser::lexer;
use parser::block_tree::BlockTree;


const USAGE: &'static str = "
the poli language

usage:
    poli run <source>
    poli trees <source>
    poli build <source> [--optimize]
    poli new <name>
    poli repl
    poli (-h | --help)
    poli --version

    poli lexer <source>
    poli ast <source>

options:
    -h --help   show this message
    --version   show compiler version
    --optimize  optimize compiled LLVM IR
";

fn run_repl() {
    println!("the poli language\n");

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

                println!("=> ");

                for t in lexer::tokenize(&input_line) {
                    println!("{:?}", t.token_type)
                }

                println!()
            },

            Err(e) => panic!(e),
        }
    }
}

fn run_file(path: &str) {
    let mut source_file = match File::open(path) {
        Ok(f)  => f,
        Err(_) => panic!("failed to open path: {}", path),
    };

    let mut source_buffer = String::new();
    source_file.read_to_string(&mut source_buffer).unwrap();

    println!("=> ");

    for t in lexer::tokenize(&source_buffer) {
        println!("{:?}", t.token_type)
    }
}

fn run_block_tree(path: &str) {

    let mut source_file = match File::open(path) {
        Ok(f)  => f,
        Err(_) => panic!("failed to open path: {}", path),
    };

    let mut source_buffer = String::new();
    source_file.read_to_string(&mut source_buffer).unwrap();

    println!("=> ");

    let mut tree = BlockTree::new(&source_buffer, 0);
    let mut collection = tree.collect_indents();
    let mut root_chunk = tree.make_tree(&collection);

    println!("{:?}", root_chunk)
}

fn main() {

    let argv: Vec<String> = env::args().collect();

    let args = Docopt::new(USAGE)
                       .and_then(|d| d.argv(argv.into_iter()).parse())
                       .unwrap_or_else(|e| e.exit());

    if args.get_bool("repl") {
        run_repl()
    } else if args.get_bool("run") {
        run_file(args.get_str("<source>"))
    } else if args.get_bool("trees") {
        run_block_tree(args.get_str("<source>"))
    }
}