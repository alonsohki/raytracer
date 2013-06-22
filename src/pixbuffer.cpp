#include "pixbuffer.h"

PixBuffer::PixBuffer ( unsigned int width, unsigned int height, unsigned int depth )
: mBuffer(nullptr)
{
    create(width, height, depth);
}

PixBuffer::PixBuffer ()
: mBuffer(nullptr)
{
}

PixBuffer::~PixBuffer ()
{
    delete [] mBuffer;
}

void PixBuffer::create ( unsigned int width, unsigned int height, unsigned int depth )
{
    if ( mBuffer != nullptr )
        delete [] mBuffer;
    mBuffer = nullptr;

    if ( (depth % 8) != 0 || depth > 32 )
        throw "Invalid PixBuffer depth";

    mWidth = width;
    mHeight = height;
    mDepth = depth;
	mBuffer = new unsigned char [ mWidth * mHeight * mDepth ];
}

void PixBuffer::clear ( unsigned int clearColor )
{
    if ( mDepth == 24 )
    {
        union
        {
            unsigned int ui [ 3 ];
            unsigned char uc [ 12 ];
        } value;
        memset ( value.ui, 0, sizeof(value.ui) );
        for ( int i = 0; i < 4; ++i )
        {
            memcpy ( &value.uc[i * 3], &clearColor, 3 );
        }

        unsigned int numIterations = mWidth * mHeight / 4;
        for ( unsigned int i = 0; i < numIterations; ++i )
            memcpy ( &mBuffer[i * 12], &value.uc[0], 12 );
    }
    else
    {
        union
        {
            unsigned short us [ 2 ];
            unsigned char uc [ 4 ];
            unsigned int ui;
        } value;

        if ( mDepth == 8 )
        {
            for ( unsigned int i = 0; i < 4; ++i )
                value.uc[i] = clearColor;
        }
        else if ( mDepth == 16 )
        {
            for ( unsigned int i = 0; i < 2; ++i )
                value.us[i] = clearColor;
        }
        else if ( mDepth == 32 )
        {
            value.ui = clearColor;
        }
        
        memset ( &mBuffer[0], value.ui, mWidth * mHeight * mDepth / 4 );
    }
}
