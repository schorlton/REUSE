#ifndef KMERS_
#define KMERS_
#include <iterator>
#include <seqan/sequence.h>


using encode_type = unsigned long int;


template <typename T>
class KmerIterator;


template <typename T>
KmerIterator<T> get_begin(T *str, int k){
    KmerIterator<T> iter(str,k);
    return iter;
}

template <typename T>
KmerIterator<T> get_end(T *str, int length, int k){
    KmerIterator<T> iter(str,k);

    return    iter+(length-k);
}

template <typename T>
encode_type kmer_encode(KmerIterator<T> start, KmerIterator<T> end){
    encode_type encoding = 0;


    for(;start!=end;start++){
        
        encoding = encoding << 2;
        encoding = start | encoding;
    }

    return encoding;
}

template <typename T>
class KmerIterator{
    public:
        KmerIterator(const T *str, int k){
            ptr = str;
            this->k = k;

        }
        ~KmerIterator(){}

        KmerIterator operator+=(int val){ 
            KmerIterator i = *this; 

            ptr+=val; 
            return i;
        }
        
        KmerIterator &operator++(){ 
            ptr += 1; 
            return *this;
        }
        KmerIterator operator++(int val){ 
            KmerIterator i = *this; 
            val = 1;
            ptr += val; 
            return i;
        }
        encode_type operator*(){
            KmerIterator i = *this;
            return kmer_encode<T>(i,i+k);

        }
        
        encode_type operator|(encode_type val){
            return ordValue(*ptr) | val;
        }
        
        T operator->(){
            return ptr;
        }

        bool operator==(const KmerIterator& rhs){
            return ptr == rhs.ptr;
        }

        bool operator!=(const KmerIterator& rhs){
            return ptr != rhs.ptr;
        }
        
        KmerIterator operator+(int integer_value){
            KmerIterator i = *this;
            i.ptr+=integer_value;
            return i;
        }
        KmerIterator operator-(int integer_value){
            KmerIterator i = *this;
            i.ptr-=integer_value;
            return i;
        }
    private:
        
        T const * ptr;
        int k;
};


#endif
