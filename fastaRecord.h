/*
 * fastaRecord.h
 *
 *  Created on: Oct 13, 2018
 *      Author: jassi
 */
#ifndef FASTARECORD_H_
#define FASTARECORD_H_

#include <seqan/sequence.h>
#include <seqan/seq_io.h>

struct FastaRecord {
	seqan::CharString id;
	seqan::Dna5String seq;
	FastaRecord(FastaRecord&) = default;
	FastaRecord(FastaRecord&&) = default;
	FastaRecord(seqan::CharString id, seqan::Dna5String seq): id(id), seq(seq) {};
};

struct FastqRecord : FastaRecord {
    seqan::CharString qual;

	FastqRecord(FastqRecord&) = default;
	FastqRecord(FastqRecord&&) = default;
    FastqRecord(seqan::CharString id, seqan::Dna5QString seq, seqan::CharString qual): FastaRecord(id, seq), qual(qual) {};
};

#endif /* FASTARECORD_H_ */
