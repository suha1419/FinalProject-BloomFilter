# CSCI 2270 – Final Project BLOOMFILTER
## Sulman Haque, December 2023
## Overview

In this project we implement a Scalable Bloom Filter in C++

### What is a Bloom Filter

Bloom filters are a probabilistic data structure that prioritize space and time efficiency for item lookup.

Often in applications, data needs to be looked up in an external source. This can be computationally expensive, especially if it requires disk access or making a call to an external service. This becomes problematic if the data needed to be searched does not exist in the disk/external service. Resources were consumed that weren't necessary. Bloom Filters are used in a variety of applications including: malicious URL tracking, cryptocurrencies, video games, user name checking, etc.

A bloom filter is a space and time-efficient way to filter requests by determining if the item is definitely not in the data source, or probably likely. By filtering the requests, you can avoid consuming resources on data that is definitely not in the source and only send requests that are probably likely. With this probablistic approach, there is a non-zero chance it will yield a false positive. In many use cases, this is acceptable and a certain amount of failure can be tolerated to achieve the improvement in performance. However, due to the nature of this data structure, you will never get a false negative.

Bloom filters take advantage of multiple hashing functions and bit arrays. Multiple hashing allows for an input to generate unique hashcodes that can be bucketed, similar to a hashtable. Based on this bucket, a bit array at that index can be updated to 1, to indicate the result of the hash. So for a given input such as a string, there are x number of hashcodes based on the number of hashes applied, and thus, x number of indices in the bit array will be updated from 0 to 1. If the bit is already updated, then there is no need to account for that, as there are x-1 other hashes unique to that input.

The hashing component allows for quick item lookup, and the bit array allows for minimal memory to store.
When determining if an new input is contained in the filter, the input goes through the same hashing functions and the bit array evaluates if all the indices are 1. If so, the input is probably in the array. If any of the indices are 0, we know the input is definitely not in the array. Since there is a chance that the bits at the index were already converted to 1 by other inputs, the result is at best a probability. As more inputs are added to the bit array, the subsequent bits will converge to 1, which will increase the false positive rate and a new (or scalable) bloom filter must be created.

Careful selection bloom filter parameters are necessary to achieve the desired results. This implementation calculates the optimal parameters based on two user friendly inputs: Number of inputs expected and desired false probability rate. With these two inputs, the size of the bit array and the optimal hashing functions required are calculated and embedded in the implementation, so the user does not need to manage those details and can achieve the desired result. See source [2] for the equations.




### Data Structures and Classes 
project data structure implemented
short explanation of the data structure

This implementation uses three core classes.
1) BitArray
2) BloomFilter
3) ScalableBloomFilter

Bit arrays are created and managed by the BloomFilter. BloomFilters are created and managed by the ScalableBloomFilter.
You need not use just the Scalable, as the orignial BloomFilter class will suffice if youR requirements dont change.

1) Bit Array is a class that creates a bit array of size M, the fundamental data structure
    @params bits - bit array
    @params size - size of bit arrray
    -Get,Set,Print methods
    
2) BloomFilter is a class that creates a unique BitArray, with additional methods to interact with the data structure
    @params - shared ptr to a BitArray 
    @params - M_memory - space allocated to BitArray
    @params - N_inputs - number of inputs or capacity of the bit array (user defined)
    @params - P_false_rate - both target false positive rate and current false positive rate. 
              Target is set by user or defaults to .01 (user defined. Use something <=.01. The smaller the false positive, the larger the memory for a given N_input)
    @params - K_hashes - the number of hashes that need to be applied to a given input
    @params - elementCount - number of elements added to the filter
    -Two constructors for the user to use default settings or apply their own requirements to the filter
    -calculate_filter_parameters - function that determines the optimal size and hash functions required to maintain the false positive rate
    -add - method to add strings to the bit array
    -contains - method to check if the string has been added
    -hash - hashing function using Murmur3 implementation[3].
    -NestedHash - method that generates a vector containing x number of hashes, to be used as indices for the bit array
    -isFull - method to determine if the false positive rate has surpassed the current filters requirements
    -print functions for filter details and to print the filter
    
    
3) ScalableBloomFilter is a class that allows for multiple BloomFilters to be created once the capacity of a BloomFilter has been reached
    @params - N_inputs - number of inputs provided by the user. 
    @params - P_false_rate_target - false positive rate provided by the user for this filter
              other parameters for the bloom filter are calculated via the calculate_filter_parameters method in the BloomFilter Class
    @params - filters vector to store all the filters created for scaling
    @params - scaling factor by which to increase the size of the next filter 
    -Two constructors for the user to use default settings or apply their own
    -add - method to add strings to the bit arrays
    -contains - method to check if strings have been added



## Where to Start
After having accepted your assignment in GitHub, _clone_ your private repository to your development environment.  We recommend that you use JupyterHub as your coding environment.
You will need to run ```cmake``` from within the _build_ directory to create the rules for making the applications. (YOU DONT NEED TO RUN CMAKE)
Next, run ```make``` to use those rules to actually build the executable from the source code.
There will be a local executable created that you can use.

(Note: Tests are in the main file. Run ./run_app to see results)

Project has following folder tree:
```
.  
├── CMakeLists.txt  
├── CMakeLists.txt.in  
├── README.md  
├── app  
│   └── main.cpp  
├── build  
├── code  
│   ├── bloom.cpp  
│   └── bloom.h  
└── tests   
```




Sources:
[1] https://medium.com/the-little-bit-ninja/bloom-filters-and-when-to-use-them-ab64028996d4
[2] https://hur.st/bloomfilter/?n=5&p=1.0E-02&m=&k=
[3] https://tsapps.nist.gov/publication/get_pdf.cfm?pub_id=903775#:~:text=The%20probability%20of%20a%20false,mapped%20into%20the%20Bloom%20filter).
[4] https://github.com/aappleby/smhasher/blob/master/src/MurmurHash3.cpp
[5] https://www.youtube.com/watch?v=qZNJTh2NEiU
[6] https://llimllib.github.io/bloomfilter-tutorial/
[7] https://en.wikipedia.org/wiki/Bloom_filter

