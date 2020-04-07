# 2. Foundations
## 1. Introduction to the C++ Language
```cpp
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>
using std::cout;
using std::ifstream;
using std::istringstream;
using std::string;
using std::vector;

enum class State {kEmpty, kObstacle};

vector<State> ParseLine(string line) {
    istringstream sline(line);
    int n;
    char c;
    vector<State> row;
    while (sline >> n >> c && c == ',') {
      if (n == 0) {
        row.push_back(State::kEmpty);
      } else {
        row.push_back(State::kObstacle);
      }
    }
    return row;
}
vector<vector<State>> ReadBoardFile(string path) {
  ifstream myfile (path);
  vector<vector<State>> board{};
  if (myfile) {
    string line;
    while (getline(myfile, line)) {
      vector<State> row = ParseLine(line);
      board.push_back(row);
    }
  }
  return board;
}
string CellString(State cell) {
  switch(cell) {
    case State::kObstacle: return "⛰️   ";
    default: return "0   "; 
  }
}
void PrintBoard(const vector<vector<State>> board) {
  for (int i = 0; i < board.size(); i++) {
    for (int j = 0; j < board[i].size(); j++) {
      cout << CellString(board[i][j]);
    }
    cout << "\n";
  }
}
int main() {
  auto board = ReadBoardFile("1.board");
  PrintBoard(board);
}
```
## 2. A* Search
### 2.1. Passing by Reference
```cpp
#include <iostream>
using std::cout;


int MultiplyByTwo(int &i) {
    i = 2*i;
    return i;
}

int main() {
    int a = 5;
    cout << "The int a equals: " << a << "\n";
    int b = MultiplyByTwo(a);
    cout << "The int b equals: " << b << "\n";
    cout << "The int a now equals: " << a << "\n";
}
```
### 2.2. `const` vs. `constexpr`
```cpp
#include <iostream>
int main()
{
    int i;
    std::cout << "Enter an integer value for i: ";
    std::cin >> i;
    const int j = i * 2;  // "j can only be evaluated at run time."
                          // "But I promise not to change it after it is initialized."
    constexpr int k = 3;  // "k, in contrast, can be evaluated at compile time."
    std::cout << "j = " << j << "\n";
    std::cout << "k = " << k << "\n";
}
```
### 2.3. Arrays vs. Vectors
Arrays are a lower level data structure than vectors, and can be slightly more efficient, in terms of memory and element access. However, this efficiency comes with a price. Unlike vectors, which can be extended with more elements, arrays have a fixed length. Additionally, arrays may require careful memory management, depending how they are used.
### 2.4. A* Search full Program
```cpp
#include <algorithm>  // for sort
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>
using std::cout;
using std::ifstream;
using std::istringstream;
using std::sort;
using std::string;
using std::vector;
using std::abs;

enum class State {kEmpty, kObstacle, kClosed, kPath, kStart, kFinish};

// directional deltas
const int delta[4][2]{{-1, 0}, {0, -1}, {1, 0}, {0, 1}};


vector<State> ParseLine(string line) {
    istringstream sline(line);
    int n;
    char c;
    vector<State> row;
    while (sline >> n >> c && c == ',') {
      if (n == 0) {
        row.push_back(State::kEmpty);
      } else {
        row.push_back(State::kObstacle);
      }
    }
    return row;
}


vector<vector<State>> ReadBoardFile(string path) {
  ifstream myfile (path);
  vector<vector<State>> board{};
  if (myfile) {
    string line;
    while (getline(myfile, line)) {
      vector<State> row = ParseLine(line);
      board.push_back(row);
    }
  }
  return board;
}


/**
 * Compare the F values of two cells.
 */
bool Compare(const vector<int> a, const vector<int> b) {
  int f1 = a[2] + a[3]; // f1 = g1 + h1
  int f2 = b[2] + b[3]; // f2 = g2 + h2
  return f1 > f2; 
}


/**
 * Sort the two-dimensional vector of ints in descending order.
 */
void CellSort(vector<vector<int>> *v) {
  sort(v->begin(), v->end(), Compare);
}


// Calculate the manhattan distance
int Heuristic(int x1, int y1, int x2, int y2) {
  return abs(x2 - x1) + abs(y2 - y1);
}


/** 
 * Check that a cell is valid: on the grid, not an obstacle, and clear. 
 */
bool CheckValidCell(int x, int y, vector<vector<State>> &grid) {
  bool on_grid_x = (x >= 0 && x < grid.size());
  bool on_grid_y = (y >= 0 && y < grid[0].size());
  if (on_grid_x && on_grid_y)
    return grid[x][y] == State::kEmpty;
  return false;
}


/** 
 * Add a node to the open list and mark it as open. 
 */
void AddToOpen(int x, int y, int g, int h, 
               vector<vector<int>> &openlist,
               vector<vector<State>> &grid) {
  // Add node to open vector, and mark grid cell as closed.
  openlist.push_back(vector<int>{x, y, g, h});
  grid[x][y] = State::kClosed;
}


/** 
 * Expand current nodes's neighbors and add them to the open list.
 */
void ExpandNeighbors(const vector<int> &current, int goal[2], 
                     vector<vector<int>> &openlist, 
                     vector<vector<State>> &grid) {
  // Get current node's data.
  int x = current[0];
  int y = current[1];
  int g = current[2];

  // Loop through current node's potential neighbors.
  for (int i = 0; i < 4; i++) {
    int x2 = x + delta[i][0];
    int y2 = y + delta[i][1];

    // Check that the potential neighbor's x2 and y2 
    // values are on the grid and not closed.
    if (CheckValidCell(x2, y2, grid)) {
      // Increment g value and add neighbor to open list.
      int g2 = g + 1;
      int h2 = Heuristic(x2, y2, goal[0], goal[1]);
      AddToOpen(x2, y2, g2, h2, openlist, grid);
    }
  }
}


/** 
 * Implementation of A* search algorithm
 */
vector<vector<State>> Search(vector<vector<State>> grid, int init[2], int goal[2]) {
  // Create the vector of open nodes.
  vector<vector<int>> open {};
  
  // Initialize the starting node.
  int x = init[0];
  int y = init[1];
  int g = 0;
  int h = Heuristic(x, y, goal[0],goal[1]);
  AddToOpen(x, y, g, h, open, grid);

  while (open.size() > 0) {
    // Get the next node
    CellSort(&open);
    auto current = open.back();
    open.pop_back();
    x = current[0];
    y = current[1];
    grid[x][y] = State::kPath;

    // Check if we're done.
    if (x == goal[0] && y == goal[1]) {
      grid[init[0]][init[1]] = State::kStart;
      grid[goal[0]][goal[1]] = State::kFinish;
      return grid;
    }
    
    // If we're not done, expand search to current node's neighbors.
    ExpandNeighbors(current, goal, open, grid);
  }
  
  // We've run out of new nodes to explore and haven't found a path.
  cout << "No path found!" << "\n";
  return std::vector<vector<State>>{};
}


string CellString(State cell) {
  switch(cell) {
    case State::kObstacle: return "⛰️   ";
    case State::kPath: return "🚗   ";
    case State::kStart: return "🚦   ";
    case State::kFinish: return "🏁   ";
    default: return "0   "; 
  }
}


void PrintBoard(const vector<vector<State>> board) {
  for (int i = 0; i < board.size(); i++) {
    for (int j = 0; j < board[i].size(); j++) {
      cout << CellString(board[i][j]);
    }
    cout << "\n";
  }
}

#include "test.cpp"

int main() {
  int init[2]{0, 0};
  int goal[2]{4, 5};
  auto board = ReadBoardFile("1.board");
  auto solution = Search(board, init, goal);
  PrintBoard(solution);
  // Tests
  TestHeuristic();
  TestAddToOpen();
  TestCompare();
  TestSearch();
  TestCheckValidCell();
  TestExpandNeighbors();
}
```
## 3. Writing Multifile Programs
### 3.1. Include Guard
there is a preprocessor directive:
```cpp
#ifndef HEADER_EXAMPLE_H
#define HEADER_EXAMPLE_H
```
at the top of the header, along with an `#endif` at the end. This is called an "include guard". Since the header will be included into another file, and #include just pastes contents into a file, the include guard prevents the same file from being pasted multiple times into another file. This might happen if multiple files include the same header, and then are all included into the same main.cpp, for example. The ifndef checks if HEADER_EXAMPLE_H has not been defined in the file already. If it has not been defined yet, then it is defined with #define HEADER_EXAMPLE_H, and the rest of the header is used. If HEADER_EXAMPLE_H has already been defined, then the preprocessor does not enter the ifndef block. 

Note: There are other ways to do this. Another common way is to use an `#pragma once` preprocessor directive.
### 3.2. Multifile Program Example
#### `vect_add_one.h` and `vect_add_one.cpp`
```cpp
#ifndef VECT_ADD_ONE_H
#define VECT_ADD_ONE_H
#include <vector>
using std::vector;
// AddOneToEach method declaration.
void AddOneToEach(vector<int> &v);
#endif
```
and
```cpp
#include "vect_add_one.h"
void AddOneToEach(vector<int> &v) 
{
    for (auto& i: v) {
        i++;
    }
}
```
#### `increment_and_sum.h` and `increment_and_sum.cpp`
```cpp
#ifndef INCREMENT_AND_SUM_H
#define INCREMENT_AND_SUM_H
#include <vector>
using std::vector;
// IncrementAndComputeVectorSum method declaration.
int IncrementAndComputeVectorSum(vector<int> v);
#endif
```
and
```cpp
#include "vect_add_one.h"

int IncrementAndComputeVectorSum(vector<int> v) {
    int total = 0;
    AddOneToEach(v);

    for (auto i: v) {
        total += i;
    }
    return total;
}
```
and `main.cpp` function:
```cpp
#include <iostream>
#include <vector>
#include "increment_and_sum.h"
using std::vector;
using std::cout;

int main() 
{
    vector<int> v{1, 2, 3, 4};
    int total = IncrementAndComputeVectorSum(v);
    cout << "The total is: " << total << "\n";
}
```
### 3.3. References
```cpp
#include <iostream>
using std::cout;

int main() 
{
    int i = 1;
    
    // Declare a reference to i.
    int& j = i;
    cout << "The value of j is: " << j << "\n";  //1
    
    // Change the value of i.
    i = 5;
    cout << "The value of i is changed to: " << i << "\n";   //5
    cout << "The value of j is now: " << j << "\n";    //5
    
    // Change the value of the reference.
    // Since reference is just another name for the variable,
    // th
    j = 7;
    cout << "The value of j is now: " << j << "\n";    //7
    cout << "The value of i is changed to: " << i << "\n";    //7
}
```
