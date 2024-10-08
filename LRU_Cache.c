#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <sys/resource.h>
#include "cache.h"

#define KEY_SIZE 32
#define VALUE_SIZE 256
#define CACHE_SIZE 2000
#define CACHE_CAPACITY 5
#define ENDEC_KEY 3


typedef struct CacheEntry {
    char key[KEY_SIZE];
    char value[VALUE_SIZE];
    struct CacheEntry *next;
    struct CacheEntry *prev;
} CacheEntry;

typedef struct Cache {
    CacheEntry *items[CACHE_SIZE];
    CacheEntry *head;
    CacheEntry *tail;
    int size;
} Cache;

void init(Cache* cache) {
    for (int i = 0; i < CACHE_SIZE; i++) {
        cache->items[i] = NULL;
    }
    cache->head = NULL;
    cache->tail = NULL;
    cache->size = 0;
}

void add_to_cache(Cache *cache, const char *key, const char *value) {
    unsigned int ind = hash(key);

    // If the cache is full, remove the least recently used entry
    if (cache->size == CACHE_CAPACITY) {
        CacheEntry *old_tail = cache->tail;
        if (cache->tail) {
            unsigned int old_ind = hash(cache->tail->key);
            if (cache->tail->prev) {
                cache->tail = cache->tail->prev;
                cache->tail->next = NULL;
            } else {
                cache->head = cache->tail = NULL;
            }
            cache->items[old_ind] = NULL;
            free(old_tail);
            cache->size--;
        }
    }

    // Create a new cache entry
    CacheEntry *entry = (CacheEntry *)malloc(sizeof(CacheEntry));
    if (entry == NULL) {
        perror("Failed to allocate memory for cache entry");
        exit(EXIT_FAILURE);
    }
    strncpy(entry->key, key, KEY_SIZE - 1);
    entry->key[KEY_SIZE - 1] = '\0';
    strncpy(entry->value, value, VALUE_SIZE - 1);
    entry->value[VALUE_SIZE - 1] = '\0';
    entry->next = NULL;
    entry->prev = NULL;

    // Add the entry to the head of the list
    if (cache->head) {
        cache->head->prev = entry;
        entry->next = cache->head;
    }
    cache->head = entry;
    if (cache->tail == NULL) {
        cache->tail = entry;
    }

    cache->items[ind] = entry;
    cache->size++;
}

const char *retrieve_from_cache(Cache *cache, const char *key) {
    unsigned int ind = hash(key);
    CacheEntry *entry = cache->items[ind];
    while (entry) {
        if (strncmp(entry->key, key, KEY_SIZE) == 0) {
            // Move the entry to the head of the list
            if (entry != cache->head) {
                if (entry == cache->tail) {
                    cache->tail = entry->prev;
                }
                if (entry->prev) {
                    entry->prev->next = entry->next;
                }
                if (entry->next) {
                    entry->next->prev = entry->prev;
                }

                entry->next = cache->head;
                entry->prev = NULL;
                cache->head->prev = entry;
                cache->head = entry;
            }
            return entry->value;
        }
        entry = entry->next;
    }
    return NULL;
}

void free_memory(Cache *cache) {
    CacheEntry *temp = cache->head;
    while (temp) {
        CacheEntry *next = temp->next;
        unsigned int ind = hash(temp->key);
        if (cache->items[ind] == temp) {
            cache->items[ind] = temp->next;
        }
        free(temp);
        temp = next;
    }
}

// Encrypt funciton which encrypts  each entry in the cache 
void encrypt(Cache *cache)
{
  CacheEntry *temp=cache->head;
  while(temp)
  {
    custom_encrypt(temp->key);
    custom_encrypt(temp->value);
    temp=temp->next;
  }
}

// Decrypt funciton which decrypts  each entry in the cache 
void decrypt(Cache *cache)
{
  CacheEntry *temp=cache->head;
  while(temp)
  {
    custom_decrypt(temp->key);
    custom_decrypt(temp->value);
    temp=temp->next;
  }
}

// function to print all the entries of the cache
void print_func(Cache *cache)
{
    CacheEntry *temp=cache->head;
    while(temp)
    {
         printf("Key: %s and Value: %s\n",temp->key,temp->value);
         temp=temp->next;
    }
    printf("\n");
}

void test() {
    clock_t start, end;
    start = clock();
    struct rusage usage_start, usage_end;
    getrusage(RUSAGE_SELF, &usage_start);

    Cache cache;
    init(&cache);

    int lo = 0;
    int hi = 7;
    int miss = 0;
    int hit = 0;

    for (int i = 0; i < 5; i++) {
        char k[KEY_SIZE];
        int el = (rand() % (hi - lo + 1)) + lo;
        snprintf(k, KEY_SIZE, "%d", el);
        trim_newline(k);
        char v[VALUE_SIZE];
        fgets(v, VALUE_SIZE, stdin);
        trim_newline(v);
        add_to_cache(&cache, k, v);
    }

    for (int i = 0; i < 1000; i++) {
        int key = (rand() % (hi - lo + 1)) + lo;
        char s[KEY_SIZE];
        snprintf(s, KEY_SIZE, "%d", key);
        const char *value = retrieve_from_cache(&cache, s);
        if (value) {
            hit++;
        } else {
            miss++;
        }
    }
    printf("Before encryption: \n");
    print_func(&cache);
    encrypt(&cache);
    printf("After encryption: \n");
    print_func(&cache);
    decrypt(&cache);
    printf("After decryption: \n");
    print_func(&cache);
    
    end = clock();
    double diff = (double)(end - start) / CLOCKS_PER_SEC;
    metric(hit,miss);
    getrusage(RUSAGE_SELF, &usage_end);
    long mem_used = usage_end.ru_maxrss - usage_start.ru_maxrss;
    printf("| %-30s | %f seconds         |\n", "Time utilized", diff);
    printf("| %-30s | %ld KB             |\n", "Memory Used", mem_used);
    printf("-------------------------------------------------\n");

    free_memory(&cache);
}

int main() {
    test();
    return 0;
}

