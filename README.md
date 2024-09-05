# Cache Implementation and replacement algorithms

## Abstract
A caching system is a mechanism that stores frequently accessed data in a temporary storage space called a cache. This temporary storage is much faster to access than the original data source, allowing for faster response times. Cache memory acts as a high-speed buffer between the CPU and the main memory. Caching allows frequently accessed data to be readily available in a faster storage space, which helps alleviate the load on the backend systems and minimizes latency for users.

## Cache-Implementation-using-hashmap
Since caches are finite in size, they cannot accommodate all the data that a process might require.Here, cache replacement algorithms comes into play.They determines the data which needs to be evicted and which is to be retained.Effective algorithms enhance performance by increasing the likelihood of cache hits thereby reducing the need for slower memory accesses. 

### Implementation details
+ **Data Structures**
  + Structures
    + (Cache)-to store cache entries,size,next pointer.
    + (CacheEntry)- to store data in form of key-value pair.
  + HashMap- to map each key to a corresponding index in the cache.
  + LinkedList- to store all values in case of hashing collision by maintaining next pointer.
+ **Methods** -insertion and retrieval.
+ **Memory deallocation** function to free up the heap memory.

## FIFO(First-In-First-Out) cache replacement algorithm

### Overview
FIFO is a cache replacement policy where the oldest items are removed first when the cache reaches its maximum capacity. Imagine it as a queue,where the first item that was added to the cache is the first one to be evicted.This ensures that the data that has been in the cache the longest is replaced, making room for new or more frequently accessed data.

<p align="center"> <img width="200" src="https://github.com/user-attachments/assets/b6c2d47a-169c-4683-98be-990194341997"></p>

### Implementation
- The program uses a cache structure to store the items in the form of key-value pair.
- Uses queue(implemented using linked list,i.e.pointers) to maintain the order of insertion.
- It calculates the CPU time and memory utilization.

## LRU(Least Recently Used) cache replacement algorithm

### Overview
The Least Recently Used (LRU) cache is a popular caching strategy that discards the least recently used items first to make room for new elements when the cache is full. It organizes items in the order of their use, allowing us to easily identify items that have not been used for a long time. This strategy is useful for improving the performance of caching systems.

<p align="center">  <img width="350" src="https://github.com/user-attachments/assets/158b3d4d-9cfb-4777-b347-9f37f8adb69d"></p>

### Implementation
- It encompasses the use of a doubly-linked list (implemented using pointers) to keep track of least used item for eviction when cache is full.
- Metric evaluation -Hit ratio, Miss ratio, CPU time and memory.

## MRU(Most-Recently-Used) Cache replacement algorithm

### Overview
MRU focuses on removing the most recently used items when the cache reaches its capacity. This approach can be particularly useful in scenarios where recently accessed items are less likely to be needed again soon.Examples of such scenarios include certain types of web caches or buffer caches where recent data is not necessarily the most relevant for future operations. 

|      Costs     |  Worst Case |
| -------------  | ------------|
| Insert an item |      O(1)   |
| Access an item |      O(1)   |
|      Space     |      O(N)   |
