#include <iostream>
#include <vector>
#pragma GCC optimize("Ofast") //Optimization flag
#pragma GCC target("avx,avx2,fma") //Optimization flag
using namespace std;

// Constants used for a 16x16 Sudoku
const int N = 16;
const int rootN = 4;

// Function to count the number of empty cells in the Sudoku grid
int EmptyCells(vector<vector<int>>& sudokuGrid) {
    // Initialize a variable to keep track of the count of empty cells
    int numOfZero = 0;

    // Loop through rows (i) and columns (j) of the Sudoku grid
    for (int i = 0; i <= (N - 1); i++) {
        for (int j = 0; j <= (N - 1); j++) {
            // Check if the value in the current cell is equal to 0 (empty)
            if (sudokuGrid[i][j] == 0)
                // If it's empty, increment the count of empty cells
                numOfZero++;
        }
    }

    // Return the total count of empty cells in the Sudoku grid
    return numOfZero;
}

/* Function to check for clashes at a given cell:
 It checks if a number you want to put in a specific spot in the puzzle would cause a problem. 
 It looks to see if the number is already in the same row, column, or small 4x4 box.*/
bool ClashCheck(vector<vector<int>>& sudokuGrid, int i, int j) {
    // Initialize a boolean variable to track if there's a conflict (initially set to false)
    bool clash = false;
    
    // Retrieve the value in the current cell and store it in variable k
    int k = sudokuGrid[i][j];
    
    // Check for conflicts in the same row and column
    for (int a = 0; a <= (N - 1); a++) {
        // Check if there's a cell in the same row with the same value (k)
        if (sudokuGrid[i][a] == k && a != j) {
            // If such a cell is found, set the clash flag to true
            clash = true;
        }
        // Check if there's a cell in the same column with the same value (k)
        if (sudokuGrid[a][j] == k && a != i) {
            // If such a cell is found, set the clash flag to true
            clash = true;
        }
    }
    
    // Check for conflicts in the 4x4 box
    // Determine the indices of the top-left cell of the 4x4 box that contains the current cell (i, j)
    int boxi = i / rootN;
    int boxj = j / rootN;
    
    // Iterate through the cells within the same 4x4 box
    for (int a = boxi * rootN; a <= boxi * rootN + (rootN - 1); a++) {
        for (int b = boxj * rootN; b <= boxj * rootN + (rootN - 1); b++) {
            // Check if there's a cell within the same box with the same value (k)
            if (sudokuGrid[a][b] == k && (a != i || b != j)) {
                // If such a cell is found, set the clash flag to true
                clash = true;
            }
        }
    }
    
    // Return true if a conflict was found, otherwise return false
    return clash;
}

/* Function to perform Naked Single technique:
It tries to fill in the empty spots one by one, 
considering which number could go in each empty spot based on the numbers already in the puzzle. 
If there's only one possible number, it fills it in.
*/
int NakedSingle(vector<vector<int>>& sudokuGrid) {
    // Initialize a variable to keep track of the number of empty cells in the previous iteration
    int prevEmptyCells = 0;

    // Continue the loop until no more progress can be made (no change in the number of empty cells)
    while (prevEmptyCells != EmptyCells(sudokuGrid)) {
        // Update the previous empty cell count to the current count
        prevEmptyCells = EmptyCells(sudokuGrid);

        // Loop through all rows and columns in the Sudoku grid
        for (int i = 0; i < N; i++) {
            for (int j = 0; j < N; j++) {
                int validGuessCounter = 0;  // Counter for possible valid guesses
                int value = 0;         // Variable to store the valid guess

                // Check if the current cell is empty
                if (sudokuGrid[i][j] == 0) {
                    
                    // Try each number (1 to N) in the empty cell and check for conflicts
                    for (int guess = 1; guess <= N; guess++) {
                        // Set the current empty cell with a guess
                        sudokuGrid[i][j] = guess;

                        // Check if the guess doesn't cause any conflicts (using ClashCheck function)
                        if (!ClashCheck(sudokuGrid, i, j)) {
                            validGuessCounter++;  // Increment the count of valid guesses
                            value = guess;    // Store the valid guess
                        }
                    }
                    
                    // If there's a unique valid guess, set it as the cell's value
                    if (validGuessCounter != 1) {
                        sudokuGrid[i][j] = 0;  // Clear the cell if there is no unique valid guess
                    } else {
                        sudokuGrid[i][j] = value;  // Set the cell value to the unique valid guess
                    }
                }
            }
        }
    }
    return 0;  // Return 0 to indicate completion of the Naked Single technique
}

/* Function to find hidden singles in columns:
 It checks each column to see if there's only one spot 
 where a specific number could go without causing a problem. 
 If so, it fills in that spot.
*/
void ColumnHiddenSingle(vector<vector<int>>& sudokuGrid) {
    int z = 0;

    // Continue the loop until no more progress can be made (no change in the number of empty cells)
    while (z != EmptyCells(sudokuGrid)) {
        // Update z with the current count of empty cells
        z = EmptyCells(sudokuGrid);

        // Loop through all rows in the Sudoku grid
        for (int i = 0; i < N; i++) {
            // Loop through all possible numbers (1 to N)
            for (int k = 1; k <= N; k++) {
                int col = 0;    // Variable to store the column of a possible hidden single
                int numPos = 0; // Counter for possible valid positions for the number k in the row

                // Loop through all columns in the row
                for (int j = 0; j < N; j++) {
                    // Check if the cell is empty
                    if (sudokuGrid[i][j] == 0) {
                        // Try setting the current empty cell with the number k
                        sudokuGrid[i][j] = k;

                        // Check if setting the cell with the number k causes any conflicts
                        if (!ClashCheck(sudokuGrid, i, j)) {
                            numPos++; // Increment the count of valid positions for the number k
                            col = j;  // Store the column where a valid position was found
                        }

                        // Reset the cell back to empty (0)
                        sudokuGrid[i][j] = 0;
                    }
                }

                // If there's a unique valid position for the number k in the column, set it as the cell's value
                if (numPos == 1) {
                    sudokuGrid[i][col] = k;
                }
            }
        }
    }
}

// Function to find hidden singles in rows, similar to the CulumnHiddenSingle.
void RowHiddenSingle(vector<vector<int>>& sudokuGrid) {
    int z = 0;

    // Continue the loop until no more progress can be made (no change in the number of empty cells)
    while (z != EmptyCells(sudokuGrid)) {
        // Update z with the current count of empty cells
        z = EmptyCells(sudokuGrid);

        // Loop through all columns in the Sudoku grid
        for (int j = 0; j < N; j++) {
            // Loop through all possible numbers (1 to N)
            for (int k = 1; k <= N; k++) {
                int row = 0;       // Variable to store the row of a possible hidden single
                int numPos = 0;    // Counter for possible valid positions for the number k in the column

                // Loop through all rows in the column
                for (int i = 0; i < N; i++) {
                    // Check if the cell is empty
                    if (sudokuGrid[i][j] == 0) {
                        // Try setting the current empty cell with the number k
                        sudokuGrid[i][j] = k;

                        // Check if setting the cell with the number k causes any conflicts
                        if (!ClashCheck(sudokuGrid, i, j)) {
                            numPos++; // Increment the count of valid positions for the number k
                            row = i;  // Store the row where a valid position was found
                        }

                        // Reset the cell back to empty (0)
                        sudokuGrid[i][j] = 0;
                    }
                }

                // If there's a unique valid position for the number k in the row, set it as the cell's value
                if (numPos == 1) {
                    sudokuGrid[row][j] = k;
                }
            }
        }
    }
}

/* Function to find hidden singles in boxes:
It looks at each small 4x4 box and tries to find a spot 
where a specific number can only go in one place. 
If it finds one, it fills it in.*/
void BoxHiddenSingle(vector<vector<int>>& sudokuGrid) {
    int z = 0;

    // Continue the loop until no more progress can be made (no change in the number of empty cells)
    while (z != EmptyCells(sudokuGrid)) {
        // Update z with the current count of empty cells
        z = EmptyCells(sudokuGrid);

        // Loop through each box (a 4x4 box) in the Sudoku grid
        for (int boxi = 0; boxi < rootN; boxi++) {
            for (int boxj = 0; boxj < rootN; boxj++) {
                // Loop through all possible numbers (1 to N)
                for (int k = 1; k <= N; k++) {
                    int row = 0;    // Variable to store the row of a possible hidden single
                    int col = 0;    // Variable to store the column of a possible hidden single
                    int numPos = 0; // Counter for possible valid positions for the number k in the box

                    // Loop through all cells within the current box
                    for (int a = boxi * rootN; a < boxi * rootN + rootN; a++) {
                        for (int b = boxj * rootN; b < boxj * rootN + rootN; b++) {
                            // Check if the cell is empty
                            if (sudokuGrid[a][b] == 0) {
                                // Try setting the current empty cell with the number k
                                sudokuGrid[a][b] = k;

                                // Check if setting the cell with the number k causes any conflicts
                                if (!ClashCheck(sudokuGrid, a, b)) {
                                    numPos++; // Increment the count of valid positions for the number k
                                    row = a;  // Store the row where a valid position was found
                                    col = b;  // Store the column where a valid position was found
                                }

                                // Reset the cell back to empty (0)
                                sudokuGrid[a][b] = 0;
                            }
                        }
                    }

                    // If there's a unique valid position for the number k in the box, set it as the cell's value
                    if (numPos == 1) {
                        sudokuGrid[row][col] = k;
                    }
                }
            }
        }
    }
}

/* Function to find hidden singles:
This function combines the strategies from ColumnHiddenSingle, 
RowHiddenSingle, and BoxHiddenSingle to fill in more empty spots in the Sudoku puzzle.*/
void HiddenSingle(vector<vector<int>>& sudokuGrid) {
    ColumnHiddenSingle(sudokuGrid); // Find hidden singles in columns
    RowHiddenSingle(sudokuGrid);    // Find hidden singles in rows
    BoxHiddenSingle(sudokuGrid);    // Find hidden singles in boxes
}

//This function keeps using the strategies from NakedSingle and HiddenSingle until it can't fill in any more spots using these methods.
void Singles(vector<vector<int>>& sudokuGrid) {
    int i = 1; // Initialize i to 1

    // Continue the loop until no more progress can be made (no change in the number of empty cells)
    while (i != EmptyCells(sudokuGrid)) {
        i = EmptyCells(sudokuGrid); // Update i with the current count of empty cells

        // Apply two solving techniques: Naked Single and Hidden Single
        NakedSingle(sudokuGrid);   // Apply Naked Single technique
        HiddenSingle(sudokuGrid);  // Apply Hidden Single technique
    }
}

// Function checks if the puzzle is completely filled yet. If not, it helps keep track of which spot to work on next.
bool sudokuCheck(vector<vector<int>>& sudokuGrid, int& row, int& col) {
    for (row; row < N; ++row) { // Loop through rows
        for (col = 0; col < N; ++col) { // Loop through columns within each row
            if (sudokuGrid[row][col] == 0) { // Check if a cell is empty (has a value of 0)
                return false; // If an empty cell is found, the Sudoku is not complete, so return false
            }
        }
    }
    return true; // If no empty cells are found in the entire grid, the Sudoku is complete, so return true
}

/* Function checks if it's safe to put a number in a specific spot without breaking the Sudoku rules,
 like having the same number in the same row, column, or small box.*/
bool IsSafe(vector<vector<int>>& sudokuGrid, int row, int col, int num) {
    // Check for conflicts in the same row and same column
    for (int i = 0; i < N; ++i) {
        if (sudokuGrid[row][i] == num || sudokuGrid[i][col] == num) {
            // If the number num is already in the same row or same column, it's not safe
            return false;
        }
    }
    
    // Calculate the top-left cell indices of the 4x4 box containing the current cell
    int boxrow = row - row % rootN;
    int boxcol = col - col % rootN;

    // Check for conflicts in the 4x4 box
    for (int r = 0; r < rootN; ++r) {
        for (int c = 0; c < rootN; ++c) {
            if (sudokuGrid[r + boxrow][c + boxcol] == num) {
                // If the number num is already in the 4x4 box, it's not safe
                return false;
            }
        }
    }

    // If num doesn't conflict with the row, column, or 4x4 box, it's safe to place
    return true;
}

/* Function tries different numbers in different spots, and if it reaches a dead end, 
it goes back and tries a different number until it finds a solution or determines there's no solution.*/
bool SolveSudoku(vector<vector<int>>& sudokuGrid, int row) {
    int col;

    // Check if the Sudoku is completed up to the current row
    if (sudokuCheck(sudokuGrid, row, col)) {
        return true; // If completed, return true to indicate success
    }

    // Loop through numbers from 1 to N and try to place them in the current cell
    for (int num = 1; num <= N; ++num) {
        if (IsSafe(sudokuGrid, row, col, num)) {
            sudokuGrid[row][col] = num; // Place the number in the current cell

            // Recursively attempt to solve the Sudoku grid with the new number placed
            if (SolveSudoku(sudokuGrid, row)) {
                return true; // If the recursive call succeeds, return true to propagate success
            }

            sudokuGrid[row][col] = 0; // If the recursive call fails, backtrack by resetting the cell to 0
        }
    }

    return false; // If no number can be placed in the current cell, return false to trigger backtracking
}

// Once the puzzle is solved (or determined to be unsolvable), this function shows the solved Sudoku on the screen.
void printSudoku(vector<vector<int>>& sudokuGrid) {
    for (int r = 0; r < N; ++r) {
        if (sudokuGrid[r][0] > 9) {
            char out = (char)(sudokuGrid[r][0] + 55);
            cout << out;
        } else {
            cout << sudokuGrid[r][0];
        }

        // Loop through columns within the row
        for (int c = 1; c < N; ++c) {
            if (sudokuGrid[r][c] > 9) {
                char out = (char)(sudokuGrid[r][c] + 55);
                cout << ' ' << out;
            } else {
                cout << ' ' << sudokuGrid[r][c];
            }
        }

        cout << endl; // Move to the next line after printing all columns in the current row
    }
}

/*sets up the puzzle, uses the strategies to fill in spots, 
and if needed, it uses the backtracking method to solve the puzzle completely.
 Then, it displays the solution or tells you if there's no solution.
*/
int main() {
    vector<vector<int>> sudokuGrid(N, vector<int>(N, 0)); // Initializing Sudoku as an empty vector

    // Input Sudoku values
    for (int row = 0; row < N; ++row) {
        for (int col = 0; col < N; ++col) {
            char input;
            cin >> input;
            int num = input;
            
            // Convert input characters to integers
            if (num < 65) {
                sudokuGrid[row][col] = num - 48;
            } else {
                sudokuGrid[row][col] = num - 55;
            }
        }
    }

    // Apply solving techniques
    Singles(sudokuGrid);
    
    // Solve the Sudoku using backtracking
    if (SolveSudoku(sudokuGrid, 0)) {
        // Print the solved Sudoku
        printSudoku(sudokuGrid);
    } else {
        cout << "No Solution" << endl;
    }

    return 0;
}
