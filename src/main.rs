use std::io;
use std::io::prelude::*;

use std::fs::File;
use std::env;

mod parser;
use parser::lexer;

fn repl_loop() {
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
                    println!("{:?}", t)
                }

                println!()
            },

            Err(e) => panic!(e),
        }
    }
}

fn main() {
    let args: Vec<String> = env::args().collect();

    if args.len() > 1 {
        let path = &args[1];

        let mut source_file = match File::open(path) {
            Ok(f)  => f,
            Err(_) => panic!("failed to open path: {}", path),
        };

        let mut source_buffer = String::new();
        source_file.read_to_string(&mut source_buffer).unwrap();

        println!("=> {}", source_buffer)
        
    } else {
        repl_loop();
    }
}