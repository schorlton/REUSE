# REUSE
Rapid Elimination of Useless Sequences

RESUSE is a k-mer based filtering tool for extracting foreign sequences from NGS datasets that do not match a reference sequence.
REUSE takes FASTA/FASTAQ file inputs with reference sequences, and outputs k-mer filtered mismatches.


## Quick start
`reuse build hg38.fa hg38`

`reuse filter -x hg38 -U input.fq -o filtered.fq`


## Software Requirements
REUSE will run on most flavo(u)rs of Linux. Prerequisites include:
- pigz

<!!!!> What compiler/interpreter is needed (e.g. GCC v4.7, Python v3.6)<marked>
<!!!!> What libraries you depend on (e.g. Boost v1.58, NumPy v1.15)


## Installation
Download the binary from https://github.com/chorltsd/REUSE/releases/latest
Alternatively, this repository can be cloned:
`git clone https://github.com/chorltsd/REUSE.git REUSE`

<!!!!> further instructions/example install:
### Usage:
reuse build [options] -o \<output_path\>

-o = Location to save index k-mer dataset to disk


### Options:
-i = reference_in.  A comma-separated list of FASTA files containing the reference sequences to be aligned to.  (default: read from STDIN )

-o = Location to save index k-mer dataset to disk

-p/--threads =Number of threads used (default: 8)

-r = Maximum RAM usage in MB (default: 400)

-k = k-mer length (default: 21)

-m = Mask k-mers found in this fasta file from the reference database. This option is used to minimize false positive filtering of related species or species of interest.

-g = Compress index when saving to disk. May take longer to generate the index and load when searching.

-h/--help = Print usage information and quit

-v/--version = Print version information and quit



## Searching the index
-Eliminate all reads or read pairs when 1 or more k-mers is found within the read

### Usage:
reuse filter [options] -x \<index\> -1 \<m1\> -2 \<m2\> 
 ### Main arguments
-x \<index\>
The basename of the index for the reference dataset. This can either be generated with reuse-build (.db.gz) or with an alternative program for k-mer counting, such as Jellyfish, KAnalyze or others. K-mer count files in Jellyfish dump format/KAnalyze default output format (two column text file for both) may be compressed with gzip.

 -1 \<m1\>
Comma-separated list of files containing mate 1s (filename usually includes _1), e.g. -1 flyA_1.fq,flyB_1.fq. Sequences specified with this option must correspond file-for-file and read-for-read with those specified in <m2>. Reads may be a mix of different lengths. If - is specified, reuse will read the mate 1s from the “standard in” or “stdin” filehandle. Reads may be in FASTQ or FASTA format.
 
 -2 \<m2\>
Comma-separated list of files containing mate 2s (filename usually includes _2), e.g. -2 flyA_2.fq,flyB_2.fq. Sequences specified with this option must correspond file-for-file and read-for-read with those specified in <m1>. Reads may be a mix of different lengths. If - is specified, resuse will read the mate 2s from the “standard in” or “stdin” filehandle. Reads may be in FASTQ or FASTA format.

### Options:
-o <output> = Save reads not matching the k-mer filter to <output>.fast(q/a) for single-end reads, or <output>_1.fast(q/a) and <output>_2.fast(q/a) for paired-end reads. By default, reads are output to STDOUT.
  
-f <filtered> = Save reads matching the k-mer filter to <filtered>.fast(q/a) for single-end reads, or <filtered>_1.fast(q/a) and <filtered>_2.fast(q/a) for paired-end reads. By default, reads are discarded.
  
-g = Compress outputted reads with gzip

-z <command> = Compress outputted reads with alternate command, such as "bzip2"

-r = Maximum RAM usage (default: all available RAM)

-p/--threads = Threads to use (default: available number of threads)

-l <log> = Log file
  
-k = Minimum number of k-mers per read to filter it (default: 1)

-s = Split pairs


## Performance optimization:
REUSE will run fastest with filtering after the first k-mer is found (-mk 1), maximum thread and RAM usage, and a lower k-mer size. Lower k-mer sizes reduce the index size but are less accurate at differentiating species.
