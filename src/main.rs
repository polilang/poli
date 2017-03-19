extern crate clap;
use clap::App;

fn main() {
    App::new("the poli language")
        .version("v0.0-dev")
        .about("a compiled programming language")
        .args_from_usage(
            "<input>              'specifies input source file'")
        .get_matches();
}