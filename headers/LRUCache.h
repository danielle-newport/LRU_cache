#pragma once
#include <unordered_map>
#include <vector>
#include <list>

class LRUCache
{
public:
    LRUCache(int capacity);
    void put(std::string pageName, int value);
    int get(std::string pageName);

private:
    int _capacity;
    std::unordered_map<std::string, std::list<std::pair<std::string, int>>::iterator> LRUHashmap;
    std::list<std::pair<std::string, int>> LRUList;
};
