#include <stdlib.h>
#include <time.h>
#include <iostream>
#include <unordered_map>
#include <list>
#include <cassert>
using namespace std;

class LRUCache
{
public:
    int capacity;
    unordered_map<int, list<pair<int, int>>::iterator> dic;
    list<pair<int, int>> lru;

    LRUCache(int capacity)
    {
        this->capacity = capacity;
    }

    int get(int key)
    {
        auto it = dic.find(key);

        if (it == dic.end())
        {
            return -1;
        }

        int value = it->second->second;
        lru.erase(it->second);
        lru.push_front({key, value});

        dic.erase(it);
        dic[key] = lru.begin();
        return value;
    }

    void put(int key, int value)
    {
        auto it = dic.find(key);

        if (dic.find(key) != dic.end())
        {
            lru.erase(it->second);
            dic.erase(it);
        }

        lru.push_front({key, value});
        dic[key] = lru.begin();

        if (dic.size() > capacity)
        {
            auto it = dic.find(lru.rbegin()->first);
            dic.erase(it);
            lru.pop_back();
        }
    }

    void print()
    {
        cout << "\nLRU: ";
        for (auto &p : lru)
        {
            cout << p.first << ":" << p.second << " ";
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