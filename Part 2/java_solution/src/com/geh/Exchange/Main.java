package com.geh.Exchange;

import java.util.*;

public class Main {

    public static void main(String[] args) {
        try {
            var exchange = new Exchange();
            var scanner = new Scanner(System.in);
            while (scanner.hasNextLine()) {
                var line = scanner.nextLine();
                if (line.isEmpty()) {
                    continue;
                }
                var order = new Order(line);
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
