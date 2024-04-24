/***************************************************************************************
 *
 * Given a nested list of integers, implement an iterator to flatten it.
 *
 * Each element is either an integer, or a list -- whose elements may also be integers
 * or other lists.
 *
 * Example 1:
 * Given the list [[1,1],2,[1,1]],
 *
 * By calling next repeatedly until hasNext returns false, the order of elements
 * returned by next should be: [1,1,2,1,1].
 *
 * Example 2:
 * Given the list [1,[4,[6]]],
 *
 * By calling next repeatedly until hasNext returns false, the order of elements
 * returned by next should be: [1,4,6].
 ***************************************************************************************/

#include <iostream>
#include <functional>
#include <stack>
#include <variant>
#include <vector>
#include <cassert>
using namespace std;

// This is the interface that allows for creating nested lists.
// You should not implement it, or speculate about its implementation
class NestedInteger
{
private:
    variant<int, vector<NestedInteger>> data;

public:
    NestedInteger(int val) : data(val)
    {
    }
    NestedInteger(initializer_list<NestedInteger> val) : data(val)
    {
    }

    // Return true if this NestedInteger holds a single integer, rather than a nested list.
    bool isInteger() const
    {
        return holds_alternative<int>(data);
    }

    // Return the single integer that this NestedInteger holds, if it holds a single integer
    // The result is undefined if this NestedInteger holds a nested list
    int getInteger() const
    {
        return get<int>(data);
    }

    // Return the nested list that this NestedInteger holds, if it holds a nested list
    // The result is undefined if this NestedInteger holds a single integer
    const vector<NestedInteger> &getList() const
    {
        return get<vector<NestedInteger>>(data);
    }
};

class NestedIterator
{
    // example input: [[1,1],2,[1,1]]
    // the design: (try performing invariant analysis on the example input)
    // * we have a vector<NestedInteger> on hand --> this is an invariant
    // * need a data structure to store the state of the iterator: where it is within the nestedList and its NestedIntegers
    // --> the index into nestedList --> if it is nestedList.size(), then the iterator is at the end
    // --> the index into the current NestedInteger, which is either an integer or a list
    // --> we need a stack to store the state of the index into the current NestedInteger
    // --> each element in the stack contains:
    // ----> the index into the current vector<NestedInteger>
    // ----> ref to the vector<NestedInteger> itself
    // --> next():
    // ----> look at the top of the stack
    // ----> if the current NestedInteger is an integer, then advance the index, return the integer
    // ----> if the current NestedInteger is a list, then push the current index and the current NestedInteger into the stack until an integer is found
    // --> hasNext():
    // ----> if the stack is empty, then the iterator is at the end
    // NOTE: the list can be empty!!!
    // --> so, it is better to move the stack handling logic to hasNext() and keep next() simple
private:
    using NestedListRef = const vector<NestedInteger> &;
    struct StackElement
    {
        int index;
        NestedListRef nestedList;
    };
    stack<StackElement> S;

public:
    NestedIterator(vector<NestedInteger> &nestedList)
    {
        S.push(StackElement{0, nestedList});
    }

    int next()
    {
        assert(hasNext());
        auto &top_idx = S.top().index;
        auto const &top_nestedList = S.top().nestedList;
        assert(top_idx < top_nestedList.size());

        return top_nestedList.at(top_idx++).getInteger();
    }

    bool hasNext()
    {
        if (S.empty())
        {
            return false;
        }

        auto &top_idx = S.top().index;
        auto const &top_nestedList = S.top().nestedList;
        if (top_idx < top_nestedList.size())
        {
            if (top_nestedList.at(top_idx).isInteger())
            {
                return true;
            }
            else
            {
                S.push(StackElement{0, top_nestedList.at(top_idx).getList()});
                ++top_idx;
                return hasNext();
            }
        }

        S.pop();

        return hasNext();
    }
};

/**
 * Your NestedIterator object will be instantiated and called as such:
 * NestedIterator i(nestedList);
 * while (i.hasNext()) cout << i.next();
 */

int main()
{
    {
        // input: nestedList = []
        // output: []
        auto input = vector<NestedInteger>({});
        NestedIterator i(input);
        while (i.hasNext())
        {
            cout << i.next();
        }
        cout << endl;
    }
    {
        // input: nestedList = [[]]
        // output: []
        auto input = vector<NestedInteger>({NestedInteger({})});
        NestedIterator i(input);
        while (i.hasNext())
        {
            cout << i.next();
        }
        cout << endl;
    }
    {
        //     Input: nestedList = [[1,1],2,[1,1]]
        // Output: [1,1,2,1,1]
        auto input = vector<NestedInteger>({NestedInteger({NestedInteger{1}, NestedInteger{1}}), NestedInteger{2}, NestedInteger({NestedInteger{1}, NestedInteger{1}})});
        NestedIterator i(input);

        while (i.hasNext())
        {
            cout << i.next();
        }

        cout << endl;
    }

    //     Input: nestedList = [1,[4,[6]]]
    // Output: [1,4,6]
    {
        auto input = vector<NestedInteger>({NestedInteger{1}, NestedInteger({NestedInteger{4}, NestedInteger({NestedInteger{6}})})});
        NestedIterator i(input);

        while (i.hasNext())
        {
            cout << i.next();
        }

        cout << endl;
    }
}