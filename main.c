#include <stdio.h>
#include "Map.h"
#include "Read.h"
#include "Merge.h"
#include "Write.h"

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
        printf("Improper invocation. Usage:\n"
               "\tMapMerge <AncestorFile (filename)> <CurrentFile (filename)> <MergingFile (filename)> [<MergeMarker length (integer, optional)>]\n");
        return 1;
    }

    Map *ancestor, *current, *merge, *output;

    printf("\nReading ancestor map...\n");
    ancestor = ReadMap(argv[1]);
    if(!ancestor)
    {
        printf("Usage:\n"
               "\tMapMerge <AncestorFile (filename)> <CurrentFile (filename)> <MergingFile (filename)> [<MergeMarker length (integer, optional)>]\n");
        return 1;
    }

    printf("\nReading current map...\n");
    current = ReadMap(argv[2]);
    if(!current)
    {
        printf("Usage:\n"
               "\tMapMerge <AncestorFile (filename)> <CurrentFile (filename)> <MergingFile (filename)> [<MergeMarker length (integer, optional)>]\n");
        return 1;
    }

    printf("\nReading merging map...\n");
    merge = ReadMap(argv[3]);
    if(!merge)
    {
        printf("Usage:\n"
               "\tMapMerge <AncestorFile (filename)> <CurrentFile (filename)> <MergingFile (filename)> [<MergeMarker length (integer, optional)>]\n");
        return 1;
    }

    printf("\nMerging maps...\n");
    output = MergeMaps(ancestor, current, merge);
    if(!output)
    {
        printf("Error during merge; aborting.\n");
        return 1;
    }

    return !WriteMap(output, argv[2]);
}
