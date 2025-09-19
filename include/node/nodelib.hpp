#ifndef INCLUDE_NODE_NODELIB_H_
#define INCLUDE_NODE_NODELIB_H_

#include <vector>   // std::vector
#include <cstddef>  // std::size_t
#include <tuple>    // std::tuple
#include <memory>   // std::shared_ptr
#include <span>     // std::span

class Node
{
public:
    Node();
    Node(std::vector<int> input);
    Node(std::vector<int> input, int posX);
    Node(std::vector<int> input, int posX, unsigned long d, std::shared_ptr<const Node> p, short m);
    ~Node() = default;

    /// @brief Gets all the available moves
    /// @return A vectors contains all the available moves
    std::vector<short> AvailableMoves() const;

    /// @brief Gets all child nodes
    /// @param  The depth of the parent
    /// @return A vector of all child nodes
    std::vector<Node> GetChildNodes(unsigned long d, std::shared_ptr<const Node> p) const;

    std::tuple<std::vector<int>, int> GetNextLayout(short dir) const;

    /// @brief Gets the Manhattan distance of the puzzle (l-2 distance)
    /// @return The manhattan distance
    unsigned int GetManhattanDistance() const noexcept;

    /// @brief Gets the linear conflic of the puzzle
    /// @return The linear conflic
    unsigned int GetLinearConflict() const noexcept;

    /// @brief Gets the hash value of the node
    /// @return The hash value of the node
    std::size_t GetHashValue() const noexcept;

    /// @brief Gets the depth of the node
    /// @return The depth
    unsigned int GetDepth() const noexcept;

    /// @brief Gets the total cost of the node
    /// @return The total cost
    unsigned int GetTotalCost() const noexcept;

    /// @brief Prints the node
    void Print() const;

    /// @brief Compares if two nodes (puzzles) are identical
    /// @param rhs The right hand side node (puzzle)
    /// @return True if they are identical
    inline bool operator==(const Node &rhs) const
    {
        return state_ == rhs.state_;
    }

    /// @brief Compares if two nodes (puzzles) are NOT identical
    /// @param rhs The right hand side node (puzzle)
    /// @return True if they are NOT identical
    inline bool operator!=(const Node &rhs) const
    {
        return !(*this == rhs);
    }

    /// @brief Returns if the puzzle is solved
    /// @return Is solved or not
    bool IsSolved() const noexcept;

    /// @brief Gets the pointer of its parent
    /// @return The pointer of its parent
    std::shared_ptr<const Node> GetParent() const noexcept;

    /// @brief Gets the move used to reach this node
    /// @return The move
    short GetMove() const noexcept;

    /// @brief Gets the state of the node
    /// @return The state
    std::span<const int> GetState() const;

    /// @brief Get the position of the empty piece
    /// @return The position of the empty piece
    inline int GetPosX() const noexcept
    {
        return posX_;
    }

    /// @brief Move the empty piece
    /// @param dir The direction of the empty piece
    /// @return True if the empty piece is successfully moved
    bool Move(short dir);

private:
    /// @brief Calcualte the Manhattan distance
    void CalculateManhattanDistance();

    /// @brief Calcualte the linear conflict value
    void CalculateLinearConflict();

protected:
    /// @brief The state of the node
    std::vector<int> state_;

    /// @brief The position of The empty piece
    int posX_;

    /// @brief The Manhattan distance
    unsigned int manhattanDistance_;

    /// @brief The number of linear conflicts
    unsigned int linearConflict_;

    /// @brief The hash value
    std::size_t hashValue_;

    /// @brief The depth of the node
    unsigned int depth_;

    /// @brief The parent of this node
    std::shared_ptr<const Node> parent_;

    /// @brief The move used to reach this node in terms of the empty space
    short move_;
};

/// @brief The comparison function for the priority queue
struct NodeCmp
{
    bool operator()(std::shared_ptr<Node> lhs, std::shared_ptr<Node> rhs)
    {
        // Check if the two nodes are identical
        if (lhs != rhs)
        {
            // Check if the two nodes have the same total cost (f value)
            // if so then we prefer the node that has a lower Manhattan distance (h value)
            if (lhs->GetTotalCost() == rhs->GetTotalCost())
            {
                return lhs->GetManhattanDistance() > rhs->GetManhattanDistance();
            }

            return lhs->GetTotalCost() > rhs->GetTotalCost();
        }

        // If they are identical, then we use the hash value for comparison.
        // Since this is for the min. priority queue, we return true if the lhs is greater than the rhs.
        return lhs->GetDepth() > rhs->GetDepth();
    }
};

#endif // INCLUDE_NODE_NODELIB_H_
