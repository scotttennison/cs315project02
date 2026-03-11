// Scott Tennison
// Project 2: Maze Solver
// Reads a maze from a file, validates it, and solves it using BFS (queue).
// Prints the original maze, then the solved maze with '-' marking the path.

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <queue>

using namespace std;

// Prints every row of the maze to the screen
void printMaze(vector<string> maze)
{
    for (int i = 0; i < maze.size(); i++)
    {
        cout << maze[i] << endl;
    }
}

// Solves the maze using BFS (Breadth-First Search with a queue).
// Returns true if a solution was found, false if no solution exists.
// If solved, replaces '.' along the solution path with '-' directly in mazeRows.
bool solveMaze(vector<string> &mazeRows, int width, int height)
{
    // Queue holds positions (row, col) to explore next
    queue<pair<int, int>> q;

    // Find the starting position 'S' in the maze
    int startRow = 0, startCol = 0;
    for (int i = 0; i < mazeRows.size(); i++)
    {
        for (int j = 0; j < width; j++)
        {
            if (mazeRows[i][j] == 'S')
            {
                startRow = i;
                startCol = j;
            }
        }
    }

    // Add the start position to the queue to begin BFS
    q.push(make_pair(startRow, startCol));

    // Track which cells we've already visited so we don't loop
    vector<vector<bool>> visited(height, vector<bool>(width, false));

    // Track where we came from for each cell so we can trace back the path
    // parent[r][c] = the cell we came from to reach (r, c)
    vector<vector<pair<int, int>>> parent(height, vector<pair<int, int>>(width, {-1, -1}));

    visited[startRow][startCol] = true;

    bool solved = false;

    // Keep exploring until the queue is empty (no more positions to check)
    while (!q.empty())
    {
        // Grab the next position to explore from the front of the queue
        pair<int, int> current = q.front();
        q.pop();
        int r = current.first;
        int c = current.second;

        // If we reached the End 'E', trace back the path using parent pointers
        if (mazeRows[r][c] == 'E')
        {
            // Walk backwards from E to S, marking each step with '-'
            while (mazeRows[r][c] != 'S')
            {
                if (mazeRows[r][c] == '.')
                {
                    mazeRows[r][c] = '-';
                }
                // Move to the cell we came from
                pair<int, int> p = parent[r][c];
                r = p.first;
                c = p.second;
            }
            solved = true;
            break;
        }

        // Check all 4 neighbors (up, down, left, right)
        // For each: make sure it's in bounds, not visited, and not a wall

        // Up
        if (r - 1 >= 0 && !visited[r - 1][c] && mazeRows[r - 1][c] != '#')
        {
            q.push(make_pair(r - 1, c));
            visited[r - 1][c] = true;
            parent[r - 1][c] = make_pair(r, c);
        }
        // Down
        if (r + 1 < height && !visited[r + 1][c] && mazeRows[r + 1][c] != '#')
        {
            q.push(make_pair(r + 1, c));
            visited[r + 1][c] = true;
            parent[r + 1][c] = make_pair(r, c);
        }
        // Left
        if (c - 1 >= 0 && !visited[r][c - 1] && mazeRows[r][c - 1] != '#')
        {
            q.push(make_pair(r, c - 1));
            visited[r][c - 1] = true;
            parent[r][c - 1] = make_pair(r, c);
        }
        // Right
        if (c + 1 < width && !visited[r][c + 1] && mazeRows[r][c + 1] != '#')
        {
            q.push(make_pair(r, c + 1));
            visited[r][c + 1] = true;
            parent[r][c + 1] = make_pair(r, c);
        }
    }
    return solved;
}

// Validates the maze contents after reading.
// Checks: correct number of rows, correct row width,
// only legal characters, and exactly one S and one E.
// Prints a specific error to cerr and returns false if anything is wrong.
bool validateMaze(vector<string> &mazeRows, int width, int height)
{
    // Check that the number of rows matches the declared height
    if (mazeRows.size() != height)
    {
        cerr << "Error: Rows don't equal height" << endl;
        return false;
    }

    int numS = 0, numE = 0;

    // Check each row for correct width and valid characters
    for (int i = 0; i < mazeRows.size(); i++)
    {
        // Each row must be exactly 'width' characters wide
        if (mazeRows[i].length() != width)
        {
            cerr << "Error: Columns don't equal width" << endl;
            return false;
        }

        // Each character must be one of: # . S E
        for (int j = 0; j < width; j++)
        {
            if (mazeRows[i][j] != '#' && mazeRows[i][j] != '.' &&
                mazeRows[i][j] != 'S' && mazeRows[i][j] != 'E')
            {
                cerr << "Error: Invalid character" << endl;
                return false;
            }
            if (mazeRows[i][j] == 'S')
                numS++;
            if (mazeRows[i][j] == 'E')
                numE++;
        }
    }

    // There must be exactly one start and one end
    if (numS != 1)
    {
        cerr << "Error: Must have exactly one S" << endl;
        return false;
    }
    if (numE != 1)
    {
        cerr << "Error: Must have exactly one E" << endl;
        return false;
    }

    return true;
}

int main(int argc, char *argv[])
{
    // Must provide exactly one argument: the maze filename
    if (argc != 2)
    {
        cerr << "Usage: ./proj2 <mazefile>" << endl;
        return 1;
    }

    // The maze filename is the first (and only) argument
    string filename = argv[1];

    // Try to open the file
    ifstream file(filename);
    if (!file.is_open())
    {
        cerr << "Error: Could not open " << filename << endl;
        return 1;
    }

    // Read the dimensions from the first line of the file
    int width, height;
    if (!(file >> width >> height))
    {
        cerr << "Error: Could not read width and height" << endl;
        return 1;
    }

    // Dimensions must be positive
    if (width <= 0 || height <= 0)
    {
        cerr << "Error: Width and height must be positive integers" << endl;
        return 1;
    }

    // Read the remaining lines as maze rows, skipping any blank lines
    vector<string> mazeRows;
    string row;
    while (getline(file, row))
    {
        if (!row.empty())
        {
            mazeRows.push_back(row);
        }
    }

    file.close();

    // Validate the maze contents before trying to solve
    if (!validateMaze(mazeRows, width, height))
        return 1;

    // Print the original maze, then a blank line
    printMaze(mazeRows);
    cout << endl;

    // Try to solve the maze and print the result
    bool solved = solveMaze(mazeRows, width, height);
    if (solved)
        printMaze(mazeRows);
    else
        cout << "This maze has no solution." << endl;

    return 0;
}