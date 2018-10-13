/*
 * fastqRecord.h
 *
 *  Created on: Oct 13, 2018
 *      Author: jassi
 */
#ifndef FASTQRECORD_H_
#define FASTQRECORD_H_

#include <seqan/sequence.h>
#include <seqan/seq_io.h>
#include "FastaRecord.h"

struct FastqRecord: FastaRecord {
private:
	seqan::CharString qual;
public:
	FastqRecord(seqan::CharString id, seqan::Dna5QString seq, seqan::CharString qual);

	seqan::CharString GetQual() {
		return qual;
	}
};

#endif /* FASTQRECORD_H_ */
