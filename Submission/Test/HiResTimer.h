#ifndef HIRESTIMER_H
#define HIRESTIMER_H
///////////////////////////////////////////////////////////////////////
///  HiResTimer.h  - High Resolution Timer                          ///
///  ver 1.1           Uses Win32 Performance Counters              ///
///                                                                 ///
///  Language:     Visual C++ 2008                                  ///
///  Platform:     Dell Dimension 9200, Windows Vista Ultimate      ///
///  Application:  CSE687 Example                                   ///
///  Author:       Jim Fawcett, CST 2-187, Syracuse Univ.           ///
///                (315) 443-3948, jfawcett@twcny.rr.com            ///
///////////////////////////////////////////////////////////////////////
/* 
     Module Operations:
     ==================
     This module provides a high resolution timer that can resolve time
     differences to the microsecond level, about 4 orders of magnitude
     more precise than the windows default timers.  You use the timer
     in your code by declaring an instance, starting it, stopping it, 
     and extracting the elapsed time in microseconds.

     Public Interface:
     =================
     HRTimer::HiResTimer hrt;
     hrt.Start();
     // code you want to time here
     hrt.Stop();
     cout << "\n\tTime is: " << hrt.ElapsedMicroseconds() << "\n\n";

     This code is based on a C# application I wrote in 2003, which, 
     was based on:
       Windows Developer Magazine Column: Tech Tips, August 2002
       Author: Shawn Van Ness, shawnv@arithex.com
 */
/*
     Required Files:
     ===============
       HiResTimer.h, HiResTimer.cpp, myTimedSource.cpp

     Build Command:
     ==============
       cl /EHsc HiResTimer.cpp myTimedSource.cpp

     Maintenance History:
     ====================
       ver 1.1 : 12 Jan 08
       - added these comments
       ver 1.0 : 20 Mar 06
       - first release
 */

#include <windows.h>

namespace HRTimer
{
  class HiResTimer
  {
  protected:
    __int64 a, b, f;
     
  public:
    HiResTimer();
    __int64 ElapsedTicks();
    __int64 ElapsedMicroseconds();
    __int64 Frequency();
    void Start();
    __int64 Stop();
  };
}
#undef _UNICODE
#endif
