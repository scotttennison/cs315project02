#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <queue>

using namespace std;

void printMaze(vector<string> maze)
{
    for (int i = 0; i < maze.size(); i++)
    {
        cout << maze[i] << endl;
    }
}

bool solveMaze(vector<string> &mazeRows, int width, int height)
{
    queue<pair<int, int>> q;

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

    q.push(make_pair(startRow, startCol));

    vector<vector<bool>> visited(height, vector<bool>(width, false));
    vector<vector<pair<int, int>>> parent(height, vector<pair<int, int>>(width, {-1, -1}));

    visited[startRow][startCol] = true;

    bool solved = false;

    while (!q.empty())
    {
        pair<int, int> current = q.front();
        q.pop();
        int r = current.first;
        int c = current.second;
        if (mazeRows[r][c] == 'E')
        {
            while (mazeRows[r][c] != 'S')
            {
                if (mazeRows[r][c] == '.')
                {
                    mazeRows[r][c] = '-';
                }
                pair<int, int> p = parent[r][c];
                r = p.first;
                c = p.second;
            }
            solved = true;
            break;
        }
        // Checks up
        if (r - 1 >= 0 && !visited[r - 1][c] && mazeRows[r - 1][c] != '#')
        {
            q.push(make_pair(r - 1, c));
            visited[r - 1][c] = true;
            parent[r - 1][c] = make_pair(r, c);
        }
        // Checks down
        if (r + 1 < height && !visited[r + 1][c] && mazeRows[r + 1][c] != '#')
        {
            q.push(make_pair(r + 1, c));
            visited[r + 1][c] = true;
            parent[r + 1][c] = make_pair(r, c);
        }
        // Checks left
        if (c - 1 >= 0 && !visited[r][c - 1] && mazeRows[r][c - 1] != '#')
        {
            q.push(make_pair(r, c - 1));
            visited[r][c - 1] = true;
            parent[r][c - 1] = make_pair(r, c);
        }
        // Check right
        if (c + 1 < width && !visited[r][c + 1] && mazeRows[r][c + 1] != '#')
        {
            q.push(make_pair(r, c + 1));
            visited[r][c + 1] = true;
            parent[r][c + 1] = make_pair(r, c);
        }
    }
    return solved;
}

bool validateMaze(vector<string> &mazeRows, int width, int height)
{
    if (mazeRows.size() != height)
    {
        cerr << "Error: Rows don't equal height" << endl;
        return false;
    }

    int numS = 0, numE = 0;

    for (int i = 0; i < mazeRows.size(); i++)
    {
        if (mazeRows[i].length() != width)
        {
            cerr << "Error: Columns don't equal width" << endl;
            return false;
        }
        for (int j = 0; j < width; j++)
        {
            if (mazeRows[i][j] != '#' && mazeRows[i][j] != '.' && mazeRows[i][j] != 'S' && mazeRows[i][j] != 'E')
            {
                cerr << "Error: Invalid character" << endl;
                return false;
            }
            if (mazeRows[i][j] == 'S') numS++;
            if (mazeRows[i][j] == 'E') numE++;
        }
    }

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
    // Check for one argument
    if (argc != 2)
    {
        cerr << "Usage: ./proj2 <mazefile>" << endl;
        return 1;
    }

    // Get the prefix
    string prefix = argv[1];

    string filename = prefix;

    ifstream file(filename);

    if (!file.is_open())
    {
        cerr << "Error: Could not open " << filename << endl;
        return 1;
    }

    
    int width, height;

    if(!(file >> width >> height))
    {
        cerr << "Error: Could not read width and height"
             << endl;
        return 1;
    }

    if(width <= 0 || height <= 0)
    {
        cerr << "Error: Width and height must be positive integers"
             << endl;
        return 1;
    }

    vector<string> mazeRows;
    
    string row;

    while (getline(file, row))
    {
        if (!row.empty()){
            mazeRows.push_back(row);
        }
    }

    if (!validateMaze(mazeRows, width, height))
        return 1;

    printMaze(mazeRows);

    cout << endl;

    bool solved = solveMaze(mazeRows, width, height);
    if (solved)
        printMaze(mazeRows);
    else
        cout << "This maze has no solution." << endl;

    file.close();

    return 0;
}


