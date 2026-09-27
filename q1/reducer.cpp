#include <iostream>
#include <sstream>
#include <vector>
#include <string>

using namespace std;

int main() {

    string line;

    while (getline(cin, line)) {

        if (line.empty()) {
            continue;
        }

        stringstream ss(line);

        int rowIndex;
        ss >> rowIndex;

        cout << rowIndex;

        long long value;

        while (ss >> value) {
            cout << " " << value;
        }

        cout << "\n";
    }

    return 0;
}