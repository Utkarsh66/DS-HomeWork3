#include <iostream>
#include <fstream>
#include <vector>
#include <string>

using namespace std;

int main(int argc, char* argv[]) {

    if (argc < 2) {
        cerr << "Usage: mapper <B_file>\n";
        return 1;
    }

    string bFileName = argv[1];

    // Read B
    ifstream bFile(bFileName);

    if (!bFile.is_open()) {
        cerr << "Error: Could not open B file: "
             << bFileName << "\n";
        return 1;
    }

    int n, p;

    if (!(bFile >> n >> p) || n <= 0 || p <= 0) {
        cerr << "Error: Invalid dimensions for B\n";
        return 1;
    }

    vector<vector<long long>> B(n, vector<long long>(p));

    for (int i = 0; i < n; i++) {
        for (int j = 0; j < p; j++) {

            if (!(bFile >> B[i][j])) {
                cerr << "Error: Invalid data in B\n";
                return 1;
            }
        }
    }

    bFile.close();


    // Process rows of A
    int rowIndex;

    while (cin >> rowIndex) {

        vector<long long> Arow(n);

        for (int k = 0; k < n; k++) {

            if (!(cin >> Arow[k])) {
                cerr << "Error: Invalid data in A\n";
                return 1;
            }
        }


        // Calculate C row
        vector<long long> CRow(p, 0);

        for (int k = 0; k < n; k++) {

            for (int j = 0; j < p; j++) {

                CRow[j] += Arow[k] * B[k][j];
            }
        }


        // Emit:
        // rowIndex value1 value2 ... valueP

        cout << rowIndex;

        for (long long value : CRow) {
            cout << " " << value;
        }

        cout << "\n";
    }

    return 0;
}