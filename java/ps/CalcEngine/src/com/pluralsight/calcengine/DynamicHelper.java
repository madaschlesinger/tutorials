package com.pluralsight.calcengine;

public class DynamicHelper {
    private MathProcessing[] handlers;

    public DynamicHelper( MathProcessing[] handlers)
    {
        this.handlers = handlers;
    }

    public String process( String statement ) {
        // IN: add 1.0 2.0
        // OUT 1.0 + 2.0 = 3.0
        String stripped = statement.trim().replaceAll(" +", " ");
        String[] parts = stripped.split(MathProcessing.SEPERATOR);
        String keyword = parts[0];

        MathProcessing theHandler = null;

        for(MathProcessing h:handlers)
        {
            if(keyword.equalsIgnoreCase(h.getKeyword())) {
                theHandler = h;
                break;
            }
        }

        double lval = Double.parseDouble( parts[1]);
        double rval = Double.parseDouble((parts[2]));

        double result = theHandler.doCalculate(lval, rval);


        StringBuilder sb = new StringBuilder(20);

        sb.append(lval);
        sb.append( ' ');
        sb.append(theHandler.getSymbol());
        sb.append( ' ' );
        sb.append(rval);
        sb.append( " = " );
        sb.append(result);
        return sb.toString();


    }
}
