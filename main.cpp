#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
using namespace std;

// Declare table of best alignments, penalties, etc.
const long MATCH = 2;
const long PENALTY = -1;

long MOD(long l);
long countPaths(vector<vector<long>>& dp, long i, long j, string& s1, string& s2);
vector<vector<long>> doSolveAlignment(string s1, string s2, vector<vector<long>> table);
vector<vector<long>> initialize_DPT(size_t l1, size_t l2, const long penalty);
bool checkMatch(string& s1, string& s2, long i1, long i2);
long solveAlignment(string s1, string s2);

int main() {
    // Testing:

    string s1;
    string s2;

    cin >> s1;
    cin >> s2;

    solveAlignment(s1, s2);

    return 0;
}

long MOD(long l){
    long check_large = 1000000009;
    if ((l >= check_large) | (l < 0)){
        long out = (l % check_large);
        if (out < 0){
            return ((out + check_large) % check_large);
        }
        return out;
    }
    return l;
}

// Wrapper function to return score of optimal path:
long solveAlignment(string s1, string s2){
    // Creates DP table of appropriate size with extra row for alignments against empty strings (values pre-initialized):
    static vector<vector<long>> DPTable = initialize_DPT(s1.length(), s2.length(), PENALTY);
    DPTable = doSolveAlignment(s1, s2, DPTable);
    // Return optimal value
    return DPTable[s1.length()][s2.length()];
}

long countPaths(vector<vector<long>>& dp, long i, long j, string& s1, string& s2) {

    vector<vector<long>> traceback(dp.size(), vector<long>(dp[0].size(), 0));
    for (long i = 0; i < dp.size(); i++) traceback[i][0] = 1;
    for (long i = 0; i < dp[0].size(); i++) traceback[0][i] = 1;
    // traceback[0][0] = 1;

    for (long row = 1; row < i + 1; row++) {
        for (long col = 1; col < j + 1; col++) {
            long a1 = dp[row - 1][col] + PENALTY;
            long a2 = dp[row][col - 1] + PENALTY;
            long a3;
            if (checkMatch(s1, s2, row - 1, col - 1)) a3 = dp[row - 1][col - 1] + MATCH;
            else a3 = dp[row - 1][col - 1] + PENALTY;

            if (dp[row][col] == a1) {
                long value = traceback[row][col] + traceback[row - 1][col];
                traceback[row][col] = MOD(value);
            }
            if (dp[row][col] == a2) {
                long value = traceback[row][col] + traceback[row][col - 1];
                traceback[row][col] = MOD(value);
            }
            if (dp[row][col] == a3) {
                long value = traceback[row][col] + traceback[row - 1][col - 1];
                traceback[row][col] = MOD(value);
            }
        }
    }
    return traceback[i][j];
}


// Find score of optimal path via DP:
vector<vector<long>> doSolveAlignment(string s1, string s2, vector<vector<long>> table) {

    const long matrix_rows = table.size();
    const long matrix_cols = table[0].size();

    // Fill table with optimal values from initialized values:
    for (long row = 1; row < matrix_rows; row++){
        for (long col = 1; col < matrix_cols; col++){
            // Initialize possible additions:
            long diag = PENALTY;
            long down = PENALTY;
            long right = PENALTY;

            // If there is a match, it will only be on the diagonal, so update if applicable:
            if (checkMatch(s1, s2, row - 1, col - 1)) diag = MATCH;

            // Add to best scores of previous alignments:
            diag += table[row - 1][col - 1];
            down += table[row - 1][col];
            right += table[row][col - 1];

            // Find the best score for the alignment at this cell:
            long opt_value = max({diag, down, right});
            // Insert the score to the table:
            table[row][col] = opt_value;
        }
    }
    long num_paths = countPaths(table, matrix_rows - 1, matrix_cols - 1, s1, s2);
    cout << to_string(table[matrix_rows - 1][matrix_cols - 1]) << std::endl;
    cout << to_string(num_paths) << std::endl;
    return table;
}


vector<vector<long>> initialize_DPT(size_t l1, size_t l2, const long penalty){

    // Initializes matrix for alignment. Conditionals check length of strings and fill arrays properly for
    // given lengths.

    size_t first_array_len = l1 + 1;
    size_t secnd_array_len = l2 + 1;

    vector<vector<long>> table (first_array_len, vector<long> (secnd_array_len, 0));

    if (l1 == l2){
        for (long i = 1; i < first_array_len; i++) table[0][i] = table[i][0] = i * penalty;
    }
    else if (l1 > l2){
        for (long i = 1; i < secnd_array_len; i++) table[0][i] = table[i][0] = i * penalty;
        for (long i = secnd_array_len; i < first_array_len; i++) table[i][0] = i * penalty;
    }
    else {
        for (long i = 1; i < first_array_len; i++) table[0][i] = table[i][0] = i * penalty;
        for (long i = first_array_len; i < secnd_array_len; i++) table[0][i] = i * penalty;
    }
    return table;
}

bool checkMatch(string& s1, string& s2, long i1, long i2) {
    // Return true if the characters at two indices are the same, false if otherwise
    char check1 = s1[i1];
    char check2 = s2[i2];
    return !(check1 - check2);
}