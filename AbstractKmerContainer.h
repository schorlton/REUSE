//
// Created by Munk on 10/13/2018.
//

#ifndef REUSE_ABSTRACTKMERCONTAINER_H
#define REUSE_ABSTRACTKMERCONTAINER_H


template <typename Iter, typename stringType>
class AbstractKmerContainer {
public:
    AbstractKmerContainer();
    virtual bool contains(stringType&) = 0; //TODO change to seqan type
    virtual void add(stringType&) = 0; //TODO change to seqan type

    virtual void addRange(Iter&, Iter&) = 0;
};

#endif //REUSE_ABSTRACTKMERCONTAINER_H
