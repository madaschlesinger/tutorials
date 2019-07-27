package com.pluralsight.calcengine;

public class Multiplier extends  CalculateBase{

    Multiplier() {}
    Multiplier( double leftVal, double rightVal){
        super(leftVal, rightVal);
    }

    @Override
    public void calculate() {
        setResult( getLeftVal() * getRightVal() );
    }
}
