#include <iostream>
#include "../code/bloom.h"
#include <gtest/gtest.h>
#include <cassert>




using namespace std;

void TEST_1(){
    // Add and check
    BloomFilter myFilter;  
    myFilter.add("Hello");
    myFilter.add("world");
    assert(myFilter.contains("Hello"));
    assert(myFilter.contains("world"));
    assert(!myFilter.contains("Test"));
    
    cout << "Passed TEST 1a: Add and Check" << endl;
    
    
    BloomFilter myFilter2(100,.0001);
    myFilter2.add("Hello");
    myFilter2.add("world");
    myFilter2.add("This is a sentence.");
    myFilter2.add("Life finds a way");
    myFilter2.add("I'm almost done.");
    myFilter2.add("May the");
    myFilter2.add("Force");
    myFilter2.add("Be with");
    myFilter2.add("You");
    myFilter2.add("You are a wizard Harry");

    assert(myFilter2.contains("Hello"));
    assert(myFilter2.contains("Life finds a way"));
    assert(myFilter2.contains("May the"));
    assert(myFilter2.contains("Force"));
    assert(!myFilter2.contains("Not in test1"));
    assert(!myFilter2.contains("You too"));

    cout << "Passed TEST 1b: Add and Check with Parameters" << endl;

    
    
}

void TEST_2(){
    // Test False Positive Rate
    BloomFilter myFilter(1000,.05);
    int falsePositives = 0;
    int tests = 100;
    
    myFilter.add("Hello");
    myFilter.add("world");
    myFilter.add("RandomString");
    
    for (int i = 0; i < tests; ++i) {
        if (myFilter.contains("RandomString" + to_string(i))) {
            falsePositives++;
        }
    }

    double falsePositiveRate = static_cast<double>(falsePositives) / tests;
    assert(falsePositiveRate <= 0.05);

    cout << "Passed TEST 2: False Positive Rate: " << falsePositives << endl;
}


void TEST_3(){
    // Test is Full

    BloomFilter myFilter(10, 0.01); 
    for (int i = 0; i < 100; ++i) {
        myFilter.add("Item" + to_string(i));
    }
    
    assert(myFilter.isFull());

    cout << "Passed TEST 3: Is Full" << endl;
    
 
}


void TEST_4(){
    // Test Scalable Bloom Filter
     ScalableBloomFilter sBloomFilter(5,.01,2);
    

    for (int i = 0; i < 100; i++) {
        sBloomFilter.add("Item " + to_string(i));
    }
    
    assert(sBloomFilter.contains("Item 1"));
    assert(sBloomFilter.contains("Item 33"));
    assert(sBloomFilter.contains("Item 99"));
    assert(!sBloomFilter.contains("Item 250"));
    assert(!sBloomFilter.contains("Item"));
    assert(!sBloomFilter.contains("Random input"));
    
    cout << "Passed TEST 4: Scalable Bloom Filter Add and Check" << endl;

    
    cout << endl;
    vector<BloomFilter> filters = sBloomFilter.filters;
    cout << "Number of filters: " << filters.size() << endl;
    cout << "Printing Filter " << 3 << " Below: " << endl;
   
    filters[3].PrintFilterDetails();
    filters[3].PrintArray();
    //filters[1].PrintFilterDetails();
    //filters[1].PrintArray();
    //filters[2].PrintFilterDetails();
    //filters[2].PrintArray();
    //filters[3].PrintFilterDetails();
    //filters[3].PrintArray();
    //filters[4].PrintFilterDetails();
    //filters[4].PrintArray();
    
}








int main() {
    TEST_1();
    TEST_2();
    TEST_3();
    TEST_4();
        
    return 0;
}



