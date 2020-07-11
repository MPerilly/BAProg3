#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
using namespace std;

// Declare table of best alignments, penalties, etc.
const int MATCH = 2;
const int PENALTY = -1;

int countPaths(vector<vector<string>> dp_trace, size_t i, size_t j);
vector<vector<int>> doSolveAlignment(string s1, string s2);
vector<vector<int>> initialize_DPT(size_t l1, size_t l2, const int penalty);
bool checkMatch(string s1, string s2, int i1, int i2);
int solveAlignment(string s1, string s2);

int main() {
    // Testing:

    string s1;
    string s2;

    cin >> s1;
    cin >> s2;

    int result = solveAlignment(s1, s2);

    cout << to_string(result);

    return 0;
}


// Wrapper function to return score of optimal path:
int solveAlignment(string s1, string s2){
    vector<vector<int>> DPTable = doSolveAlignment(s1, s2);
    // Return optimal value
    return DPTable[s1.length()][s2.length()];
}

int countPaths(vector<vector<string>> dp_trace, size_t i, size_t j) {
    int num_paths = 0;
    if (i == 0 && j == 0) return 1;
    // else if (i == 0 || j == 0) return 0;
    else {
        string optimal = dp_trace[i][j];
        for (int char_at_ind = 0; char_at_ind < optimal.size(); char_at_ind++){
            if (optimal[char_at_ind] == 'd') num_paths += countPaths(dp_trace, i - 1, j - 1);
            if (optimal[char_at_ind] == 'u') num_paths += countPaths(dp_trace, i - 1, j);
            if (optimal[char_at_ind] == 'l') num_paths += countPaths(dp_trace, i, j - 1);
        }

    }

    return num_paths;

}

// Find score of optimal path via DP:
vector<vector<int>> doSolveAlignment(string s1, string s2) {

    // Creates DP table of appropriate size with extra row for alignments against empty strings (values pre-initialized):
    vector<vector<int>> table = initialize_DPT(s1.length(), s2.length(), PENALTY);
    vector<vector<string>> traceback(table.size(), vector<string>(table[0].size()));

    const size_t matrix_rows = table.size();
    const size_t matrix_cols = table[0].size();

    // Fill table with optimal values from initialized values:
    for (int row = 1; row < matrix_rows; row++){
        for (int col = 1; col < matrix_cols; col++){
            // Initialize possible additions:
            int diag = PENALTY;
            int down = PENALTY;
            int right = PENALTY;
            string tb = "";

            // If there is a match, it will only be on the diagonal, so update if applicable:
            if (checkMatch(s1, s2, row - 1, col - 1)) diag = MATCH;

            // Add to best scores of previous alignments:
            diag += table[row - 1][col - 1];
            down += table[row - 1][col];
            right += table[row][col - 1];

            // Find the best score for the alignment at this cell:
            int opt_value = max({diag, down, right});
            if (diag == opt_value) tb += "d";
            if (down == opt_value) tb += "u";
            if (right == opt_value) tb += "l";
            // Insert the score to the table:
            table[row][col] = opt_value;
            traceback[row][col] = tb;
        }
    }

    int num_paths = countPaths(traceback, matrix_rows - 1, matrix_cols - 1);
    cout << to_string(num_paths) << std::endl;
    return table;
}


vector<vector<int>> initialize_DPT(size_t l1, size_t l2, const int penalty){

    // Initializes matrix for alignment. Conditionals check length of strings and fill arrays properly for
    // given lengths.

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
    // Return true if the characters at two indices are the same, false if otherwise
    char check1 = s1[i1];
    char check2 = s2[i2];
    return !(check1 - check2);
}
