#include <stdio.h>

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
               "MapMerge <AncestorFile (filename)> <CurrentFile (filename)> <MergingFile (filename)> [<MergeMarker length (integer, optional)>]\n");
        return 1;
    }

    return 0;
}
