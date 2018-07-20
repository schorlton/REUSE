# REUSE
Rapid Elimination of Useless Sequences

## Quick start
reuse-build hg38.fa hg38

reuse -x hg38 -U input.fq -o filtered.fq

## Building an index
-Identify all k-mers within a reference dataset and store that library to disk.

-Optionally filter k-mers from the reference that match to a target dataset so as to reduce false positive filtering. For example, filter all viral k-mers from the human genome so as not to filter reads originating from viruses in the sample.

### Usage:
reuse-build [options] <reference_in> <index>

### Main arguments
<reference_in> = A comma-separated list of FASTA files containing the reference sequences to be aligned to, or, if -c is specified, the sequences themselves. E.g., <reference_in> might be chr1.fa,chr2.fa,chrX.fa,chrY.fa, or, if -c is specified, this might be GGTCATCCT,ACGGGTCGT,CCGTTCTATGCGGCTTA.

<index> = Location to save index k-mer dataset to disk


### Options:
-p/--threads threads (default: 1)

-m maximum RAM usage (default: all available RAM)

-k = k-mer length (default: 21)

-c = The reference sequences are given on the command line. I.e. <reference_in> is a comma-separated list of sequences rather than a list of FASTA files.

-h = Hide (ie mask) k-mers found in this fasta file from the reference database. This option is used to minimize false positive filtering of related species or species of interest.

-g = Compress index when saving to disk. May take longer to generate the index and load when searching.

-h/--help = Print usage information and quit

-v/--version = Print version information and quit



## Searching the index
-Eliminate all reads or read pairs when 1 or more k-mers is found within the read

### Usage:
reuse [options] -x <index> {-1 <m1> -2 <m2> | -U <r> | --interleaved \<i\>\}

### Main arguments
-x <index>
The basename of the index for the reference dataset.

-1 <m1>
Comma-separated list of files containing mate 1s (filename usually includes _1), e.g. -1 flyA_1.fq,flyB_1.fq. Sequences specified with this option must correspond file-for-file and read-for-read with those specified in <m2>. Reads may be a mix of different lengths. If - is specified, reuse will read the mate 1s from the “standard in” or “stdin” filehandle. Reads may be in FASTQ or FASTA format.

-2 <m2>
Comma-separated list of files containing mate 2s (filename usually includes _2), e.g. -2 flyA_2.fq,flyB_2.fq. Sequences specified with this option must correspond file-for-file and read-for-read with those specified in <m1>. Reads may be a mix of different lengths. If - is specified, resuse will read the mate 2s from the “standard in” or “stdin” filehandle. Reads may be in FASTQ or FASTA format.

-U <r>
Comma-separated list of files containing unpaired reads to be aligned, e.g. lane1.fq,lane2.fq,lane3.fq,lane4.fq. Reads may be a mix of different lengths. If - is specified, reuse gets the reads from the “standard in” or “stdin” filehandle. Reads may be in FASTQ or FASTA format.


### Options:
-o <output> = Save reads not matching the k-mer filter to <output>.fast(q/a) for single-end reads, or <output>_1.fast(q/a) and <output>_2.fast(q/a) for paired-end reads. By default, reads are output to STDOUT.
  
-f <filtered> = Save reads matching the k-mer filter to <filtered>.fast(q/a) for single-end reads, or <filtered>_1.fast(q/a) and <filtered>_2.fast(q/a) for paired-end reads. By default, reads are discarded.
  
-g = Compress outputted reads with gzip

-z <command> = Compress outputted reads with alternate command, such as "bzip2"

-m = Maximum RAM usage (default: all available RAM)

-p/--threads = Threads to use (default: available number of threads)

-l <log> = Log file
  
-mk = Minimum number of k-mers per read to filter it (default: 1)

-sp = Split pairs


## Performance optimization:
REUSE will run fastest with filtering after the first k-mer is found (-mk 1), maximum thread and RAM usage, and a lower k-mer size. Lower k-mer sizes reduce the index size but are less accurate at differentiating species.
