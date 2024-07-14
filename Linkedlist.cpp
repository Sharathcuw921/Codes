#include <iostream>
using namespace std;

struct Node {
    int row;
    int col;
    int value;
    Node* next;
};

class SparseMatrix {
private:
    Node* head;
    int numRows;
    int numCols;

public:
    SparseMatrix(int rows, int cols) : numRows(rows), numCols(cols), head(nullptr) {}

    ~SparseMatrix() {
        Node* current = head;
        while (current != nullptr) {
            Node* nextNode = current->next;
            delete current;
            current = nextNode;
        }
    }

    void addValue(int row, int col, int value) {
        if (value != 0) {
            Node* newNode = new Node();
            newNode->row = row;
            newNode->col = col;
            newNode->value = value;
            newNode->next = head;
            head = newNode;
        }
    }

    int getValue(int row, int col) const {
        Node* current = head;
        while (current != nullptr) {
            if (current->row == row && current->col == col) {
                return current->value;
            }
            current = current->next;
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

    void printCompactMatrix() const {
        Node* current = head;
        while (current != nullptr) {
            cout << "Row: " << current->row << ", Col: " << current->col << ", Value: " << current->value << endl;
            current = current->next;
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

    cout << "\nCompact Representation of Sparse Matrix:" << endl;
    sm.printCompactMatrix();

    int rowQuery, colQuery;
    cout << "\nEnter the row and column to get the value: ";
    cin >> rowQuery >> colQuery;
    cout << "Value at (" << rowQuery << ", " << colQuery << "): " << sm.getValue(rowQuery, colQuery) << endl;

    return 0;
}
