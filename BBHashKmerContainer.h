#ifndef BBHASHKMERCONTAINER_H
#define BBHASHKMERCONTAINER_H

#include "AbstractKmerContainer.h"
#include "./lib/BBHash/BooPHF.h"
#include "KmerIterator.h"

// from BBhash example
#include <time.h>
#include <sys/types.h>
#include <random>
#include <algorithm>


//tells bbhash to use included hash function working on u_int64_t input keys :
using hasher_t = boomphf::SingleHashFunctor<u_int64_t>;
using boophf_t = boomphf::mphf<u_int64_t, hasher_t>;

template<typename Iter, typename charType>
class BBHashKmerContainer : public AbstractKmerContainer<Iter, charType> {
public:
    // default constructor
    BBHashKmerContainer();

    ~BBHashKmerContainer() = default;

    // variable constructor
    BBHashKmerContainer(int numThreads, double gammaFactor, int numElements, int kmer_size);

    bool contains(charType *s); //TODO change to seqan type
    void add(charType *s, int length); //TODO change to seqan type

    void addRange(Iter &start, Iter &end);

    void load(const char *fname);

    void save(const char *fname);

private:
    int kmers_size;
    std::unique_ptr<boophf_t> bphf;
    int numThreads;
    double gammaFactor;
    int numElements;
};

#include "BBHashKmerContainer.cpp"

#endif
