package com.pluralsight.calcengine;

public class PowerOf implements MathProcessing {
    @Override
    public String getKeyword() {
        return "pow";
    }

    @Override
    public char getSymbol() {
        return '^';
    }

    @Override
    public double doCalculate(double leftVal, double rightVal) {

        return Math.pow(leftVal,rightVal);
    }
}
