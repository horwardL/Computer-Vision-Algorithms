#include <iostream>
#include <fstream>
using namespace std;

class DistMedialAxis {
private:
	int numRows, numCols, minVal, maxVal, newMinVal, newMaxVal;
	int** zeroFramedAry;
	int** skeletonAry;

	~DistMedialAxis() {
		for (int i = 0; i < numRows + 2; ++i) {
			delete[] zeroFramedAry[i];
			delete[] skeletonAry[i];
		}
		delete[] zeroFramedAry;
		delete[] skeletonAry;
	}

	void prettyPrint(int** ary, ofstream& outFile) {
		for (int i = 1; i <= numRows; ++i) {
			for (int j = 1; j <= numCols; ++j) {
				if (ary[i][j] <= 0)
					outFile << "  ";
				else
					outFile << ary[i][j] << " ";
			}
			outFile << endl;
		}
	}

	int frontMin(int i, int j) {
		int min = zeroFramedAry[i][j - 1];
		for (int ioff = i - 1, joff = j - 1; joff < j + 2; ++joff) {
			if (zeroFramedAry[ioff][joff] < min)
				min = zeroFramedAry[ioff][joff];
		}
		return min + 1;
	}

	int backMin(int i, int j) {
		int min = zeroFramedAry[i][j]-1 < zeroFramedAry[i][j + 1] ? zeroFramedAry[i][j] - 1 : zeroFramedAry[i][j + 1];
		for (int ioff = i + 1, joff = j - 1; joff < j + 2; ++joff) {
			if (zeroFramedAry[ioff][joff] < min)
				min = zeroFramedAry[ioff][joff];
		}
		return min + 1;
	}

	int localMax(int i, int j) {
		int max = 0;
		for (int ioff = i-1; ioff < i+2; ++ioff) {
			for (int joff = j-1; joff < j+2; ++joff) {
				if (zeroFramedAry[ioff][joff] > max)
					max = zeroFramedAry[ioff][joff];
			}
		}
		return max;
	}

	void resetZero(int** ary) {
		for (int i = 1; i <= numRows; ++i) {
			for (int j = 1; j <= numCols; ++j) {
					zeroFramedAry[i][j] = 0;
			}
		}
	}

	void load(ifstream& inFile) {
		int i = 0, j = 0;
		inFile >> numRows >> numCols >> newMinVal >> newMaxVal;
		while (!inFile.eof()) {
			inFile >> i >> j;
			inFile >> zeroFramedAry[i][j];
		}
	}

	void firstPass_8distance() {
		for (int i = 1; i <= numRows; ++i) {
			for (int j = 1; j <= numCols; ++j) {
				if (zeroFramedAry[i][j] > 0)
					zeroFramedAry[i][j] = frontMin(i, j);
			}
		}
	}

	void secondPass_8distance() {
		for (int i = numRows; i > 0; --i) {
			for (int j = numCols; j > 0; --j) {
				zeroFramedAry[i][j] = backMin(i, j);
			}
		}
	}

	void compute_localMaxima() {
		for (int i = 1; i <= numRows; ++i) {
			for (int j = 1; j <= numCols; ++j) {
				if (zeroFramedAry[i][j] > 0 && zeroFramedAry[i][j] >= localMax(i, j)) {
					skeletonAry[i][j] = zeroFramedAry[i][j];
					newMaxVal = zeroFramedAry[i][j];
				}
			}
		}
	}

	void extraclocalMaxima(ofstream& outFile) {
		outFile << numRows << " " << numCols << " " << newMinVal << " " << newMaxVal << endl;
		for (int i = 1; i <= numRows; ++i) {
			for (int j = 1; j <= numCols; ++j) {
				if (skeletonAry[i][j] > 0) {
					outFile << i << " " << j << " " << skeletonAry[i][j] << endl;
				}
			}
		}
	}

	void firstPass_Expension() {
		int max = 0;
		for (int i = 1; i <= numRows; ++i) {
			for (int j = 1; j <= numCols; ++j) {
				max = localMax(i, j);
				max = max <= zeroFramedAry[i][j] ? zeroFramedAry[i][j] : max - 1;
				zeroFramedAry[i][j] = max;
			}
		}
	}

	void secondPass_Expension() {
		int max = 0;
		for (int i = numRows; i >= 1; --i) {
			for (int j = numCols; j >= 1; --j) {
				max = localMax(i, j);
				max = max <= zeroFramedAry[i][j] ? zeroFramedAry[i][j] : max - 1;
				zeroFramedAry[i][j] = max;
			}
		}
	}

public:
	DistMedialAxis(ifstream& inFile) {
		inFile >> numRows >> numCols >> minVal >> maxVal;
		newMinVal = newMaxVal = 0;
		zeroFramedAry = new int* [numRows + 2];
		skeletonAry = new int* [numCols + 2];
		for (int i = 0; i < numRows + 2; ++i) {
			zeroFramedAry[i] = new int[numCols + 2]();
			skeletonAry[i] = new int[numCols + 2]();
		}
	}

	void loadImage(ifstream& inFile) {
		for (int i = 1; i <= numRows; ++i) {
			for (int j = 1; j <= numCols; ++j) {
				inFile >> zeroFramedAry[i][j];
			}
		}
	}

	void compute8Distance(ofstream& outFile) {
		firstPass_8distance();
		prettyPrint(zeroFramedAry, outFile);
		secondPass_8distance();
		outFile << endl;
		prettyPrint(zeroFramedAry, outFile);
	}

	void skeletonExtraction(ofstream& outFile, ofstream& skeletonFile) {
		compute_localMaxima();
		outFile << endl;
		prettyPrint(skeletonAry, outFile);
		extraclocalMaxima(skeletonFile);
	}

	void skeletonExpansion(string skeletonFileName, ofstream& outFile) {
		ifstream skeletonFile;
		skeletonFile.open(skeletonFileName);
		resetZero(zeroFramedAry);
		load(skeletonFile);
		skeletonFile.close();
		firstPass_Expension();
		prettyPrint(zeroFramedAry, outFile);
		secondPass_Expension();
		outFile << endl;
		prettyPrint(zeroFramedAry, outFile);
	}

	void ary2File(ofstream& outFile) {
		outFile << numRows << " " << numCols << " " << minVal << " " << maxVal << endl;
		for (int i = 1; i <= numRows; ++i) {
			for (int j = 1; j <= numCols; ++j) {
				if (zeroFramedAry[i][j] >= 1)
					outFile << 1 << " ";
				else
					outFile << 0 << " ";
			}
			outFile << endl;
		}
	}
};

int main(int args, char** argv) {
	string inFileName = string(argv[1]).substr(0, string(argv[1]).size() - 4);
	string skeletonFileName = inFileName + "_skeleton.txt";
	string deCompressFileName = inFileName + "_deCompressed.txt";
	ifstream inFile;
	ofstream outFile1, outFile2, skeletonFile, deCompressFile;
	inFile.open(argv[1]);
	if (inFile.fail()) {
		cout << "ERROR: cannot find \"" << argv[1] << "\"\n";
		exit(1);
	}
	outFile1.open(argv[2]);
	outFile2.open(argv[3]);
	skeletonFile.open(skeletonFileName);
	deCompressFile.open(deCompressFileName);
	DistMedialAxis* distSkeDeComp = new DistMedialAxis(inFile);
	distSkeDeComp->loadImage(inFile);
	inFile.close();
	distSkeDeComp->compute8Distance(outFile1);
	distSkeDeComp->skeletonExtraction(outFile1, skeletonFile);
	outFile1.close();
	skeletonFile.close();
	distSkeDeComp->skeletonExpansion(skeletonFileName, outFile2);
	outFile2.close();
	distSkeDeComp->ary2File(deCompressFile);
	deCompressFile.close();
}