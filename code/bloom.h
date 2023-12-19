#include <string>
#include <memory>
#include <vector>
#include <string>
#include <iostream>


using namespace std;


class BitArray{
    public:
        BitArray(int sizeM);
        ~BitArray();
        int GetSize();
        void PrintArray();
        void Set(int index, bool value);
        int Get(int index);
     
    private:
        bool* bits;
        int size;    
};

class BloomFilter {
public:
    BloomFilter();
    BloomFilter(double N, double P);
    ~BloomFilter();
    void add(string key);
    bool contains(string key);
    void PrintArray();
    void PrintFilterDetails();    
    double currentFalsePositiveRate();
    bool isFull();    
    

private:    
    shared_ptr<BitArray> bitArray;
    double M_memory;
    double N_inputs;
    double P_false_rate_target;
    double P_false_rate_current;
    int K_hashes;
    int elementCount;
    void calculate_filter_parameters(double N, float P);
    uint32_t hash(const uint8_t* key, size_t len, uint32_t seed);
    vector<unsigned int> NestedHash(string key);
};


class ScalableBloomFilter {
public:
    ScalableBloomFilter();
    ScalableBloomFilter(double N, double P, double scaleFactor);
    ~ScalableBloomFilter();
    void add(string key);
    bool contains(string key);
    vector<BloomFilter> filters;

private:
    double scalingFactor;
    void addFilter();
    double N_inputs;
    double P_false_rate_target;
    
};


