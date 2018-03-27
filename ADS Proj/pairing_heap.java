import java.io.*;
import java.time.Clock;
import java.util.Date;
import java.util.HashMap;
import java.util.Vector;

public class pairing_heap {
	public HeapNode Huffman_Tree_root;
	private int TabtSize; 
	int currsize;    //current size of heap
	public HeapNode[] nodes;
	public HeapNode[] Heapnodes;
	
	HeapNode min1 = new HeapNode();
	HeapNode min2 = new HeapNode();
	
	public HeapNode getRoot() {
		return Huffman_Tree_root;
	}
	
	public void setRoot(HeapNode root) {
	this.Huffman_Tree_root = root;
	}

	public pairing_heap(HashMap<String,Integer> freqtab) throws Exception {
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
		build_pairing_heap();
		//printHeap();
		build_huffman_tree();
		//printHeap();
	}

	public void build_pairing_heap() {		
		for (int i = 0; i < TabtSize; i++) {
			insert(nodes[i]);
		}
		//System.out.println("CurrSize of Heap is: " + currsize);
	}
	
	public void build_huffman_tree() throws Exception {		
		while ((min1=removeMin())!=null) {
			min2 = removeMin();
			HeapNode parent = new HeapNode();
			if(min2!=null)
			{
				parent.sum(min1, min2);
				insert(parent);
			}
			else
			{
				Huffman_Tree_root = min1;
				return;
			}			
			//System.out.println("Node1: " + min1.data + " frequency: " + min1.frequency + " merge with Node2: " + min2.data + " frequency: " + min2.frequency);
		}
		//System.out.println("CurrSize of Heap is: " + currsize);
		//Huffman_Tree_root = Heapnodes[0];
		//System.out.println("Huffman Tree Root is: " + Huffman_Tree_root.data +", Frequency is: " + Huffman_Tree_root.frequency);
	}
    
    public HeapNode insert(HeapNode x)
    {
    	currsize++;
    	HeapNode newNode =  x ;
        if (Huffman_Tree_root == null)
        	Huffman_Tree_root = newNode;
        else
        	Huffman_Tree_root = compare_Link(Huffman_Tree_root, newNode);
        return newNode;
    }
   
    private HeapNode compare_Link(HeapNode first, HeapNode second)
    {
        if (second == null)
            return first;
 
        if (second.frequency < first.frequency)
        {
            /* Attach first as leftmost child of second */
        	second.prev = first.prev;
            first.prev = second;
            first.nextSibling = second.leftChild;
            if (first.nextSibling != null)
                first.nextSibling.prev = first;
            second.leftChild = first;
            return second;
        }
        else
        {
            /* Attach second as leftmost child of first */
            second.prev = first;
            first.nextSibling = second.nextSibling;
            if (first.nextSibling != null)
                first.nextSibling.prev = first;
            second.nextSibling = first.leftChild;
            if (second.nextSibling != null)
                second.nextSibling.prev = second;
            first.leftChild = second;
            return first;
        }
    }
    private HeapNode combineSiblings(HeapNode firstSibling)
    {
        if( firstSibling.nextSibling == null )
            return firstSibling;
       
        int numSiblings = 0;
        for ( ; firstSibling != null; numSiblings++)
        {
            Heapnodes = enlargeArray( Heapnodes, numSiblings );
            Heapnodes[ numSiblings ] = firstSibling;
            
            firstSibling.prev.nextSibling = null;  
            firstSibling = firstSibling.nextSibling;
        }
        Heapnodes = enlargeArray( Heapnodes, numSiblings );
        Heapnodes[ numSiblings ] = null;
        
        int i = 0;
        for ( ; i + 1 < numSiblings; i += 2)
            Heapnodes[ i ] = compare_Link(Heapnodes[i], Heapnodes[i + 1]);
        int j = i - 2;
        
        if (j == numSiblings - 3)
            Heapnodes[ j ] = compare_Link( Heapnodes[ j ], Heapnodes[ j + 2 ] );
        
        for ( ; j >= 2; j -= 2)
            Heapnodes[j - 2] = compare_Link(Heapnodes[j-2], Heapnodes[j]);
        return Heapnodes[0];
    }
    
    private HeapNode[] enlargeArray(HeapNode [ ] array, int index)
    {
        if (index == array.length)
        {
        	HeapNode [ ] oldArray = array;
            array = new HeapNode[index * 2];
            for( int i = 0; i < index; i++ )
                array[i] = oldArray[i];
        }
        return array;
    }
    
    public HeapNode removeMin( )
    {
        if (isEmpty( ))
            return null;
        HeapNode x = Huffman_Tree_root;
        if (Huffman_Tree_root.leftChild == null)
        	Huffman_Tree_root = null;
        else
        	Huffman_Tree_root = combineSiblings(Huffman_Tree_root.leftChild );
        currsize--;
        return x;
    }
	
	public boolean isFull() {
		return currsize == Heapnodes.length;
	}

	public boolean isEmpty() {
		return Huffman_Tree_root == null;
	}

	public HeapNode findMin() {
		if (isEmpty())
			return null;
		return Heapnodes[0];
	}

	public void printHeap() {
		for (int i = 0; i<currsize; i++) {
			//System.out.println("BinHeap Node is: " + Heapnodes[i].data +", Frequency is: " + Heapnodes[i].frequency);
		}
	}
}