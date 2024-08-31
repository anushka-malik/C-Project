 #include <stdio.h>
 #include <stdlib.h>
 #include <limits.h>
 #include <stdbool.h>
 #include <time.h>
 #include <sys/resource.h>
 #define MAX_CAP 100

 typedef struct CacheItem{
      int key;
      int val;
 }CacheItem;

 typedef struct  Cache{
      int size;
      CacheItem* Arr_item[MAX_CAP];
 }Cache;

 void add_to_cache(Cache* basic_cache,int k,int val)
 {
      if(k<0 || k>100)
              printf("Sorry ! item cannot be inserted\n");
      CacheItem* item=(CacheItem*)malloc(sizeof(CacheItem));
  item->key=k;
      item->val=val;
      basic_cache->Arr_item[k]=item;
      basic_cache->size++;
 }
 int retrieve(Cache* basic_cache,int key)
 {
      if(key<0 || key>100)
           return INT_MIN;
      CacheItem* item=basic_cache->Arr_item[key];
      return item->val;
 }
 void test()
 {
        clock_t start,end;
        struct rusage usage_start;
        getrusage(RUSAGE_SELF,&usage_start);
        start=clock();
        Cache* basic_cache=(Cache*)malloc(sizeof(Cache));
        basic_cache->size=0;
        for(int i=0;i<MAX_CAP;i++)
        {
         basic_cache->Arr_item[i]=NULL;
        }
         add_to_cache(basic_cache,1,10);
         add_to_cache(basic_cache,2,20);
         add_to_cache(basic_cache,3,30);
         //add_to_cache(basic_cache,4,40);
         printf("%d\n",retrieve(basic_cache,2));
         printf("%d\n",retrieve(basic_cache,104));
         printf("%d\n",basic_cache->size);

         struct rusage usage_end;
         getrusage(RUSAGE_SELF,&usage_end);

         long mem_used=usage_end.ru_maxrss -  usage_start.ru_maxrss;
         printf("Memory used : %ld KB \n",mem_used);

         end=clock();
         double diff=((double)(end-start))/CLOCKS_PER_SEC;
         printf("Time utilized = %f seconds\n",diff);

         for(int i=0;i<MAX_CAP;i++)
         {
                 free(basic_cache->Arr_item[i]);
         }
         free(basic_cache);
  }

 int main()
 {
         test();
         return 0;
 }

