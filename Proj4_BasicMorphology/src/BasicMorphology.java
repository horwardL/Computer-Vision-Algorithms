import java.io.PrintWriter;
import java.util.Scanner;

public class BasicMorphology {
	private int numRowsImg, numColsImg, minImg, maxImg;
	private int numRowsStructEle, numColsStructEle, minStructEle, maxStructEle;
	private int rowOrigin, colOrigon;
	private int rowFrameSize, colFrameSize;
	private int[][] imgAry;
	private int[][] structEleAry;
	private int[][] morphAry;
	private int[][] tempAry;

	public BasicMorphology(Scanner img, Scanner structEle) {
		numRowsImg = img.nextInt();
		numColsImg = img.nextInt();
		minImg = img.nextInt();
		maxImg = img.nextInt();
		numRowsStructEle = structEle.nextInt();
		numColsStructEle = structEle.nextInt();
		minStructEle = structEle.nextInt();
		maxStructEle = structEle.nextInt();
		rowOrigin = structEle.nextInt();
		colOrigon = structEle.nextInt();
		rowFrameSize = rowOrigin;
		colFrameSize = colOrigon;
		imgAry = new int[numRowsImg + rowFrameSize * 2][numColsImg + colFrameSize * 2];
		structEleAry = new int[numRowsStructEle][numColsStructEle];
		morphAry = new int[numRowsImg + rowFrameSize * 2][numColsImg + colFrameSize * 2];
		tempAry = new int[numRowsImg + rowFrameSize * 2][numColsImg + colFrameSize * 2];
	}

	public void loadImage(Scanner img, String s) {
		for (int i = rowFrameSize; i < numRowsImg + rowFrameSize; ++i) {
			for (int j = colFrameSize; j < numColsImg + colFrameSize; ++j) {
				imgAry[i][j] = img.nextInt();
			}
		}
		prettyPrint(imgAry, s);
	}

	private void prettyPrint(int[][] ary, String s) {
		System.out.format("%s\n", s);
		for (int[] i : ary) {
			for (int j : i) {
				if (j > 0)
					System.out.format("1 ");
				else
					System.out.format("  ");
			}
			System.out.format("\n");
		}
	}

	private void outputPrint(int[][] ary, PrintWriter outFile) {
		outFile.format("%d %d %d %d\n", numRowsImg, numColsImg, minImg, maxImg);
		for (int m = rowFrameSize; m < numRowsImg + rowFrameSize; ++m) {
			for (int n = colFrameSize; n < numColsImg + colFrameSize; ++n) {
				if (ary[m][n] > 0)
					outFile.format("1 ");
				else
					outFile.format("0 ");
			}
			outFile.format("\n");
		}
	}

	public void loadStruct(Scanner structEle, String s) {
		for (int i = 0; i < numRowsStructEle; ++i) {
			for (int j = 0; j < numColsStructEle; ++j) {
				structEleAry[i][j] = structEle.nextInt();
			}
		}
		prettyPrint(structEleAry, s);
	}

	private void initAry(int[][] ary) {
		for (int i = 0; i < ary.length; ++i) {
			for (int j = 0; j < ary[i].length; ++j)
				ary[i][j] = 0;
		}
	}

	private void dilation(int[][] image, int[][] processedImg) {
		int iOffset = 0, jOffset = 0;
		for (int m = rowFrameSize; m < numRowsImg + rowFrameSize; ++m) {
			for (int n = colFrameSize; n < numColsImg + colFrameSize; ++n) {
				if (image[m][n] >= 1) {
					iOffset = m + (0 - rowOrigin);
					jOffset = n + (0 - colOrigon);
					for (int r = 0; r < numRowsStructEle; ++r)
						for (int c = 0; c < numColsStructEle; ++c)
							if (structEleAry[r][c] >= 1)
								processedImg[iOffset + r][jOffset + c] = 1;
				}
			}
		}
	}

	private void erosion(int[][] image, int[][] processedImg) {
		int iOffset = 0, jOffset = 0;
		for (int m = rowFrameSize; m < numRowsImg + rowFrameSize; ++m) {
			for (int n = colFrameSize; n < numColsImg + colFrameSize; ++n) {
				if (image[m][n] >= 1) {
					iOffset = m + (0 - rowOrigin);
					jOffset = n + (0 - colOrigon);
					out: {
						for (int r = 0; r < numRowsStructEle; ++r)
							for (int c = 0; c < numColsStructEle; ++c)
								if (structEleAry[r][c] >= 1 && image[iOffset + r][jOffset + c] <= 0)
									break out;
						processedImg[m][n] = 1;
					}
				}
			}
		}
	}

	public void dilationOp(PrintWriter outFile, String fname) {
		initAry(morphAry);
		dilation(imgAry, morphAry);
		prettyPrint(morphAry, "Dilation Operation");
		outputPrint(morphAry, outFile);
	}

	public void erosionOp(PrintWriter outFile, String fname) {
		initAry(morphAry);
		erosion(imgAry, morphAry);
		prettyPrint(morphAry, "Erosion Operation");
		outputPrint(morphAry, outFile);
	}

	public void closingOp(PrintWriter outFile, String fname) {
		initAry(morphAry);
		initAry(tempAry);
		dilation(imgAry, tempAry);
		erosion(tempAry, morphAry);
		prettyPrint(morphAry, "Closing Operation");
		outputPrint(morphAry, outFile);
	}

	public void openingOp(PrintWriter outFile, String fname) {
		initAry(morphAry);
		initAry(tempAry);
		erosion(imgAry, tempAry);
		dilation(tempAry, morphAry);
		prettyPrint(morphAry, "Opening Operation");
		outputPrint(morphAry, outFile);
	}
}
