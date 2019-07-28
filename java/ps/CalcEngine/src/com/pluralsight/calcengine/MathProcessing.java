package com.pluralsight.calcengine;

public interface MathProcessing {
    String SEPERATOR =  " ";
    String getKeyword() ; // ex add
    char getSymbol(); // ex +
    double doCalculate( double leftVal, double rightVal );
}
