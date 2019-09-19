/*
	given histogram of a gray-scale image to fit a Gaussian curve
*/
#define _USE_MATH_DEFINES
#include <fstream>
#include <iostream>
#include <math.h>
#include <random>
using namespace std;

class Hgcp {
private:
	int numRows, numCols, minVal, maxVal, grayScale, grayCount, maxCount, totalPixel;
	double mean, var;
	int* histAry;
	int* gaussAry;
	int** histImg;
	int** gaussImg;
	int** gapImg;

	double gaus(int i) {
		return maxCount * (exp(-1 * (pow(i - mean, 2) / (2 * var))));
	}

	double computeVar() {
		double x = 0;
		for (int i = 0; i < grayCount; ++i) {
			x += (pow((i - mean), 2) * histAry[i]);
		}
		return x / totalPixel;
	}

	void prettyPrint(int** plot, ofstream& outFile) {
		for (int i = 0; i < grayCount; ++i) {
			for (int j = 0; j <= maxCount; ++j) {
				if (plot[i][j] <= 0)
					outFile << ' ';
				else
					outFile << '.';
			}
			outFile << '\n';
		}
	}

	~Hgcp() {
		for (int i = 0; i < grayCount; ++i) {
			delete[] histImg[i];
			delete[] gaussImg[i];
			delete[] gapImg[i];
		}
		delete[] histImg;
		delete[] gaussImg;
		delete[] gapImg;
	}

public:
	Hgcp(ifstream & inFile) {	//loadHist()
		inFile >> numRows >> numCols >> minVal >> maxVal;
		grayCount = maxVal + 1;
		histAry = new int[grayCount]();
		gaussAry = new int[grayCount]();
		maxCount = 0;
		mean = 0.0;
		totalPixel = 0;
		grayScale = 0;
		var = 0.0;
		for (int i = 0; i < grayCount; ++i) {
			inFile >> grayScale >> histAry[i];
			totalPixel += histAry[i];
			mean += (i * (double)histAry[i]);
			maxCount = maxCount > histAry[i] ? maxCount : histAry[i];
		}
		histImg = new int* [grayCount];
		gaussImg = new int* [grayCount];
		gapImg = new int* [grayCount];
		for (int r = 0; r < grayCount; ++r) {
			histImg[r] = new int[maxCount + 1]();
			gaussImg[r] = new int[maxCount + 1]();
			gapImg[r] = new int[maxCount + 1]();
		}
	}

	void plotHist(ofstream& outFile) {
		for (int i = 0; i < grayCount; ++i) {
			++histImg[i][histAry[i]];
		}
		prettyPrint(histImg, outFile);
	}

	void plotGauss(ofstream& outFile) {
		double j = 0.0; //Gval
		mean /= totalPixel;
		var = computeVar();
		for (int i = 0; i < grayCount; ++i) {
			j = gaus(i);
			gaussAry[i] = (int) j;
			++gaussImg[i][(int)j];
		}
		prettyPrint(gaussImg, outFile);
	}

	void plotGaps(ofstream& outFile) {
		for (int i = 0; i < grayCount; ++i) {
			if (histAry[i] < gaussAry[i]) {
				for (int j = histAry[i]; j <= gaussAry[i]; ++j) {
					++gapImg[i][j];
				}
			}
			else {
				for (int j = gaussAry[i]; j <= histAry[i]; ++j) {
					++gapImg[i][j];
				}
			}
		}
		prettyPrint(gapImg, outFile);
	}
};

int main(int args, char** argv) {
	ifstream inFile;
	ofstream outFile1, outFile2, outFile3;
	inFile.open(argv[1]);
	if (inFile.fail()) {
		cout << "ERROR: cannot find \"" << argv[1] << "\"\n";
		exit(1);
	}
	outFile1.open(argv[2]);
	outFile2.open(argv[3]);
	outFile3.open(argv[4]);
	Hgcp* hgcp = new Hgcp(inFile);
	hgcp->plotHist(outFile1);
	hgcp->plotGauss(outFile2);
	hgcp->plotGaps(outFile3);
	inFile.close();
	outFile1.close();
	outFile2.close();
	outFile3.close();
}