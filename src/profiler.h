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

#endif
