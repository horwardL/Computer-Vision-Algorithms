#include <iostream>
#include <fstream>
#include <string>
using namespace std;

class ImagePP {
public:
	class Box {
	public:
		int minR, minC, maxR, maxC;

		Box() {
			minR = minC = maxR = maxC = 0;
		}

		Box(int miR, int miC) {
			minR = miR;
			minC = miC;
			maxR = maxC = 0;
		}
	};
	class BoxNode {
	public:
		int boxType; //1 - doc box, 2 - paragraph, 3 - textLine
		Box* bBox;
		BoxNode* next;

		BoxNode() {
			boxType = -1;
			bBox = NULL;
			next = NULL;
		}

		BoxNode(int boxT, Box* bB) {
			boxType = boxT;
			bBox = bB;
			next = NULL;
		}
	};
	class BoxQ {
	public:
		BoxNode* qFront;
		BoxNode* qBack;

		BoxQ(BoxNode* front) {
			qFront = front;
			qBack = qFront;
		}

		void insert(BoxNode* newBoxNode) {
			qBack->next = newBoxNode;
			qBack = newBoxNode;
		}
	};

	int** imgAry;
	Box* imgBox;
	int numRows, numCols, minVal, maxVal;
	int thrVal, hppRuns, vppRuns;
	int* hpp;
	int* vpp;
	int* hppBin;
	int* vppBin;
	int* hppMorph;
	int* vppMorph;
	string readingDir;
	BoxQ* boxQ;

	ImagePP(ifstream& inFile, int t) {
		inFile >> numRows >> numCols >> minVal >> maxVal;
		imgBox = new Box();
		imgBox->minR = numRows;
		imgBox->minC = numCols;
		imgAry = new int* [numRows];
		for (int i = 0; i < numRows; ++i)
			imgAry[i] = new int[numCols]();
		thrVal = t;
		hppRuns = vppRuns = 0;
		hpp = new int[numRows + 2]();
		vpp = new int[numCols + 2]();
		hppBin = new int[numRows + 2]();
		vppBin = new int[numCols + 2]();
		hppMorph = new int[numRows + 2]();
		vppMorph = new int[numCols + 2]();
		readingDir = "";
		boxQ = new BoxQ(new BoxNode());
		for (int i = 0; i < numRows; ++i)
			for (int j = 0; j < numCols; ++j) {
				inFile >> imgAry[i][j];
			}
	}

	void computePP(ofstream &outFile) {
		for (int i = 0; i < numRows; ++i)
			for (int j = 0; j < numCols; ++j) {
				if (imgAry[i][j] > 0) {
					++hpp[i + 1];
					++vpp[j + 1];
				}
			}
		outFile << "HPP\n";
		printPP(hpp, numRows, outFile);
		outFile << "VPP\n";
		printPP(vpp, numCols, outFile);
	}

	void printPP(int* pp, int size, ofstream& outFile) {
		for (int i = 1; i <= size; ++i) {
			outFile << pp[i] << " ";
		}
		outFile << endl << endl;
	}

	void threshold(ofstream& outFile) {
		for (int i = 1; i <= numRows; ++i)
			if (hpp[i] > thrVal) {
				hppBin[i] = 1;
				//compute image horizontal boundary box
				imgBox->minR = imgBox->minR < i ? imgBox->minR : i;
				imgBox->maxR = imgBox->maxR > i ? imgBox->maxR : i;
			}
		for (int j = 1; j <= numCols; ++j)
			if (vpp[j] > thrVal) {
				vppBin[j] = 1;
				//compute image vertical boundary box
				imgBox->minC = imgBox->minC < j ? imgBox->minC : j;
				imgBox->maxC = imgBox->maxC > j ? imgBox->maxC : j;
			}
		--imgBox->minR;
		--imgBox->minC;
		--imgBox->maxR;
		--imgBox->maxC;
		outFile << "HPPBinary\n";
		printPP(hppBin, numRows, outFile);
		outFile << "VPPBinary\n";
		printPP(vppBin, numCols, outFile);
	}

	void morphClosing(ofstream& outFile) {
		//strcElep[] = { 1, 1, 1 };
		int* hppTempAry = new int[numRows + 2]();
		int* vppTempAry = new int[numCols + 2]();
		dilation(hppBin, hppTempAry, numRows);
		dilation(vppBin, vppTempAry, numCols);
		erosion(hppTempAry, hppMorph, numRows);
		erosion(vppTempAry, vppMorph, numCols);
		outFile << "HPPMorph\n";
		printPP(hppMorph, numRows, outFile);
		outFile << "VPPMorph\n";
		printPP(vppMorph, numCols, outFile);
	}

	void dilation(int* pp, int* processedPP, int size) {
		for (int i = 1; i <= size; ++i) {
			if (pp[i] > 0) {
				for (int j = i - 1; j <= i + 1; ++j) {
					processedPP[j] = 1;
				}
			}
		}
	}

	void erosion(int* pp, int* processedPP, int size) {
		for (int i = 1; i <= size; ++i)
			if (pp[i] > 0 && pp[i - 1] > 0 && pp[i + 1] > 0)
				processedPP[i] = 1;
	}

	void determineReadingDir(ofstream& outFile) {
		int factor = 3;
		hppRuns = computeRuns(hppMorph, numRows);
		vppRuns = computeRuns(vppMorph, numCols);
		if (hppRuns >= factor * vppRuns) {
			readingDir = "horizontal";
			outFile << "Reading Direction : " << readingDir << endl << endl;
		}else if (vppRuns >= factor * hppRuns) {
			readingDir = "vertical";
			outFile << "Reading Direction: " << readingDir << endl << endl;
		}else {
			cout << "Cannot determine the reading direction.\n";
			exit(1);
		}
	}

	int computeRuns(int* pp, int size) {
		int numRuns = 0;
		for (int i = 1; i <= size; ++i)
			if (pp[i] > 0 && pp[i + 1] <= 0)
				++numRuns;
		return numRuns;
	}

	void findLineBoxes(ofstream& outFile) {
		if (readingDir.compare("horizontal") == 0) {
			findLineBoxes(hppMorph,imgBox->minR, imgBox->minC, imgBox->maxR, imgBox->maxC);
		}
		else {
			findLineBoxes(vppMorph, imgBox->minC, imgBox->minR, imgBox->maxC, imgBox->maxR);
		}
		outFile << "text-line bounding box\n";
		printBoxQueue(outFile);
	}

	void findLineBoxes(int* pp, int minSize, int otherMinSize, int maxSize, int otherMaxSize) {
		BoxNode* newBoxNode;
		Box* box;
		for (int i = minSize; i <= maxSize; ++i) {
			if (pp[i + 1] > 0) {
				box = new Box(numRows, numCols);
				newBoxNode = new BoxNode(3, box);
				while (pp[i + 1] > 0) {
					for (int j = otherMinSize; j <= otherMaxSize; ++j) {
						if (readingDir.compare("horizontal") == 0) {
							if (imgAry[i][j] > 0) {
								box->minR = box->minR < i ? box->minR : i;
								box->minC = box->minC < j ? box->minC : j;
								box->maxR = box->maxR > i ? box->maxR : i;
								box->maxC = box->maxC > j ? box->maxC : j;
							}
						}
						else {
							if (imgAry[j][i] > 0) {
								box->minR = box->minR < j ? box->minR : j;
								box->minC = box->minC < i ? box->minC : i;
								box->maxR = box->maxR > j ? box->maxR : j;
								box->maxC = box->maxC > i ? box->maxC : i;
							}
						}
					}
					++i;
				}
				boxQ->insert(newBoxNode);
			}
		}
	}

	void printBoxQueue(ofstream& outFile) {
		BoxNode* current = boxQ->qFront->next;
		while (current) {
			outFile << current->boxType << endl;
			outFile << current->bBox->minR << " " << current->bBox->minC << " ";
			outFile << current->bBox->maxR << " " << current->bBox->maxC << endl << endl;
			current = current->next;
		}
	}
};

int main(int argc, char* argv[]) {
	ifstream inFile;
	ofstream outFile;
	int thrVal;
	if (argc < 3) {
		cout << "ERROR: missing argument(s).\n<input file> <threshold value> <output file>";
		exit(1);
	}
	inFile.open(argv[1]);
	if (inFile.fail()) {
		cout << "ERROR: cannot find \"" << argv[1] << "\"\n";
		exit(1);
	}
	thrVal = atoi(argv[2]);
	outFile.open(argv[3]);
	ImagePP imagePP(inFile, thrVal);
	inFile.close();
	imagePP.computePP(outFile);
	imagePP.threshold(outFile);
	imagePP.morphClosing(outFile);
	imagePP.determineReadingDir(outFile);
	imagePP.findLineBoxes(outFile);
	outFile.close();
}