//
// Created by Munk on 10/13/2018.
//

#ifndef REUSE_ABSTRACTKMERCONTAINER_H
#define REUSE_ABSTRACTKMERCONTAINER_H

#include <string>

class AbstractKmerContainer {
public:
    AbstractKmerContainer() {}

    bool contains(std::string&); //TODO change to seqan type
    void add(std::string&); //TODO change to seqan type
};


#endif //REUSE_ABSTRACTKMERCONTAINER_H
