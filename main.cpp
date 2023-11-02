#include <iostream>
#include <vector>
#include <random>

struct cell
{
    int east_wall = 1;
    int south_wall = 1;
    bool visited = false;
};

struct coords
{
    int x = 0;
    int y = 0;
};

int random_int(int min, int max)
{
    int n = 0;

    std::random_device rd;
    std::uniform_int_distribution<int> dist(min, max);

    n = dist(rd);

    return n;
}

void print_board(std::vector<std::vector<cell>> maze)
{
    int rows = maze[0].size();
    int cols = maze.size();

    std::string ceiling = "  "; // this is to have a start-point a the top left corner
    for (int c = 0; c < cols*2-2; c++)
    {
        ceiling += "_";
    }
    std::cout << ceiling << std::endl;

    for (int i = 0; i < rows; ++i)
    {
        std::cout << "|";
        for (int j = 0; j < cols; ++j)
        {
            if (maze[i][j].south_wall)
            {
                std::cout << "_";
            }
            else
            {
                std::cout << " ";
            }

            if (maze[i][j].east_wall)
            {
                std::cout << "|";
            }
            else
            {
                std::cout << " ";
            }
        }
        std::cout << std::endl;
    }
}

std::vector<coords> list_available_neighbors(std::vector<std::vector<cell>> board, coords current_pos)
{
    std::vector<coords> neighbors;

    if (current_pos.y - 1 >= 0) // preventing seg fault
    {
        if (board[current_pos.y - 1][current_pos.x].visited == false) // check if cell was visited
        {
            coords n = current_pos;
            n.y = n.y - 1;
            neighbors.push_back(n);
        }
    }
    if (current_pos.y + 1 <= board.size() - 1)
    {
        if (board[current_pos.y + 1][current_pos.x].visited == false)
        {
            coords n = current_pos;
            n.y = n.y + 1;
            neighbors.push_back(n);
        }
    }
    if (current_pos.x - 1 >= 0)
    {
        if (board[current_pos.y][current_pos.x - 1].visited == false)
        {
            coords n = current_pos;
            n.x = n.x - 1;
            neighbors.push_back(n);
        }
    }
    if (current_pos.x + 1 <= board[0].size() - 1)
    {
        if (board[current_pos.y][current_pos.x + 1].visited == false)
        {
            coords n = current_pos;
            n.x = n.x + 1;
            neighbors.push_back(n);
        }
    }

    return neighbors;
}

std::vector<std::vector<cell>> generate_maze(int size_x, int size_y)
{
    std::vector<std::vector<cell>> board;

    // generating the maze
    for (int y = 0; y < size_y; y++)
    {
        std::vector<cell> row;
        for (int x = 0; x < size_x; x++)
        {
            cell c;
            row.push_back(c);
        }
        board.push_back(row);
    }

    std::vector<coords> path; // this is a/the stack
    coords visitor;          // this is the current element (stack[-1])

    while (1)
    {
        board[visitor.y][visitor.x].visited = true; // setting the current pos to vistited

        std::vector<coords> neighbors = list_available_neighbors(board, visitor);

        if (neighbors.empty())
        {
            if (path.empty()) // if the path is empty, we visted all the cells
            {
                break;
            }
            // going backwards
            visitor = path[path.size() - 1];
            path.pop_back();
            continue;
        }

        // choose a random naxt move
        coords next_move = neighbors[random_int(0,neighbors.size() - 1)];

        // checking which way the visitor went and removing the corresponding wall
        if (visitor.x < next_move.x)
        {
            board[visitor.y][visitor.x].east_wall = 0;
        }
        if (visitor.y < next_move.y)
        {
            board[visitor.y][visitor.x].south_wall = 0;
        }
        if (visitor.x > next_move.x)
        {
            board[next_move.y][next_move.x].east_wall = 0;
        }
        if (visitor.y > next_move.y)
        {
            board[next_move.y][next_move.x].south_wall = 0;
        }

        path.push_back(visitor); // pushing the current pos to the stack
        visitor = next_move;    // and move to the next pos
    }

    // seting endpoint in the bottom left corner
    board[board.size() - 1][board[0].size() - 1].south_wall = 0;

    return board;
}

int main()
{

    int MAZE_SIZE = 50;

    std::vector<std::vector<cell>> board = generate_maze(MAZE_SIZE, MAZE_SIZE);
    std::cout << "start in the top left corner and go to the buttom left" << std::endl;
    print_board(board);
    system("pause>0");

    return 0;
}