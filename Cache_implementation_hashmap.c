#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <sys/resource.h>

#define CACHE_SIZE 1000
#define KEY_SIZE 32
#define VALUE_SIZE 256

// define a structure for cache entry

typedef struct CacheEntry{
    char key[KEY_SIZE];
    char value[VALUE_SIZE];
    struct CacheEntry *next;
}CacheEntry;


// define a structure for cache

typedef struct Cache{
   CacheEntry *items[CACHE_SIZE];
}Cache;


//initialize cache  

void init(Cache *cache)
{
    for(int i=0;i<CACHE_SIZE;i++)
    {
        cache->items[i]=NULL;
    }
}

// DJB2 Hash function to ensure uniform distribution & reduce collision.
// Hashing -> to compute the index for a given key
unsigned int hash(const char* key)
{
   unsigned int hash=0;
   while(*key)
   {
     hash=(hash<<5)+hash + *key++ ;
   }
   return (hash%CACHE_SIZE);
}


// function to add an entry to  cache 
void add_to_cache(Cache* cache,const char *key,const char *value)
{
    unsigned int index=hash(key);
    CacheEntry *entry=cache->items[index];
   
    // checks if key already exists and update the value if found
    while(entry!=NULL)
    {
        if(strcmp(entry->key,key)==0)
        {
            strncpy(entry->value,value,VALUE_SIZE-1);
            entry->value[VALUE_SIZE-1]='\0';
            return ;
        }
        entry=entry->next;
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
    newentry->next=cache->items[index];
    cache->items[index]=newentry;
}


// function to retrieve an entry from the cache
const char* retrieve_from_cache(Cache *cache,const char *key)
{
    unsigned int index=hash(key);
    CacheEntry *entry=cache->items[index];

    while(entry!=NULL)
    {
        if(strcmp(entry->key,key)==0)
        {
            return entry->value;
        }
        entry=entry->next;
    }

    //key not found in cache 
    return NULL;
}

// function to free the memory used by cache ->memory deallocation
void free_cache(Cache *cache)
{
    for(int i=0;i<CACHE_SIZE;i++)
    {
        CacheEntry *entry=cache->items[i];
        while(entry!=NULL)
        {
            CacheEntry *temp=entry;
            free(temp);
            entry=entry->next;
        }
    }
}
// function to trim newline character from the key and value for clean output
void trim_newline(char *str)
{
	char *pos;
	if((pos= strchr(str,'\n'))!= NULL)
	{
		*pos='\0';
	}
}

// function to test the working of cache 
void test_cache()
{
    clock_t start,end;
    start=clock();
    struct rusage usage_start;
    getrusage(RUSAGE_SELF,&usage_start);

    Cache cache;
    init(&cache);
    for(int i=0;i<5;i++)
    {
    char k[KEY_SIZE];
    fgets(k,KEY_SIZE,stdin);
    trim_newline(k);
    char v[VALUE_SIZE];
    fgets(v,VALUE_SIZE,stdin);
    trim_newline(v);
    add_to_cache(&cache,k,v);
    }

    for(int i=0;i<3;i++)
    {
	    printf("Enter the key to retrive:");
	    char search[KEY_SIZE];
	    fgets(search,KEY_SIZE,stdin);
	    trim_newline(search);
	    const char *val=retrieve_from_cache(&cache,search);
	    if(val)
		    printf("Value of %s is %s \n",search,val);
	    else
		    printf("Given key doesnot exist !\n");
    }
    end=clock();
    double diff= (double)(end-start)/(CLOCKS_PER_SEC);
    printf("\nTime utilized is: %f seconds\n",diff);
    struct rusage usage_end;
    getrusage(RUSAGE_SELF,&usage_end);
    long mem_used=usage_end.ru_maxrss -  usage_start.ru_maxrss;
    printf("Memory Used: %ld KB \n",mem_used);
    free_cache(&cache);

}

int main()
{
   test_cache();
   return 0;
}
