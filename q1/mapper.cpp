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

    ifstream bFile(bFileName);

    if (!bFile.is_open()) {
        cerr << "Error: Could not open B file: "
             << bFileName << "\n";
        return 1;
    }

    int n, p;

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
    // Step 2: Read rows of A from standard input
    // --------------------------------------------------

    int rowIndex;

    /*
       Each input line looks like:

       rowIndex  A[row][0]  A[row][1] ... A[row][n-1]

       Example:

       0 1 2
       1 0 3
       2 -1 4
    */

    while (cin >> rowIndex) {

        // Store one row of A
        vector<int> Arow(n);

        // Read the n values of this row
        for (int k = 0; k < n; k++) {

            if (!(cin >> Arow[k])) {
                cerr << "Error: Invalid data in A\n";
                return 1;
            }
        }


        // --------------------------------------------------
        // Step 3: Calculate the corresponding row of C
        // --------------------------------------------------

        // C has p columns
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
        // Step 4: Emit MapReduce key-value pair
        // --------------------------------------------------

        // Key   = row index
        // Value = complete row of C

        cout << rowIndex;

        for (int j = 0; j < p; j++) {
            cout << " " << CRow[j];
        }

        cout << "\n";
    }

    return 0;
}