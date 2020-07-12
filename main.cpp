#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
using namespace std;

long MOD(long l);

int main() {
    const long MATCH = 2;
    const long PENALTY = -1;

    string s1;
    string s2;
    cin >> s1;
    cin >> s2;

    // Initialize DP Table:
    const size_t s1len = s1.length();
    const size_t s2len = s1.length();
    const size_t DPTable_rows = s1len + 1;
    const size_t DPTable_cols = s1len + 1;

    static vector<vector<long>> DPTable (DPTable_rows, vector<long> (DPTable_cols, 0));
    // Fill first column and row with proper values:
    if (s1len == s2len){
        for (long i = 1; i < DPTable_rows; i++) DPTable[0][i] = DPTable[i][0] = i * PENALTY;
    }
    else if (s1len > s2len){
        for (long i = 1; i < DPTable_cols; i++) DPTable[0][i] = DPTable[i][0] = i * PENALTY;
        for (long i = DPTable_cols; i < DPTable_rows; i++) DPTable[i][0] = i * PENALTY;
    }
    else {
        for (long i = 1; i < DPTable_rows; i++) DPTable[0][i] = DPTable[i][0] = i * PENALTY;
        for (long i = DPTable_rows; i < DPTable_cols; i++) DPTable[0][i] = i * PENALTY;
    }

    // Fill remaining DPTable:
    for (long row = 1; row < DPTable_rows; row++){
        for (long col = 1; col < DPTable_cols; col++){
            // Initialize possible additions:
            long diag = PENALTY;
            long down = PENALTY;
            long right = PENALTY;

            // If there is a match, it will only be on the diagonal, so update if applicable:
            long s1ind = row - 1;
            long s2ind = col - 1;
            if (!(s1[s1ind] - s2[s2ind])) diag = MATCH;

            // Add to best scores of previous alignments:
            diag += DPTable[row - 1][col - 1];
            down += DPTable[row - 1][col];
            right += DPTable[row][col - 1];

            // Find the best score for the alignment at this cell:
            long opt_value = max({diag, down, right});
            // Insert the score to the table:
            DPTable[row][col] = opt_value;
        }
    }

    // Count number of paths:

    // Initialize traceback table:
    vector<vector<long>> traceback(DPTable_rows, vector<long>(DPTable_cols, 0));
    traceback[0][0] = 1;

    // Fill Traceback:
    for (long row = 1; row < DPTable_rows; row++) {
        for (long col = 1; col < DPTable_cols; col++) {
            // Initialize optimal values:
            long a1 = DPTable[row - 1][col] + PENALTY;
            long a2 = DPTable[row][col - 1] + PENALTY;
            long a3;

            // Check for a match on the diagonal & adjust accordingly:
            long s1ind = row - 1;
            long s2ind = col - 1;
            if (!(s1[s1ind] - s2[s2ind])) a3 = DPTable[row - 1][col - 1] + MATCH;
            else a3 = DPTable[row - 1][col - 1] + PENALTY;

            // Sum per class psuedocode:
            if (DPTable[row][col] == a1) {
                long value = traceback[row][col] + traceback[row - 1][col];
                traceback[row][col] = MOD(value);
            }
            if (DPTable[row][col] == a2) {
                long value = traceback[row][col] + traceback[row][col - 1];
                traceback[row][col] = MOD(value);
            }
            if (DPTable[row][col] == a3) {
                long value = traceback[row][col] + traceback[row - 1][col - 1];
                traceback[row][col] = MOD(value);
            }
        }
    }

    // Print results:
    cout << to_string(DPTable[s1len][s2len]) << endl;
    cout << to_string(traceback[s1len][s2len]) << endl;

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

