#include "cache.h"

#include <iostream>

struct hits
{
    int hits_lru = 0;
    int hits_lfu = 0;
    int hits_beauty = 0;
};

hits get_hits ();
int slow_get_page_int (int key);
char slow_get_page_char (int key);

int main ()
{
    hits hits = get_hits ();
    std::cout << "lru hits: " << hits.hits_lru << "\n";
    std::cout << "lfu hits: " << hits.hits_lfu << "\n";
    std::cout << "beauty hits: " << hits.hits_beauty << "\n";

    return 0; 
}

hits get_hits ()
{
    int capacity = 0;

    std::cin >> capacity;
    std::cout << "capacity: " << capacity << "\n";

    caches::cache_lru<char> cache_lru (capacity);
    caches::cache_lfu_lists<char> cache_lfu (capacity);

    int num_keys = 0;
    std::cin >> num_keys;
    std::cout << "num_keys: " << num_keys << "\n";

    hits hits {};
    for (int i = 0; i < num_keys; i++)
    {
        int key = 0;
        std::cin >> key;
        std::cout << "key" << i << ": " << key << "\n";

        if (cache_lru.lookup_update (key, slow_get_page_char))
            hits.hits_lru++;

        if (cache_lfu.lookup_update (key, slow_get_page_char))
            hits.hits_lfu++;
    }

    return hits;
}

char slow_get_page_char (int key)
{
    return char (key);
}

int slow_get_page_int (int key)
{
    return key; 
}


