#ifndef KMERS_
#define KMERS_
#include <iterator>


template <typename T, typename E>
class KMerIterator;

template <typename T, typename E>
E kmer_encode(KMerIterator<T,E> start, KMerIterator<T,E> end){
    E encoding = 0;


    for(;start!=end;start++){
        encoding = start | encoding;
        encoding = encoding << 2;
    }

    return encoding;
}

template <typename T, typename E>
class KMerIterator{
    public:
        KMerIterator();
        ~KMerIterator();
        KMerIterator operator++(){ KMerIterator i = *this; ptr++; return i;}
        E operator*(){
            KMerIterator i = *this;
            return kmer_encode<T,E>(i);

        }
        
        E operator|(E val){
            return ordValue(*ptr) | val;
        }


    private:
        T* ptr;

};


#endif
