#include <iostream>
#include <fstream>
#include <vector>
#include <string>

using namespace std;

int main(int argc, char* argv[]) {

    // --------------------------------------------------
    // Validate command-line arguments
    // --------------------------------------------------

    if (argc < 2) {
        cerr << "Usage: mapper <B_file>\n";
        return 1;
    }

    string bFileName = argv[1];


    // --------------------------------------------------
    // Read matrix B
    // --------------------------------------------------

    ifstream bFile(bFileName);

    if (!bFile.is_open()) {
        cerr << "Error: Could not open B file: "
             << bFileName << "\n";
        return 1;
    }

    int n, p;

    // B has dimensions n x p
    if (!(bFile >> n >> p) || n <= 0 || p <= 0) {
        cerr << "Error: Invalid dimensions for matrix B\n";
        return 1;
    }

    vector<vector<int>> B(n, vector<int>(p));

    for (int i = 0; i < n; i++) {
        for (int j = 0; j < p; j++) {

            if (!(bFile >> B[i][j])) {
                cerr << "Error: Invalid data in B.txt\n";
                return 1;
            }
        }
    }

    bFile.close();


    // --------------------------------------------------
    // Read rows of A from standard input
    // --------------------------------------------------

    int rowIndex;

    /*
       Input format:

       rowIndex  A[row][0]  A[row][1] ... A[row][n-1]

       Example:

       0 1 2
       1 0 3
       2 -1 4
    */

    while (cin >> rowIndex) {

        vector<int> Arow(n);

        for (int k = 0; k < n; k++) {

            if (!(cin >> Arow[k])) {
                cerr << "Error: Invalid data in A\n";
                return 1;
            }
        }


        // --------------------------------------------------
        // Calculate corresponding row of C
        // --------------------------------------------------

        vector<int> CRow(p, 0);

        /*
            Row-Row multiplication:

            C[rowIndex] =
                A[rowIndex][0] * B[0]
              + A[rowIndex][1] * B[1]
              + ...
              + A[rowIndex][n-1] * B[n-1]
        */

        for (int k = 0; k < n; k++) {

            for (int j = 0; j < p; j++) {

                CRow[j] += Arow[k] * B[k][j];
            }
        }


        // --------------------------------------------------
        // Emit key-value pair
        // --------------------------------------------------

        // Key   = row index
        // Value = complete C row

        cout << rowIndex;

        for (int j = 0; j < p; j++) {
            cout << " " << CRow[j];
        }

        cout << "\n";
    }

    return 0;
}