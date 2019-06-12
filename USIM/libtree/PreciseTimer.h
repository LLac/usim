/*************************************************************************
  Dieses Modul ist Bestandteil der SONARIS-Funktionsbibliotheken.        
                                                                        
  (C) 1991-2003  ORIMOS Financial Analytics GmbH                         
                 Cotheniusstrasse 3, 10407 Berlin    
                                                                        
  Alle Rechte vorbehalten. Die Verwendung der Software ohne schriftliche 
  Genehmigung von ORIMOS Financial Analytics GmbH ist nicht gestattet.   

  This module is part of the SONARIS Financial Function Libraries.       
                                                                        
  (C) 1991-2003  ORIMOS Financial Analytics GmbH                         
                 Cotheniusstrasse 3, 10407 Berlin    
                                                                         
  All rights reserved. The use of the software without express written   
  permission of ORIMOS Financial Analytics GmbH is strictly prohibited.  

**************************************************************************/

#ifndef _PRECISETIMER_H_
#define _PRECISETIMER_H_

#include <algorithm>
#include <string>
using namespace std;
//More precise Timer for measuring time intervals in microseconds.
//The performance of this Timer is dependent on the performance of the system.
class CPreciseTimer
{
public:
	//CONSTRUCTOR
	CPreciseTimer(const bool& bstart=true);

	bool SupportsHighResCounter();
	void StartTimer();
	CPreciseTimer& StopTimer();
	__int64 GetTime();

  CString GetDurationString(void) const;
#ifdef _DEBUG
  void DumpDuration(const CString& stitel) const;
#endif
private:
	//Auxiliary Function
	void UpdateElapsed();

	//Member variables
	bool m_bRunning;	
	__int64 m_i64Start;
	__int64 m_i64Elapsed;

	//Some auxiliary variables
	__int64 m_i64Counts;
	LARGE_INTEGER m_liCount;

	//Static Variables
	static bool sm_bInit;
	static bool sm_bPerformanceCounter;
	static __int64 sm_i64Freq;
};

inline bool CPreciseTimer::SupportsHighResCounter()
{
	return sm_bPerformanceCounter;
}

//Auxiliary Function
inline void CPreciseTimer::UpdateElapsed()
{
	if(true == sm_bPerformanceCounter)
	{
		QueryPerformanceCounter(&m_liCount);
		m_i64Counts = ((__int64)m_liCount.HighPart << 32) + (__int64)m_liCount.LowPart;
		//Transform in microseconds
		(m_i64Counts *= 1000000) /= sm_i64Freq;
	}
	else
		//Transform milliseconds to microseconds
		m_i64Counts = (__int64)GetTickCount() * 1000;
	if(m_i64Counts > m_i64Start)
		m_i64Elapsed = m_i64Counts - m_i64Start;
	else
		//Eliminate possible number overflow (0x7fffffffffffffff is the maximal __int64 positive number)
		m_i64Elapsed = (0x7fffffffffffffff - m_i64Start) + m_i64Counts;
}

string  Int64ToString(__int64 const& ri64, __int64 const& iRadix=10);
CString Int64ToDurationString(__int64 const& ri64);


#endif // _PRECISETIMER_H_


