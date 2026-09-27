#include <iostream>
#include <sstream>
#include <vector>
#include <string>

using namespace std;

int main() {

    string line;

    int currentKey = -1;
    vector<long long> currentRow;
    bool hasCurrentKey = false;

    while (getline(cin, line)) {

        if (line.empty()) {
            continue;
        }

        stringstream ss(line);

        int key;
        ss >> key;

        vector<long long> row;
        long long value;

        while (ss >> value) {
            row.push_back(value);
        }

        if (!hasCurrentKey) {

            currentKey = key;
            currentRow = row;
            hasCurrentKey = true;

        }
        else if (key == currentKey) {

            // Same key: add the corresponding elements

            if (row.size() != currentRow.size()) {
                cerr << "Error: Inconsistent row size in combiner\n";
                return 1;
            }

            for (size_t i = 0; i < row.size(); i++) {
                currentRow[i] += row[i];
            }

        }
        else {

            // Output previous key

            cout << currentKey;

            for (long long value : currentRow) {
                cout << " " << value;
            }

            cout << "\n";


            // Start new key

            currentKey = key;
            currentRow = row;
        }
    }


    // Output final key

    if (hasCurrentKey) {

        cout << currentKey;

        for (long long value : currentRow) {
            cout << " " << value;
        }

        cout << "\n";
    }

    return 0;
}