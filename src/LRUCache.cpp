#include <LRUCache.h>

using namespace std;

LRUCache::LRUCache(int capacity) : _capacity(capacity) {}

void LRUCache::put(std::string pageName, int value)
{
    // check if page already in cache
    int pageValue = get(pageName);

    // if it's not in cache, we have to
    // 1. evict if at capacity
    // 2. add to end of list and insert into hashmap
    if (pageValue == -1)
    {
        if (LRUList.size() == _capacity)
        {
            // remove first elt (oldest)
            list<pair<string, int>>::iterator oldestPageValIt = LRUList.begin();
            string pageName = oldestPageValIt->first;
            LRUList.erase(oldestPageValIt);
            // remove from hashmap
            LRUHashmap.erase(pageName);
        }

        // add to list
        pair<string, int> pairToAdd = pair(pageName, value);
        LRUList.push_back(pairToAdd);
        LRUHashmap.insert(pair(pageName, prev(LRUList.end())));
    }
    else
    {
        LRUHashmap[pageName]->second = value;
    }
}

int LRUCache::get(string pageName)
{
    auto listIt = LRUHashmap.find(pageName);

    // if the page is already in the cache, reorder the list and update index in hashmap
    if (listIt != LRUHashmap.end())
    {
        // update list order
        LRUList.splice(LRUList.end(), LRUList, listIt->second);

        // update pointer in hashmap to be last elt ? is this needed, pointer will be moved?

        // return value
        return listIt->second->second;
    }
    else
    {
        return -1;
    }
}