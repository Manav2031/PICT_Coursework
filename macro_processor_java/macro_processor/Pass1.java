package macro_processor;

import java.io.*;
import java.util.*;

public class Pass1 implements Serializable {

    class MNTabEntry implements Serializable {

        public String macroName ; 
        public int numKPD = 0 ;
        public int numPP = 0 ;
        public int mdtabPtr = 0;
        public int kpdtabPtr = 0;

        public MNTabEntry() {
        }
        
        public MNTabEntry( String macroName , int numKPD , int numPP , int mdtabPtr , int kpdtabPtr ) {
            this.macroName = macroName ; 
            this.numKPD = numKPD ; 
            this.numPP = numPP ; 
            this.mdtabPtr = mdtabPtr ; 
            this.kpdtabPtr = kpdtabPtr ; 
        }

    }

    class MDTabEntry implements Serializable {

        public String mnemonic ; 
        public String operand1 ; 
        public String operand2 ; 
        public int operand1Index = -1; 
        public int operand2Index = -1; 
    
    }

    private ArrayList<String[]> tokens = new ArrayList<>() ;
    private ArrayList<String[]> kpdtab = new ArrayList<>() ;
    private ArrayList<String> pntab = new ArrayList<>() ;  
    private ArrayList<MNTabEntry> mntab = new ArrayList<>() ; 
    private ArrayList<MDTabEntry> mdtab = new ArrayList<>() ; 
    private String currentMacroName ; 

    private HashMap<String,List<String>> pntabMap = new HashMap<>() ; 

    public Pass1( String sourceFilepath ) {
        ArrayList<String> lines = readLines( sourceFilepath ) ; 
        for( String line : lines ) {
            tokens.add( getTokens( line ) ) ; 
        }
    }

    public void perform() {
        int mdtabPtr = 0 ;
        int kpdtabPtr = 0 ;
        for( int i = 1 ; i < tokens.size() ; i++ ) { 
            String[] lineTokens = tokens.get( i ) ; 
            System.out.println( Arrays.toString( lineTokens ) ) ;
            if( tokens.get( i - 1 )[0].equals( "MACRO" ) )  {
                currentMacroName = lineTokens[ 0 ] ; 
                int numKPD = 0 ; 
                int numPP = 0 ; 
                pntab.clear();
                for( int j = 1 ; j < lineTokens.length ; j++ ) {
                    String parameter = lineTokens[ j ] ; 
                    if( parameter.contains( "=" ) ) {
                        kpdtab.add( parameter.split( "=" ) ); 
                        numKPD++ ; 
                    }
                    else {
                        numPP++ ; 
                    }
                    System.out.println( "Parameter added : " + parameter ) ; 
                    pntab.add( parameter.split("=")[0] ) ; 
                }
                mntab.add( new MNTabEntry( currentMacroName , numKPD , numPP , mdtabPtr , kpdtabPtr ) ) ;
                System.out.println( "Added to map : " + currentMacroName ) ; 
                kpdtabPtr += numKPD ; 
                pntabMap.put( currentMacroName , pntab ) ;  
            }
            else if( !lineTokens[0].equals( "MACRO" ) &&
             !lineTokens[0].equals( "MEND" ) ) {
                MDTabEntry entry = new MDTabEntry(); 
                entry.mnemonic = lineTokens[0] ;   
                List<String> currentPntab = pntabMap.get( currentMacroName ) ; 
                if( lineTokens[1].startsWith( "&" ) ) {
                    int index = currentPntab.indexOf(lineTokens[1]) ; 
                    entry.operand1 = lineTokens[1] ;
                    if( index >= 0 ) { 
                        entry.operand1Index = index ; 
                    }
                }
                if( lineTokens[2].startsWith( "&" ) ) {
                    int index = currentPntab.indexOf(lineTokens[2]) ; 
                    entry.operand2 = lineTokens[2] ;
                    if( index >= 0 ) { 
                        entry.operand2Index = index ; 
                    }
                }
                mdtab.add( entry ) ; 
            }
            else if( lineTokens[0].equals( "MEND" ) ) {
                MDTabEntry entry = new MDTabEntry(); 
                entry.mnemonic = "MEND" ;
                mdtab.add(entry) ;    
            }
            mdtabPtr++ ; 
        }

        saveTable( mntab , "mntab.dat" ) ;
        saveTable( kpdtab , "kpdtab.dat" ) ;
        saveTable( pntabMap , "pntab.dat" ) ;
        saveTable( mdtab , "mdtab.dat" ) ;

        printPNTAB();
        printMNTAB();
        printKPDTAB();
    }

    private static void saveTable( Object table , String filepath ) {
        try {
            FileOutputStream fos = new FileOutputStream( new File( filepath ) ) ; 
            ObjectOutputStream outputStream = new ObjectOutputStream(fos) ; 
            outputStream.writeObject( table );
            outputStream.flush();
            outputStream.close();
            fos.close();
        }
        catch( FileNotFoundException e ){
            e.printStackTrace();
        }
        catch( IOException e ) {
            e.printStackTrace(); 
        }
    }

    private static String[] getTokens( String line ) {
        return line.split( " " ) ; 
    }

    private static ArrayList<String> readLines( String filepath ) {
        File textFile = new File( filepath ) ;
        ArrayList<String> lines = new ArrayList<>();  
        try {
            FileInputStream inputStream = new FileInputStream( textFile ) ; 
            BufferedReader reader = new BufferedReader( new InputStreamReader( inputStream ) ) ; 
            while( reader.ready() ) {
                lines.add( reader.readLine() ) ; 
            }
            inputStream.close();
        }
        catch( FileNotFoundException e ) {
            e.printStackTrace();
        }
        catch( IOException e ) {
            e.printStackTrace();
        }
        return lines ; 
    }

    private void printMNTAB() {
        for( MNTabEntry entry : mntab ) {
            System.out.println( 
                entry.macroName + " " + 
                entry.numKPD + " " + 
                entry.numPP + " " + 
                entry.mdtabPtr + " " + 
                entry.kpdtabPtr );
        }
    }

    private void printKPDTAB() {
        for( String[] entry : kpdtab ) {
            System.out.println( entry[0] + " " + entry[1] );
        }
    }

    private void printPNTAB() {
        Iterator<String> it = pntabMap.keySet().iterator() ; 
        while( it.hasNext() ) {
            String macroName = it.next() ; 
            System.out.println( "Macro Name: " + macroName ) ; 
            List<String> params = pntabMap.get( macroName ) ; 
            for( String param : params ) {
                System.out.println( param ) ; 
            }
        }
    }

}