#pragma once

#include <string>
#include <vector>

using value_type = unsigned;
using size_type = std::size_t;

class Board
{
    std::vector<std::vector<value_type>> board;

    // position of 0
    std::pair<size_type, size_type> position;

public:
    static Board create_goal(value_type size);

    static Board create_random(value_type size);

    Board() = default;

    explicit Board(const std::vector<std::vector<value_type>> & data);

    size_type size() const;

    bool is_goal() const;

    value_type hamming() const;

    value_type manhattan() const;

    std::string to_string() const;

    bool is_solvable() const;

    // moves 0 to position + (dx, dy)
    Board move_to(int dx, int dy);

    friend bool operator<(const Board & lhs, const Board & rhs)
    {
        return lhs.board < rhs.board;
    }

    friend bool operator==(const Board & lhs, const Board & rhs)
    {
        return lhs.board == rhs.board;
    }

    friend bool operator!=(const Board & lhs, const Board & rhs)
    {
        return lhs.board != rhs.board;
    }

    friend std::ostream & operator<<(std::ostream & out, const Board & board_)
    {
        return out << board_.to_string();
    }

    const std::vector<value_type> & operator[](size_type index) const
    {
        return board[index];
    }
};
