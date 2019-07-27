package com.pluralsight.calcengine;

public class Main {

    public static void main(String[] args) {

        System.out.println("Calculator:");

        CalculateBase[] calculators = {

                new Divider(100.0d, 50.0d),
                new Adder( 25.0d, 92.0d),
                new Subtracter( 225.0d, 17.0d),
                new Multiplier( 11.0d, 3.0d),
        };

        for( CalculateBase c: calculators)  {
            c.calculate();

            System.out.print("result=");
            System.out.println(c.getResult());
        }


    }
}
