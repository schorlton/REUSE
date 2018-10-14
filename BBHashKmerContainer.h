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
#include <iostream>

//tells bbhash to use included hash function working on u_int64_t input keys :
typedef boomphf::SingleHashFunctor<u_int64_t>  hasher_t;
typedef boomphf::mphf<  u_int64_t, hasher_t  > boophf_t;

template <typename Iter, typename charType>
class BBHashKmerContainer: public AbstractKmerContainer<Iter, charType> {
	public:
		// default constructor
	    BBHashKmerContainer();   
        ~BBHashKmerContainer();
	    // variable constructor
	    BBHashKmerContainer(int numThreads, double gammaFactor, int numElements, int k);

	    bool contains(charType* s); //TODO change to seqan type
	    void add(charType* s, int length); //TODO change to seqan type

	    void addRange(Iter& start, Iter& end);

	void load_index(char *file) override;

	void save_index(char *file) override;

private:
		int kmers_size;
		boophf_t * bphf;
		int numThreads;
		double gammaFactor;
		int numElements;
};


template <typename Iter, typename charType>
void BBHashKmerContainer<Iter,charType>::load(char *fname){


    std::ifstream ifn(fname);

    bphf->load(ifn);
}
template <typename Iter, typename charType>
void BBHashKmerContainer<Iter,charType>::save(char *fname){

    std::ofstream out(fname);

    bphf->save(out);
}
template <typename Iter, typename charType>
BBHashKmerContainer<Iter, charType>::~BBHashKmerContainer(){
    delete bphf;
}

template <typename Iter, typename charType>
BBHashKmerContainer<Iter, charType>::BBHashKmerContainer() {
	BBHashKmerContainer(1, 1.0, 1);
}

template <typename Iter, typename charType>
BBHashKmerContainer<Iter, charType>::BBHashKmerContainer(int numThreads, double gammaFactor, int numElements, int k) {
	this->numThreads = numThreads;
	this->gammaFactor = gammaFactor;
	this->numElements = numElements;
	kmers_size = k;

	bphf = new boomphf::mphf<u_int64_t,hasher_t>();
}

template <typename Iter, typename charType>
bool BBHashKmerContainer<Iter, charType>::contains(charType* s) {
	//TODO change to seqan type
    KMerIterator<charType> iter(s, kmers_size);
	// kmer_encode(iter, iter+kmers_size);
	//query mphf like this
	uint64_t idx = bphf->lookup(kmer_encode(iter, iter+kmers_size));
	// printf(" example query  %lli ----->  %llu \n",data[0],idx);

	return idx != static_cast<uint64_t>(-1);
}

template <typename Iter, typename charType>
void BBHashKmerContainer<Iter, charType>::add(charType* s, int length) {
	//TODO change to seqan type
} 


template <typename Iter, typename charType>
void BBHashKmerContainer<Iter, charType>::addRange(Iter& start, Iter& end) {

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
	

	// free(data);

}

template<typename Iter, typename charType>
void BBHashKmerContainer<Iter, charType>::load_index(char *file) {

}

template<typename Iter, typename charType>
void BBHashKmerContainer<Iter, charType>::save_index(char *file) {

}


#endif
