#include <stdio.h>
#include "Main.h"
#include "Map.h"
#include "Read.h"
#include "Merge.h"
#include "Write.h"

void Usage()
{
    printf("Usage:\n"
           "\tMapMerge <AncestorFile (filename)> <CurrentFile (filename)> <MergingFile (filename)> [<MergeMarker length (integer, optional)>]\n");
}

int main(int argc, char *argv[])
{
    int mergeMarker = 7;
    if(argc == 5)
    {
        mergeMarker = atoi(argv[4]);
        argc--;
    }

    if(argc != 4 || !mergeMarker)
    {
        printf("Improper invocation.\n");
        Usage();
        return 1;
    }

    Map *ancestor, *current, *merge, *output;

    printf("Reading ancestor map...\n");
    ancestor = ReadMap(argv[1]);
    if(!ancestor)
      return 1;

    printf("Reading current map...\n");
    current = ReadMap(argv[2]);
    if(!current)
      return 1;

    printf("Reading merging map...\n");
    merge = ReadMap(argv[3]);
    if(!merge)
      return 1;

    printf("Merging maps...\n");
    output = MergeMaps(ancestor, current, merge);
    if(!output)
    {
        printf("Error during merge; aborting.\n");
        return 1;
    }

    printf("Writing merged map...\n");
    if(WriteMap(output, argv[2]))
      exit(0);
    else
      return 1;
}
