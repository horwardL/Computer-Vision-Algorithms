/*
	find Histogram of an image
*/

#include <fstream>
#include <iostream>
using namespace std;

class Histogram {
private:
	int numRows;
	int numCols;
	int minVal;
	int maxVal;
	int *hist;

	~Histogram() {
		delete[] hist;
	}

public:
	Histogram(ifstream &inFile) {
		inFile >> numRows >> numCols >> minVal >> maxVal;
		hist = new int[maxVal + 1]();
	}

	void computeHistogram(ifstream &inFile, ofstream &outFile) {
		for (int r = 0; r < numRows; ++r) {
			for (int c = 0; c < numCols; ++c) {
				int p;
				inFile >> p;
				++hist[p];
			}
		}
		outFile << numRows << " " << numCols << " " << minVal << " " << maxVal;
		for (int i = 0; i <= maxVal; ++i) {
			outFile << endl << i << " " << hist[i];
		}
		inFile.close();
		outFile.close();
	}
};

int main(int args, char **argv) {
	ifstream inFile;
	ofstream outFile;
	inFile.open(argv[1]);
	if (inFile.fail()) {
		cout << "ERROR: cannot find \"" << argv[1] << "\"\n";
		exit(1);
	}
	outFile.open(argv[2]);
	Histogram *hist = new Histogram(inFile);
	hist->computeHistogram(inFile, outFile);
	return 0;
}