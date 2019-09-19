import java.io.PrintWriter;
import java.util.Scanner;

public class Filter {
    private int numRows, numCols, minVal, maxVal, newMin, newMax, templateRows, templateCols, totalWeight;
    private int[][] imgArg;
    private int[][] gaussTemplate;
    private int[][] medianAry;
    private int[][] gaussAry;
    private int[] neighborAry;

    public Filter(Scanner inFile1, Scanner inFile2) {
        numRows = numCols = minVal = maxVal = newMin = newMax = templateRows = templateCols = totalWeight = 0;
        imgArg = gaussTemplate = medianAry = gaussAry = null;
        numRows = inFile1.nextInt();
        numCols = inFile1.nextInt();
        minVal = inFile1.nextInt();
        maxVal = inFile1.nextInt();
        templateRows = inFile2.nextInt();
        templateCols = inFile2.nextInt();
        neighborAry = new int[9];
        imgArg = new int[numRows][numCols];
        medianAry = new int[numRows][numCols];
        gaussAry = new int[numRows][numCols];
        gaussTemplate = new int[templateRows][templateCols];
        for (int i = 0; i < numRows; ++i) {
            for (int j = 0; j < numCols; ++j) {
                imgArg[i][j] = 0;
                medianAry[i][j] = 0;
                gaussAry[i][j] = 0;
            }
        }
        for (int i = 0; i < 9; ++i)
            neighborAry[i] = 0;
        for (int i = 0; i < templateRows; ++i)
            for (int j = 0; j < templateCols; ++j)
                gaussTemplate[i][j] = 0;
    }

    public void loadImage(Scanner inFile1) {
        for (int r = 0; r < numRows; ++r) {
            for (int c = 0; c < numCols; ++c) {
                imgArg[r][c] = inFile1.nextInt();
                medianAry[r][c] = imgArg[r][c];
                gaussAry[r][c] = imgArg[r][c];
            }
        }
    }

    public void loadTemplate(Scanner inFile2) {
        for (int r = 0; r < templateRows; ++r) {
            for (int c = 0; c < templateCols; ++c) {
                gaussTemplate[r][c] = inFile2.nextInt();
            }
        }
    }

    public void medianFilter() {
        newMin = minVal;
        newMax = maxVal;
        for (int r = 1; r < numRows - 1; ++r) {
            for (int c = 1; c < numCols - 1; ++c) {
                loadNeighbors(r, c);
                sort(neighborAry);
                medianAry[r][c] = neighborAry[4];
                newMin = newMin > neighborAry[4] ? neighborAry[4] : newMin;
                newMax = newMax < neighborAry[4] ? neighborAry[4] : newMax;
            }
        }
    }

    private void loadNeighbors(int i, int j) {
        int x = 0;
        for (int r = i - 1; r <= i + 1; ++r) {
            for (int c = j - 1; c <= j + 1; ++c) {
                neighborAry[x++] = imgArg[r][c];
            }
        }
    }

    private void sort(int[] array) {
        int min, t;
        for (int i = 0; i < 5; ++i) {
            min = i;
            for (int j = i + 1; j < 9; ++j) {
                if (array[j] < array[min])
                    min = j;
            }
            t = array[i];
            array[i] = array[min];
            array[min] = t;
        }
    }

    public void printMedianAry(PrintWriter outFile1) {
        outFile1.format("%d %d %d %d\n", numRows, numCols, newMin, newMax);
        for (int[] r : medianAry) {
            for (int c : r)
                outFile1.format("%d ", c);
            outFile1.println();
        }
    }

    public void gaussianFilter() {
        newMin = minVal;
        newMax = maxVal;
        totalWeight = computeWeight();
        for (int r = 2; r < numRows - 2; ++r) {
            for (int c = 2; c < numCols - 2; ++c) {
                gaussAry[r][c] = convolution(r, c) / totalWeight;
                newMin = newMin > gaussAry[r][c] ? gaussAry[r][c] : newMin;
                newMax = newMax < gaussAry[r][c] ? gaussAry[r][c] : newMax;
            }
        }
    }

    private int computeWeight() {
        int total = 0;
        for (int[] i : gaussTemplate) {
            for (int x : i) {
                total += x;
            }
        }
        return total;
    }

    private int convolution(int i, int j) {
        int result = 0;
        int iOffset = i - (templateRows / 2);   // 5/2 = 2
        int jOffset = j - (templateCols / 2);   // 5/2 = 2
        for (int m = 0; m < templateRows; ++m) {
            for (int n = 0; n < templateCols; ++n) {
                result += imgArg[iOffset + m][jOffset + n] * gaussTemplate[m][n];
            }
        }
        return result;
    }

    public void printGaussianAry(PrintWriter outFile2) {
        outFile2.format("%d %d %d %d\n", numRows, numCols, newMin, newMax);
        for (int[] r : gaussAry) {
            for (int c : r)
                outFile2.format("%d ", c);
            outFile2.println();
        }
    }
}
