//
// FILE:        Profiler.h
// LICENSE:     The MIT license
// PURPOUSE:    Profiler to time execution of routines.
// AUTHORS:     Alberto Alonso <rydencillo@gmail.com>
//

#pragma once

#ifdef WIN32

#include <windows.h>

class Profiler
{
public:
	Profiler ()
	: mRunning(false)
	{
		LARGE_INTEGER li;
		if ( !QueryPerformanceFrequency(&li) )
			throw "QueryPerformanceFrequency failed";

		mFreq = double(li.QuadPart) / 1000.0;
	}

	void start ()
	{
		LARGE_INTEGER li;
		mRunning = true;

		QueryPerformanceCounter(&li);
		mStart = li.QuadPart;
	}

	double stop ()
	{
		LARGE_INTEGER li;
		QueryPerformanceCounter(&li);
		mEnd = li.QuadPart;

		mRunning = false;
		return query ();
	}

	double query () const
	{
		if ( mRunning )
		{
			LARGE_INTEGER li;
			QueryPerformanceCounter(&li);
			return double(li.QuadPart - mStart)/mFreq; 
		}
		else
		{
			return (mEnd - mStart)/mFreq;
		}
	}

	bool isRunning () const { return mRunning; }

private:
	bool			mRunning;
	double			mFreq;
	__int64			mStart;
	__int64			mEnd;
};

#else

#include <sys/time.h>

class Profiler {
public:
    Profiler ()
    : mRunning(false)
    {
    }

    void start ()
    {
        mRunning = true;
        gettimeofday(&mStart, 0);
    }

    double stop()
    {
        gettimeofday(&mEnd, 0);
        mRunning = false;
        return query();
    }

    double query () const
    {
        if (mRunning)
        {
            timeval now;
            gettimeofday(&now, 0);
            return timediff(now, mStart);
        }
        else
        {
            return timediff(mEnd, mStart);
        }
    }

    bool isRunning () const { return mRunning; }

private:
    static double timediff (timeval end, timeval start)
    {
        double result = (end.tv_sec - start.tv_sec) * 1000.0;

        if (end.tv_usec > start.tv_usec)
        {
            result += (end.tv_usec - start.tv_usec) / 1000.0;
        }
        else
        {
            result -= 1000.0;
            result += (start.tv_usec - end.tv_usec) / 1000.0;
        }

        return result;
    }


private:
    bool            mRunning;
    timeval         mStart;
    timeval         mEnd;
};

#endif
