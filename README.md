# Cache Implementation and replacement algorithms

## Abstract
A caching system is a mechanism that stores frequently accessed data in a temporary storage space called a cache. This temporary storage is much faster to access than the original data source, allowing for faster response times. Cache memory acts as a high-speed buffer between the CPU and the main memory. Caching allows frequently accessed data to be readily available in a faster storage space, which helps alleviate the load on the backend systems and minimizes latency for users.

## Cache-Implementation-using-hashmap
Since caches are finite in size, they cannot accommodate all the data that a process might require.Here, cache replacement algorithms comes into play.They determines the data which needs to be evicted and which is to be retained.Effective algorithms enhance performance by increasing the likelihood of cache hits thereby reducing the need for slower memory accesses. 

### Prerequisites
``` GCC compiler ```

### Compilation
 + Include header file in the program.<br>
    ```
    #include "cache.h"
    ```                                                                       
 + Compile and include header file into the program from the library<br>
    ```
    gcc -c cache_implementation_hashmap -L. lib_cachelib.a
    ```
+ Run
  ```
  ./a.out
  ```


### Implementation details
+ **Data Structures**
  + Structures
    + (Cache)-to store cache entries,size,next pointer.
    + (CacheEntry)- to store data in form of key-value pair.
  + HashMap- to map each key to a corresponding index in the cache.
  + LinkedList- to store all values in case of hashing collision by maintaining next pointer.
+ **Methods** -insertion and retrieval.
+ **Memory deallocation** function to free up the heap memory.

#### Complexity Analysis

|      Costs     |  Worst Case |
| -------------  | ------------|
| Insert an item |      O(1)   |
| Access an item |      O(1)   |
|      Space     |      O(N)   |

## FIFO(First-In-First-Out) cache replacement algorithm

### Overview
FIFO is a cache replacement policy where the oldest items are removed first when the cache reaches its maximum capacity. Imagine it as a queue,where the first item that was added to the cache is the first one to be evicted.This ensures that the data that has been in the cache the longest is replaced, making room for new or more frequently accessed data.

<p align="center"> <img width="400" src="https://github.com/user-attachments/assets/b6c2d47a-169c-4683-98be-990194341997"></p>

### Implementation
- The program uses a cache structure to store the items in the form of key-value pair.
- Uses queue(implemented using linked list,i.e.pointers) to maintain the order of insertion.
- It calculates the CPU time and memory utilization.

### Usage                                                                     
 + Compile and include header file into the program from the library<br>
    ```
    gcc -c fifo_cache.c -L. lib_cachelib.a
    ./a.out
    ```

#### Metrics evaluation
```
Cache Metrics:
----------------------------------------------------------
| Total number of cache hits     | 180                    |
| Total number of cache miss     | 320                    |
| Hit ratio                      | 36.00%                 |
| Miss ratio                     | 64.00%                 |
| Time utilized                  | 0.001485 seconds       |
| Memory Used                    | 0 KB                   |
----------------------------------------------------------
```

## LRU(Least Recently Used) cache replacement algorithm

### Overview
The Least Recently Used (LRU) cache is a popular caching strategy that discards the least recently used items first to make room for new elements when the cache is full. It organizes items in the order of their use, allowing us to easily identify items that have not been used for a long time. This strategy is useful for improving the performance of caching systems.

<p align="center">  <img width="500" src="https://github.com/user-attachments/assets/158b3d4d-9cfb-4777-b347-9f37f8adb69d"></p>

### Implementation
- It encompasses the use of a doubly-linked list (implemented using pointers) to keep track of least used item for eviction when cache is full.
- Metric evaluation -Hit ratio, Miss ratio, CPU time and memory.

### Usage                                                                     
 + Compile and include header file into the program from the library<br>
    ```
    gcc -c LRU_Cache.c -L. lib_cachelib.a
    ./a.out
    ```

#### Metrics evaluation
```
Cache Metrics:
---------------------------------------------------------
| Total number of cache hits     | 485                    |
| Total number of cache misses   | 515                    |
| Hit ratio                      | 48.50%                 |
| Miss ratio                     | 51.50%                 |
| Time utilized                  | 0.001795 seconds       |
| Memory Used                    | 0 KB                   |
--------------------------------------------------------

```

## MRU(Most-Recently-Used) Cache replacement algorithm

### Overview
MRU focuses on removing the most recently used items when the cache reaches its capacity. This approach can be particularly useful in scenarios where recently accessed items are less likely to be needed again soon.Examples of such scenarios include certain types of web caches or buffer caches where recent data is not necessarily the most relevant for future operations. 

### Usage                                                                     
 + Compile and include header file into the program from the library<br>
    ```
    gcc -c MRU-Cache.c -L. lib_cachelib.a
    ./a.out
    ```

#### Metrics evaluation
```
Cache Metrics:
---------------------------------------------------------
| Total number of cache hits     | 485                    |
| Total number of cache misses   | 515                    |
| Hit ratio                      | 48.50%                 |
| Miss ratio                     | 51.50%                 |
| Time utilized                  | 0.000568 seconds       |
| Memory Used                    | 0 KB                   |
---------------------------------------------------------
 ```
### Encryption-Decryption Algorithm
+ Implemented Caesar cipher encryption-decryption to enhance cache security and privacy.
+ In scenarios where cache data needs to be protected (e.g., sensitive information in secure systems),encryption can ensure that even if an attacker gains access to the cache, they cannot easily access the data.
+ It adds an extra layer of security making the system more robust against data breaches.

<p><img width="2000" src="https://github.com/user-attachments/assets/8809bf84-d88f-48b9-9782-537f6ef3475e"> </p>


