

package com.pluralsight.app;

import com.pluralsight.calcengine.*;

public class Main {


    public static void caclulate_01() {

        System.out.println("Calculator:");

        CalculateBase[] calculators = {

                new Divider(100.0d, 50.0d),
                new Adder(25.0d, 92.0d),
                new Subtracter(225.0d, 17.0d),
                new Multiplier(11.0d, 3.0d),
        };

        for (CalculateBase c : calculators) {
            c.calculate();

            System.out.print("result=");
            System.out.println(c.getResult());
        }
    }

    ;


    public static void main(String[] args) {

        String[] statements = {
                "add 1.0",
                "add xxx, 1.0",
                "bad_operation 0 0",
                "divide 100.00 50.00",
                "add 25.0 92.0",
                "subtract 225 15",
                "multiply 11 3"
        };


        for (String c : statements) {
            try {
                CalculateHelper helper = new CalculateHelper();

                helper.process(c);
                System.out.println(helper);
            } catch (InvalidStatementException e) {
                System.out.println(e.getMessage());
                if (e.getCause() != null )
                    System.out.println("    Original exception:" + e.getCause().getMessage());
            }
        }
    }
}
