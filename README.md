# Cache Implementation and replacement algorithms

## Abstract
A caching system is a mechanism that stores frequently accessed data in a temporary storage space called a cache. This temporary storage is much faster to access than the original data source, allowing for faster response times. Cache memory acts as a high-speed buffer between the CPU and the main memory. Caching allows frequently accessed data to be readily available in a faster storage space, which helps alleviate the load on the backend systems and minimizes latency for users.

Since caches are finite in size, they cannot accommodate all the data that an application might need. Here, cache replacement algorithms comes into play. They determines which data to evict and which to retain, based on various strategies and access patterns.Effective algorithms enhance performance by increasing the likelihood of cache hitshereby reducing the need for slower memory accesses. 

## FIFO(First-In-First-Out) cache replacement algorithm
FIFO is a cache replacement policy where the oldest items are removed first when the cache reaches its maximum capacity. Imagine the cache as a queue, and the first item that was added to the cache is the first one to be evicted when the cache is full. This strategy ensures that the data that has been in the cache the longest is replaced, making room for new or more frequently accessed data.It eliminates starvation , thus giving chance to the new elements.

<p align="center"> <img width="200" src="https://github.com/user-attachments/assets/b6c2d47a-169c-4683-98be-990194341997"></p>

### Implementation Details

- The program uses a cache structure to store the items in the form of key-value pair.
- Uses queue(implemented using linked list,i.e.pointers) to maintain the order of insertion.
- It calculates the CPU time and meo=mory utilization.

## LRU(Least Recently Used) cache replacement algorithm
A Least Recently Used (LRU) Cache organizes items in order of use, allowing you to quickly identify which item hasn't been used for the longest amount of time.The Least Recently Used (LRU) cache is a popular caching strategy that discards the least recently used items first to make room for new elements when the cache is full. It organizes items in the order of their use, allowing us to easily identify items that have not been used for a long time. This strategy is useful for optimizing the use of limited cache space and improving the performance of caching systems.

## MRU(Most-Recently-Used) Cache replacement algorithm
MRU focuses on removing the most recently used items when the cache reaches its capacity. This approach can be particularly useful in scenarios where recently accessed items are less likely to be needed again soonMRU caching is particularly suited for applications where the pattern of data access is such that the most recently used data is less likely to be reused soon. Examples of such scenarios include certain types of web caches or buffer caches where recent data is not necessarily the most relevant for future operations. The MRU algorithm may not be ideal for all scenarios, as it can lead to suboptimal performance if the access pattern does not align with the assumptions of the MRU strategy.

|      Costs     |  Worst Case |
| -------------  | ------------|
| Insert an item |      O(1)   |
| Access an item |      O(1)   |
|      Space     |      O(N)   |
