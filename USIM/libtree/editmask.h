/*********************************************************************
Copyright (C) 2004 Cornelius Eichhorst
///////////////////////////////////
// tree displaying xml nodes
**********************************************************************/

#if !defined EDITMASK_H
#define EDITMASK_H


// disable stl linker warnings //
#pragma warning ( disable : 4786)


#define FLOAT_MASK					        "^(-?)([0-9]*)[.,]?([0-9]*)$"
#define INT_MASK                    "^(-?)([0-9]*)([0-9]*)$"
#define FLOAT_MASK_POSITIVE					"^([0-9]*)[.,]?([0-9]*)$"
#define INT_MASK_POSITIVE           "^([0-9]*)([0-9]*)$"


typedef int (*EDITINPUTCALLBACK)(CString&) ;

/////////////////////////////////////////////////////////////////////////////
// Fenster CEditMask 

class CEditMask : public CEdit
{
// Konstruktion
public:
	CEditMask();
// Attribute
public:

// Operationen
public:

// Überschreibungen
	// Vom Klassen-Assistenten generierte virtuelle Funktionsüberschreibungen
	//{{AFX_VIRTUAL(CEditMask)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	//}}AFX_VIRTUAL

// Implementierung
public:
	bool SetMaxLenght(int iMaxLength);
	int ValidateInput(char *pInputText);
	CString m_stInputMask;
	int SetInputMask(const CString &stInputMask);
	virtual ~CEditMask();
	
	int SetCallback(EDITINPUTCALLBACK EditInputMask) ;
	int CallCallback(CString& stText) ;
	
	EDITINPUTCALLBACK pEditInputCallback ;

	// Generierte Nachrichtenzuordnungsfunktionen
protected:
	//{{AFX_MSG(CEditMask)
		// HINWEIS - Der Klassen-Assistent fügt hier Member-Funktionen ein und entfernt diese.
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()

  int m_iMaxLength ;
};

#endif
