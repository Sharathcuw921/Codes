#include <iostream>
#include <vector>

using namespace std;

class SparseMatrix {
private:
    vector<int> rows;
    vector<int> cols;
    vector<int> values;
    int numRows;
    int numCols;

public:
    SparseMatrix(int rows, int cols) : numRows(rows), numCols(cols) {}

    void addValue(int row, int col, int value) {
        if (value != 0) {
            rows.push_back(row);
            cols.push_back(col);
            values.push_back(value);
        }
    }

    int getValue(int row, int col) const {
        for (size_t i = 0; i < rows.size(); ++i) {
            if (rows[i] == row && cols[i] == col) {
                return values[i];
            }
        }
        return 0;
    }

    void printMatrix() const {
        for (int i = 0; i < numRows; ++i) {
            for (int j = 0; j < numCols; ++j) {
                cout << getValue(i, j) << " ";
            }
            cout << endl;
        }
    }
};

int main() {
    int numRows, numCols, numNonZero;
    cout << "Enter the number of rows: ";
    cin >> numRows;
    cout << "Enter the number of columns: ";
    cin >> numCols;
    cout << "Enter the number of non-zero elements: ";
    cin >> numNonZero;

    SparseMatrix sm(numRows, numCols);

    cout << "Enter the row index, column index, and value of each non-zero element:" << endl;
    for (int i = 0; i < numNonZero; ++i) {
        int row, col, value;
        cout << "Element " << i + 1 << ": ";
        cin >> row >> col >> value;
        sm.addValue(row, col, value);
    }

    cout << "\nSparse Matrix:" << endl;
    sm.printMatrix();

    int rowQuery, colQuery;
    cout << "\nEnter the row and column to get the value: ";
    cin >> rowQuery >> colQuery;
    cout << "Value at (" << rowQuery << ", " << colQuery << "): " << sm.getValue(rowQuery, colQuery) << endl;

    return 0;
}
