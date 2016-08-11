import java.io.BufferedReader;
import java.io.FileOutputStream;
import java.io.FileReader;
import java.io.IOException;
import java.io.ObjectOutputStream;
import java.io.PrintWriter;
import java.util.HashMap;
public class errorcorrect2 {
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
	public static void main(String[] args) throws IOException{
		String wm2="",wm3="",wm4="",wm5="";
		String metin = null;
		HashMap<String, Double > wmhash = new HashMap<String, Double >(550000);
		PrintWriter out = new PrintWriter("bozukyazi.txt");
		metin = readFileAsString("türkçe_metin.txt");
		StringBuilder met=new StringBuilder(metin);
		for (int i = 0; i < met.length()/31; i++) {
			int no =1+ (int) (Math.random() * (28));
			if(!Character.isWhitespace(met.charAt(i*31+no))){
				met.setCharAt(i*31+no, 'x');
			}
		}
		out.println(met);
		out.close();
		wm2 = readFileAsString("2_alphabetical_with_blank.txt");
		long startTime = System.currentTimeMillis();
		String lines[] = wm2.split("\\r?\\n");
		for (int i = 6; i < lines.length-13; i++) {
			String words[]={"","","","","","","",""};
			if(!lines[i].trim().isEmpty()){
				words = lines[i].split("\\s+");
				if (words[1].length()==2) {
					wmhash.put(words[1], new Double(words[5]));
				}
			}
		}
		wm3 = readFileAsString("3_alphabetical_with_blank.txt");
		lines = wm3.split("\\r?\\n");
		for (int i = 10; i < lines.length-10; i++) {
			String words[]={"","","","",""};
			if(!lines[i].trim().isEmpty()){
				words = lines[i].split("\\s+");
				if (words[1].length()==5) {
					wmhash.put(String.valueOf(words[1].charAt(0))+String.valueOf(words[1].charAt(2))+String.valueOf(words[1].charAt(4)) , new Double(words[3]));
				}
			}
		}
		wm4 = readFileAsString("4_alphabetical_with_blank.txt");
		lines = wm4.split("\\r?\\n");
		for (int i = 13; i < lines.length-45; i++) {
			String words[]={"","","","","",""};
			if(!lines[i].trim().isEmpty()){
				words = lines[i].split("\\s+");
				if (words[1].length()==7) {
					wmhash.put(String.valueOf(words[1].charAt(0))+String.valueOf(words[1].charAt(2))+String.valueOf(words[1].charAt(4))+String.valueOf(words[1].charAt(6)) , new Double(words[3]));
				}
			}
		}
		wm5 = readFileAsString("5_alphabetical_with_blank.txt");
		lines = wm5.split("\\r?\\n");
		for (int i = 4; i < lines.length-11; i++) {
			String words[]={"","","","",""};
			if(!lines[i].trim().isEmpty()){
				words = lines[i].split("\\s+");
				if (words[0].length()==9) {
					wmhash.put(String.valueOf(words[0].charAt(0))+String.valueOf(words[0].charAt(2))+String.valueOf(words[0].charAt(4))+String.valueOf(words[0].charAt(6))+String.valueOf(words[0].charAt(8)) , new Double(words[3]));
				}
			}
		}
		long endTime   = System.currentTimeMillis();
		long totalTime = endTime - startTime;
		System.out.println(totalTime+"\n");
		System.out.println("Size of HashMap : " + wmhash.size());
		FileOutputStream fos;
		ObjectOutputStream oos;
		fos = new FileOutputStream("tblank.tmp");
		oos = new ObjectOutputStream(fos);
		oos.writeObject(wmhash);
		oos.close();
	}
}
