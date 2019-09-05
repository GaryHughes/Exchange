use csv::ReaderBuilder;
use std::error::Error;
use std::process;
use std::io;

extern crate exchange;
use exchange::{order, Exchange};

pub fn run() -> Result<(), Box<dyn Error>> {
    let mut exchange = Exchange::new();

    let mut rdr = ReaderBuilder::new()
        .has_headers(false)
        .delimiter(b':')
        .from_reader(io::stdin());
    for (gen, result) in rdr.records().enumerate() {
        let record = result?;
        let order = order::read(&record[0], &record[1], &record[2], &record[3], gen);
        let trades = exchange.execute(order.unwrap());
        for t in trades {
            println!("{}", t);
        }
    }
    Ok(())
}

fn main() {
    if let Err(err) = run() {
        eprintln!("error: {}", err);
        process::exit(1);
    }
}
