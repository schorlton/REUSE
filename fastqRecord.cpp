/*
 * fastqRecord.cpp
 *
 *  Created on: Oct 13, 2018
 *      Author: jassi
 */
#include"FastaRecord.h"
#include"FastqRecord.h"
#include<seqan/seq_io.h>
#include<seqan/sequence.h>

void FastqRecord::FastqRecord(seqan::CharString id, seqan::Dna5QString seq, seqan::CharString qual){
	this->id = id;
	this->seq = seq;
	this->qual = qual;
}

