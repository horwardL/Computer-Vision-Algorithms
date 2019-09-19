//Bi-Mean Gaussian Curve auto threshold selection.

#define _USE_MATH_DEFINES
#include <fstream>
#include <iostream>
#include <math.h>
using namespace std;

class AutoThrSelect {
private:
	int numRows, numCols, minVal, maxVal, maxCount, offset, thrVal;
	double minDiff;
	int* histAry;
	int* smoothedHistAry;
	int* gaussAry;
	int** histImg;
	int** gaussImg;
	int** gapImg;

	void bubbleSort(int* ary) {
		int temp = 0;
		for (int l = 0; l < 4; ++l) {
			for (int m = 0; m < 4 - l; ++m) {
				if (ary[m] > ary[m + 1]) {
					temp = ary[m];
					ary[m] = ary[m + 1];
					ary[m + 1] = temp;
				}
			}
		}
	}

	void set1DZero(int* ary) {
		for (int i = 0; i <= maxVal; ++i) {
			ary[i] = 0;
		}
	}

	void set2DZero(int** matrix) {
		for (int i = 0; i <= maxVal; ++i) {
			for (int j = 0; j <= maxCount; ++j) {
				matrix[i][j] = 0;
			}
		}
	}

	double fitGauss(int leftIndex, int rightIndex) {
		double mean = 0, var = 0, sum = 0.0, gVal = 0.0, s;
		int maxC = 0;
		mean = computeMean(leftIndex, rightIndex, maxC);
		var = computeVar(leftIndex, rightIndex, mean);
		for (int i = leftIndex; i <= rightIndex; ++i) {
			gVal = compGaussian(i, mean, var, maxC);
			s = gVal - histAry[i];
			if (s < 0) {
				s *= -1;
			}
			sum += s;
			gaussAry[i] = (int)gVal;
			gaussImg[i][(int)gVal] = 1;
		}
		return sum;
	}

	double computeMean(int left, int right, int& maxC) {
		double totalPixel = 0.0;
		int sum = 0;
		maxC = 0;
		for (int i = left; i <= right; ++i) {
			sum += (i * histAry[i]);
			totalPixel += histAry[i];
			maxC = histAry[i] > maxC ? histAry[i] : maxC;
		}
		return sum / totalPixel;
	}

	double computeVar(int left, int right, double mean) {
		double x = 0.0, totalPixel = 0.0;
		for (int i = left; i <= right; ++i) {
			x += (pow((i - mean), 2) * histAry[i]);
			totalPixel += histAry[i];
		}
		if(totalPixel > 0)
			return x / totalPixel;
		return 0;
	}

	double compGaussian(int i, double mean, double var, int maxC) {
		if (var == 0.0)
			return 0;
		return maxC * (exp(-1 * (pow(i - mean, 2) / (2 * var))));
	}

	void plotGaps() {
		int first = 0, last = 0;
      	for (int i = 0; i <= maxVal; ++i) {
			if (histAry[i] < gaussAry[i]) {
				first = histAry[i];
				last = gaussAry[i];
			}
			else {
				first = gaussAry[i];
				last = histAry[i];
			}
			while (first <= last) {
				gapImg[i][first] = 1;
				++first;
			}
		}
	}

	~AutoThrSelect() {
		delete[] histAry;
		delete[] gaussAry;
		delete[] smoothedHistAry;
		for (int i = 0; i <= maxVal; ++i) {
			delete[] histImg[i];
			delete[] gaussImg[i];
			delete[] gapImg[i];
		}
		delete[] histImg;
		delete[] gaussImg;
		delete[] gapImg;
	}

public:
	AutoThrSelect(ifstream & inFile) {
		numRows = numCols = minVal = maxVal = maxCount = offset = thrVal = 0;
		minDiff = 0.0;
		histImg = NULL;
		gaussImg = NULL;
		gapImg = NULL;
		inFile >> numRows >> numCols >> minVal >> maxVal;
		histAry = new int[maxVal + 1]();
		gaussAry = new int[maxVal + 1]();
		smoothedHistAry = new int[maxVal + 1]();
		thrVal = offset = maxVal / 10;
	}

	void loadHist(ifstream & inFile) {
		int tem = 0;
		for (int i = 0; i <= maxVal; ++i) {
			inFile >> tem >> histAry[i];
			maxCount = (histAry[i] > maxCount) ? histAry[i] : maxCount;
		}
		histImg = new int* [maxVal + 1];
		gaussImg = new int* [maxVal + 1];
		gapImg = new int* [maxVal + 1];
		for (int i = 0; i <= maxVal; ++i) {
			histImg[i] = new int[maxCount + 1]();
			gaussImg[i] = new int[maxCount + 1]();
			gapImg[i] = new int[maxCount + 1]();
		}
	}

	void plotHist(ofstream & outFile) {
		for (int i = 0; i <= maxVal; ++i) {
			++histImg[i][histAry[i]];
		}
		prettyPrint(histImg, outFile);
	}

	void prettyPrint(int** plot, ofstream & outFile) {
		for (int i = 0; i <= maxVal; ++i) {
			for (int j = 0; j <= maxCount; ++j) {
				if (plot[i][j] <= 0)
					outFile << ' ';
				else
					outFile << '.';
			}
			outFile << '\n';
		}
	}

	void median1D() {
		int temp[5] = { 0 };
		int j = 0;
		for (int i = 2; i <= maxVal - 2; ++i) {
			j = 0;
			for (int k = i - 2; k <= i + 2; ++k) {
				temp[j++] = histAry[k];
			}
			bubbleSort(temp);
			smoothedHistAry[i] = temp[2];
		}
		int front[6] = { 0 };
		int back[6] = { 0 };
		front[0] = histAry[2]; //mirror
		front[1] = histAry[1];
		back[4] = histAry[maxVal - 1];
		back[5] = histAry[maxVal - 2];
		for (int i = 2; i < 6; ++i) {
			front[i] = histAry[i - 1];
			back[i - 2] = histAry[maxVal - 5 + i];
		}
		for (int i = 2; i < 4; ++i) {
			j = 0;
			for (int k = i - 2; k <= i + 2; ++k)
				temp[j++] = front[k];
			bubbleSort(temp);
			smoothedHistAry[i - 2] = temp[2];
			j = 0;
			for (int k = i - 2; k <= i + 2; ++k)
				temp[j++] = back[k];
			bubbleSort(temp);
			smoothedHistAry[maxVal - 3 + i] = temp[2];
		}
	}

	void copyArys() {
		for (int i = 0; i <= maxVal; ++i) {
			histImg[i][histAry[i]] = 0;	//set2DZero
			histAry[i] = smoothedHistAry[i];
		}
	}

	int biMeanGauss(ofstream & outGauss, ofstream & outGap) {
		double sum1 = 0.0, sum2 = 0.0, total = 0.0;
		int bestThr = thrVal;
		minDiff = INT_MAX;
		while (thrVal < (maxVal - offset)) {
			set1DZero(gaussAry);
			sum1 = fitGauss(0, thrVal);
			sum2 = fitGauss(thrVal, maxVal);
			total = sum1 + sum2;
			if (total < minDiff) {
				minDiff = total;
				bestThr = thrVal;
			}
			outGauss << thrVal << endl;
			prettyPrint(gaussImg, outGauss);
			outGauss << endl;

			plotGaps();
			outGap << thrVal << endl;
			prettyPrint(gapImg, outGap);
			outGap << endl;

			++thrVal;
			set2DZero(gaussImg);
			set2DZero(gapImg);
		}
		return bestThr;
	}

	void bestThrPlot(int bestThrVal, ofstream & outFile) {
		double sum1 = 0.0, sum2 = 0.0;
		//set2DZero(gaussImg);
		//set2DZero(gapImg);
		//set1DZero(gaussAry);
		sum1 = fitGauss(0, bestThrVal);
		sum2 = fitGauss(bestThrVal, maxVal);
		plotGaps();
		outFile << "Best Thrshold value = " << bestThrVal << endl;
		outFile << "Gaussian Curves\n";
		prettyPrint(gaussImg, outFile);
		outFile << "gaps plot" << endl;
		prettyPrint(gapImg, outFile);
	}
};

int  main(int args, char** argv) {
	int bestThrVal = 0;
	ifstream inFile;
	ofstream outFile1, outFile2, outFile3, outFile4;
	inFile.open(argv[1]);
	if (inFile.fail()) {
		cout << "ERROR: cannot find \"" << argv[1] << "\"\n";
		exit(1);
	}
	outFile1.open(argv[2]);
	outFile2.open(argv[3]);
	outFile3.open(argv[4]);
	outFile4.open(argv[5]);
	AutoThrSelect* bi_mean = new AutoThrSelect(inFile);
	bi_mean->loadHist(inFile);
	outFile1 << "Before smooth\n";
	bi_mean->plotHist(outFile1);
	bi_mean->median1D();
	bi_mean->copyArys();
	outFile1 << "\nAfter smooth\n";
	bi_mean->plotHist(outFile1);
	bestThrVal = bi_mean->biMeanGauss(outFile2, outFile3);
	bi_mean->bestThrPlot(bestThrVal, outFile4);
	inFile.close();
	outFile1.close();
	outFile2.close();
	outFile3.close();
	outFile4.close();
	return 0;
}