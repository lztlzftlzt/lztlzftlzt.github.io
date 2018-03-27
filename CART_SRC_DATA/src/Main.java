import java.util.Scanner;
import java.io.File;
import java.io.IOException;
import java.util.ArrayList;
import java.util.HashMap;

public class Main {
    public static void main(String[] args){
        String filePath = "ProjectData.txt";
        //String filePath = "C:\\ProjectData.txt";
        Cart tool = new Cart(filePath);
        long starttime = System.currentTimeMillis();
        tool.startBuildingTree();
        long afterbuildtree = System.currentTimeMillis();

        tool.accuracy_test();
        long afterclassi = System.currentTimeMillis();
        long t1 = afterbuildtree - starttime;
        long t2 = afterclassi - afterbuildtree;
        System.out.println("Build decision tree cost: "+ t1 + "milliseconds");
        System.out.println("Classification cost: "+ t2 + "milliseconds");
    }
}

class Cart {

    private int numrows = 9328622; //number of rows in data
    private int startLoadIndex = 0;
    private int numvalidrows   = 1000000;  //number of valid rows to read. startLoadIndex + numvalidrows should be less
                                          // than or equal to 9075358, better less than 5000000
    private int attrNum;
    private String filePath;
    private int NumPartition=4;

    private Float[][] data= new Float[numvalidrows][16];

    private String[] attrNames;

    private HashMap<String, Float[]> attrValue;

    private ArrayList<String> validAttr = new ArrayList<String>();
    private ArrayList<String> remainAttr = new ArrayList<>();

    private String[] OWNTYPE = new String[numvalidrows];
    private String[] luGen = new String[numvalidrows];

    private ArrayList<String> lugenTypes = new ArrayList<>();
    public Float[] valueTypes = new Float[NumPartition];
    AttrNode rootNode = new AttrNode();

    public Cart(String filePath) {
        this.filePath = filePath;
        attrValue = new HashMap<>();
    }

    public void startBuildingTree() {
        readDataFile();
        initAttrValue();

        System.out.println(attrNum);

        rootNode = new AttrNode();
        Integer[] dataindex = new Integer[data.length];

        for(int i = 0; i<data.length; i++)
        {
            dataindex[i]=i;
        }

        buildDecisionTree(rootNode, "", data, remainAttr, false);
    }

    public void readDataFile() {
        File file = new File(filePath);

        try {
            Scanner sc = new Scanner(file);
            String str;
            int rowcount=0;
            String[] tempArray;

            str = sc.nextLine();
            attrNames = str.split(",");
            attrNum=attrNames.length;

            while (sc.hasNextLine()) {
                str = sc.nextLine();

                if(str.contains("NA")||str.contains(",,"))
                {

                }
                else
                {
                    tempArray = str.split(",");
                    int ind = rowcount-startLoadIndex;
                    if((rowcount>=startLoadIndex)&&(ind<numvalidrows))
                    {
                        AddAttr(tempArray,ind);
                        System.out.println(rowcount);
                    }
                    rowcount++;
                }
            }
            sc.close();
            System.out.println("Finished reading data");
            System.out.println(rowcount);
        } catch (IOException e) {
            e.getStackTrace();
        }
    }

    public void AddAttr(String[] tmp, int index){
        data[index][0] = ((float) index);
        data[index][1]=(Float.parseFloat(tmp[1]));
        data[index][2]=(Float.parseFloat(tmp[2]));
        data[index][3]=(Float.parseFloat(tmp[3]));
        data[index][4]=(Float.parseFloat(tmp[4]));
        data[index][5]=(Float.parseFloat(tmp[5]));
        data[index][6]=(Float.parseFloat(tmp[6]));
        data[index][7]=(Float.parseFloat(tmp[7]));
        data[index][8]=(Float.parseFloat(tmp[8]));
        data[index][9]=(Float.parseFloat(tmp[9]));
        data[index][10]=(Float.parseFloat(tmp[10]));
        OWNTYPE[index]=(tmp[11]);
        data[index][11]=(Float.parseFloat(tmp[12]));
        luGen[index]=(tmp[13]);
        if(!lugenTypes.contains(tmp[13]))
        {
            lugenTypes.add(tmp[13]);
        }
        data[index][12]=(Float.parseFloat(tmp[14]));
        if(tmp[15].equals("Y"))
        {
            data[index][13]=1.0f;
        }
        else
        {
            data[index][13]=0.0f;
        }
        data[index][14]=(Float.parseFloat(tmp[17]));
        data[index][15]=(Float.parseFloat(tmp[18]));
    }

    public void initAttrValue() {
        validAttr.add(attrNames[0]);
        validAttr.add(attrNames[1]);
        validAttr.add(attrNames[2]);
        validAttr.add(attrNames[3]);
        validAttr.add(attrNames[4]);
        validAttr.add(attrNames[5]);
        validAttr.add(attrNames[6]);
        validAttr.add(attrNames[7]);
        validAttr.add(attrNames[8]);
        validAttr.add(attrNames[9]);
        validAttr.add(attrNames[10]);
        validAttr.add(attrNames[12]);
        validAttr.add(attrNames[14]);
        validAttr.add(attrNames[15]);
        validAttr.add(attrNames[17]);
        validAttr.add(attrNames[18]);

        remainAttr.add(attrNames[1]);
        remainAttr.add(attrNames[2]);
        remainAttr.add(attrNames[3]);
        remainAttr.add(attrNames[4]);
        remainAttr.add(attrNames[5]);
        remainAttr.add(attrNames[6]);
        remainAttr.add(attrNames[7]);
        remainAttr.add(attrNames[8]);
        remainAttr.add(attrNames[9]);
        remainAttr.add(attrNames[10]);
        remainAttr.add(attrNames[12]);
        remainAttr.add(attrNames[14]);
        remainAttr.add(attrNames[15]);
        remainAttr.add(attrNames[17]);
        remainAttr.add(attrNames[18]);

        Float[] tempValues;

        float max=0,min=0;

        for (int j = 1; j < validAttr.size(); j++) {
            tempValues = new Float[NumPartition];
            max=data[0][j];
            min=data[0][j];
            for (int i = 0; i < data.length; i++) {
                if (max<data[i][j]) {
                    max=data[i][j];
                }
                if (min>data[i][j]) {
                    min=data[i][j];
                }
            }

            float seg = (max-min)/(NumPartition+1);
            System.out.print(validAttr.get(j)+": ");
            for(int k = 1; k<=NumPartition; k++) {
                float temp = min + seg * k;
                System.out.print(temp+" ");
                tempValues[k-1]=temp;
            }
            System.out.println("");
            attrValue.put(validAttr.get(j), tempValues);
        }

    }

    public double computeGini(Float[][] remainData, String attrName,
                              Float value, boolean beLongValue) {
        Integer total = 0;
        Float gini = 1.0f;

        Integer[] GenCount= new Integer[lugenTypes.size()];
        for(int l=0;l<GenCount.length;l++)
        {
            GenCount[l]=0;
        }

        for (int j = 1; j < validAttr.size(); j++) {
            if (attrName.equals(validAttr.get(j))) {
                for (int i = 0; i < remainData.length; i++) {
                    if ((beLongValue && belongTo(remainData[i][j],value))
                            || (!beLongValue && !belongTo(remainData[i][j],value))) {
                        for(int k=0; k<lugenTypes.size();k++)
                        {
                            if(luGen[i].equals(lugenTypes.get(k)))
                            {
                                GenCount[k]++;
                            }
                        }
                    }
                }
            }
        }

        for(int m = 0; m < GenCount.length; m++)
        {
            total+=GenCount[m];
        }
        gini = 1.0f;
        for(int m = 0; m < GenCount.length; m++)
        {
            gini -= ((float)GenCount[m]/(float)total)*((float)GenCount[m]/(float)total);
        }
        if(total==0)
        {
            gini=1.0f;
        }
        return gini;
    }

    public Float[] computeAttrGini(Float[][] remainData, String attrName) {
        Float[] splitInfo = new Float[2];
        Float spiltValue = 0.0f;
        int tempNum = 0;
        float minGini = Integer.MAX_VALUE;

        //System.out.println(attrName);
        valueTypes = attrValue.get(attrName);
        HashMap<Float, Integer> belongNum = new HashMap<>();

        for (int k=0;k<NumPartition;k++) {
            tempNum = 0;
            for (int j = 1; j < validAttr.size(); j++) {
                if (attrName.equals(validAttr.get(j))) {
                    for (int i = 0; i < remainData.length; i++) {
                        if (belongTo(remainData[i][j],valueTypes[k])) {
                            tempNum++;
                        }
                    }
                }
            }
            belongNum.put(valueTypes[k], tempNum);
        }

        float tempGini = 0.0f;
        float posProbably = 1.0f;
        float negProbably = 1.0f;
        for (int k=0;k<valueTypes.length;k++) {
            tempGini = 0;

            posProbably = 1.0f * (float)belongNum.get(valueTypes[k]) / (float)(remainData.length);
            negProbably = 1.0f - posProbably;

            tempGini += posProbably
                    * computeGini(remainData, attrName, valueTypes[k], true);
            tempGini += negProbably
                    * computeGini(remainData, attrName, valueTypes[k], false);

            if (tempGini < minGini) {
                minGini = tempGini;
                spiltValue = valueTypes[k];
            }
        }

        splitInfo[0] = spiltValue;
        splitInfo[1] = minGini;
        return splitInfo;
    }

    public void buildDecisionTree(AttrNode node, String parentAttrValue,
                                  Float[][] remainData, ArrayList<String> remainAttr,
                                  boolean beLongParentValue) {
        String SplitValue = "";
        String spiltAttrName = "";
        double minGini = Integer.MAX_VALUE;
        double tempGini= Integer.MAX_VALUE;
        Float[] giniArray;

        if(parentAttrValue!="")
        {
            if (beLongParentValue) {
                node.setParentAttrValue("GE" + parentAttrValue);
                node.setsplitInfo(Float.parseFloat(parentAttrValue));
            } else {
                node.setParentAttrValue("LT" + parentAttrValue);
                node.setsplitInfo(Float.parseFloat(parentAttrValue));
            }
        }
        else
        {
            node.setParentAttrValue("Root");
        }

        if (remainAttr.size() == 0) {
            if (remainData.length >= 1) {
                ArrayList<String> indexArray = new ArrayList<>();
                Integer[] LeafGenCount= new Integer[lugenTypes.size()];
                for(int i=0; i<LeafGenCount.length; i++)
                {
                    LeafGenCount[i]=0;
                }

                for (int i = 0; i < remainData.length; i++) {
                    indexArray.add(remainData[i][0].toString());
                    //count Gen of remaining data
                    for(int k=0; k<lugenTypes.size();k++)
                    {
                        if(luGen[remainData[i][0].intValue()].equals(lugenTypes.get(k)))
                        {
                            LeafGenCount[k]++;
                        }
                    }
                }
                int maxGen = 0;
                String maxAttr="";
                for(int k=0; k<lugenTypes.size();k++) {
                    if(maxGen<LeafGenCount[k])
                    {
                        maxGen=LeafGenCount[k];
                        maxAttr = lugenTypes.get(k);
                    }
                }
                node.setDataIndex(indexArray);
                node.setGen(maxAttr);
            }
            System.out.println("Leaf Node"+" Node_Gen: "+node.getGen());
            return;
        }

        for(int i=0; i<remainAttr.size();i++)
        {
            giniArray = computeAttrGini(remainData, remainAttr.get(i));
            tempGini = (double)(giniArray[1]);

            if (tempGini < minGini) {
                spiltAttrName = remainAttr.get(i);
                minGini = tempGini;
                SplitValue = giniArray[0].toString();
            }
        }

        node.setsplitInfo(Float.parseFloat(SplitValue));

        remainAttr.remove(spiltAttrName);
        node.setAttrName(spiltAttrName);

        AttrNode[] childNode = new AttrNode[2];
        Float[][] rData;

        boolean[] bArray = new boolean[] { true, false };
        for (int i = 0; i < bArray.length; i++) {
            rData = removeData(remainData, spiltAttrName, SplitValue, bArray[i]);

            boolean sameClass = true;
            for (int k = 0; k < rData.length; k++) {
                int ind1= rData[k][0].intValue();
                int ind2= rData[0][0].intValue();
                if (!luGen[ind1]
                        .equals(luGen[ind2])) {
                    sameClass = false;
                    break;
                }
            }

            childNode[i] = new AttrNode();
            if(remainAttr!=null)
            {
                if (!sameClass) {
                    ArrayList<String> rAttr = new ArrayList<>();
                    for (String str : remainAttr) {
                        rAttr.add(str);
                    }
                    buildDecisionTree(childNode[i], SplitValue, rData, rAttr,
                            bArray[i]);
                } else {
                    String pAtr = (bArray[i] ? "GT"+SplitValue : "LT" + SplitValue);
                    childNode[i].setParentAttrValue(pAtr);
                }
            }

        }
        node.setChildAttrNode(childNode);
    }

    private Float[][] removeData(Float[][] srcData, String attrName,
                                 String SplitValue, boolean beLongValue) {
        Float[][] desDataArray;
        ArrayList<Float[]> desData = new ArrayList<>();

        ArrayList<Float[]> selectData = new ArrayList<>();

        for (int i = 0; i < srcData.length; i++) {
            desData.add(srcData[i]);
        }

        for (int j = 1; j <validAttr.size(); j++) {
            if (validAttr.get(j).equals(attrName)) {
                for (int i = 0; i < desData.size(); i++) {
                    if (belongTo(desData.get(i)[j], Float.parseFloat(SplitValue))) {
                        selectData.add(desData.get(i));
                    }
                }
            }
        }

        if (beLongValue) {
            desDataArray = new Float[selectData.size()][];
            selectData.toArray(desDataArray);
        } else {
            desData.removeAll(selectData);
            desDataArray = new Float[desData.size()][];
            desData.toArray(desDataArray);
        }
        return desDataArray;
    }

    public boolean belongTo(float data, float splitvalue)
    {
        if(data>=splitvalue)
        {
            return true;
        }
        else
        {
            return false;
        }
    }

    public void accuracy_test()
    {
        Float accuracy = 0.0f;
        Integer matched = 0;
        AttrNode pivot = rootNode;
        int startIndex = 0;  //search start index
        int endIndex = data.length;    //search end index. should be less than numvalidrows
        int searchlength = endIndex-startIndex;

        for(int i = startIndex; i< endIndex; i++)
        {
            pivot = rootNode;
            while(pivot.getChildAttrNode()!=null)
            {
                String attrname = pivot.getAttrName();
                for(int j=1; j<data[0].length; j++)
                {
                    if(validAttr.get(j).equals(attrname))
                    {
                        //System.out.println(pivot.getsplitInfo());
                        if(belongTo(data[i][j],pivot.getsplitInfo()))
                        {
                            pivot = pivot.getChildAttrNode()[0];
                        }
                        else
                        {
                            pivot = pivot.getChildAttrNode()[1];
                        }
                    }
                }
            }
            if(luGen[i].equals(pivot.getGen()))
            {
                matched++;
            }
        }
        accuracy = (float)matched/(float)searchlength;
        System.out.println("Accuracy is: "+accuracy);

    }

}

class AttrNode {
    // Node Attribute Name
    private String attrName;
    // Node Index Name
    private int nodeIndex;
    // Parent Attribute Value
    private String parentAttrValue;
    // Child Nodes
    private AttrNode[] childAttrNode;
    // Data Record Index
    private ArrayList<String> dataIndex;
    // Node luGen record
    private String Category = "";
    //SplitValue Info
    private Float splitInfo=0.0f;

    public void setParentAttrValue(String pav)
    {
        this.parentAttrValue = pav;
    }
    public void setDataIndex(ArrayList<String> dind)
    {
        this.dataIndex = dind;
    }
    public void setAttrName(String atN)
    {
        this.attrName = atN;
    }
    public void setChildAttrNode(AttrNode[] ch)
    {
        this.childAttrNode = ch;
    }
    public void setNodeIndex(int nind)
    {
        this.nodeIndex = nind;
    }
    public void setsplitInfo(Float spInfo)
    {
        this.splitInfo = spInfo;
    }
    public void setGen(String Cate)
    {
        this.Category = Cate;
    }
    public String getParentAttrValue()
    {
        return this.parentAttrValue;
    }
    public ArrayList<String> getDataIndex()
    {
        return this.dataIndex;
    }
    public int getNodeIndex()
    {
        return this.nodeIndex;
    }
    public String getAttrName()
    {
        return this.attrName;
    }
    public AttrNode[] getChildAttrNode()
    {
        return this.childAttrNode;
    }
    public String getGen()
    {
        return this.Category;
    }
    public Float getsplitInfo()
    {
        return this.splitInfo;
    }
}
