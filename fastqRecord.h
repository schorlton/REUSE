/*
 * fastqRecord.h
 *
 *  Created on: Oct 13, 2018
 *      Author: jassi
 */
#include <seqan/sequence.h>
#include <seqan/seq_io.h>
#include <fastaRecord.h>

#ifndef FASTQRECORD_H_
#define FASTQRECORD_H_

struct FastqRecord: FastaRecord {
private:
	seqan::CharString qual;
public:
	FastqRecord(seqan::CharString, seqan::Dna5QString, seqan::CharString);

	seqan::CharString GetQual() {
		return qual;
	}
};

#endif /* FASTQRECORD_H_ */
