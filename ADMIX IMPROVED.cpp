#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <vector>

using namespace std;

void admix();

int main() {
    while (true) {
        admix();
        char choice;
        cout << "\nDo you want to run the program again? (y/n): ";
        cin >> choice;
        if (choice != 'y' && choice != 'Y') {
            break;
        }
    }
    return 0;
}

void admix() {
    string fileName;
    int targetColumn, numSources;

    cout << "Enter File Name: ";
    cin >> fileName;
    
    cout << "\nEnter Population to be modeled (integer, column number): ";
    cin >> targetColumn;

    cout << "\nEnter number of source populations (max 3): ";
    cin >> numSources;

    if (numSources < 1 || numSources > 3) {
        cout << "Error: Number of sources must be between 1 and 3." << endl;
        return;
    }

    vector<int> sourceColumns(numSources);
    for (int i = 0; i < numSources; ++i) {
        cout << "Enter Source " << (i + 1) << " (integer, column number): ";
        cin >> sourceColumns[i];
    }

    ifstream file(fileName);
    if (!file) {
        cout << "Unable to open file" << endl;
        return;
    }

    vector<float> sourceDistances(numSources, 0);
    vector<float> sourcePairwiseDistances(numSources * numSources, 0);
    float targetValue;

    string line;
    while (getline(file, line)) {
        stringstream ss(line);
        float value;
        int columnCount = 0;
        vector<float> sourceValues(numSources);

        while (ss >> value) {
            columnCount++;
            if (columnCount == targetColumn) {
                targetValue = value;
            } else {
                for (int i = 0; i < numSources; ++i) {
                    if (columnCount == sourceColumns[i]) {
                        sourceValues[i] = value;
                    }
                }
            }
        }

        for (int i = 0; i < numSources; ++i) {
            sourceDistances[i] += (targetValue - sourceValues[i]) * (targetValue - sourceValues[i]);
            for (int j = i + 1; j < numSources; ++j) {
                sourcePairwiseDistances[i * numSources + j] += (sourceValues[i] - sourceValues[j]) * (sourceValues[i] - sourceValues[j]);
            }
        }
    }

    for (int i = 0; i < numSources; ++i) {
        cout << "S" << (i + 1) << "dist = " << sourceDistances[i] << endl;
    }

    for (int i = 0; i < numSources; ++i) {
        for (int j = i + 1; j < numSources; ++j) {
            cout << "S" << (i + 1) << "S" << (j + 1) << " = " << sourcePairwiseDistances[i * numSources + j] << endl;
        }
    }

    cout << "\nVahaduoCoords:\n";
    for (int i = 0; i < numSources; ++i) {
        cout << "Source" << (i + 1) << "," << 0 << "," << sourcePairwiseDistances[i * numSources + 1] << "," << sourcePairwiseDistances[i * numSources + 2] << endl;
    }
    cout << "Target," << sourceDistances[0] << "," << sourceDistances[1] << "," << sourceDistances[2] << endl;
}