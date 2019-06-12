// TREEThread.h: interface for the CTREEThread class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_TREETHREAD_H__16D0632B_2475_48D6_B4EF_95AF40E23213__INCLUDED_)
#define AFX_TREETHREAD_H__16D0632B_2475_48D6_B4EF_95AF40E23213__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

/////////////////////////////////////////////////////////////////////////////

class CTreeThread
{
public:
// Construction & Destruction
	CTreeThread();
	virtual ~CTreeThread();
	void TreeWorker();

// Attributes, Operations
protected:
	int		m_delta_T;
	HTREEITEM	m_hItemPrev;

private:
	void SetOutputItemIcon(LPVOID, int, int);
	void SetOutputTabIcon(int, int);
	void SetMuxItemIcon(LPVOID, int, int);
	void SetMuxTabIcon(int, int);
	void SetDisplayItemIcon(LPVOID, int, int);
	void SetDispTabIcon(int, int);
	void SetCrtRwrItemIcon(LPVOID, int, int);
	void SetRWRTabIcon(int, int);
	void SetCLCDItemIcon(LPVOID, int, int);
	void SetCLCDTabIcon(int, int);
	void SetGLCDItemIcon(LPVOID, int, int);
	void SetGLCDTabIcon(int, int);
	void SetInputItemIcon(LPVOID, int, int);
	void SetInputTabIcon(int, int);
	void SetLatchedItemIcon(LPVOID, int, int);
	void SetLatchedTabIcon(int, int);
	void SetDotMatrixItemIcon(LPVOID, int, int);
	void SetDotMatrixTabIcon(int, int);
	void SetSPIItemIcon(LPVOID, int, int);
	void SetSPITabIcon(int, int);
	void SetSTEPItemIcon(LPVOID, int, int);
	void SetSTEPTabIcon(int, int);
};

#endif // !defined(AFX_TREETHREAD_H__16D0632B_2475_48D6_B4EF_95AF40E23213__INCLUDED_)
