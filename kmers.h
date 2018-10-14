#ifndef KMERS_
#define KMERS_
#include <iterator>
#include <seqan/sequence.h>


using encode_type = unsigned long long int;


template <typename T>
class KMerIterator;


template <typename T>
KMerIterator<T> get_begin(T *str, int k){
    KMerIterator<T> iter(str,k);
    return iter;
}

template <typename T>
KMerIterator<T> get_end(T *str, int length, int k){
    KMerIterator<T> iter(str,k);
    return    iter+=(length-k);
}

template <typename T>
encode_type kmer_encode(KMerIterator<T> start, KMerIterator<T> end){
    encode_type encoding = 0;


    for(;start!=end;start++){
        
        encoding = encoding << 2;
        encoding = start | encoding;
    }

    return encoding;
}

template <typename T>
class KMerIterator{
    public:
        KMerIterator(const T *str, int k){
            ptr = str;
            this->k = k;

        }
        ~KMerIterator(){}

        KMerIterator operator+=(int val){ 
            KMerIterator i = *this; 

            ptr+=val; 
            return i;
        }
        
        KMerIterator operator++(){ 
            ptr += 1; 
            return *this;
        }
        KMerIterator operator++(int val){ 
            KMerIterator i = *this; 
            val = 1;
            ptr += val; 
            return i;
        }
        encode_type operator*(){
            KMerIterator i = *this;
            return kmer_encode<T>(i,i+k);

        }
        
        encode_type operator|(encode_type val){
            return ordValue(*ptr) | val;
        }
        
        T operator->(){
            return ptr;
        }

        bool operator==(const KMerIterator& rhs){
            return ptr == rhs.ptr;
        }

        bool operator!=(const KMerIterator& rhs){
            return ptr != rhs.ptr;
        }
        
        KMerIterator operator+(int integer_value){
            KMerIterator i = *this;
            i.ptr+=integer_value;
            return i;
        }
        KMerIterator operator-(int integer_value){
            KMerIterator i = *this;
            i.ptr-=integer_value;
            return i;
        }
    private:
        
        T const * ptr;
        int k;
};


#endif
