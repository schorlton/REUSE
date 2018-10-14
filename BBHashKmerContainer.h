#ifndef BBHASHKMERCONTAINER_H
#define BBHASHKMERCONTAINER_H

#include "AbstractKmerContainer.h"
#include "./lib/BBHash/BooPHF.h"
#include "kmers.h" 

// from BBhash example
#include <time.h>
#include <sys/types.h>
#include <random>
#include <algorithm>

//tells bbhash to use included hash function working on u_int64_t input keys :
typedef boomphf::SingleHashFunctor<u_int64_t>  hasher_t;
typedef boomphf::mphf<  u_int64_t, hasher_t  > boophf_t;

template <typename Iter, typename strType>
class BBHashKmerContainer: public AbstractKmerContainer<Iter, strType> {
	public:
		// default constructor
	    BBHashKmerContainer();

	    // variable constructor
	    BBHashKmerContainer(int numThreads, double gammaFactor, int numElements);

	    bool contains(strType&); //TODO change to seqan type
	    void add(strType&); //TODO change to seqan type

	    void addRange(Iter& start, Iter& end);

	private:
		boophf_t * bphf;
		int numThreads;
		double gammaFactor;
		int numElements;
};


template <typename Iter, typename strType>
BBHashKmerContainer<Iter, strType>::BBHashKmerContainer() {
	BBHashKmerContainer(1, 1.0, 1);
}

template <typename Iter, typename strType>
BBHashKmerContainer<Iter, strType>::BBHashKmerContainer(int numThreads, double gammaFactor, int numElements) {
	this->numThreads = numThreads;
	this->gammaFactor = gammaFactor;
	this->numElements = numElements;

	bphf = nullptr;
}

template <typename Iter, typename strType>
bool BBHashKmerContainer<Iter, strType>::contains(strType&) {
//TODO change to seqan type
	return true;
}

template <typename Iter, typename strType>
void BBHashKmerContainer<Iter, strType>::add(strType&) {
	
//TODO change to seqan type
} 


template <typename Iter, typename strType>
void BBHashKmerContainer<Iter, strType>::addRange(Iter& start, Iter& end) {

	double t_begin,t_end; struct timeval timet;
	
	printf("Construct a BooPHF with  %lli elements  \n", numElements);

	gettimeofday(&timet, NULL); t_begin = timet.tv_sec +(timet.tv_usec/1000000.0);

	// mphf takes as input a c++ range. A simple array of keys can be wrapped with boomphf::range
	// but could be from a user defined iterator (enabling keys to be read from a file or from some complex non-contiguous structure)
	auto data_iterator = boomphf::range(start, end);

	//build the mphf
	bphf = new boomphf::mphf<u_int64_t,hasher_t>(numElements, data_iterator, numThreads, gammaFactor);
	
	gettimeofday(&timet, NULL); t_end = timet.tv_sec +(timet.tv_usec/1000000.0);
	double elapsed = t_end - t_begin;
	
	
	printf("BooPHF constructed perfect hash for %llu keys in %.2fs\n", numElements, elapsed);
	printf("boophf  bits/elem : %f\n",(float) (bphf->totalBitSize())/numElements);
	
	//query mphf like this
	// uint64_t  idx = bphf->lookup(data[0]);
	// printf(" example query  %lli ----->  %llu \n",data[0],idx);

	// free(data);
	// delete bphf;

}


#endif
