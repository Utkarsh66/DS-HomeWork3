#include <iostream>
#include <sstream>
#include <string>

using namespace std;

int main() {
    string line;

    while (getline(cin, line)) {
        if (line.empty()) {
            continue;
        }

        stringstream ss(line);

        // Read and discard the row index
        int rowIndex;
        ss >> rowIndex;

        // Print only the matrix values
        long long value;
        bool firstValue = true;

        while (ss >> value) {
            if (!firstValue) {
                cout << " ";
            }

            cout << value;
            firstValue = false;
        }

        cout << "\n";
    }

    return 0;
}