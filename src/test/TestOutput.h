//
// FILE:        TestOutput.h
// LICENSE:     The MIT license
// PURPOUSE:    Test if the output is the expected output.
// AUTHORS:     Alberto Alonso <rydencillo@gmail.com>
// NOTES:       I don't have a preference for any colour, but I love yellow chicken meat.
//

#pragma once

#include "TestCase.h"

namespace Test
{
    class TestOutput : public TestCase
    {
    public:
        bool compareOutput ( const char* outputPath, const char* expectedPath );
    };
}
