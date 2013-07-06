//
// FILE:        TestOutput.h
// LICENSE:     The MIT license
// PURPOUSE:    Test if the output is the expected output.
// AUTHORS:     Alberto Alonso <rydencillo@gmail.com>
// NOTES:       I don't have a preference for any colour, but I love yellow chicken meat.
//

#define _CRT_SECURE_NO_WARNINGS

#include <cstdio>
#include <cstring>
#include <sys/stat.h>
#include "TestOutput.h"
#include "../Utils.h"

using namespace Test;

bool TestOutput::compareOutput ( const char* outputPath, const char* expectedPath )
{
    struct stat outputStat;
    struct stat expectedStat;

    auto status = stat(outputPath, &outputStat);
    if ( !Assert(status != -1, "Cannot stat the output file") )
        return false;

    status = stat(expectedPath, &expectedStat);
    if ( !Assert(status != -1, "Cannot stat the expected file") )
        return false;

    if ( !Assert(outputStat.st_size == expectedStat.st_size, "File sizes mismatch") )
        return false;

    FILE* output = fopen ( outputPath, "r" );
    if ( !Assert(output != nullptr, "Cannot open the output file") )
        return false;

    FILE* expected = fopen ( expectedPath, "r" );
    if ( !Assert(expected != nullptr, "Cannot open the expected file") )
        return false;

    char buffer1 [ 64 ];
    char buffer2 [ 64 ];
    size_t len1, len2;
    while ( !feof(output) )
    {
        len1 = fread(&buffer1[0], sizeof(char), NUMELEMS(buffer1), output);
        len2 = fread(&buffer2[0], sizeof(char), NUMELEMS(buffer2), expected);

        if ( !Assert(len1 == len2, "Didn't read the same amount of bytes from both files") )
            return false;

        if ( !Assert(memcmp(&buffer1[0], &buffer2[0], len1) == 0, "Files mismatch") )
            return false;
    }

    fclose ( output );
    fclose ( expected );

    return true;
}
