package com.geh.Exchange;

import java.io.*;
import java.nio.charset.StandardCharsets;
import java.util.*;

public class Main {

    public static void main(String[] args) {
        BufferedWriter writer = null;
        try {
            var exchange = new Exchange();
            var reader = new BufferedReader(new InputStreamReader(System.in));
            writer = new BufferedWriter(new OutputStreamWriter(new FileOutputStream(java.io.FileDescriptor.out), StandardCharsets.US_ASCII), 64 * 1024);
            while (reader.ready()) {
                var line = reader.readLine();
                if (line.isEmpty()) {
                    continue;
                }
                var order = Order.get(line);
                for (var trade : exchange.execute(order)) {
                    writer.write(trade.getBuyer());
                    writer.write(':');
                    writer.write(trade.getSeller());
                    writer.write(':');
                    writer.write(trade.getInstrument());
                    writer.write(':');
                    writer.write(Long.toString(trade.getQuantity()));
                    writer.write(':');
                    writer.write(Double.toString(trade.getPrice()));
                    writer.write('\n');
                }
            }
            writer.close();
        }
        catch (Exception ex) {
            System.err.print(ex.getMessage());
            // Flush whatever trades were already buffered before failing, otherwise
            // System.exit would drop them without ever reaching the output stream.
            if (writer != null) {
                try {
                    writer.flush();
                } catch (IOException ignored) {
                }
            }
            System.exit(1);
        }
    }
}
