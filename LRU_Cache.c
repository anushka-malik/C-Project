#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define KEY_SIZE 32
#define VALUE_SIZE 256
#define CACHE_SIZE 1000
#define CACHE_CAPACITY 4

// define a structure for cache entry

typedef struct CacheEntry{
    char key[KEY_SIZE];
    char value[VALUE_SIZE];
    struct CacheEntry *next;
    struct CacheEntry *prev;
}CacheEntry;

// define a structure for cache

typedef struct Cache{
    CacheEntry *items[CACHE_CAPACITY];
    CacheEntry *head;
    CacheEntry *tail;
    int size;
}Cache;


// function to initialize the cache and its items
void init(Cache* cache)
{
   for(int i=0;i<CACHE_CAPACITY;i++)
   {
    cache->items[i]=NULL;
   }
   cache->head=NULL;
   cache->tail=NULL;
   cache->size=0;
}

// DJB2 Hash function to ensure uniform distribution & reduce collision.
// Hashing -> to compute the index for a given key

unsigned int hash(const char *key)
{
   unsigned int hash=0;
   while(*key)
   {
     hash=(hash<<5)+hash + *key++ ;
   }
   return (hash%CACHE_SIZE);
}

// function to add an entry to  cache and in the linked list as well

void add_to_cache(Cache *cache,const char *key,const char *value)
{
     int ind=hash(key);
     if(cache->size==CACHE_CAPACITY)
     {
          int ind=hash(cache->tail->key);
          cache->items[ind]=NULL;
          cache->tail=(cache->tail)->prev;      
          if (cache->tail) 
          {
            (cache->tail)->next = NULL;
          } 
        else {
            cache->head = NULL;
        }
        cache->size--;
     }
     CacheEntry *entry=(CacheEntry*)malloc(sizeof(CacheEntry));
     strncpy(entry->key,key,KEY_SIZE-1);
     entry->key[KEY_SIZE-1]='\0';
     strncpy(entry->value,value,VALUE_SIZE-1);
     entry->value[VALUE_SIZE-1]='\0';

     entry->next=cache->head;
     entry->prev=NULL;
     if (cache->head) 
     {
        cache->head->prev = entry;
     }
     cache->head=entry;
     if(cache->tail==NULL)
     {
        cache->tail=entry;
     }
     cache->items[ind]=entry;
     cache->size++;
}

// function to return the value corresponding to a key , if it exists
const char *retrieve_from_cache(Cache *cache,const char *key)
{
    int ind=hash(key);
    CacheEntry *entry=cache->items[ind];
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
    printf("No key exists in cache!\n");
    return NULL;
}

// function to print the elements of cache placed in the least recently used (lru) order
void print_cache(Cache *cache)
{
   CacheEntry *temp=cache->head;
   while(temp)
   {
    printf("Key: %s has Value: %s\n",temp->key,temp->value);
    temp=temp->next;
   }
   printf("\n");
}

// function to free the memory allocated -> memory deallocation
 
void free_memory(Cache *cache)
{
   CacheEntry *temp=cache->head;
   while(temp)
   {
    CacheEntry *item=temp;
    int ind=hash(temp->key);
    cache->items[ind]=NULL;
    
    free(item);
    temp=temp->next;
   }
}

// function to test the working of the logic and implementation.
void test()
{
    Cache cache;
    init(&cache);
    add_to_cache(&cache,"1","a");
    add_to_cache(&cache,"2","b");
    add_to_cache(&cache,"3","c");
    add_to_cache(&cache,"4","d");

   if( retrieve_from_cache(&cache,"1")!=NULL)
   {
    const char *str=retrieve_from_cache(&cache,"1");
    printf("%s\n",str);
   }
    add_to_cache(&cache,"5","e");
    print_cache(&cache);

    free_memory(&cache);

}

int main()
{
    test();
    return 0;
}

