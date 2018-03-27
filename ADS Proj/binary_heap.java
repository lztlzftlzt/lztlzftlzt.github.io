import java.io.*;
import java.time.Clock;
import java.util.ArrayList;
import java.util.Date;
import java.util.HashMap;
import java.util.Vector;

public class binary_heap {
	public HeapNode Huffman_Tree_root;
	private int TabtSize; 
	int currsize;    //current size of heap
	public HeapNode[] nodes;
	public HeapNode[] Heapnodes;
	
	public HeapNode getRoot() {
		return Huffman_Tree_root;
	}
	
	public void setRoot(HeapNode root) {
	this.Huffman_Tree_root = root;
	}

	public binary_heap(HashMap<String,Integer> freqtab) throws Exception {
		Huffman_Tree_root = new HeapNode();
		TabtSize = freqtab.size();
		currsize = 0;
		nodes = new HeapNode[TabtSize];
		Heapnodes = new HeapNode[TabtSize];
		//System.out.println("Number of Nodes is: " + nodes.length);
		
		//initialize each node
		for(int j=0;j<TabtSize;j++)
		{
			nodes[j] = new HeapNode();
			Heapnodes[j] = new HeapNode();
		}
		
		int i = 0;
		for (String key : freqtab.keySet()) {		
			nodes[i].data = key;
			nodes[i].frequency = freqtab.get(key);
			//System.out.println("Nodes is: " + nodes[i].data + ", frequency is: " + nodes[i].frequency);
			i++;
		}
		build_binary_heap();
		//printHeap();
		build_huffman_tree();
		//printHeap();
	}

	public void build_binary_heap() {		
		for (int i = 0; i < TabtSize; i++) {
			insert(nodes[i]);
		}
		//System.out.println("CurrSize of Heap is: " + currsize);
	}
	
	public void build_huffman_tree() throws Exception {		
		while (currsize>1) {
			HeapNode min1 = removeMin();
			HeapNode min2 = removeMin();
			HeapNode parent = new HeapNode();
			parent.sum(min1, min2);
			//System.out.println("Node1: " + min1.data + " frequency: " + min1.frequency + " merge with Node2: " + min2.data + " frequency: " + min2.frequency);
			insert(parent);
		}
		//System.out.println("CurrSize of Heap is: " + currsize);
		Huffman_Tree_root = Heapnodes[0];
		//System.out.println("Huffman Tree Root is: " + Huffman_Tree_root.data +", Frequency is: " + Huffman_Tree_root.frequency);
	}

	private void Downward_Swap(int hole) {
		int child;
		HeapNode tmp = Heapnodes[hole];
		for (; hole * 2 + 1 < currsize; hole = child) {
			child = hole * 2 + 1;
			if (child < (currsize-1) && Heapnodes[child + 1].compareTo(Heapnodes[child]) < 0) 
			{
				child++;
			}
			if (Heapnodes[child].compareTo(tmp) < 0) 
			{
				Heapnodes[hole] = Heapnodes[child];
			} 
			else 
			{
				break;
			}
		}
		Heapnodes[hole] = tmp;
	}

	public void insert(HeapNode x) {
		if (isFull()) {
			System.out.println("Enlarge array!");
			enlargeArray(Heapnodes.length * 2 + 1);
		}
		int hole = currsize;  //insert a new node at the tail of the heap
		//System.out.println("Hole is: "+ hole +", frequency is: " + x.frequency);
		currsize++;
	
		//moving up, exchange with parents until reach root
		while(hole > 0 && x.compareTo(Heapnodes[(hole-1)/2])<= 0)
		{
			Heapnodes[hole] = Heapnodes[(hole-1) / 2];
			//System.out.println("Nodes " + hole + ", swap with Node: " + (hole-1) / 2);
			hole = (hole-1) / 2;
		}
		Heapnodes[hole] = x;		
	}

	public boolean isFull() {
		return currsize == Heapnodes.length;
	}

	public boolean isEmpty() {
		return currsize == 0;
	}

	public HeapNode findMin() {
		if (isEmpty())
			return null;
		return Heapnodes[0];
	}

	public HeapNode removeMin() throws Exception {
		if (isEmpty()) {
			throw new Exception();
		}
		HeapNode minItem = findMin();
		currsize--;
		//move the last node in the queue to root
		Heapnodes[0] = Heapnodes[currsize];
		Heapnodes[currsize] = new HeapNode();
		
		//move new root downward
		Downward_Swap(0);
		return minItem;
	}

	@SuppressWarnings("unchecked")
	private void enlargeArray(int newSize) {
		HeapNode[] old = Heapnodes;
		Heapnodes = new HeapNode[newSize];
		for (int j = 0; j < Heapnodes.length; j++) {
			Heapnodes[j] = new HeapNode();
		}
		
		for (int i = 0; i < old.length; i++) {
			Heapnodes[i] = old[i];
		}
	}

	public void printHeap() {
		for (int i = 0; i<currsize; i++) {
			//System.out.println("BinHeap Node is: " + Heapnodes[i].data +", Frequency is: " + Heapnodes[i].frequency);
		}
	}

}