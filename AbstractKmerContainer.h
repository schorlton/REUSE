//
// Created by Munk on 10/13/2018.
//

#ifndef REUSE_ABSTRACTKMERCONTAINER_H
#define REUSE_ABSTRACTKMERCONTAINER_H


template <typename Iterator, typename stringType>
class AbstractKmerContainer {
public:
    AbstractKmerContainer() {}

	template <typename stringType>
    bool contains(stringType&); //TODO change to seqan type
	template <typename stringType>
    void add(stringType&); //TODO change to seqan type

    template <typename Iterator>
    void addRange(Iterator&);
};

#endif //REUSE_ABSTRACTKMERCONTAINER_H
