use csv::{ReaderBuilder, StringRecord};
use std::error::Error;
use std::io;
use std::io::{Write, BufWriter};
use std::process;

extern crate exchange;
use exchange::{order, Exchange};

pub fn run() -> Result<(), Box<dyn Error>> {
    let mut exchange = Exchange::new();

    let mut rdr = ReaderBuilder::new()
        .has_headers(false)
        .delimiter(b':')
        .from_reader(io::stdin());
    let mut record = StringRecord::with_capacity(100, 4);
    let mut gen = 0;
    let stdout = io::stdout();
    let mut buf_stdout = BufWriter::new(stdout);
    while rdr.read_record(&mut record)? {
        gen += 1;
        let instrument = &record[1];
        let order = order::read(&record[0], &record[2], &record[3], gen)?;
        let trades = exchange.execute(instrument, order);
        for t in trades {
            writeln!(buf_stdout, "{}", t)?;
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
