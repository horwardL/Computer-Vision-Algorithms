import java.io.FileOutputStream;
import java.io.FileReader;
import java.io.IOException;
import java.io.PrintWriter;
import java.util.Scanner;

public class Main {

	public static void main(String[] args) {
		Scanner inFile1, inFile2;
		PrintWriter outFile1, outFile2, outFile3, outFile4;
		BasicMorphology basicMorphology;
		try {
			inFile1 = new Scanner(new FileReader(args[0]));
			inFile2 = new Scanner(new FileReader(args[1]));
			outFile1 = new PrintWriter(new FileOutputStream(args[2]));
			outFile2 = new PrintWriter(new FileOutputStream(args[3]));
			outFile3 = new PrintWriter(new FileOutputStream(args[4]));
			outFile4 = new PrintWriter(new FileOutputStream(args[5]));

			basicMorphology = new BasicMorphology(inFile1, inFile2);
			basicMorphology.loadImage(inFile1, args[0]);
			basicMorphology.loadStruct(inFile2, args[1]);
			basicMorphology.dilationOp(outFile1, args[2]);
			basicMorphology.erosionOp(outFile2, args[3]);
			basicMorphology.closingOp(outFile3, args[4]);
			basicMorphology.openingOp(outFile4, args[5]);

			inFile1.close();
			inFile2.close();
			outFile1.close();
			outFile2.close();
			outFile3.close();
			outFile4.close();
		} catch (IOException e) {
			e.printStackTrace();
		}
	}
}
