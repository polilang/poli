extern crate docopt;

use docopt::Docopt;

use std::io;
use std::io::prelude::*;

use std::fs::File;
use std::env;

mod parser;
use parser::lexer;
use parser::ast;
use parser::block_tree::{
    BlockTree, Branch
};


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

fn test_parser(token_stack: Vec<lexer::Token>) {
    let mut parser = ast::Parser::new(token_stack);

    // simple assignment between identifiers
    #[derive(Debug)]
    struct Test {}

    #[derive(Debug)]
    struct Something {}

    impl Test {
        pub fn new(a: lexer::TokenType, b: lexer::TokenType) -> Test {
            println!("{:#?}, {:#?}", a, b);

            Test {}
        }
    }

    impl ast::ParserNode for Test {
        fn parse(&self, p: &ast::Parser) -> Box<ast::ParserNode> {
            println!("ye, test bb <4");

            Box::new(Test::new(p.get_backward(1).token_type, p.get(1).token_type))
        }
    }

    impl Something {
        pub fn new(a: Vec<lexer::TokenType>) -> Something {
            println!("{:#?}", a);

            Something {}
        }
    }

    impl ast::ParserNode for Something {
        fn parse(&self, p: &ast::Parser) -> Box<ast::ParserNode> {
            println!("ye, test call <4");

            let mut arg_stack: Vec<lexer::TokenType> = Vec::new();
            let mut off = 0usize;

            while !p.match_sequence(&vec![lexer::TokenType::RParen], off) {
                arg_stack.push(p.get(off).token_type);
                off += 1
            }

            Box::new(Something::new(arg_stack))
        }
    }


    parser.introduce_node(
        vec![lexer::TokenType::Identifier(String::from("")), 
            lexer::TokenType::Assign,
            lexer::TokenType::Identifier(String::from(""))],

        Box::new(Test {}),
    );

    parser.introduce_node(
        vec![lexer::TokenType::Identifier(String::from("")), lexer::TokenType::LParen],

        Box::new(Something {}),
    );

    parser.parse();
}

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

                let mut tree   = BlockTree::new(&input_line, 0);
                let collection = tree.collect_indents();
                let root_chunk = lexer::Lexer::tokenize_branch(
                    &tree.make_tree(&collection),
                );

                println!("=> {:#?}", root_chunk);

                test_parser(lexer::Lexer::tokenize(&input_line))
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

    for t in lexer::Lexer::tokenize(&source_buffer) {
        println!("{:?}", t.token_type)
    }
}

fn build_file(_: &str, destination: &str) {

    let module = Module::new("poli");

    module.write_object(destination);

    println!("=> writing to: {}\n", destination);
    
    module.dump();

    println!("")
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
    let collection = tree.collect_indents();
    let root_chunk = lexer::Lexer::tokenize_branch(
        &tree.make_tree(&collection),
    );

    println!("{:#?}", root_chunk)
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
    } else if args.get_bool("build") {
        build_file(args.get_str("<source>"), args.get_str("<destination>"))
    } else if args.get_bool("trees") {
        run_block_tree(args.get_str("<source>"))
    }
}