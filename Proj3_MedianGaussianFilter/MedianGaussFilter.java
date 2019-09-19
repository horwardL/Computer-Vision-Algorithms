//implmenting 2D 3*3 Median filter and 2D 5x5 Gaussian filter using given template

import java.io.BufferedWriter;
import java.io.File;
import java.io.FileWriter;
import java.io.IOException;
import java.io.PrintWriter;
import java.util.Scanner;

public class MedianGaussFilter {
    public static void main(String[] args) {
        try {
            Scanner inFile1 = new Scanner(new File(args[0]));
            Scanner inFile2 = new Scanner(new File(args[1]));
            PrintWriter outFile1 = new PrintWriter(new BufferedWriter(new FileWriter(new File(args[2]))));
            PrintWriter outFile2 = new PrintWriter(new BufferedWriter(new FileWriter(new File(args[3]))));
            Filter medianGaussFilter = new Filter(inFile1, inFile2);
            medianGaussFilter.loadImage(inFile1);
            medianGaussFilter.loadTemplate(inFile2);
            medianGaussFilter.medianFilter();
            medianGaussFilter.printMedianAry(outFile1);
            medianGaussFilter.gaussianFilter();
            medianGaussFilter.printGaussianAry(outFile2);
            inFile1.close();
            inFile2.close();
            outFile1.close();
            outFile2.close();
        } catch (IOException e) {
            System.out.printf("ERROR: %s", e);
        }
    }
}
