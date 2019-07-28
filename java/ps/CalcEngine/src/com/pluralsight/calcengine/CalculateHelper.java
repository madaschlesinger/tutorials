package com.pluralsight.calcengine;

public class CalculateHelper {

    private static final char ADD_SYMBOL = '+';
    private static final char DIV_SYMBOL = '/';
    private static final char SUB_SYMBOL = '-';
    private static final char MULT_SYMBOL = '*';

    private MathCommand command;
    private double leftVal;
    private double rightVal;
    private double result;

    public void process( String statement ) throws InvalidStatementException
    {
        String[] parts = statement.split( " ");

        if (parts.length != 3 )
            throw new InvalidStatementException("Incorrect number of fields", statement );

        String  cmd = parts[0];

        try {
            leftVal = Double.parseDouble(parts[1]);
            rightVal = Double.parseDouble(parts[2]);
        }
        catch( NumberFormatException e ) {
            throw new InvalidStatementException("Invalid #", statement, e);
        }

        setCommand(cmd);

        CalculateBase calculator = null;

        switch(command) {
            case Add:
                calculator = new Adder(leftVal, rightVal);
                break;
            case Subtract:
                calculator = new Subtracter(leftVal, rightVal);
                break;
            case Divide:
                calculator = new Divider(leftVal, rightVal);
                break;
            case Multiply:
                calculator = new Multiplier(leftVal, rightVal);
                break;
            default:
                throw new InvalidStatementException("Invalid Command", statement);
        }

        calculator.calculate();

        result = calculator.getResult();
    }

    public void setCommand(String cmd ) throws InvalidStatementException {
        if ( cmd.equalsIgnoreCase(MathCommand.Add.toString()))
            command = MathCommand.Add;
        else if ( cmd.equalsIgnoreCase(MathCommand.Subtract.toString()))
            command = MathCommand.Subtract;
        else if ( cmd.equalsIgnoreCase(MathCommand.Divide.toString()))
            command = MathCommand.Divide;
        else if ( cmd.equalsIgnoreCase(MathCommand.Multiply.toString()))
            command = MathCommand.Multiply;
        else
            throw new InvalidStatementException( "Invalid command", cmd );
    }

    @Override
    public String toString()
    {
        // could also have used string classes format functions

        char symbol = ' ' ;

        switch (command) {
            case Add:
                symbol = ADD_SYMBOL;
                break;
            case Subtract:
                symbol = SUB_SYMBOL;
                break;
            case Divide:
                symbol = DIV_SYMBOL;
                break;
            case Multiply:
                symbol = MULT_SYMBOL;
                break;

        }

        StringBuilder sb = new StringBuilder(20);

        sb.append(leftVal);
        sb.append( ' ');
        sb.append(symbol);
        sb.append( ' ' );
        sb.append(rightVal);
        sb.append( " = " );
        sb.append(result);
        return sb.toString();




    }
}
