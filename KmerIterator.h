#ifndef KMER_ITERATOR_H
#define KMER_ITERATOR_H

using encode_type = unsigned long int;

template <typename T>
class KmerIterator;

template <typename T>
KmerIterator<T> get_begin(T *str, int k);

template <typename T>
KmerIterator<T> get_end(T *str, int length, int k);

template <typename T>
encode_type kmer_encode(KmerIterator<T> start, KmerIterator<T> end);

template <typename T>
class KmerIterator{
    public:
        KmerIterator(const T *str, int k);
        ~KmerIterator() = default;

        KmerIterator operator+=(int val);
        
        KmerIterator &operator++();
        KmerIterator operator++(int val);

        encode_type operator*();
        
        encode_type operator|(encode_type val);
        
        T operator->();

        bool operator==(const KmerIterator& rhs);

        bool operator!=(const KmerIterator& rhs);
        
        KmerIterator operator+(int integer_value);
        KmerIterator operator-(int integer_value);

    private:
        T const * ptr;
        int k;
};

#include "KmerIterator.cpp"

#endif
