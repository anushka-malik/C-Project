# Cache Implementation and replacement algorithms

## Abstract
Caching  play a crucial role in enhancing the performance and efficiency of applications. A caching system is a mechanism that stores frequently accessed data in a temporary storage space called a cache. This temporary storage is much faster to access than the original data source, allowing for faster response times. Cache memory is an indispensable component of modern computer systems, acting as a high-speed buffer between the CPU and the main memory. Caching allows frequently accessed data to be readily available in a faster storage space, which helps alleviate the load on the backend systems and minimizes latency for users.

Since caches are finite in size, they cannot accommodate all the data that an application might need. Here, cache replacement algorithms come into play. They determines which data to evict and which to retain, based on various strategies and access patterns.Effective algorithms enhance performance by increasing the likelihood of cache hits, where requested data is found in the cache, thereby reducing the need for slower memory accesses. 

## FIFO(First-In-First-Out) cache replacement algorithm
FIFO is a cache replacement policy where the oldest items are removed first when the cache reaches its maximum capacity. Imagine the cache as a queue, and the first item that was added to the cache is the first one to be evicted when the cache is full. This strategy ensures that the data that has been in the cache the longest is replaced, making room for new or more frequently accessed data.It eliminates starvation , thus giving chance to the new elements.



