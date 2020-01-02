package com.geh.Exchange;

import java.io.*;
import java.util.*;

public class Main {

    public static void main(String[] args) {
        try {
            var exchange = new Exchange();
            BufferedReader reader = new BufferedReader(new InputStreamReader(System.in));
            while (reader.ready()) {
                var line = reader.readLine();
                if (line.isEmpty()) {
                    continue;
                }
                var order = Order.get(line);
                for (var trade : exchange.execute(order)) {
                    System.out.printf("%s:%s:%s:%d:%g\n", trade.getBuyer(), trade.getSeller(), trade.getInstrument(), trade.getQuantity(), trade.getPrice());
                }
            }
        }
        catch (Exception ex) {
            System.err.printf(ex.getMessage());
            System.exit(1);
        }
    }
}
