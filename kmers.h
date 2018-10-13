#ifndef KMERS_
#define KMERS_
#include <iterator>
#include <seqan/sequence.h>

template <typename T, typename E>
KMerIterator<T,E> get_begin(T str, int k){
    KMerIterator<T,E> iter(str,k);
    return iter;
}

template <typename T, typename E>
KMerIterator<T,E> get_end(T str, int k){
    KMerIterator<T,E> iter(str,k);
    iter+=length(str)-k;
}




template <typename T, typename E>
class KMerIterator;

template <typename T, typename E>
E kmer_encode(KMerIterator<T,E> start, KMerIterator<T,E> end){
    E encoding = 0;


    for(;start!=end;start++){
        
        encoding = encoding << 2;
        encoding = start | encoding;
    }

    return encoding;
}

template <typename T, typename E>
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
        KMerIterator operator++(int val){ 
            KMerIterator i = *this; 
            val = 1;
            ptr += val; 
            return i;
        }
        E operator*(){
            KMerIterator i = *this;
            return kmer_encode<T,E>(i,i+k);

        }
        
        E operator|(E val){
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
