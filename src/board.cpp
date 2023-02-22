#include "board.h"

#include <algorithm>
#include <numeric>
#include <random>
#include <stdexcept>

namespace {
// returns number of digits in value to make good-looking output
size_type get_length(value_type value)
{
    size_type result = 0;
    while (value) {
        ++result;
        value /= 10;
    }
    if (result == 0) {
        ++result;
    }
    return result;
}

} // namespace

Board Board::create_goal(const unsigned size)
{
    std::vector<std::vector<value_type>> board(size, std::vector<value_type>(size));
    if (size == 0) {
        return Board(board);
    }
    for (size_type i = 0; i < size; ++i) {
        for (size_type j = 0; j < size; ++j) {
            board[i][j] = (i * size) + j + 1;
        }
    }
    board.back().back() = 0;
    return Board(board);
}

Board Board::create_random(const unsigned size)
{
    std::vector<value_type> fields(size * size);
    std::iota(fields.begin(), fields.end(), value_type(0));
    // g is pseudo-random number generator with random seed by random_device
    std::shuffle(fields.begin(), fields.end(), std::mt19937(std::random_device{}()));
    // fields now is a random permutation
    Board result = create_goal(size);
    for (size_type i = 0; i < size; ++i) {
        for (size_type j = 0; j < size; ++j) {
            result.board[i][j] = fields[(i * size) + j];
            if (result.board[i][j] == 0) {
                result.position = {i, j};
            }
        }
    }
    return result;
}

Board::Board(const std::vector<std::vector<value_type>> & data)
    : board(data)
{
    // finds 0 to set position
    for (size_type i = 0; i < board.size(); ++i) {
        for (size_type j = 0; j < board.size(); ++j) {
            if (board[i][j] != 0) {
                continue;
            }
            position = {i, j};
        }
    }
}

size_type Board::size() const
{
    return board.size();
}

bool Board::is_goal() const
{
    return (hamming() == 0);
}

unsigned Board::hamming() const
{
    unsigned result = 0;
    value_type current = 1;
    for (std::size_t i = 0; i < board.size(); ++i) {
        for (std::size_t j = 0; j < board.size(); ++j) {
            // if 0 is supposed to be there
            if (i == board.size() - 1 && j == board.size() - 1) {
                if (board[i][j] != 0) {
                    ++result;
                }
                continue;
            }
            // current is the number that supposed to be there otherwise
            if (board[i][j] != current) {
                ++result;
            }
            ++current;
        }
    }
    return result;
}

unsigned Board::manhattan() const
{
    unsigned result = 0;
    size_type goal_i, goal_j;
    for (std::size_t i = 0; i < board.size(); ++i) {
        for (std::size_t j = 0; j < board.size(); ++j) {
            if (board[i][j] == 0) {
                continue;
            }
            // (goal_i, goal_j) is position there board[i][j] should be
            goal_i = (board[i][j] - 1) / board.size();
            goal_j = (board[i][j] - 1) % board.size();
            // result += |i - goal_i| + |j - goal_j|
            result += (i > goal_i) ? (i - goal_i) : (goal_i - i);
            result += (j > goal_j) ? (j - goal_j) : (goal_j - j);
        }
    }
    return result;
}

std::string Board::to_string() const
{
    std::string result;
    for (size_type i = 0; i < board.size(); ++i) {
        for (size_type j = 0; j < board.size(); ++j) {
            // adding padding
            result += std::string(get_length((board.size() * board.size()) - 1) - get_length(board[i][j]), ' ');
            result += std::to_string(board[i][j]) + " ";
        }
        result.pop_back();
        result += "\n";
    }
    return result;
}

bool Board::is_solvable() const
{
    if (size() == 0) {
        return true;
    }
    bool inversions_parity = true;
    for (size_type x = 0; x < board.size(); ++x) {
        for (size_type y = 0; y < board.size(); ++y) {
            if (board[x][y] == 0) {
                continue;
            }
            for (size_type i = x; i < board.size(); ++i) {
                for (size_type j = ((i == x) ? y : 0); j < board.size(); ++j) {
                    if (board[i][j] == 0) {
                        continue;
                    }
                    if (board[x][y] > board[i][j]) {
                        inversions_parity = !inversions_parity;
                    }
                }
            }
        }
    }
    if ((board.size() % 2) || ((board.size() - position.first) % 2))
        inversions_parity = !inversions_parity;
    return !inversions_parity;
}

// moves 0 to position + (dx, dy)
Board Board::move_to(const int dx, const int dy)
{
    // returns empty board if move is impossible
    if (((position.first == 0) && (dx < 0)) ||
        ((position.first + 1 == board.size()) && (dx > 0)) ||
        (((position.second == 0) && (dy < 0))) ||
        ((position.second + 1 == board.size()) && (dy > 0))) {
        return create_goal(0);
    }
    Board result = *this;
    std::swap(result.board[position.first][position.second], result.board[position.first + dx][position.second + dy]);
    result.position = {position.first + dx, position.second + dy};
    return result;
}
