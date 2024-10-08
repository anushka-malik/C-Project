#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <sys/resource.h>
#include "cache.h"

#define CACHE_SIZE 1000
#define KEY_SIZE 32
#define VALUE_SIZE 256
#define CACHE_CAPACITY 5


// define a structure for cache entry

typedef struct CacheEntry{
    char key[KEY_SIZE];
    char value[VALUE_SIZE];
}CacheEntry;


// define a structure for cache
typedef struct Cache{
   CacheEntry *items[CACHE_SIZE];
   int curr_size;
}Cache;


//initialize cache  
void init(Cache *cache)
{
    for(int i=0;i<CACHE_SIZE;i++)
    {
        cache->items[i]=NULL;
    }
    cache->curr_size=0;
}

// function to add an entry to  cache 
void add_to_cache(Cache* cache,const char *key,const char *value)
{
    unsigned int index=hash(key);
    CacheEntry *entry=cache->items[index];
   
    // checks if key already exists and update the value if found
    if(entry!=NULL)
    {
            strncpy(entry->value,value,VALUE_SIZE-1);
            entry->value[VALUE_SIZE-1]='\0';
            return ;
    }

    // if key not found, create a new entry
    CacheEntry *newentry=(CacheEntry *)malloc(sizeof(CacheEntry));
    if(newentry==NULL)
    {
        perror("Failed to allocate memory !");
        exit(EXIT_FAILURE);
    }

    strncpy(newentry->key,key,KEY_SIZE-1);
    newentry->key[KEY_SIZE-1]='\0';
    strncpy(newentry->value,value,VALUE_SIZE-1);
    newentry->value[VALUE_SIZE-1]='\0';
    cache->items[index]=newentry;
    cache->curr_size++;
}

// function to retrieve an entry from the cache
const char* retrieve_from_cache(Cache *cache,const char *key)
{
    unsigned int index=hash(key);
    CacheEntry *entry=cache->items[index];
    if(entry!=NULL)
     return entry->value;
 
    //key not found in cache 
    return NULL;
}

// function to free the memory used by cache ->memory deallocation
void free_cache(Cache *cache)
{
    for(int i=0;i<CACHE_CAPACITY;i++)
    {
        if(cache->items[i]!=NULL)
        {
        CacheEntry *entry=cache->items[i];
        free(entry);
        }
    }
}
// function to print all the cache entries
void print_cache(Cache *cache)
{
   printf("Keys             Values\n");
   for(int i=0;i<CACHE_SIZE;i++)
   {
    if(cache->items[i]!=NULL)
      printf("Key:%s    --->    Value:%s\n",cache->items[i]->key,cache->items[i]->value);
   }
   printf("\n");
}

// function to test the working of cache 
void test_cache()
{
    clock_t start,end;
    start=clock();
    struct rusage usage_start,usage_end;
    getrusage(RUSAGE_SELF,&usage_start);

    Cache cache;
    init(&cache);

    int lo=0;
    int hi=10;
    int miss=0;
    int hit=0;

    for(int i=0;i<CACHE_CAPACITY;i++)
    {
        char k[KEY_SIZE];
        int el=(rand()%(hi-lo+1))+lo;
        snprintf(k, KEY_SIZE, "%d", el);
        trim_newline(k);
         char v[VALUE_SIZE];
         fgets(v,VALUE_SIZE,stdin);
         trim_newline(v);
         add_to_cache(&cache,k,v);
    }
  
    for(int i=0;i<500;i++)
    {
        int key=(rand()%(hi-lo+1))+lo;
        char s[KEY_SIZE];
        snprintf(s,KEY_SIZE,"%d",key);
        const char *value = retrieve_from_cache(&cache, s);
        if (value) 
            hit++;
        else
            miss++;
    }
    print_cache(&cache);
    metric(hit,miss);

    double diff= (double)(end-start)/(CLOCKS_PER_SEC);
    getrusage(RUSAGE_SELF, &usage_end);
    long mem_used = usage_end.ru_maxrss - usage_start.ru_maxrss;

    printf("| %-30s | %f seconds         |\n", "Time utilized", diff);
    printf("| %-30s | %ld KB             |\n", "Memory Used", mem_used);
    printf("-------------------------------------------------\n");

    free_cache(&cache);
}

int main()
{
   test_cache();
   return 0;
}
