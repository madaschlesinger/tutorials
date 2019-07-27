package com.pluralsight.calcengine;

public class Divider extends  CalculateBase{

    Divider() {}
    Divider(double leftVal, double rightVal){
        super(leftVal, rightVal);
    }

    @Override
    public void calculate() {
        setResult( getLeftVal() / getRightVal() );
    }
}
