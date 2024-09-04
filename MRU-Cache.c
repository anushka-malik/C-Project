
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define KEY_SIZE 32
#define VALUE_SIZE 256
#define CACHE_SIZE 1000
#define CACHE_CAPACITY 3

// Define a structure for cache entry
typedef struct CacheEntry {
    char key[KEY_SIZE];
    char value[VALUE_SIZE];
    struct CacheEntry *next;
    struct CacheEntry *prev;
} CacheEntry;

// Define a structure for cache
typedef struct Cache {
    CacheEntry *items[CACHE_SIZE]; // Hash table to store entries
    CacheEntry *head;  // Most recently used entry
    CacheEntry *tail;  // Least recently used entry
    int size;
} Cache;

// Function to initialize the cache and its items
void init(Cache *cache) {
    for (int i = 0; i < CACHE_SIZE; i++) {
        cache->items[i] = NULL;
    }
    cache->head = NULL;
    cache->tail = NULL;
    cache->size = 0;
}

// DJB2 Hash function to ensure uniform distribution & reduce collision
unsigned int hash(const char *key) {
    unsigned int hash = 0;
    while (*key) {
        hash = (hash << 5) + hash + *key++;
    }
    return hash % CACHE_SIZE;
}

// Function to remove an entry from the linked list
void remove_entry(Cache *cache, CacheEntry *entry) {
    if (entry->prev) {
        entry->prev->next = entry->next;
    } else {
        cache->head = entry->next;
    }
    if (entry->next) {
        entry->next->prev = entry->prev;
    } else {
        cache->tail = entry->prev;
    }
}

// Function to add an entry to the cache and linked list
void add_to_cache(Cache *cache, const char *key, const char *value) {
    int ind = hash(key);

    // Check if the key already exists
    CacheEntry *existing = cache->items[ind];
    while (existing) {
        if (strcmp(existing->key, key) == 0) {
            // Update value if key already exists
            strncpy(existing->value, value, VALUE_SIZE - 1);
            existing->value[VALUE_SIZE - 1] = '\0';

            // Move this entry to the head of the list
            if (existing != cache->head) {
                remove_entry(cache, existing);

                // Add to head
                existing->next = cache->head;
                existing->prev = NULL;
                if (cache->head) {
                    cache->head->prev = existing;
                }
                cache->head = existing;
                if (cache->tail == NULL) {
                    cache->tail = existing;
                }
            }
            return;
        }
        existing = existing->next;
    }

    // Create a new entry
    CacheEntry *entry = (CacheEntry *)malloc(sizeof(CacheEntry));
    strncpy(entry->key, key, KEY_SIZE - 1);
    entry->key[KEY_SIZE - 1] = '\0';
    strncpy(entry->value, value, VALUE_SIZE - 1);
    entry->value[VALUE_SIZE - 1] = '\0';
    entry->next = cache->items[ind];
    entry->prev = NULL;
    if (cache->items[ind]) {
        cache->items[ind]->prev = entry;
    }
    cache->items[ind] = entry;

    // Add entry to the head of the linked list
    entry->next = cache->head;
    entry->prev = NULL;
    if (cache->head) {
        cache->head->prev = entry;
    }
    cache->head = entry;
    if (cache->tail == NULL) {
        cache->tail = entry;
    }

    // Eviction logic
    if (cache->size == CACHE_CAPACITY) {
        // Remove the tail (least recently used)
        CacheEntry *to_remove = cache->tail;
        if (to_remove) {
            int tail_index = hash(to_remove->key);
            CacheEntry *entry_to_remove = cache->items[tail_index];
            CacheEntry *prev = NULL;
            while (entry_to_remove) {
                if (entry_to_remove == to_remove) {
                    if (prev) {
                        prev->next = entry_to_remove->next;
                    } else {
                        cache->items[tail_index] = entry_to_remove->next;
                    }
                    if (entry_to_remove->next) {
                        entry_to_remove->next->prev = prev;
                    }
                    break;
                }
                prev = entry_to_remove;
                entry_to_remove = entry_to_remove->next;
            }
            remove_entry(cache, to_remove);
            cache->size--;
        }
    } else {
        cache->size++;
    }
}

// Function to return the value corresponding to a key, if it exists
const char *retrieve_from_cache(Cache *cache, const char *key) {
    int ind = hash(key);
    CacheEntry *entry = cache->items[ind];
    while (entry) {
        if (strcmp(entry->key, key) == 0) {
            // Move entry to the head of the list
            if (entry != cache->head) {
                remove_entry(cache, entry);

                // Add to head
                entry->next = cache->head;
                entry->prev = NULL;
                if (cache->head) {
                    cache->head->prev = entry;
                }
                cache->head = entry;
                if (cache->tail == NULL) {
                    cache->tail = entry;
                }
            }
            return entry->value;
        }
        entry = entry->next;
    }
    return NULL;
}

// Function to print the elements of the cache placed in the most recently used (MRU) order
void print_cache(Cache *cache) {
    CacheEntry *temp = cache->head;
    while (temp) {
        printf("Key: %s has Value: %s\n", temp->key, temp->value);
        temp = temp->next;
    }
    printf("\n");
}

// Function to free the memory allocated
void free_memory(Cache *cache) {
    CacheEntry *temp = cache->head;
    while (temp) {
        CacheEntry *item = temp;
        temp = temp->next;
        int ind = hash(item->key);
        CacheEntry *entry = cache->items[ind];
        CacheEntry *prev = NULL;
        while (entry) {
            if (entry == item) {
                if (prev) {
                    prev->next = entry->next;
                } else {
                    cache->items[ind] = entry->next;
                }
                if (entry->next) {
                    entry->next->prev = prev;
                }
                break;
            }
            prev = entry;
            entry = entry->next;
        }
        free(item);
    }
}

// Function to test the working of the logic and implementation
void test() {
    Cache cache;
    init(&cache);
    add_to_cache(&cache, "1", "a");
    add_to_cache(&cache, "2", "b");
    add_to_cache(&cache, "3", "c");
    add_to_cache(&cache, "4", "d");
    print_cache(&cache);

    const char *str = retrieve_from_cache(&cache, "2");
    if (str) {
        printf("Retrieved key '1': %s\n", str);
    } else {
        printf("Key '1' not found\n");
    }
    print_cache(&cache);

    const char *str2 = retrieve_from_cache(&cache, "3");
    if (str2) {
        printf("Retrieved key '4': %s\n", str2);
    } else {
        printf("Key '4' not found\n");
    }
    print_cache(&cache);

    free_memory(&cache);
}

int main() {
    test();
    return 0;
}
