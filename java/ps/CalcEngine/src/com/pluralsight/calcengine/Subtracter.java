package com.pluralsight.calcengine;

public class Subtracter extends CalculateBase {

    Subtracter() {}
    Subtracter(double leftVal, double rightVal){
        super(leftVal, rightVal);
    }

    @Override
    public void calculate() {
        setResult( getLeftVal() - getRightVal() );
    }
}
