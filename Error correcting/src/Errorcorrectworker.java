import java.io.BufferedReader;
import java.io.FileInputStream;
import java.io.FileReader;
import java.io.IOException;
import java.io.ObjectInputStream;
import java.io.PrintWriter;
import java.util.HashMap;
public class Errorcorrectworker {
    private static String readFileAsString(String filePath)
    throws java.io.IOException{
        StringBuffer fileData = new StringBuffer(1000);
        BufferedReader reader = new BufferedReader(
                new FileReader(filePath));
        char[] buf = new char[1024];
        int numRead=0;
        while((numRead=reader.read(buf)) != -1){
            String readData = String.valueOf(buf, 0, numRead);
            fileData.append(readData);
            buf = new char[1024];
        }
        reader.close();
        return fileData.toString();
    }
	public static void main(String[] args) throws ClassNotFoundException, IOException {
		System.out.print("Loading HashMap from tblank.tmp\n");
		HashMap<String, Double > wmhash = new HashMap<String, Double >(450000);
		ObjectInputStream ois;
		FileInputStream fis;
		fis= new FileInputStream("tblank.tmp");
		ois= new ObjectInputStream(fis);
		wmhash=(HashMap<String, Double >)ois.readObject();
		String alfabe= new String("ABC›DEFG!HI$JKLMNO#PRS@TU\"VYZ");
		String metin = readFileAsString("bozukyazi.txt");
		StringBuilder met=new StringBuilder(metin);
		System.out.print("hashmap loaded\n");
		int xcount=0;
		for (int i = 0; i < met.length(); i++) {
			if(met.charAt(i)=='x'){
				xcount++;
				StringBuilder left=new StringBuilder("");
				StringBuilder right=new StringBuilder("");
				StringBuilder left1=new StringBuilder("");
				for (int j = i+1; j < i+5; j++) {
					if(!Character.isWhitespace(met.charAt(j))){
						right.append(met.charAt(j));
					}
					else{
						break;
					}
				}
				for (int j = i-1; j > i-5; j--) {
					if(!Character.isWhitespace(met.charAt(j))){
						left1.append(met.charAt(j));
					}
					else{
						break;
					}
				}
				char leftbest[]=new char[10];
				char rightbest[]=new char[10];
				Double []leftbestval={0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0};
				Double []rightbestval={0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0};	
				if (left1.length()>0){
					for(int j = left1.length() -1 ; j >= 0 ; j--)
					{
						left.append(left1.charAt(j)) ;
					}
					for (int j=0;j<alfabe.length();j++){
						Double a=0.0;
						if(wmhash.containsKey(left.toString() + alfabe.charAt(j))){
							a=wmhash.get(left.toString() + alfabe.charAt(j));
							for (int m = 0; m < 10; m++) {
								if (a > leftbestval[m]) {
									double stack = leftbestval[m];
									char stackch = leftbest[m];
									leftbestval[m] = a;
									leftbest[m] = alfabe.charAt(j);
									for (int k = m+1; k < 10; k++) {
										if (stack > leftbestval[k]) {
											double stack2 = leftbestval[k];
											char stackch2 = leftbest[k];
											leftbestval[k]=stack;
											leftbest[k]=stackch;
											stack=stack2;
											stackch=stackch2;
										}
									}
									break;
								}
							}
						}
					}
				}
				if (right.length()>0){
					for (int j=0;j<alfabe.length();j++){
						Double a=0.0;
						if(wmhash.containsKey(alfabe.charAt(j)+right.toString())){
							a=wmhash.get(alfabe.charAt(j)+right.toString());
							for (int m = 0; m < 10; m++) {
								if (a > rightbestval[m]) {
									double stack = rightbestval[m];
									char stackch = rightbest[m];
									rightbestval[m] = a;
									rightbest[m] = alfabe.charAt(j);
									for (int k = m+1; k < 10; k++) {
										if (stack > rightbestval[k]) {
											double stack2 = rightbestval[k];
											char stackch2 = rightbest[k];
											rightbestval[k]=stack;
											rightbest[k]=stackch;
											stack=stack2;
											stackch=stackch2;
										}
									}
									break;
								}
							}
						}
					}
				}
				double max=0.0;
				char maxch='.';
				if((leftbest[0]==0.0) && (rightbest[0]==0.0)){
					met.deleteCharAt(i);
					met.insert(i, 'O');
					continue;
				}
				if(leftbestval[0]==0){
					max=rightbestval[0];
					maxch=rightbest[0];
					met.deleteCharAt(i);
					met.insert(i, maxch);
					continue;
				}
				if(rightbestval[0]==0){
					max=leftbestval[0];
					maxch=leftbest[0];
					met.deleteCharAt(i);
					met.insert(i, maxch);
					continue;
				}	
				Double []best={0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0};
				char []bestch=new char[10];
				int n=0;
				for (int k = 0; k < leftbestval.length; k++) {
					for (int j = 0; j < rightbestval.length; j++) {
						if ((rightbest[j] == leftbest[k])&&rightbest[j]!=0) {
							best[n] = rightbestval[j] * leftbestval[k];
							bestch[n]=rightbest[j];
							n++;
						}
					}
				}
				for (int j = 0; j < bestch.length; j++) {
					if (best[j]!=0.0){
						StringBuilder str = new StringBuilder("");
						str.append(left);
						str.append(bestch[j]);
						str.append(right);
						str.append("    ");
						if(wmhash.containsKey(str.substring(0, 5)))
						best[j]=best[j]*wmhash.get(str.substring(0, 5));
						else best[j]=0.0;
					}
				}
				for (int k = 0; k < 10; k++) {
					if (max < best[k]) {
						max = best[k];
						maxch = bestch[k];
					}
				}
				if(maxch=='.'){
					if(left.length()>=right.length())
					{
						met.deleteCharAt(i);
						met.insert(i,leftbest[0]);
						continue;
					}
					else{
						met.deleteCharAt(i);
						met.insert(i,rightbest[0]);
						continue;
					}
				}	
				met.deleteCharAt(i);
				met.insert(i, maxch);
			}
		}
		String turkce = readFileAsString("türkçe_metin.txt");
		turkce=turkce.trim().replaceAll("\\s+", " ");
		String duz=met.toString().trim().replaceAll("\\s+", " ");
		double count=0;
		for (int j = 0; j < turkce.length(); j++) {
			if (duz.charAt(j) != turkce.charAt(j)) {
				count++;
			}
		}	
		Double yuzde=(count/xcount);
		System.out.print("x doðru tahmin yüzdesi=");
		System.out.print(100*(1-yuzde));
		PrintWriter out = new PrintWriter("duzeltme.txt");
		out.println(met.toString());
		out.close();
		System.out.print("\nduzeltme.txt dosyaya yazýldý");
		}
	}