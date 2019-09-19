/*
	Threshold to extract foreground objects in grey-scale image
*/

#include <fstream>
#include <iostream>
using namespace std;

class Threshold {
public:
	int numRows;
	int numCols;
	int minVal;
	int maxVal;
	int threshold;

	Threshold(ifstream &inFile) {
		inFile >> numRows >> numCols >> minVal >> maxVal;
		threshold = 0;
	}
};

int main(int args, char **argv) {
	ifstream inFile1;
	ofstream outFile1, outFile2;
	Threshold *thr;
	int t = 0, p = 0;

	inFile1.open(argv[1]);
	if (inFile1.fail()) {
		cout << "ERROR: cannot find \"" << argv[1] << "\"\n";
		exit(1);
	}
	outFile1.open(argv[2]);
	outFile2.open(argv[3]);
	thr = new Threshold(inFile1);
	cout << "Enter a threshold: ";
	cin >> t;
	while (t < thr->minVal || t > thr->maxVal) {
		cout << "Enter a valid threshold: ";
		cin >> t;
	}
	thr->threshold = t;
	outFile1 << thr->numRows << " " << thr->numCols << " " << thr->minVal << " " << thr->maxVal;
	for (int r = 0; r < thr->numRows; ++r) {
		outFile1 << endl;
		outFile2 << endl;
		for (int c = 0; c < thr->numCols; ++c) {
			inFile1 >> p;
			if (p >= t) {
				outFile1 << 1 << " ";
				outFile2 << 1 << " ";
			}
			else {
				outFile1 << 0 << " ";
				outFile2 << "  ";
			}
		}
	}
	inFile1.close();
	outFile1.close();
	outFile2.close();
	return 0;
}