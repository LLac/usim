// VariablesPage.h: interface for the CVariablesPage class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_VARIABLESPAGE_H__DD8D8F82_4D09_4DD4_85C5_4BFF696E304F__INCLUDED_)
#define AFX_VARIABLESPAGE_H__DD8D8F82_4D09_4DD4_85C5_4BFF696E304F__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CVariablesPage
{
public:
	CVariablesPage();
	virtual ~CVariablesPage();
	void Serialize(CArchive& ar, UINT uiFileVersion);
	void CopyData(CVariablesPage *pArray);
	void PasteData(CVariablesPage *pArray);
	void Clear();
	BOOL IsSet();

public:
	// DIGITAL PAGE
	CString		m_Notes;
	UINT		m_OPVarType, m_ORVarType;
	CString		m_OPVarTokenName, m_ORVarTokenName;
	UINT		m_OPVarTokenIndex, m_ORVarTokenIndex, m_OPVarArrayTokenIndex, m_ORVarArrayTokenIndex;
	UINT		m_OPSimType, m_ORSimType;
	BOOL		m_OPRead, m_ORRead;
	BOOL		m_OPWrite, m_ORWrite;

	DWORD		m_OPMask, m_ORMask;
	UCHAR		m_OPOperator, m_OROperator;
	UCHAR		m_OPRadio, m_ORRadio;
	double		m_OPValue, m_ORValue;
	UINT		m_OPInc, m_OPDec, m_ORInc, m_ORDec;

	
protected:

};

#endif // !defined(AFX_VARIABLESPAGE_H__DD8D8F82_4D09_4DD4_85C5_4BFF696E304F__INCLUDED_)
