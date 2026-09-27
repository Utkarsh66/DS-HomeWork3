#include <iostream>
#include <fstream>
#include <vector>

using namespace std;

bool readMatrixFromFile(
    ifstream &file,
    vector<vector<int>>& matrix,
    int& rows,
    int& cols
) {

    // Read dimensions
    if (!(file >> rows >> cols) || rows <= 0 || cols <= 0) {
        cerr << "Error: Invalid matrix dimensions.\n";
        return false;
    }

    // Create the matrix
    matrix.resize(rows, vector<int>(cols));

    // Read matrix values
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {

            if (!(file >> matrix[i][j])) {
                cerr << "Error: Invalid matrix data.\n";
                return false;
            }
        }
    }

    return true;
}


int main() {

    string fileName =
        "C:/Users/Utkar/source/repos/DS-HomeWork2/Q1/test.txt";

    vector<vector<int>> matrixA;
    vector<vector<int>> matrixB;
    vector<vector<int>> MatrixM;

    int rowsA = 0;
    int colsA = 0;

    int rowsB = 0;
    int colsB = 0;

        ifstream file(fileName);

    if (!file.is_open()) {
        cerr << "Error: Could not open file '" << fileName << "'\n";
        return false;
    }

    // Read matrix A
    if (!readMatrixFromFile(file, matrixA, rowsA, colsA)) {
        return 1;
    }

        // Read matrix A
    if (!readMatrixFromFile(file, matrixB, rowsB, colsB)) {
        return 1;
    }

    // Display matrix
    cout << "Matrix (" << rowsA << "x" << colsA << "):\n";

    for (const auto& row : matrixA) {

        for (int val : row) {
            cout << val << " ";
        }

        cout << "\n";
    }

     // Display matrixB
    cout << "Matrix (" << rowsB << "x" << colsB << "):\n";

    for (const auto& row : matrixB) {

        for (int val : row) {
            cout << val << " ";
        }

        cout << "\n";
    }

    MatrixM.resize(rowsA,vector<int>(colsB));
    // Matrix multiplication, Row-Row method
    for(int i = 0; i < rowsA; i++){
        for(int j = 0; j < colsA; j++){
            for(int k = 0; k < colsB; k++){
                MatrixM[i][k] += matrixA[i][j] * matrixB[j][k];
            }
        }
    }

    //Display matrixM 
    cout<< "Matrix" << rowsA << "x" << colsB << "\n";
    for(const auto &row : MatrixM){
        for(int val: row){
            cout << val << " ";
        }
        cout << "\n";
    }

    return 0;
}