//
// FILE:        TestCase.h
// LICENSE:     The MIT license
// PURPOUSE:    Test if the output is the expected output.
// AUTHORS:     Alberto Alonso <rydencillo@gmail.com>
// NOTES:       I don't have a preference for any colour, but I love yellow chicken meat.
//

#pragma once

namespace Test
{
    class TestCase
    {
    public:
        const char*     getError    () const { return mErrmsg; }
        
    protected:
        bool Assert ( const bool condition, const char* errmsg )
        {
            if ( !condition )
                mErrmsg = errmsg;
            return condition;
        }

    private:
        const char* mErrmsg;
    };
}
