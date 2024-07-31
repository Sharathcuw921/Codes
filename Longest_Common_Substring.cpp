#include <iostream>
#include <cstring>
using namespace std;

void printTable(int** table, int m, int n) {
    cout << "Lookup Table:" << endl;
    for (int i = 0; i <= m; ++i) {
        for (int j = 0; j <= n; ++j) {
            cout << table[i][j] << " ";
        }
        cout << endl;
    }
}

void printLongestCommonSubstring(char* X, int endIndex, int maxLength) {
    cout << "Longest Common Substring: ";
    for (int i = endIndex - maxLength + 1; i <= endIndex; ++i) {
        cout << X[i];
    }
    cout << endl;
}

int LCSubStr(char* X, char* Y, int m, int n) {
    int** LCSuff = new int*[m + 1];
    for (int i = 0; i <= m; ++i) {
        LCSuff[i] = new int[n + 1];
    }

    int result = 0; 
    int endIndex = 0; 

    for (int i = 0; i <= m; i++) {
        for (int j = 0; j <= n; j++) {
            if (i == 0 || j == 0) {
                LCSuff[i][j] = 0;
            } else if (X[i - 1] == Y[j - 1]) {
                LCSuff[i][j] = LCSuff[i - 1][j - 1] + 1;
                if (LCSuff[i][j] > result) {
                    result = LCSuff[i][j];
                    endIndex = i - 1;
                }
            } else {
                LCSuff[i][j] = 0;
            }
        }
    }

    printTable(LCSuff, m, n);
    printLongestCommonSubstring(X, endIndex, result);

    for (int i = 0; i <= m; ++i) {
        delete[] LCSuff[i];
    }
    delete[] LCSuff;

    return result;
}

int main() {
    string input1, input2;
    cout << "Enter first string: ";
    cin >> input1;
    cout << "Enter second string: ";
    cin >> input2;

    if (input1.length() != input2.length()) {
        cout << "The strings must be of identical length." << endl;
        return 1;
    }

    int m = input1.length();
    int n = input2.length();

    char* X = new char[m + 1];
    char* Y = new char[n + 1];
    strcpy(X, input1.c_str());
    strcpy(Y, input2.c_str());

    cout << "Length of Longest Common Substring is " << LCSubStr(X, Y, m, n) << endl;

    delete[] X;
    delete[] Y;

    return 0;
}

