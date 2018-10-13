/*
 * fastaRecord.h
 *
 *  Created on: Oct 13, 2018
 *      Author: jassi
 */
#include <seqan/sequence.h>
#include <seqan/seq_io.h>

#ifndef FASTARECORD_H_
#define FASTARECORD_H_

struct FastaRecord {
private:
	seqan::CharString id;seqan::Dna5String seq;
public:
	FastaRecord(seqan::CharString, seqan::Dna5String);seqan::CharString GetID() {
		return id;
	}
	seqan::Dna5String GetSeq() {
		return seq;
	}
};

#endif /* FASTARECORD_H_ */
