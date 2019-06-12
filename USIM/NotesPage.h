// NotesPage.h: interface for the CNotesPage class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_NOTESPAGE_H__DD8D8F82_4D09_4DD4_85C5_4BFF696E304F__INCLUDED_)
#define AFX_NOTESPAGE_H__DD8D8F82_4D09_4DD4_85C5_4BFF696E304F__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CNotesPage
{
public:
	CNotesPage();
	virtual ~CNotesPage();
	void Serialize(CArchive& ar);
	void CopyData(CNotesPage *pArray);
	void PasteData(CNotesPage *pArray);
	void Clear();
	BOOL IsSet();

public:
	// DIGITAL PAGE
	CString		m_Desc, m_Notes;

protected:

};

#endif // !defined(AFX_NOTESPAGE_H__DD8D8F82_4D09_4DD4_85C5_4BFF696E304F__INCLUDED_)
