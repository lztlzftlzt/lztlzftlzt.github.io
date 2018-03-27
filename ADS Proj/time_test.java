import java.io.*;
import java.time.Clock;
import java.util.Date;
import java.util.HashMap;
import java.util.Vector;

public class time_test {
	
	public static HashMap<String,Integer> freq_table= new HashMap<String,Integer>();
	public static BufferedReader buf;
	public static String filepath;  //location of sample input
	
	public static void crea_freq_table() throws Exception{
		long start_time;
		long elapsed_time;		
		
		start_time = System.currentTimeMillis();
		File file = new File(filepath);   //location of the sample input
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
		binary_heap binheap=new binary_heap(freqtab);
	}
	
	public static void build_tree_using_4way_heap(HashMap<String,Integer> freqtab) throws Exception{
		four_way_heap fourheap = new four_way_heap(freqtab);
	}
	
	public static void build_tree_using_pairing_heap(HashMap<String,Integer> freqtab) throws Exception{
		pairing_heap pairheap=new pairing_heap(freqtab);
	}
	
	public static void test_binary() throws Exception{
		long start_time;
		long elapsed_time;		
		
		start_time = System.currentTimeMillis();
		for(int i = 0; i < 10; i++)
		{    //run 10 times on given data set  
			build_tree_using_binary_heap(freq_table);
		}
		elapsed_time = (System.currentTimeMillis() - start_time) * 1000;
		System.out.println("Time using binary heap (microsecond):  "+elapsed_time);
	}
	
	public static void test_4_way() throws Exception{
		long start_time;
		long elapsed_time;
		
		start_time = System.currentTimeMillis();
		for(int i = 0; i < 10; i++)
		{    //run 10 times on given data set  
			build_tree_using_4way_heap(freq_table); 
		}
		elapsed_time = (System.currentTimeMillis() - start_time) * 1000;
		System.out.println("Time using 4-way heap (microsecond):  "+elapsed_time);
	}
	
	public static void test_pairing() throws Exception{
		long start_time;
		long elapsed_time;
		
		start_time = System.currentTimeMillis();
		for(int i = 0; i < 10; i++)
		{    //run 10 times on given data set  
			build_tree_using_pairing_heap(freq_table); 
		}
		elapsed_time = (System.currentTimeMillis() - start_time) * 1000;
		System.out.println("Time using pairing heap (microsecond):  "+elapsed_time);
	}
	
	public static void main(String[] args) throws Exception {
		filepath = args[0];
		//filepath = "D://sample2//sample_input_large.txt";
		crea_freq_table();
		test_binary();
		test_4_way();
		test_pairing();
	}
	
}
