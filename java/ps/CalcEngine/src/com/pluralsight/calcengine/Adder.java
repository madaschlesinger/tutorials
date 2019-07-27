package com.pluralsight.calcengine;

public class Adder  extends  CalculateBase {

    Adder() {}
    Adder( double leftVal, double rightVal){
        super(leftVal, rightVal);
    }

    @Override
    public void calculate() {
        setResult( getLeftVal() + getRightVal() );
    }
}
