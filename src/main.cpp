#include "solver.h"

#include <iostream>

int main()
{
    const unsigned N = 3;
    auto board = Board::create_random(N);
    while (!board.is_solvable()) {
        board = Board::create_random(N);
    }
    const auto solution = Solver::solve(board);
    std::cout << solution.moves() << std::endl;
    for (const auto & move : solution) {
        std::cout << move << std::endl;
    }
}
