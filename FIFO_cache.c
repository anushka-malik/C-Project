#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <sys/resource.h>
#include "cache.h"

#define CACHE_SIZE 2000
#define KEY_SIZE 32
#define VALUE_SIZE 256
#define CACHE_CAPACITY 5
#define ENDEC_KEY 3

// define a structure for cache entry
typedef struct CacheEntry{
    char key[KEY_SIZE];
    char value[VALUE_SIZE];
    // struct CacheEntry *next;
}CacheEntry;


// define a queue linked list with front and rear pointers to maintain the FIFO order
typedef struct QueueNode{
   CacheEntry *entry;
   struct QueueNode *next;
}QueueNode;


// define a structure for cache

typedef struct Cache{
   CacheEntry *items[CACHE_SIZE];
   QueueNode *front;
   QueueNode *rear;
   int size;
}Cache;


//initialize cache  

void init(Cache *cache)
{
    for(int i=0;i<CACHE_SIZE;i++)
    {
        cache->items[i]=NULL;
    }
    cache->front=NULL;
    cache->rear=NULL;
    cache->size=0;
}


//function to create a new queue node

QueueNode* create_node(CacheEntry *entry)
{
      QueueNode *new_node=(QueueNode *)malloc(sizeof(QueueNode));
     if (new_node == NULL) 
     {
        perror("Failed to allocate memory for queue node");
        exit(EXIT_FAILURE);
     }
     new_node->entry=entry;
     new_node->next=NULL;
     return new_node;
}

// function to add an entry to  cache 
void add_to_cache(Cache *cache,const char *key,const char *value)
{
    unsigned int index=hash(key);
    CacheEntry *entry=cache->items[index];
   
    // checks if key already exists and update the value if found
    if(entry!=NULL)
    {
            strncpy(entry->value, value, VALUE_SIZE - 1);
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
 
   // Simultaneously , add the entry to queue as well
   QueueNode *new_node= create_node(newentry);
   if(cache->front==NULL )
   {
    cache->front=cache->rear=new_node;
   }
   else
   {
      cache->rear->next=new_node;
      cache->rear=new_node;
   }
   cache->size++;

   // Check if cache size exceeds capacity and evict if necessary 
   if(cache->size > CACHE_CAPACITY)
   {
     QueueNode *old_item=cache->front;
      cache->front = cache->front->next;
        if (cache->front == NULL) {
            cache->rear = NULL;
        }
        cache->size--;

     unsigned int index=hash(old_item->entry->key);
     CacheEntry *current=cache->items[index];
     CacheEntry *prev=NULL;

     if(current !=NULL)
     {
            if(prev==NULL)
             cache->items[index]=current;
            
            free(current);
     }

     free(old_item);
   }
}

// function to retrieve an entry from the cache
const char* retrieve_from_cache(Cache *cache,const char *key)
{
    unsigned int index=hash(key);
    CacheEntry *entry=cache->items[index];

    if(entry!=NULL)
    {
        return entry->value;
    }

    //key not found in cache 
    return NULL;
}


// function to free the memory allocated -> memory deallocation
void free_cache(Cache *cache)
{
     for (int i = 0; i < CACHE_SIZE; i++) 
     {
        CacheEntry *entry = cache->items[i];
        if (entry != NULL)
        {
            free(entry);
        }
    }

    while (cache->front != NULL) 
    {
        QueueNode *temp = cache->front;
        cache->front = cache->front->next;
        free(temp);
    }
}

// Encrypt funciton which encrypts  each entry in the cache 
void encrypt(Cache *cache)
{
  QueueNode *temp=cache->front;
  while(temp)
  {
    custom_encrypt(temp->entry->key);
    custom_encrypt(temp->entry->value);
    temp=temp->next;
  }
}

// Decrypt funciton which decrypts  each entry in the cache 
void decrypt(Cache *cache)
{
  QueueNode *temp=cache->front;
  while(temp)
  {
    custom_decrypt(temp->entry->key);
    custom_decrypt(temp->entry->value);
    temp=temp->next;
  }
}

// function to print all the entries of the cache
void print_func(Cache *cache)
{
    QueueNode *temp=cache->front;
    while(temp)
    {
         printf("Key: %s and Value: %s\n",temp->entry->key,temp->entry->value);
         temp=temp->next;
    }
    printf("\n");
}

// function to test the working of cache 
void test_cache()
{
    clock_t start,end;
    start=clock();
    struct rusage usage_start, usage_end;
    getrusage(RUSAGE_SELF,&usage_start);

    Cache cache;
    init(&cache);

    int lo=0;
    int hi=10;
    int miss=0;
    int hit=0;

    for(int i=0;i<5;i++)
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
        {
            hit++;
        }
        else
        {
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
    
    end=clock();
    double diff= (double)(end-start)/(CLOCKS_PER_SEC);
    
    metric(hit,miss);
    getrusage(RUSAGE_SELF,&usage_end);
    long mem_used=usage_end.ru_maxrss -  usage_start.ru_maxrss;
        
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

