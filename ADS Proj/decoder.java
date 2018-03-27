import java.io.BufferedInputStream;
import java.io.BufferedReader;
import java.io.BufferedWriter;
import java.io.File;
import java.io.FileInputStream;
import java.io.FileReader;
import java.io.FileWriter;
import java.io.IOException;
import java.util.HashMap;

public class decoder {
	public static HashMap<String,String> huffman_table= new HashMap<String,String>();  //read code table and record huffman codes as key
	public static BufferedReader buf;
	public static String code="";
	public static FileWriter fw;
	public static String Encodedfilepath;
	public static String CodeTabfilepath;
	public static String Decodedfilepath;
	public static byte bin;
	public static int bytecount;
	public static StringBuilder sb;
	public static String BinString="";
	public static int BinStrVal;
	public static int minCodeLen=99; //records the minimal code length
	
	public static void crea_huffman_table() throws Exception{
		long start_time;
		long elapsed_time;
		
		start_time = System.currentTimeMillis();
		File file = new File(CodeTabfilepath);   //location of the sample input
		buf= new BufferedReader(new FileReader(file));
		String huffman_codes = null;
		while((huffman_codes=buf.readLine())!=null)
		{
			String [] arr = huffman_codes.split(" ");
			if(arr[1].length()<minCodeLen)
			{
				minCodeLen=arr[1].length();
			}
			huffman_table.put(arr[1],arr[0]);			
		}
		buf.close();
		elapsed_time = (System.currentTimeMillis() - start_time) * 1000;
		//System.out.println("Time creating huffman table (microsecond):  "+elapsed_time);
		//System.out.println("Minimal Code Length is:  "+minCodeLen);
		//System.out.println("Code 001 represents :  "+huffman_table.get("001"));
	}
	
	public static void huffman_decoding() throws Exception{
		long start_time;
		long elapsed_time;		
		
		BinaryInput input = new BinaryInput(Encodedfilepath);
		String code = "";
		start_time = System.currentTimeMillis();
		
		File output = new File(Decodedfilepath);
		BufferedWriter writer = new BufferedWriter(new FileWriter(output));
        StringBuilder outputString= new StringBuilder("");
		//fw = new FileWriter("D://decoded1.txt");
        // While there is still more to read
        // Read bits and add that to the code
        while(!input.isEmpty()) {
            boolean bit = input.readBit();            
            if (bit)
            {
            	code += "1";
            }                
            else
            {
            	code += "0";
            }
            // Attempt to match the code in the hashmap if it is write the code
            if(code.length()>=minCodeLen)
            {
            	if (huffman_table.containsKey(code)) {
                    writer.write(huffman_table.get(code)+"\n");
                    outputString.append(code);
                    //System.out.println("Huffman code is: "+ code + " ,code is: " + huffman_table.get(code));
                    code = "";
                    //System.out.println("Encoded code is: "+ outputString);
                }
            }
            
        }
        //fw.write(outputString.toString());
        //fw.close();         
        
        input.input.close();
        writer.flush();
        writer.close();
		
		elapsed_time = (System.currentTimeMillis() - start_time) * 1000;
		System.out.println("Time creating huffman table (microsecond):  "+elapsed_time);
		//System.out.println("Minimal Code Length is:  "+minCodeLen);
	}
	
	static class BinaryInput {
	    private BufferedInputStream input;
	    private int byteToRead;
	    private int bitsRemains;
	    
	    /**
	     * Constructor
	     * @param inputFile the input file
	     */
	    public BinaryInput(String inputFile) {
	        File file = new File(inputFile);
	        
	        try {
	            if (!file.exists()) {
	                System.out.println("Input file does not exist");
	                System.exit(0);
	            }

	            this.input = new BufferedInputStream(new FileInputStream(file));
	            fillBits();
	        }
	        catch (IOException e) {
	            System.out.println("Problem creating file to write to");
	        } 
	    }
	    
	    /**
	     * Reads a single character from the file
	     */
	    private void fillBits() {
	        try {
	            this.byteToRead = this.input.read();
	            this.bitsRemains = 8;
	        }
	        catch (IOException e) {
	            this.byteToRead = -1;
	            this.bitsRemains = -1;
	        }
	    }
	    
	    /**
	     * Reads a single bit a time and if there are no more bits it fills again
	     * @return true or false depending on the bit
	     */
	    public boolean readBit() {
	        this.bitsRemains--;
	        
	        boolean bit = ((this.byteToRead >> this.bitsRemains) & 1) == 1;
	        
	        if (this.bitsRemains == 0) {
	            fillBits();
	        }
	        
	        return bit;
	    }
	    
	    /**
	     * @return returns if there is anything left to be read from the file
	     */
	    public boolean isEmpty() {
	        return this.byteToRead == -1;
	    }
	}


	public static void main(String[] args) throws Exception {
		long start_time;
		long elapsed_time;
		start_time = System.currentTimeMillis();
		
		Encodedfilepath = args[0];
		CodeTabfilepath = args[1];
		//Encodedfilepath = "D://encoded.bin";
		//CodeTabfilepath = "D://code_table.txt";
		Decodedfilepath = "decoded.txt";
		
		crea_huffman_table();
		huffman_decoding();
		
		elapsed_time = (System.currentTimeMillis() - start_time) * 1000;
		System.out.println("Time Decoding is (microsecond):  "+elapsed_time);
	}
}
