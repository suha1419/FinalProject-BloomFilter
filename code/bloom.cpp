#include "bloom.h"
#include <iostream>
#include <vector>
#include <cstdint>
#include <cstddef>
#include <cmath>

using namespace std;
// BITARRAY CLASS
//-------------------------------------------------------------------
BitArray::BitArray(int size) : size(size) {
    // Create new arrary of bits in heap
    bits = new bool[size];
    int i;
    
    // Set each bit to 0
    for(i=0;i<size;i++){
        bits[i] = 0;
    }
    
};
BitArray::~BitArray(){
    delete[] bits;
};

int BitArray::GetSize(){
    return this->size;
};


void BitArray::Set(int index, bool value){
    if(index < size){
        bits[index] = value;
    }
};

int BitArray::Get(int index){
    int value;
    if(index < size){
        value = bits[index];
    }
    return value;
};
void BitArray::PrintArray(){
    int i;
    cout << "Print Filter: "<<endl;
    for(i=0;i<this->size;i++){
        cout << bits[i] << " ";
    }
    cout << endl;
    
};


// ADDITIONAL FUNCTIONS
//--------------------------------------------------------------------
// Bucket Function for Indexing - SRC from Hash Table Assignment
unsigned int ModuloBucketFunc(unsigned int hashcode, unsigned int cap){
  unsigned int b = hashcode % cap;
  return b;
};



// BLOOMFILTER CLASS
//--------------------------------------------------------------------
BloomFilter::BloomFilter() {
    // Set Private Variables
    N_inputs = 20;
    P_false_rate_target = .001;
    elementCount = 0;
    
    // Calculate memory and hashes required
    calculate_filter_parameters(N_inputs, P_false_rate_target);
    
    
    // Create and set bitarray for filter
    bitArray = make_shared<BitArray>(M_memory);


};

BloomFilter::BloomFilter(double N, double P){
    // Set Private Variables
    N_inputs = N;
    P_false_rate_target = P;
    elementCount = 0;

    // Calculate memory and hashes required
    calculate_filter_parameters(N, P);
    
    // Create and set bitarray for filter
    bitArray = make_shared<BitArray>(M_memory);
    
};


BloomFilter::~BloomFilter() {  
};
void BloomFilter::calculate_filter_parameters(double N, float P){ 
    // Calcuate optimal parameters for filter: src [2] in README
    // Calculate memory
    double M = ceil((N * log(P)) / log(1 / pow(2,log(2))));
    
    // Calculate hashes required
    int K = round((M / N) * log(2));

    // Set Memory (BitArray Size)
    M_memory = M;
    
    // Set Hashes Required
    K_hashes = K;
    
}


void BloomFilter::add(string key){
    // Add count and update current false positive rate
    elementCount++;
    P_false_rate_current = currentFalsePositiveRate();
    
    // Calculate multiple hashes for key
    vector<unsigned int> result = NestedHash(key);
    unsigned int i;

    // Change bit at each index from the hash values
    int index;
    for(i=0;i<result.size();i++){
        index = result[i];
        bitArray->Set(index,true);
    }
    
};


bool BloomFilter::contains(string key){
    bool does_contain = true;
    
    // Generate Hash of the Key
    vector<unsigned int> result = NestedHash(key);
    unsigned int i;
    int index;
    
    // Check array for at hash index locations
    for(i=0;i<result.size();i++){
        index = result[i];
        if (bitArray->Get(index) == 0){
            does_contain = false;
        }
    }
    
    return does_contain;
};


double BloomFilter::currentFalsePositiveRate(){
    // Calculate the current false positive rate src [3] in README
    return pow(1 - exp(-(K_hashes) * elementCount / M_memory), K_hashes);
}

bool BloomFilter::isFull(){   
    return currentFalsePositiveRate() >= P_false_rate_target;
 
}



// Murmur hash - src[4] in README
uint32_t BloomFilter::hash(const uint8_t* key, size_t len, uint32_t seed) {
    uint32_t h1 = seed;
    const uint32_t c1 = 0xcc9e2d51;
    const uint32_t c2 = 0x1b873593;
    const int r1 = 15;
    const int r2 = 13;
    const uint32_t m = 5;
    const uint32_t n = 0xe6546b64;

    const int nblocks = len / 4;
    const uint32_t* blocks = (const uint32_t*) key;
    const uint8_t* tail = (const uint8_t*) (key + nblocks * 4);

    for (int i = 0; i < nblocks; i++) {
        uint32_t k1 = blocks[i];

        k1 *= c1;
        k1 = (k1 << r1) | (k1 >> (32 - r1));
        k1 *= c2;

        h1 ^= k1;
        h1 = ((h1 << r2) | (h1 >> (32 - r2))) * m + n;
    }

    uint32_t k1 = 0;

    switch (len & 3) {
        case 3: k1 ^= tail[2] << 16;
        case 2: k1 ^= tail[1] << 8;
        case 1: k1 ^= tail[0];
                k1 *= c1; k1 = (k1 << r1) | (k1 >> (32 - r1)); k1 *= c2; h1 ^= k1;
    };

    h1 ^= len;
    h1 ^= h1 >> 16;
    h1 *= 0x85ebca6b;
    h1 ^= h1 >> 13;
    h1 *= 0xc2b2ae35;
    h1 ^= h1 >> 16;

    return h1;
}


vector<unsigned int> BloomFilter::NestedHash(string key){
    int i;
    unsigned int bucket;
    vector<unsigned int> result;
    uint32_t seed = 42;
    uint32_t hashcode;

    // Generate x number of hashcodes using different seeds for each key
    for(i=1; i<=K_hashes+1;i++){
        seed = seed + i;
        hashcode = hash(reinterpret_cast<const uint8_t*>(key.data()), key.length(), seed);
        bucket = ModuloBucketFunc(hashcode, M_memory);
        result.push_back(bucket);
    }
    
    return result;
    
};



void BloomFilter::PrintFilterDetails(){
    cout << "Bloom Filter Details: " << endl;
    cout << "Size (bits): " << M_memory << endl;
    cout << "Size (KB): " << ((M_memory / 8)/1000) << endl;
    cout << "Capacity: " << N_inputs << endl;
    cout << "Inserted Elements: " << elementCount << endl;
    cout << "Hash Functions: " << K_hashes << endl;
    cout << "False Rate Target: " << P_false_rate_target << endl;
    cout << "False Rate Current: " << P_false_rate_current << endl;
    cout << "Is Full: " << isFull() << endl;
    cout << endl;
    

};

void BloomFilter::PrintArray(){
    bitArray->PrintArray();
};



// SCALABLEBLOOMFILTER CLASS
//---------------------------------------------------------------------------------------
ScalableBloomFilter::ScalableBloomFilter(){
    N_inputs = 20;
    P_false_rate_target = .001;
    scalingFactor = 2;
    addFilter();
};
ScalableBloomFilter::ScalableBloomFilter(double N, double P, double scaleFactor){
    N_inputs = N;
    P_false_rate_target = P;
    scalingFactor = scaleFactor;
    addFilter();
};
ScalableBloomFilter::~ScalableBloomFilter(){  
};

void ScalableBloomFilter::addFilter(){
    // If existing filter, scale the input, create and add new filter 
    if (!filters.empty()){
        N_inputs = N_inputs * scalingFactor;
    }
    
    BloomFilter newFilter(N_inputs,P_false_rate_target);
    filters.push_back(newFilter);
    
};
void ScalableBloomFilter::add(string key){
    // if last filter is full, create and add a new filter
    if (filters.back().isFull()){
        addFilter();
    }
    filters.back().add(key);
};
bool ScalableBloomFilter::contains(string key){
    // Iterate through filter collection and check if key is contained
    for (auto& filter : filters) {
        if (filter.contains(key)) {
            return true;
        }
    }
    return false;  
};


    
    







    
