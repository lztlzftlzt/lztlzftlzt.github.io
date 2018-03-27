import java.io.BufferedOutputStream;
import java.io.BufferedReader;
import java.io.DataOutputStream;
import java.io.File;
import java.io.FileNotFoundException;
import java.io.FileOutputStream;
import java.io.FileReader;
import java.io.FileWriter;
import java.io.IOException;
import java.io.OutputStream;
import java.io.OutputStreamWriter;
import java.io.UnsupportedEncodingException;
import java.util.HashMap;
import java.util.Vector;

public class encoder {
	public static HashMap<String,Integer> freq_table= new HashMap<String,Integer>();
	public static HashMap<String,String> huffman_table= new HashMap<String,String>();
	public static BufferedReader buf;
	public static binary_heap binheap;
	public static pairing_heap pinheap;
	public static four_way_heap fourwayheap;
	public static String code="";
	public static FileWriter fw;
	public static String filepath;
	public static byte bin;
	public static int bytecount;
	public static StringBuilder sb;
	public static String BinString="";
	public static int BinStrVal;
	public static int minCodeLen=99; //records the minimal code length
	public static int maxCodeLen=99; //records the maximum code length
	public static HeapNode root; //store the root of huffman tree
	
	public static void crea_freq_table() throws Exception{
		long start_time;
		long elapsed_time;		
		
		start_time = System.currentTimeMillis();
		File file = new File(filepath);   //location of the sample input
		//File file = new File(filepath);   //location of the sample input
		buf= new BufferedReader(new FileReader(file));
		String nodes = null;
		while((nodes=buf.readLine())!=null)
		{
			if(freq_table.get(nodes) == null)
			{
				freq_table.put(nodes,1);
			}
			else
			{
				int freq = freq_table.get(nodes);
				freq++;
				freq_table.put(nodes,freq);
			}			
		}
		buf.close();
		elapsed_time = (System.currentTimeMillis() - start_time) * 1000;
		System.out.println("Time creating frequency table (microsecond):  "+elapsed_time);
	}
	
	public static void build_tree_using_binary_heap(HashMap<String,Integer> freqtab) throws Exception{
		binheap=new binary_heap(freqtab);
		//pinheap = new pairing_heap(freqtab);
		//fourwayheap = new four_way_heap(freqtab);
		root = binheap.Huffman_Tree_root;
	}
	
	public static void Travese(HeapNode n) throws IOException
	{
		if(n.left == null && n.right == null)
		{	
			huffman_table.put(n.data,code);
			fw.write(n.data+" "+code+"\n");
			//if(code.length()<minCodeLen)
			//{
			//	minCodeLen=code.length();
			//}
			fw.flush();
			//System.out.println("Node is: " + n.data + " Frequency is: " + n.frequency);
			return;
		}
		if(n.left != null)
		{
			code += "0";
			Travese(n.left);
		}
		code = code.substring(0, code.length()-1);
		if(n.right != null)
		{
			code += "1";
			Travese(n.right);
		}
		code = code.substring(0, code.length()-1);
	}
	
	public static void huffman_encoding(HeapNode root) throws Exception{
		System.out.println("Huffman Tree Root is: " + root.data +", Frequency is: " + root.frequency);
		//create code table
		fw = new FileWriter("code_table.txt");
		Travese(root);
		fw.close();
		
		//create encoded.bin
		//sb = new StringBuilder ("");		
		File file = new File(filepath);   //location of the sample input
		buf= new BufferedReader(new FileReader(file));
		
		
		BinStrVal=0;
		String key= "";	
		DataOutputStream Encoded_Bin_out= new DataOutputStream(new BufferedOutputStream(new FileOutputStream("encoded.bin")));

		while((key=buf.readLine())!=null)
		{
			long start_time;
			long elapsed_time;
			start_time = System.currentTimeMillis();
			
			//sb.append(huffman_table.get(key));
			BinString+=huffman_table.get(key);
			while(BinString.length()>8)
			{
				String substring = BinString.substring(0, 8);
				BinString = BinString.substring(8, BinString.length());

				//BinStrVal = Integer.valueOf(substring, 2);
				//bin = (byte)BinStrVal;
				bin = Integer.valueOf(substring, 2).byteValue();
				Encoded_Bin_out.writeByte(bin);
				//fw.write(bin);
				//fw.flush();			
			}
			
			elapsed_time = (System.currentTimeMillis() - start_time) * 1000;
			//System.out.println("Time Generating Bin is (microsecond):  "+elapsed_time);
		}
		//add "0" at the end of the string
		if(BinString.length()!=0)
		{
			for(int i=0; i<(8-BinString.length()); i++)
			{
				BinString+="0";
			}			
		}
		//BinStrVal = Integer.valueOf(BinString, 2);
		//bin = (byte)BinStrVal;
		bin = Integer.valueOf(BinString, 2).byteValue();
		Encoded_Bin_out.writeByte(bin);
		//fw.write(bin);
		//fw.flush();
		Encoded_Bin_out.close();
		buf.close();
		//fw.close();
		
		//fw2.write(sb.toString());
		//fw2.close();
		
	}

	public static void main(String[] args) throws Exception {
		long start_time;
		long elapsed_time;
		start_time = System.currentTimeMillis();
		
		filepath = args[0];
		//filepath = "D://sample2//sample_input_large.txt";
		crea_freq_table();
		build_tree_using_binary_heap(freq_table);
		huffman_encoding(root);
		//System.out.println("Minimal Code Length is:  "+minCodeLen);
		elapsed_time = (System.currentTimeMillis() - start_time) * 1000;
		System.out.println("Time Encoding is (microsecond):  "+elapsed_time);
	}
	
}
