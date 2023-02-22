#include "solver.h"

#include <algorithm>
#include <queue>
#include <set>
#include <utility>

namespace {

// represents a node of graph in A* algorithm
struct Node
{
    Board state;
    Node * parent = nullptr;
    // f = g + h, g - number of moves, h - heuristic
    unsigned int g, h;

    Node() = default;

    Node(Board state, Node * parent, unsigned int g)
        : state(std::move(state))
        , parent(parent)
        , g(g)
        // found this one heuristic
        , h((this->state.manhattan()) + ((3 * (this->state.manhattan())) / 2))
    {
    }
};

// for priority_queue<Node *>
class Compare
{
public:
    bool operator()(Node * lhs, Node * rhs)
    {
        return (lhs->g + lhs->h) > (rhs->g + rhs->h);
    }
};

} // namespace

Solver::Solution Solver::solve(const Board & board)
{
    if (board.is_goal()) {
        return Solution({board});
    }
    if (!board.is_solvable()) {
        return Solution();
    }
    std::set<Board> closed = {Board::create_goal(0)};
    std::priority_queue<Node *, std::deque<Node *>, Compare> open;
    constexpr int shiftX[] = {-1, 0, 1, 0};
    constexpr int shiftY[] = {0, 1, 0, -1};
    Board to;
    Node * current;
    // to delete nodes after
    std::queue<Node *> to_delete;

    // initial node
    open.push(new Node(board, nullptr, 0));
    while (!open.empty()) {
        // if current is the goal
        if (open.top()->h == 0) {
            break;
        }
        to_delete.push(open.top());
        current = open.top();
        open.pop();
        for (std::size_t i = 0; i < 4; ++i) {
            // to represents the node there we should go after one move
            to = current->state.move_to(shiftX[i], shiftY[i]);
            // skip it if we have already been there
            if (closed.find(to) != closed.end()) {
                continue;
            }
            closed.emplace(to);
            // put new node to our queue
            open.push(new Node(to, current, current->g + 1));
        }
    }
    // restoring path
    std::vector<Board> result;
    for (Node * v = open.top(); v != nullptr; v = v->parent) {
        result.emplace_back(v->state);
    }
    std::reverse(result.begin(), result.end());
    // delete nodes
    while (!to_delete.empty()) {
        delete to_delete.front();
        to_delete.pop();
    }
    while (!open.empty()) {
        delete open.top();
        open.pop();
    }
    return Solution(std::move(result));
}
