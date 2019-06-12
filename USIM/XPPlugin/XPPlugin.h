#if !defined(AFX_XPPLUGIN_H__1F55D07C_3AE7_49F0_8C80_040629BBCFF7__INCLUDED_)
#define AFX_XPPLUGIN_H__1F55D07C_3AE7_49F0_8C80_040629BBCFF7__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// XPPlugin.h : header file
//

#define ArraySize(X) (sizeof X / sizeof *X)

#include "..\XPdatarefs.h"
#include "..\XPFlightData.h"

#define	iInterval			0.010

XPFlightData *m_pXPlaneData;
//HANDLE m_hMutex;

float	MyFlightLoopCallback(
                                   float                inElapsedSinceLastCall,    
                                   float                inElapsedTimeSinceLastFlightLoop,    
                                   int                  inCounter,    
                                   void *               inRefcon);
void	StartSharedMemServer();
void	StopSharedMemServer();
void	UpdateSharedMem();

void*	m_ServerDataMap;

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_XPPLUGIN_H__1F55D07C_3AE7_49F0_8C80_040629BBCFF7__INCLUDED_)
