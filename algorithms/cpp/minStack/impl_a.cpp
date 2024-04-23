/**********************************************************************************
 *
 * Design a stack that supports push, pop, top, and retrieving the minimum element in constant (i.e., O(1)) time.
 *
 * push(x) -- Push element x onto stack.
 *
 * pop() -- Removes the element on top of the stack.
 *
 * top() -- Get the top element.
 *
 * getMin() -- Retrieve the minimum element in the stack.
 *
 *
 **********************************************************************************/

#include <stdlib.h>
#include <iostream>
#include <stack>
#include <optional>
#include <utility>
#include <cassert>
using namespace std;

class MinStack
{
    // the design:
    // * the normal stack will provide O(1) time for push, pop, and top
    // * the problem is with getMin()
    // --> this can be solved by saving the iterator of a running minimum element in the stack
    // --> or can it? what happens when the min element is poped? what value should the running minimum be?

    // --> so the right solution is to keep a hashmap from element value to its count in the stack
    // --> when the stack is poped, the element is looked up in the hashmap so its count is decremented
    // --> can it work? NO... there is no order in the hashmap so we cannot avoid spend O(n) time to find the minimum again

    // --> new idea: save the running minimum in the stack also! --> this should work!

    // Q: is the iterator of an element in the stack not going to change when new elements are pushed into the stack?
private:
    stack<pair<int, int>> S;

public:
    MinStack()
    {
    }

    void push(int val)
    {
        auto min_val = val;
        if (!S.empty() && getMin() < val)
        {
            min_val = getMin();
        }

        S.push(make_pair(val, min_val));
    }

    void pop()
    {
        S.pop();
    }

    int top()
    {
        return S.top().first;
    }

    int getMin()
    {
        return S.top().second;
    }

    void clear()
    {
        while (!S.empty())
        {
            S.pop();
        }
    }
};

/**
 * Your MinStack object will be instantiated and called as such:
 * MinStack* obj = new MinStack();
 * obj->push(val);
 * obj->pop();
 * int param_3 = obj->top();
 * int param_4 = obj->getMin();
 */

int main()
{
    {
        //         Input
        // ["MinStack","push","push","push","getMin","pop","top","getMin"]
        // [[],[-2],[0],[-3],[],[],[],[]]

        // Output
        // [null,null,null,null,-3,null,0,-2]

        auto minStack = MinStack();
        minStack.push(-2);
        minStack.push(0);
        minStack.push(-3);
        assert(-3 == minStack.getMin()); // return -3
        minStack.pop();
        assert(0 == minStack.top());     // return 0
        assert(-2 == minStack.getMin()); // return -2
    }

    cout << "--- expected output [0, 0, 0, 2]" << endl;
    MinStack ms;
    ms.push(2);
    ms.push(0);
    ms.push(3);
    ms.push(0);

    cout << ms.getMin() << endl;
    ms.pop();
    cout << ms.getMin() << endl;
    ms.pop();
    cout << ms.getMin() << endl;
    ms.pop();
    cout << ms.getMin() << endl;

    ms.clear();

    cout << "--- expected output [2147483647 2147483646 2147483646 2147483647 2147483647 -2147483648 -2147483648 2147483647 " << endl;

    ms.push(2147483646);
    ms.push(2147483646);
    ms.push(2147483647);

    cout << ms.top() << endl;
    ms.pop();
    cout << ms.getMin() << endl;
    ms.pop();
    cout << ms.getMin() << endl;
    ms.pop();

    ms.push(2147483647);
    cout << ms.top() << endl;
    cout << ms.getMin() << endl;
    ms.push(-2147483648);
    cout << ms.top() << endl;
    cout << ms.getMin() << endl;
    ms.pop();
    cout << ms.getMin() << endl;

    return 0;
}