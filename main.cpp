//
// Created by Manju Muralidharan on 11/22/25.
//

#include <iostream>
#include <vector>
#include <cstdlib>
#include <ctime>

using namespace std;

// Directions for DFS (students must use these)
int dr[4] = {-1, 0, 1, 0};
int dc[4] = {0, 1, 0, -1};

// ----------------------------------------------------------
// DO NOT MODIFY: Maze generation
// ----------------------------------------------------------
void generateMaze(vector<vector<int>>& maze, int N, int M) {
    srand(time(0)); // random seeded with the current time
    //
    for (int r = 0; r < N; r++) {
        for (int c = 0; c < M; c++) {
            int roll = rand() % 100;
            maze[r][c] = (roll < 70) ? 0 : 1;   // 0 = open, 1 = wall
        }
    }
}

// ----------------------------------------------------------
// DO NOT MODIFY: Choose a random boundary cell that is open
// ----------------------------------------------------------
pair<int,int> chooseBoundaryCell(const vector<vector<int>>& maze) {
    int N = maze.size();
    int M = maze[0].size();

    while (true) {
        int side = rand() % 4;
        int r, c;

        if (side == 0) {          // top row
            r = 0;
            c = rand() % M;
        } else if (side == 1) {   // bottom row
            r = N - 1;
            c = rand() % M;
        } else if (side == 2) {   // left column
            r = rand() % N;
            c = 0;
        } else {                  // right column
            r = rand() % N;
            c = M - 1;
        }

        if (maze[r][c] == 0) {
            return {r, c};
        }
    }
}

// ----------------------------------------------------------
// DO NOT MODIFY: Print the maze with S and E markers
// ----------------------------------------------------------
void printMaze(const vector<vector<int>>& maze,
               int ent_r, int ent_c,
               int exit_r, int exit_c)
{
    int N = maze.size();
    int M = maze[0].size();

    cout << "\nMaze:\n";
    for (int r = 0; r < N; r++) {
        for (int c = 0; c < M; c++) {
            if (r == ent_r && c == ent_c) {
                cout << "S ";
            } else if (r == exit_r && c == exit_c) {
                cout << "E ";
            } else {
                cout << maze[r][c] << " ";
            }
        }
        cout << "\n";
    }
}

// ----------------------------------------------------------
// DO NOT MODIFY: Print the reconstructed path
// Students must fill parent[][] correctly during DFS
// ----------------------------------------------------------
void printPath(pair<int,int> exitcell,
               const vector<vector<int>>& parent_r,
               const vector<vector<int>>& parent_c,
               int ent_r, int ent_c)
{
    int r = exitcell.first;
    int c = exitcell.second;

    vector<pair<int,int>> path;

    // Walk backward from exit to entrance
    while (!(r == ent_r && c == ent_c)) {
        path.push_back({r, c});
        int pr = parent_r[r][c];
        int pc = parent_c[r][c];
        r = pr;
        c = pc;
    }
    path.push_back({ent_r, ent_c});

    cout << "\nPath from entrance to exit:\n";
    for (int i = path.size() - 1; i >= 0; i--) {
        cout << "(" << path[i].first << ", " << path[i].second << ")\n";
    }
}

// ----------------------------------------------------------
// STUDENTS IMPLEMENT DFS HERE
// Add arguments, return type, and logic
// ----------------------------------------------------------
/*
     * TODO: travers the maze
     * 1) bounds check
     *    - ends search
     * 2) visited check
     *    - ends search
     * 3) wall check
     *    - ends search
     * 4) mark as visited
     * 5) exit check
     *    - path exists
     * 6) look for the next open cell
     *    6a) update parent_r, parent_c
     *    6b) recurse
     * 7) assume there is no path
     *    - ends search
     */
bool dfs(int r, int c, vector<vector<int>>& maze, vector<vector<bool>>& visited, vector<vector<int>>& parent_r, vector<vector<int>>& parent_c, int exit_r, int exit_c) {


    /*
     * 1) bounds check
     *    - check if the current cell is within the bounds of the maze
     */
    if (r < 0 || r >= maze.size() || c < 0 || c >= maze[0].size()) { return false; } // the cell is not within the bound is the maze and therefor cannot be part of the path
    // cout << "Wall (" << r << ", " << c << ")" << endl;


    /*
    * 3) wall check
    *    - check if the current cell is a wall
    */
    if (maze[r][c] == 1) { return false; } // the current cell is a wall and is therefor an invalid path

    /*
     * 2) visited check
     *    - check if the current cell has already been traversed
     */
    if (visited[r][c]) { return false; } // the current cell has already been traversed and should not be considered a possible path


   /*
    * 4) mark as visited
    *    - at this point the cell we are in is: in the maze, not a cell we have visited, and is not a wall... a.k.a. a valid cell
    */
    visited[r][c] = true; // we mark this cell as visited
    // cout << "visited (" << r << ", " << c << ')' << endl;
    /*
     * 5) exit check
     *    - we currently have a cell that is: in the maze, not a wall, has not been visited, and now documented as visited
     */
    if (r == exit_r && c == exit_c) { return true; } // we have found the exit, the path exist

    /*
     * 6) look for the next open cell
     *    - look: LEFT, UP, RIGHT, and DOWN to propagate the search
     */
    for (int d = 0; d < 4; d++) {
         // 6a) update parent_r, parent_c

         // calculate the row and col for the new cell
        int nr = r + dr[d];
        int nc = c + dc[d];

        // 6b) recures
        //     - if the new cell is a valid path, then the search continues
        if (dfs(nr, nc, maze, visited, parent_r, parent_c, exit_r, exit_c)) {
            // update parents
            parent_r[nr][nc] = r;
            parent_c[nr][nc] = c;
            return true;
        } // we found the exit, the path exists

    }

    // 7) assume there is no path
    return false; // there is no valid path
}


// ----------------------------------------------------------
// MAIN PROGRAM (students add DFS calls and logic)
// ----------------------------------------------------------
int main() {
    int N, M;

    cout << "Enter maze dimensions N M: ";
    cin >> N >> M;

    vector<vector<int>> maze(N, vector<int>(M));
    generateMaze(maze, N, M);

    // Pick entrance and exit
    pair<int,int> entrance = chooseBoundaryCell(maze);
    pair<int,int> exitcell = chooseBoundaryCell(maze);

    while (exitcell == entrance) {
        exitcell = chooseBoundaryCell(maze);
    }

    int ent_r = entrance.first;
    int ent_c = entrance.second;
    int exit_r = exitcell.first;
    int exit_c = exitcell.second;

    // Display the maze
    printMaze(maze, ent_r, ent_c, exit_r, exit_c);

    // Students must use these
    vector<vector<bool>> visited(N, vector<bool>(M, false));
    vector<vector<int>> parent_r(N, vector<int>(M, -1));
    vector<vector<int>> parent_c(N, vector<int>(M, -1));

    // ------------------------------------------------------
    // STUDENT WORK:
    // Call your DFS, track visited, and fill parent_r and parent_c
    // ------------------------------------------------------
    bool found = dfs(ent_r, ent_c, maze, visited, parent_r, parent_c, exit_r, exit_c);
    if (found)
        printPath(exitcell, parent_r, parent_c, ent_r, ent_c);
    else
        cout << "\nNo path exists.\n";

    // ------------------------------------------------------
    // STUDENT WORK:
    // If found, print the path
    // ------------------------------------------------------
    // if (found) {
    //     printPath(exitcell, parent_r, parent_c, ent_r, ent_c);
    // } else {
    //     cout << "\nNo path exists.\n";
    // }

    return 0;
}