#include "huffman.hpp"
#include <algorithm>
#include <tuple>
#include <queue>

// Count frequencies of characters in string_view s.
std::map<char,int> huffman_frequencies(std::string_view s)
{
    std::map<char,int> retval;
    for(auto c : s)
    {
        ++retval[c];
    }
    return retval;
}

namespace
{

// Base class to build a tree of frequency nodes for analysis.
struct huffman_tree_base
{
    huffman_tree_base(int p_frequency)
        :frequency(p_frequency)
    {
    }
    int frequency;

    // visit method facilitates tree traversal.
    virtual void visit(int bits, std::vector<symbol_bits>& output) const = 0;
};

// Leaf node class. 
struct huffman_tree_symbol : public huffman_tree_base
{
    huffman_tree_symbol(char p_symbol, int p_frequency)
        :huffman_tree_base(p_frequency),
        symbol(p_symbol)
    {
    }

    // visit on a leaf means recording that symbol's bit-count
    void visit(int bits, std::vector<symbol_bits>& output) const
    {
        output.push_back({ symbol,bits });
    }
    char symbol;
};

struct huffman_tree_inner : public huffman_tree_base
{
    huffman_tree_inner(int p_frequency, huffman_tree_base const* p_left, huffman_tree_base const* p_right)
        :huffman_tree_base(p_frequency),
        left(p_left),
        right(p_right)
    {
    }

    // visit on an inner node means visit both children, at one greater bit-count
    void visit(int bits, std::vector<symbol_bits>& output) const
    {
        left->visit(bits + 1, output);
        right->visit(bits + 1, output);
    }
    huffman_tree_base const* const left;
    huffman_tree_base const* const right;
};

// Utility class for the priority_queue in huffman_frequencies_to_bits to put lowest frequency on top()
struct huffman_tree_frequency_greater
{
    bool operator()(huffman_tree_base const* l, huffman_tree_base const* r) const
    {
        return l->frequency > r->frequency;
    }
};

}

std::vector<symbol_bits> huffman_frequencies_to_bits(std::map<char, int>&& frequencies)
{
    std::vector<huffman_tree_symbol> leaf_nodes;
    leaf_nodes.reserve(frequencies.size());
    for (auto f : frequencies)
    {
        leaf_nodes.push_back(huffman_tree_symbol(f.first, f.second ) );
    }

    // A full binary tree has a number of inner nodes equal to the number of leaves - 1
    std::vector<huffman_tree_inner> inner_nodes;
    inner_nodes.reserve(frequencies.size() - 1);

    std::priority_queue<huffman_tree_base const *, std::vector<huffman_tree_base const* >, huffman_tree_frequency_greater> pending_nodes;

    // Fill the priority queue with leaf nodes to start.
    for (huffman_tree_symbol const & n : leaf_nodes)
    {
        pending_nodes.push(&n);
    }

    while (pending_nodes.size() > 1)
    {
        // At each iteration, remove the two lowest frequency nodes and add one inner node that combines them.
        auto first = pending_nodes.top();
        pending_nodes.pop();
        auto second = pending_nodes.top();
        pending_nodes.pop();

        huffman_tree_inner new_node = { first->frequency + second->frequency, first, second };
        inner_nodes.push_back(new_node);

        pending_nodes.push(&inner_nodes.back());
    }

    //Tree built
    //Descend tree (recursively), when reaching a leaf node, add a symbol_bits record.
    std::vector<symbol_bits> retval;
    retval.reserve(frequencies.size());
    pending_nodes.top()->visit(0, retval);
    return retval;
}

static bool symbol_bits_less(symbol_bits const& l, symbol_bits const& r)
{
    return std::tie(l.bits, l.symbol) < std::tie(r.bits, r.symbol);
}

std::map<char, codeword> canonical_codes(std::vector<symbol_bits> && symbols)
{
    unsigned int code = 0;
    std::map<char, codeword> retval;
    std::sort(symbols.begin(), symbols.end(), symbol_bits_less);
    int current_bits = symbols.front().bits;
    for (auto symbol : symbols)
    {
        while (symbol.bits > current_bits)
        {
            ++current_bits;
            code <<= 1;
        }
        retval[symbol.symbol] = codeword{ current_bits,code };
        ++code;
    }
    return retval;
}
