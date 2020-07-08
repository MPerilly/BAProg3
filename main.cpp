#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
using namespace std;

vector<vector<int>> doSolveAlignment(string s1, string s2);
vector<vector<int>> initialize_DPT(size_t l1, size_t l2, const int penalty);
bool checkMatch(string s1, string s2, int i1, int i2);
int solveAlignment(string s1, string s2);

int main() {
    string s1 = "ACGCCG";
    string s2 = "ACCACG";

    int result = solveAlignment(s1, s2);

    cout << to_string(result);

    return 0;
}

int solveAlignment(string s1, string s2){
    vector<vector<int>> DPTable = doSolveAlignment(s1, s2);
    // Return optimal value
    return DPTable[s1.length()][s2.length()];
}

vector<vector<int>> doSolveAlignment(string s1, string s2) {
    // Declare table of best alignments
    const int MATCH = 2;
    const int PENALTY = -1;

    vector<vector<int>> table = initialize_DPT(s1.length(), s2.length(), PENALTY);
    //if (checkMatch(s1, s2, 0, 0)) table[0][0] = MATCH;
    //else table[0][0] = PENALTY;
    int countPaths = 0;

    const size_t matrix_rows = table.size();
    const size_t matrix_cols = table[0].size();

    for (int i = 1; i < matrix_rows; i++){
        for (int j = 1; j < matrix_cols; j++){
            int diag = PENALTY;
            int down = PENALTY;
            int right = PENALTY;
            if (checkMatch(s1, s2, i, j)) diag = MATCH;
            diag += table[i - 1][j - 1];
            down += table[i - 1][j];
            right += table[i][j - 1];

            int opt_value = max({diag, down, right});

            if ((diag == opt_value && down == opt_value) ||
                (diag == opt_value && right == opt_value) ||
                (down == opt_value && right == opt_value)) {
                countPaths++;
            }

            table[i][j] = opt_value;
        }
    }
    cout << to_string(countPaths + 1) + "\n";
    return table;
}


vector<vector<int>> initialize_DPT(size_t l1, size_t l2, const int penalty){
    size_t first_array_len = l1 + 1;
    size_t secnd_array_len = l2 + 1;

    vector<vector<int>> table (first_array_len, vector<int> (secnd_array_len, 0));

    if (l1 == l2){
        for (int i = 1; i < first_array_len; i++) table[0][i] = table[i][0] = i * penalty;
    }
    else if (l1 > l2){
        for (int i = 1; i < secnd_array_len; i++) table[0][i] = table[i][0] = i * penalty;
        for (int i = secnd_array_len; i < first_array_len; i++) table[i][0] = i * penalty;
    }
    else {
        for (int i = 1; i < first_array_len; i++) table[0][i] = table[i][0] = i * penalty;
        for (int i = first_array_len; i < secnd_array_len; i++) table[0][i] = i * penalty;
    }
    return table;
}

bool checkMatch(string s1, string s2, int i1, int i2) {
    char check1 = s1[i1];
    char check2 = s2[i2];
    return !(check1 - check2);
}
