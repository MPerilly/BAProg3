#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
using namespace std;

vector<vector<int>> doSolveAlignment(string s1, string s2);
vector<vector<int>> initialize_DPT(size_t l1, size_t l2);
bool checkMatch(string s1, string s2, int i1, int i2);
int solveAlignment(string s1, string s2);

int main() {
    string s1 = "";
    string s2 = "";

    int result = solveAlignment(s1, s2);

    cout << to_string(result);

    return 0;
}

int solveAlignment(string s1, string s2){
    vector<vector<int>> DPTable = doSolveAlignment(s1, s2);
    // Return optimal value
    return DPTable[s1.length() - 1][s2.length() - 1];
}

vector<vector<int>> doSolveAlignment(string s1, string s2) {
    // Declare table of best alignments
    const int MATCH = 2;
    const int PENALTY = -1;

    vector<vector<int>> table = initialize_DPT(s1.length(), s2.length());
    if (checkMatch(s1, s2, 0, 0)) table[0][0] = MATCH;
    else table[0][0] = PENALTY;

    for (int i = 1; i < s1.length(); i++){
        for (int j = 1; j < s2.length(); j++){
            int diag = PENALTY;
            int down = PENALTY;
            int right = PENALTY;
            if (checkMatch(s1, s2, i, j)) diag = MATCH;
            diag += table[i - 1][j - 1];
            down += table[i - 1][j];
            right += table[i][j - 1];

            table[i][j] = max({diag, down, right});
        }
    }
    return table;
}


vector<vector<int>> initialize_DPT(size_t l1, size_t l2){
    vector<vector<int>> table (l1, vector<int> (l2, 0));
    if (l1 == l2){
        for (int i = 1; i < l1; i++) table[0][i] = table[i][0] = i * -1;
    }
    else if (l1 > l2){
        for (int i = 1; i < l2; i++) table[0][i] = table[i][0] = i * -1;
        for (int i = l2; i < l1; i++) table[i][0] = i * -1;
    }
    else {
        for (int i = 1; i < l1; i++) table[0][i] = table[i][0] = i * -1;
        for (int i = l1; i < l2; i++) table[0][i] = i * -1;
    }
    return table;
}

bool checkMatch(string s1, string s2, int i1, int i2) {
    char check1 = s1[i1];
    char check2 = s2[i2];
    return !(check1 - check2);
}
