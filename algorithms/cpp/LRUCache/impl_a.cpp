/**********************************************************************************
 *
 * Design and implement a data structure for Least Recently Used (LRU) cache.
 * It should support the following operations: get and set.
 *
 *    get(key) - Get the value (will always be positive) of the key if the key exists
 *               in the cache, otherwise return -1.
 *
 *    put(key, value) - Set or insert the value if the key is not already present.
 *                      When the cache reached its capacity, it should invalidate
 *                      the least recently used item before inserting a new item.
 *
 **********************************************************************************/

#include <stdlib.h>
#include <time.h>
#include <iostream>
#include <unordered_map>
#include <cassert>
using namespace std;

class LRUCache
{
    // the design for O(1) complexity
    // * need an internal data struct -- S -- to keep track of used-ness
    // * when a key is get, push to the S --> map but needs order --> already have a map, get a queue? --> NO. Needs a doubly-linked list to allow O(1)-time moving of key
    // * when a key is put, push to the S and if the S is full, pop the S before pushing --> map but needs order --> needs a queue --> NO, needs a doubly linked list -- doubly because it actually provides O(1) insertion and deletion
    // * S should also gives O(1) time for checking contained-ness --> unordered_map

private:
    struct Node
    {
        int key, value;
        shared_ptr<Node> prev, next;

        Node(int k, int v, shared_ptr<Node> p, shared_ptr<Node> n) : key(k), value(v), prev(p), next(n) {}
    };

    void remove_node(shared_ptr<Node> node_ref)
    {
        node_ref->prev->next = node_ref->next;
        node_ref->next->prev = node_ref->prev;
        // just to be safe
        node_ref->prev = nullptr;
        node_ref->next = nullptr;
    }

    void add_node(shared_ptr<Node> node_ref)
    {
        // NOTE: there are FOUR pointers to update
        node_ref->prev = dll_head;
        node_ref->next = dll_head->next;
        dll_head->next->prev = node_ref;
        dll_head->next = node_ref;
    }

    void move_to_head(shared_ptr<Node> node_ref)
    {
        remove_node(node_ref);
        add_node(node_ref);
    }

    shared_ptr<Node> dll_head, dll_tail;

private:
    unordered_map<int, shared_ptr<Node>> M;
    uint16_t cap;

    void keep_cap()
    {
        if (M.size() > cap)
        {
            M.erase(dll_tail->prev->key);
            remove_node(dll_tail->prev);
        }
    }

    void update_freshness(int key)
    {
        if (dll_head->next->key != key)
        {
            move_to_head(M.at(key));
        }
    }

public:
    LRUCache(int capacity)
    {
        cap = capacity;
        dll_head = make_shared<Node>(-1, -1, nullptr, nullptr);
        dll_tail = make_shared<Node>(-1, -1, dll_head, nullptr);
        dll_head->next = dll_tail;
    }

    int get(int key)
    {
        if (M.contains(key))
        {
            update_freshness(key);
            return M[key]->value;
        }
        return -1;
    }

    void put(int key, int value)
    {
        if (M.contains(key))
        {
            M[key]->value = value;
            update_freshness(key);
        }
        else
        {
            M[key] = make_shared<Node>(key, value, nullptr, nullptr);
            add_node(M[key]);
            keep_cap();
        }
    }

    void print()
    {
        cout << "\nLRUCache: ";
        shared_ptr<Node> p = dll_head->next;
        while (p != dll_tail)
        {
            cout << p->key << ":" << p->value << " ";
            p = p->next;
        }
        cout << endl;
    }
};

/**
 * Your LRUCache object will be instantiated and called as such:
 * LRUCache* obj = new LRUCache(capacity);
 * int param_1 = obj->get(key);
 * obj->put(key,value);
 */

int main(int argc, char **argv)
{
    {
        // ["LRUCache","put","put","get","put","get","put","get","get","get"]
        // [[2],[1,1],[2,2],[1],[3,3],[2],[4,4],[1],[3],[4]]
        // expect: [null, null, null, 1, null, -1, null, -1, 3, 4]
        LRUCache lru(2);
        lru.put(1, 1);
        lru.put(2, 2);
        assert(lru.get(1) == 1);
        lru.print();
        lru.put(3, 3);
        lru.print();
        assert(lru.get(2) == -1);
        lru.put(4, 4);
        assert(lru.get(1) == -1);
        assert(lru.get(3) == 3);
        assert(lru.get(4) == 4);
    }

    /*
    LRUCache c(2);
    c.set(2,1);
    c.print();
    c.set(2,2);
    c.print();
    c.get(2);
    c.print();
    c.set(1,1);
    c.print();
    c.set(4,1);
    c.print();
    c.get(2);
    c.print();

    cout << "---------" << endl;
    */
    srand(time(0));

    int capacity = 5;
    int test_loop_times = 10;
    if (argc > 1)
    {
        capacity = atoi(argv[1]);
    }
    if (argc > 2)
    {
        test_loop_times = atoi(argv[1]);
    }

    LRUCache cache(capacity);

    int v;
    for (int i = 0; i < test_loop_times; i++)
    {
        v = i; // rand() % capacity;
        cout << "set " << v << ": ";
        cache.put(v, v);
        cache.print();

        v = rand() % capacity;
        cout << "get " << v << ": " << cache.get(v);
        cache.print();

        cout << endl;
    }
    return 0;
}
