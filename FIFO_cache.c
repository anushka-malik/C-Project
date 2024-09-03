#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <sys/resource.h>

#define CACHE_SIZE 10
#define KEY_SIZE 32
#define VALUE_SIZE 256
#define CACHE_CAPACITY 9

// define a structure for cache entry

typedef struct CacheEntry{
    char key[KEY_SIZE];
    char value[VALUE_SIZE];
    struct CacheEntry *next;
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
    while(entry!=NULL)
    {
        if(strcmp(entry->key,key)==0)
        {
            strncpy(entry->value, value, VALUE_SIZE - 1);
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

     while(current !=NULL)
     {
        if(strcmp(current->key,old_item->entry->key)==0)
        {
            if(prev==NULL)
             cache->items[index]=current->next;
            else
             prev->next=current->next;

            free(current);
            break;
        }
        prev=current;
        current=current->next;
     }

     free(old_item);
   }
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

// function to trim newline character from the key and value for clean output

void trim_newline(char *str)
{
	char *pos;
	if((pos= strchr(str,'\n'))!= NULL)
	{
		*pos='\0';
	}
}

// function to free the memory allocated -> memory deallocation
void free_cache(Cache *cache)
{
     for (int i = 0; i < CACHE_SIZE; i++) 
     {
        CacheEntry *entry = cache->items[i];
        while (entry != NULL)
        {
            CacheEntry *temp = entry;
            entry = entry->next;
            free(temp);
        }
    }

    while (cache->front != NULL) 
    {
        QueueNode *temp = cache->front;
        cache->front = cache->front->next;
        free(temp);
    }
}

// function to print all the entries of the cache
void print_cache(Cache *cache)
{
    QueueNode *temp=cache->front;
    while(temp)
    {
         printf("Key: %s and Value: %s\n",temp->entry->key,temp->entry->value);
         temp=temp->next;
    }
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
    for(int i=0;i<3;i++)
    {
         char k[KEY_SIZE];
         fgets(k,KEY_SIZE,stdin);
         trim_newline(k);
         char v[VALUE_SIZE];
         fgets(v,VALUE_SIZE,stdin);
         trim_newline(v);
         add_to_cache(&cache,k,v);
    }
    print_cache(&cache);
    char s[KEY_SIZE];
    fgets(s,KEY_SIZE,stdin);
    trim_newline(s);
    const char *value = retrieve_from_cache(&cache, s);
    if (value) 
    {
        printf("Value for key:%s is %s \n",s,value);
    }
    else
    {
        printf("Key %s not found in cache.\n",s);
    }

    end=clock();
    double diff= (double)(end-start)/(CLOCKS_PER_SEC);

    printf("\nTime utilized is: %f seconds\n",diff);

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

