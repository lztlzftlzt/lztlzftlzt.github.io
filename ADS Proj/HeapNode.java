import java.io.*;
import java.time.Clock;
import java.util.Date;
import java.util.HashMap;
import java.util.Vector;

public class HeapNode {
	    public HeapNode left;   //used to construct huffman tree and binary tree
	    public HeapNode right; //used to construct huffman tree and binary tree
	    public HeapNode leftChild; //used to construct pairing tree
	    public HeapNode nextSibling; //used to construct pairing tree
	    public HeapNode prev; //used to construct pairing tree
	    public String data;
	    public int frequency;
	    
	    public HeapNode() {
	        left = null;
	        right = null;
	        leftChild=null;
	        nextSibling=null;
	        prev = null;
	        this.data = "";
	        this.frequency = 0;
	    }
	    
	    public void sum(HeapNode Lchild, HeapNode Rchild) {
	        this.left = Lchild;
	        this.right = Rchild;
	        this.frequency = left.frequency+right.frequency;
	        //this.data = left.data + "+" + right.data;
	    }
	    
	    public int compareTo(HeapNode n){
	    	return this.frequency - n.frequency;
	    }
}