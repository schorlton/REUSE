//
// Created by Nolan Woods on 11/6/2018.
//

#ifndef BBHASHKMERCONTAINER_CPP
#define BBHASHKMERCONTAINER_CPP

#include <fstream>

#include <memory>

#include "BBHashKmerContainer.h"

template<typename Iter, typename charType>
void BBHashKmerContainer<Iter, charType>::load(const char *fname) {
    std::ifstream stream{fname};
    bphf->load(stream);
}

template<typename Iter, typename charType>
void BBHashKmerContainer<Iter, charType>::save(const char *fname) {
    std::ofstream stream{fname};
    bphf->save(stream);
}

template<typename Iter, typename charType>
BBHashKmerContainer<Iter, charType>::BBHashKmerContainer() : BBHashKmerContainer(1, 1.0, 1) {
}

template<typename Iter, typename charType>
BBHashKmerContainer<Iter, charType>::BBHashKmerContainer(int numThreads, double gammaFactor, int numElements, int kmer_size) {
    this->numThreads = numThreads;
    this->gammaFactor = gammaFactor;
    this->numElements = numElements;
    kmers_size = k;
    bphf = std::make_unique<boomphf::mphf<u_int64_t, hasher_t>>();
}

template<typename Iter, typename charType>
bool BBHashKmerContainer<Iter, charType>::contains(charType *s) {
    //TODO change to seqan type
    KmerIterator<charType> iter(s, kmers_size);
    // kmer_encode(iter, iter+kmers_size);
    //query mphf like this
    uint64_t idx = bphf->lookup(kmer_encode(iter, iter + kmers_size));
    // printf(" example query  %lli ----->  %llu \n",data[0],idx);

    return idx != static_cast<uint64_t>(-1);
}

template<typename Iter, typename charType>
void BBHashKmerContainer<Iter, charType>::add(charType *s, int length) {
    //TODO change to seqan type
    //TODO Implement
}


template<typename Iter, typename charType>
void BBHashKmerContainer<Iter, charType>::addRange(Iter &start, Iter &end) {

    //double t_begin,t_end; struct timeval timet;

    //printf("Construct a BooPHF with  %lli elements  \n", numElements);

    //gettimeofday(&timet, NULL); t_begin = timet.tv_sec +(timet.tv_usec/1000000.0);

    // mphf takes as input a c++ range. A simple array of keys can be wrapped with boomphf::range
    // but could be from a user defined iterator (enabling keys to be read from a file or from some complex non-contiguous structure)
    auto data_iterator = boomphf::range(start, end);

    //build the mphf
    bphf = std::make_unique<boomphf::mphf<u_int64_t, hasher_t>>(numElements, data_iterator, numThreads, gammaFactor);

    //gettimeofday(&timet, NULL); t_end = timet.tv_sec +(timet.tv_usec/1000000.0);
    //double elapsed = t_end - t_begin;


    //printf("BooPHF constructed perfect hash for %llu keys in %.2fs\n", numElements, elapsed);
    //printf("boophf  bits/elem : %f\n",(float) (bphf->totalBitSize())/numElements);


    // free(data);

}

#endif