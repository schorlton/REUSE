# REUSE
Rapid Elimination of Useless SEquences

REUSE is a k-mer based tool for filtration of reads in sequencing datasets that do not match a reference sequence.
`reuse build` takes a reference fasta file as input, and ouputs a hashed index file. `reuse filter` takes FASTA/FASTQ file inputs, along with the hashed index file, and outputs k-mer filtered reads in a user-specified format. Common applications of REUSE include filtration of host, contamination, PhiX or ribosomal sequences.


## Quick start
`reuse build hg38.fa hg38`

`reuse filter -x hg38 -U input.fq -o filtered.fq`


## Software Requirements
REUSE will run on most flavo(u)rs of Linux. Prerequisites include:
- zlib1g-dev
- libpthread-stubs0-dev
- libbz2-1.0
- libseqan2-dev
- c++ (≥14)
- cmake (≥3.5)

## Installation
Download the pre-compiled binary from https://github.com/chorltsd/REUSE/releases/latest
Alternatively, this repository can be cloned and compiled using cmake:

`git clone https://github.com/chorltsd/REUSE.git`

`cd REUSE`

`cmake .`

### Usage:
`reuse build [options] -o \<output_file\>`


### Options:
-i \<input_file\> = reference in.  A comma-separated list of FASTA files containing the reference sequences to index.  (default: read from STDIN)

-o \<output_file\>= File to save index k-mer dataset to disk

-p/--threads = Number of threads to use (default: 1)

-r = Maximum RAM usage in MB (default: 400)

-k = k-mer length (default: 21)

-m = Mask k-mers found in this fasta file from the reference database. This option is used to minimize false positive filtering of related species or species of interest.

-g = Compress index when saving to disk. May take longer to generate the index and load when searching.

-h/--help = Print usage information and quit

-v/--version = Print version information and quit



## Searching the index
-Eliminate all reads or read pairs when 1 or more reference k-mers are found within the read. Optionally, retain only those reads with matching k-mers.

### Usage:
`reuse filter [options] -x \<index\> -1 \<m1\> -2 \<m2\>`

 ### Main arguments
-x \<index\>
The index file for the reference dataset, generated with `reuse build`

 -1 \<m1\>
Comma-separated list of files containing either unpaired reads or mate 1s (filename usually includes _1), e.g. -1 flyA_1.fq,flyB_1.fq. If -2 \<m2\> is specified, it is assumed that -1 \<m1\> represents mate 1s. Sequences specified with this option must correspond file-for-file and read-for-read with those specified in <m2>. Reads may be a mix of different lengths. If - is specified, REUSE will read the mate 1s from the “standard in” or “stdin” filehandle. Reads may be in FASTQ or FASTA format, and may be gzipped or bzip2ed. REUSE detects these formats by defauly.
 
 -2 \<m2\>
Comma-separated list of files containing mate 2s (filename usually includes _2), e.g. -2 flyA_2.fq,flyB_2.fq. Sequences specified with this option must correspond file-for-file and read-for-read with those specified in <m1>. Reads may be a mix of different lengths. If - is specified, resuse will read the mate 2s from the “standard in” or “stdin” filehandle. Reads may be in FASTQ or FASTA format, and may be gzipped or bzip2ed. REUSE detects these formats by defauly.

### Options:
-o \<output\> = Save reads not matching the k-mer filter to <output>.fast(q/a) for single-end reads, or <output>_1.fast(q/a) and <output>_2.fast(q/a) for paired-end reads.
  
-f \<filtered\> = Save reads matching the k-mer filter to \<filtered\>.fast(q/a) for single-end reads, or \<filtered\>_1.fast(q/a) and \<filtered\>_2.fast(q/a) for paired-end reads. By default, reads are discarded.
  
-g = Compress outputted reads with gzip

-z <command> = Compress outputted reads with alternate command, such as "bzip2"

-r = Maximum RAM usage in MB (default: 400)

-p/--threads = Number of threads to use (default: available number of threads)

-l <log> = Log file
  
-k = Minimum number of k-mers per read to filter it (default: 1)

-s = Split pairs

### `reuse filter` Output:
By default, reads are output to STDOUT in the same format as they are input (eg. FASTQ input=FASTQ output). Paired end reads are interleaved before output to STDOUT. Please see the option `-o` for further details.

## Performance optimization:
REUSE will run fastest with filtration after the first k-mer is found (-mk 1), maximum thread and RAM usage, and a lower k-mer size. Lower k-mer sizes reduce the index size but are less specific at differentiating species.
