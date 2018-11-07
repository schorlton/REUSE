//
// Created by Nolan Woods on 11/6/2018.
//

#ifndef KMER_ITERATOR_CPP
#define KMER_ITERATOR_CPP

#include "KmerIterator.h"


template<typename T>
KmerIterator<T> get_begin(T *str, int k) {
    KmerIterator<T> iter(str, k);
    return iter;
}

template<typename T>
KmerIterator<T> get_end(T *str, int length, int k) {
    KmerIterator<T> iter(str, k);

    return iter + (length - k);
}

template<typename T>
encode_type kmer_encode(KmerIterator<T> start, KmerIterator<T> end) {
    encode_type encoding = 0;

    for (; start != end; start++) {

        encoding = encoding << 2;
        encoding = start | encoding;
    }

    return encoding;
}

template<typename T>
KmerIterator<T>::KmerIterator(const T *str, int k) {
    ptr = str;
    this->k = k;

}

template<typename T>
KmerIterator<T> KmerIterator<T>::operator+=(int val) {
    KmerIterator i = *this;
    ptr += val;
    return i;
}

template<typename T>
KmerIterator<T> &KmerIterator<T>::operator++() {
    ptr += 1;
    return *this;
}

template<typename T>
KmerIterator<T> KmerIterator<T>::operator++(int val) {
    KmerIterator i = *this;
    val = 1;
    ptr += val;
    return i;
}

template<typename T>
encode_type KmerIterator<T>::operator*() {
    KmerIterator i = *this;
    return kmer_encode<T>(i, i + k);

}

template<typename T>
encode_type KmerIterator<T>::operator|(encode_type val) {
    return ordValue(*ptr) | val;
}

template<typename T>
T KmerIterator<T>::operator->() {
    return ptr;
}

template<typename T>
bool KmerIterator<T>::operator==(const KmerIterator &rhs) {
    return ptr == rhs.ptr;
}

template<typename T>
bool KmerIterator<T>::operator!=(const KmerIterator &rhs) {
    return ptr != rhs.ptr;
}

template<typename T>
KmerIterator<T> KmerIterator<T>::operator+(int integer_value) {
    KmerIterator i = *this;
    i.ptr += integer_value;
    return i;
}

template<typename T>
KmerIterator<T> KmerIterator<T>::operator-(int integer_value) {
    KmerIterator i = *this;
    i.ptr -= integer_value;
    return i;
}

#endif