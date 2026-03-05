#include <iostream>
#include <fstream>
#include <string>
#include <vector>

using namespace std;

int main(int argc, char *argv[])
{
    // Check for one argument
    if (argc != 2)
    {
        cerr << "Usage: ./proj2 <image_prefix>" << endl;
        return 1;
    }

    // Get the prefix
    string prefix = argv[1];

    string mazeNum = prefix + ".txt";

    ifstream file(mazeNum);

    if (!file.is_open())
    {
        cerr << "Error: Could not open " << mazeNum << endl;
        return 1;
    }

    
    int width, height;

    file >> width >> height;

    cout << "Opened: " << mazeNum << endl;
    cout << "Size: " << width << " x " << height << endl;

    file.close();

    return 0;
}