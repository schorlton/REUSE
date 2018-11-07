//
// Created by Nolan Woods on 11/6/2018.
//

#include <seqan/sequence.h>
#include <seqan/seq_io.h>
#include <seqan/basic.h>

#include "BBHashKmerContainer.h"
#include "cmdline.h"

using KmerContainer = BBHashKmerContainer<KmerIterator<seqan::Dna5>, seqan::Dna5>;

void print_build_status(unsigned long total_sequences, unsigned long total_kmers) {
    if (total_sequences)
        //Delete previous output
        std::cerr << "\033[F\033[K";
    std::cerr << " Total processed: " << std::setw(10) << total_sequences << " Total kmers: " << std::setw(10)
              << total_kmers << std::endl;
}

int build(ParametersBuild &params) {

    std::cerr << "Building reference......" << std::endl;

    std::cerr << "number of thread " << params.threads << std::endl;
//    std::cout <<"input " << params.seq_filename<< std::endl;

    seqan::SeqFileIn seqfile;

    if (params.seq_filename == "-") {
        open(seqfile, std::cin);
    } else {
        open(seqfile, params.seq_filename.c_str());
    }

    seqan::StringSet<seqan::CharString> ids;
    seqan::StringSet<seqan::Dna5String> seqs;

    try {
        readRecords(ids, seqs, seqfile);
    } catch (seqan::Exception const &e) {
        std::cerr << "ERROR" << e.what() << std::endl;
        return 1;
    }

    int refTableLength = length(seqs[0]) / 2 - 21;
    KmerContainer table(params.threads, 2, refTableLength, params.kmer_length); //TODO replace hardcoded params

    for (unsigned i = 0; i < length(ids); ++i) {

        KmerIterator<seqan::Dna5> _begin = get_begin(toCString(seqs[i]), 21);
        KmerIterator<seqan::Dna5> _end = get_end(toCString(seqs[i]), length(seqs[i]), 21);

        std::cerr << "Length : " << length(seqs[i]) << std::endl;
        table.addRange(_begin, _end);
    }

    table.save(params.output_filename.c_str());


    return 0;
    // build hash table; binary encoding
}
