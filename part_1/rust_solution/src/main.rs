use csv::ReaderBuilder;
use std::error::Error;
use std::io;
use std::process;

extern crate exchange;
use exchange::Exchange;
use exchange::Order;

pub fn run() -> Result<(), Box<dyn Error>> {
    let mut exchange = Exchange::new();

    let mut rdr = ReaderBuilder::new()
        .has_headers(false)
        .delimiter(b':')
        .from_reader(io::stdin());
    let mut gen: usize = 0;
    for result in rdr.records() {
        let record = result?;
        let order = Order::read(&record[0], &record[1], &record[2], &record[3], gen);
        let trades = exchange.execute(order.unwrap());
        gen += 1;
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
