#pragma once

#include <cstring>

class PixBuffer
{
public:
                    PixBuffer   ();
                    PixBuffer   ( unsigned int width, unsigned int height, unsigned int depth = 24 );
                    ~PixBuffer  ();

	void            create      ( unsigned int width, unsigned int height, unsigned int depth = 24 );
    void            clear       ( unsigned int clearColor );

    unsigned int    getWidth    () const { return mWidth; }
    unsigned int    getHeight   () const { return mHeight; }
    unsigned int    getDepth    () const { return mDepth; }

    unsigned char*  getBuffer   () const { return mBuffer; }

    unsigned int    getPixel    ( unsigned int x, unsigned int y )
    {
        return *reinterpret_cast<unsigned int *>(&mBuffer[(y * mWidth + x) * mDepth]);
    }

    void            setPixel    ( unsigned int x, unsigned int y, unsigned int value )
    {
        memcpy ( &mBuffer[(y * mWidth + x) * mDepth], reinterpret_cast<unsigned char *>(&value), mDepth / 8 );
    }

private:
    unsigned int    mWidth;
    unsigned int    mHeight;
    unsigned int    mDepth;
    unsigned char*  mBuffer;
};
