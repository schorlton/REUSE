/*
 * fasta_record.cpp
 *
 *  Created on: Oct 13, 2018
 *      Author: jassi
 */
#include <iostream>
#include <seqan/sequence.h>
#include <seqan/seq_io.h>
#include "FastaRecord.h"

FastaRecord::FastaRecord(seqan::CharString id, seqan::Dna5String seq){
	this->id = id;
	this->seq = seq;
}
