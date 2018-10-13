#ifndef BBHashKmerContainer
#define BBHashKmerContainer

#include "AbstractKmerContainer.h"
#include "./lib/BBHash/BooPHF.h"

// from BBhash example
#include <time.h>
#include <sys/types.h>
#include <random>
#include <algorithm>

//tells bbhash to use included hash function working on u_int64_t input keys :
typedef boomphf::SingleHashFunctor<u_int64_t>  hasher_t;
typedef boomphf::mphf<  u_int64_t, hasher_t  > boophf_t;

template <typename Iterator, typename stringType>
class BBHashKmerContainer {
public:

    BBHashKmerContainer() {}

	template <typename stringType>
    bool contains(stringType&); //TODO change to seqan type
	template <typename stringType>
    void add(stringType&); //TODO change to seqan type

    template <typename Iterator>
    void addRange(Iterator&);
};


BBHashKmerContainer::BBHashKmerContainer() {
	 
	std::vector<u_int64_t> input_keys;
	//
	... fill the input_keys vector
	//build the mphf  
	boophf_t * bphf = new boomphf::mphf<u_int64_t,hasher_t>(input_keys.size(),input_keys,nthreads);
	 
	//query the mphf :
	uint64_t  idx = bphf->lookup(input_keys[0]);
}


template <typename stringType>
bool BBHashKmerContainer::contains(stringType&) {
//TODO change to seqan type
	
}

template <typename stringType>
void BBHashKmerContainer::add(stringType&) {
	
//TODO change to seqan type
} 

template <typename Iterator>
void BBHashKmerContainer::addRange(Iterator&) {

}


#endif