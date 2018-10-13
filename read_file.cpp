#include <seqan/seq_io.h>

using namespace seqan;

int main()
{
    CharString seqFileName = ("data/chrY.fa");

    SeqFileIn seqFileIn;
    if (!open(seqFileIn, toCString(seqFileName)))
    {
        std::cerr << "ERROR: Could not open the file.\n";
        return 1;
    }

    StringSet<CharString> ids;
    StringSet<Dna5String> seqs;

    try
    {
        readRecords(ids, seqs, seqFileIn);
    }
    catch (Exception const & e)
    {
        std::cout << "ERROR: " << e.what() << std::endl;
        return 1;
    }

    for (unsigned i = 0; i < length(ids); ++i)
        std::cout << ids[i] << '\t' << seqs[i] << '\n';

    return 0;
}
