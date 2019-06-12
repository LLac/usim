// Document modified at : Tuesday, May 28, 2002 03:52:33 PM , by user : Pavel Sokolov , from computer : CREA7OR
#pragma once

#include <afxtempl.h>

class CEAdvButton : public CButton
{
// Construction
public:
	CEAdvButton();

// Attributes
public:

// Operations
public:

	void AddControlToList( int l_intID );
	void ClearList();
	void SetCheck( int nCheck );
	void SetEnable( int l_intCheck );
// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CEAdvButton)
	protected:
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CEAdvButton();

	// Generated message map functions
protected:

	void EnableList( BOOL l_boolEnable );

	//{{AFX_MSG(CEAdvButton)
	afx_msg void OnClicked();
	//}}AFX_MSG

	CArray < int, int > m_caIDs;

	DECLARE_MESSAGE_MAP()
};
