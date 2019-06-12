// File    : HexEdit.cpp 
// Version : 1.0
// Date    : 21. Jan 2005
// Author  : Luo Pei'dn
// Email   : coollpe@hotmail.com
// Systems : VC6.0/7.0 and VC7.1 (Run under (Window 98/ME), Windows Nt 2000/XP/2003)
//
// For bugreport please email me
//
// You are free to use/modify this code but leave this header intact.
// You are free to use it in any of your applications. 
// HexEdit.cpp : implementation file
//

#include "stdafx.h"
#include "HexEdit.h"
#include ".\hexedit.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define MES_UNDO _T("&Undo")
#define MES_CUT _T("Cu&t")
#define MES_COPY _T("&Copy")
#define MES_PASTE _T("&Paste")
#define MES_DELETE _T("&Delete")
#define MES_SELECTALL _T("Select &All")
#define MES_CONVERT _T("Con&vert")
#define ME_SELECTALL WM_USER + 0x700
#define ME_CONVERT WM_USER + 0x701

/////////////////////////////////////////////////////////////////////////////
// CHexEdit

CHexEdit::CHexEdit():m_bHex(false),m_bEnd(false)
{
	this->m_icHexText.clrBkColor=RGB(219,247,220);
	this->m_icHexText.clrTextColor=RGB(74,115,132);
	this->m_icDecText.clrBkColor=RGB(251,244,170);
	this->m_icDecText.clrTextColor=RGB(192,97,10);
	this->m_bkHexBrush.CreateSolidBrush(this->m_icHexText.clrBkColor);
	this->m_bkDecBrush.CreateSolidBrush(this->m_icDecText.clrBkColor);
}

CHexEdit::~CHexEdit()
{
	m_bkHexBrush.DeleteObject();
	m_bkDecBrush.DeleteObject();
}


BEGIN_MESSAGE_MAP(CHexEdit, CEdit)
	//{{AFX_MSG_MAP(CHexEdit)
	ON_WM_CTLCOLOR_REFLECT()
	ON_WM_CHAR()
	ON_MESSAGE(ME_CONVERT, OnConvert)
	//}}AFX_MSG_MAP
ON_WM_CONTEXTMENU()
ON_CONTROL_REFLECT(EN_UPDATE, OnEnUpdate)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CHexEdit message handlers

HBRUSH CHexEdit::CtlColor(CDC* pDC, UINT nCtlColor) 
{
	nCtlColor;
	// TODO: Change any attributes of the DC here
	
	// TODO: Return a non-NULL brush if the parent's handler should not be called
	
	if(m_bHex)
	{
		pDC->SetTextColor(this->m_icHexText.clrTextColor);
		pDC->SetBkColor(this->m_icHexText.clrBkColor);
		return m_bkHexBrush;
	}
	else
	{
		pDC->SetTextColor(this->m_icDecText.clrTextColor);
		pDC->SetBkColor(this->m_icDecText.clrBkColor);
		return m_bkDecBrush;
	}
}

void CHexEdit::OnChar(UINT nChar, UINT nRepCnt, UINT nFlags) 
{
	// TODO: Add your message handler code here and/or call default
	
	if (m_bHex) {
		int start,end;
		GetSel(start,end);
		CString text;
		this->GetWindowText(text);
		char head=0,second=0;
		if(start > 9 && nChar != VK_BACK) return;
		if(text.GetLength()>0) head=text.GetAt(0);
		if(text.GetLength()>1) second=text.GetAt(1);

		if(start==0&&nChar=='0') {
			CEdit::OnChar(nChar, nRepCnt, nFlags);
		} else if(start==1&&(nChar=='X'||nChar=='x')&&head=='0') {
			nChar = 'x';
			CEdit::OnChar(nChar, nRepCnt, nFlags);
		} else if(start>1&&nChar>=48&&nChar<=57||nChar>='A'&&nChar<='F') {
			if(second=='x'||second=='X') {
				CEdit::OnChar(nChar, nRepCnt, nFlags);
			}
		} else if(start>1&&nChar>='a'&&nChar<='f') {
			if(second=='x'||second=='X') {
				CEdit::OnChar(nChar, nRepCnt, nFlags);
			}
		} else if(nChar == VK_BACK) {
			CEdit::OnChar(nChar, nRepCnt, nFlags);
		}
	} else {
		CEdit::OnChar(nChar, nRepCnt, nFlags);
	}
}

//void CHexEdit::OnChange() 
//{
//	// TODO: If this is a RICHEDIT control, the control will not
//	// send this notification unless you override the CEdit::OnInitDialog()
//	// function and call CRichEditCtrl().SetEventMask()
//	// with the ENM_CHANGE flag ORed into the mask.
//	
//	// TODO: Add your control notification handler code here
//	
//}

BOOL CHexEdit::PreTranslateMessage(MSG* pMsg) 
{
	// TODO: Add your specialized code here and/or call the base class
	int start,end;
	GetSel(start,end);
	CString text;
	this->GetWindowText(text);
	char head=0,second=0;
	if(text.GetLength()>0) head=text.GetAt(0);
	if(text.GetLength()>1) second=text.GetAt(1);
	bool bCut=true;

	if (pMsg->message == WM_KEYDOWN)
	{
		if (pMsg->wParam=='X')
		{
			if(::GetKeyState(VK_CONTROL) < 0)
			{
				//Cut
				if(second=='X'||second=='x'){
						//does not allow cut first char
					if(start==0&&end==1)
							bCut=false;
				}
				if(bCut) return CEdit::PreTranslateMessage(pMsg);
				else
					return TRUE;

			}
			else return CEdit::PreTranslateMessage(pMsg);
		}
		else if(pMsg->wParam=='V')
		{
			if(::GetKeyState(VK_CONTROL)<0)
			{
				//Paste
				bool bPaste=true;
				if(!IsClipboardFormatAvailable(CF_TEXT)) bPaste=false;
				if(!this->OpenClipboard())
					::AfxMessageBox("Cannot open clipboard!");
				HGLOBAL   hglb; 
				LPTSTR    lptstr; 
				hglb=GetClipboardData(CF_TEXT);
				if (hglb != NULL) 
				{ 
					lptstr =(LPTSTR) GlobalLock(hglb);
					//Check invalid hex string
					if(!this->IsHexConvertableText(lptstr))
					{
						bPaste=false;
					}
				}
				CloseClipboard();
				if(!bPaste) return TRUE;
				else
					return CEdit::PreTranslateMessage(pMsg);
			}
			else return CEdit::PreTranslateMessage(pMsg);
		}
		else if(pMsg->wParam==VK_DELETE)
		{
			if(second=='X'||second=='x'){
					//does not allow delete first char
				if(start==0&&end<=1)
						bCut=false;
			}
			if(bCut) return CEdit::PreTranslateMessage(pMsg);
			else
				return TRUE;
		}
		else
			return CEdit::PreTranslateMessage(pMsg);
	}
	else if (pMsg->message == WM_CHAR)
	{
		int c=(int)pMsg->wParam;
		if(pMsg->wParam<32)
		{
			if(pMsg->wParam==8&&(second=='x'||second=='X')&&head=='0'&&end==1) //does not allow to delete '0' before x
				return TRUE;
			else
				return CEdit::PreTranslateMessage(pMsg);//Control code
		}

		if(second=='x'||second=='X')
		{
			//does not allow to change head except select includes first and second
			if(start<=1&&end<=1) return TRUE;
		}
		if(start==1&&(c=='X'||c=='x')&&head=='0') {pMsg->wParam='x';return CEdit::PreTranslateMessage(pMsg);}
		else if(c>=48&&c<=57||c>='A'&&c<='F') return CEdit::PreTranslateMessage(pMsg);
		else if(c>='a'&&c<='f') {pMsg->wParam-=32; return CEdit::PreTranslateMessage(pMsg);}
		else return TRUE;
	}
	else
		return CEdit::PreTranslateMessage(pMsg);
}
void CHexEdit::DoDataExchange(CDataExchange* pDX)
{
	// TODO:
	::AfxMessageBox("dsaf");
	CEdit::DoDataExchange(pDX);
}

BOOL CHexEdit::OnCommand(WPARAM wParam, LPARAM lParam)
{
	switch (LOWORD(wParam))
	{
	case EM_UNDO:
	case WM_CUT:
	case WM_COPY:
	case WM_CLEAR:
	case WM_PASTE:this->FormatClipboard();
	return (BOOL)SendMessage(LOWORD(wParam));
	case ME_SELECTALL:
	return (BOOL)SendMessage (EM_SETSEL, 0, -1);
	case ME_CONVERT:
		return (BOOL)SendMessage(ME_CONVERT,0,0);
	default:
	return CEdit::OnCommand(wParam, lParam);
	}
}

bool CHexEdit::IsHexConvertableText(LPTSTR _text)
{
	int start,end;
	GetSel(start,end);
	CString text;
	this->GetWindowText(text);
	char head=0,second=0;
	bool bPaste=true;
	if(text.GetLength()>0) head=text.GetAt(0);
	if(text.GetLength()>1) second=text.GetAt(1);
	if(second=='X'||second=='x')
	{
		if(end<=1)
			bPaste=false;
	}
	if(!bPaste) return bPaste;
	//Check
	unsigned int i=0;
	if(strlen(_text)>=2)
	{
		if(_text[0]=='0'&&(_text[1]=='x'||_text[1]=='X'))
		{
			if((second=='x'||second=='X')&&(!(start==0&&end>=2)))
				bPaste=false;
			else if(start>0)
				bPaste=false;
			else
			i+=2;
		}
	}
	if(!bPaste) return bPaste;
	if(strlen(_text)>=1)
	{
		if(head=='0'&&(_text[0]=='x'||_text[0]=='X'))
		{
			i++;
		}
		if((_text[0]=='x'||_text[0]=='X'))
		{
			if(head!='0'&&start==0)
				bPaste=false;
			else if(!(start==1&&end>=1&&head=='0'))
				bPaste=false;
		}
	}
	if(!bPaste) return bPaste;
	for(;i<strlen(_text);i++)
	{
		char c=_text[i];
		if(!(c>=48&&c<=57||c>='A'&&c<='F'||c>='a'&&c<='f'))
		{
			bPaste=false;
			break;
		}
	}
	return bPaste;
}

void CHexEdit::FormatClipboard()
{
	LPTSTR  lptstr,lptstrCopy; 
    HGLOBAL hglb; 
	if(!this->OpenClipboard())
		return;
	if(!IsClipboardFormatAvailable(CF_TEXT))
		return;
	hglb=GetClipboardData(CF_TEXT);
    if (hglb != NULL) 
    { 
        lptstr =(LPTSTR) GlobalLock(hglb);
		for(unsigned int i=0;i<strlen(lptstr);i++)
		{
			if(lptstr[i]!='X'&&lptstr[i]!='x')
				lptstr[i]=(char)toupper(lptstr[i]);
			if(lptstr[i]=='X')
				lptstr[i]='x';
		}
		hglb = GlobalAlloc(GMEM_MOVEABLE, 
            (strlen(lptstr)+1) * sizeof(TCHAR));
		if (hglb == NULL) 
        { 
            CloseClipboard(); 
            return; 
        }
		lptstrCopy = (LPTSTR)GlobalLock(hglb);
		memcpy(lptstrCopy, lptstr,strlen(lptstr));
		GlobalUnlock(hglb); 
		SetClipboardData(CF_TEXT,hglb);
		CloseClipboard();
	}

}

void CHexEdit::SetHexColor(COLORREF clrBack, COLORREF clrText)
{
	this->m_icHexText.clrBkColor=clrBack;
	this->m_icHexText.clrTextColor=clrText;
	m_bkHexBrush.DeleteObject();
	this->m_bkHexBrush.CreateSolidBrush(this->m_icHexText.clrBkColor);
}

void CHexEdit::SetDecColor(COLORREF clrBack, COLORREF clrText)
{
	this->m_icDecText.clrBkColor=clrBack;
	this->m_icDecText.clrTextColor=clrText;
	m_bkDecBrush.DeleteObject();
	this->m_bkDecBrush.CreateSolidBrush(this->m_icDecText.clrBkColor);
}

void CHexEdit::OnEnUpdate()
{
    CString text;
    int nIndex = 0;
 
    this->GetWindowText(text);
    text.MakeLower();
 
    // assume true until proven otherwise
    this->m_bHex = true;
 
    if (text.Left(2) == "0x")
        nIndex = 2;
 
    for (; nIndex < text.GetLength() && true == this->m_bHex; nIndex++)
    {
        if (! isxdigit(text[nIndex]))
            this->m_bHex = false;
    }
 
    this->RedrawWindow();
/*
	CString text;
	this->GetWindowText(text);
	//Hex or Dec?
	this->m_bHex=false;
	if(text.Find("0x")==0)
		this->m_bHex=true;
	for(int i=0;i<text.GetLength();i++)
	{
		char c=text.GetAt(i);
		if(c>='A'&&c<='F')
			this->m_bHex=true;
	}
	this->RedrawWindow();
*/
}

LRESULT CHexEdit::OnConvert(WPARAM wParam, LPARAM lParam)
{
	lParam;
	wParam;
	CString text;
	this->GetWindowText(text);
	if(this->m_bHex)
	{
		//convert hex to dec
		UINT n=0;
		if(text.Find("0x")==0)
			sscanf(text.GetBuffer(0)+2,"%x",&n);
		else
			sscanf(text.GetBuffer(0),"%x",&n);
		text.Format("%u",n);
		this->SetWindowText(text);
	}
	else
	{
		//convert dec to hex
		UINT n=0;
		sscanf(text.GetBuffer(0),"%d",&n);
		text.Format("0x%X",n);
		this->SetWindowText(text);
	}
	return 0;
}

DWORD CHexEdit::GetValue(void)
{
	CString text;
	this->GetWindowText(text);
	if(this->m_bHex)
	{
		//hex
		unsigned int n=0;
		if(text.Find("0x")==0)
			sscanf(text.GetBuffer(0)+2,"%x",&n);
		else
			sscanf(text.GetBuffer(0),"%x",&n);
		return n;
	}
	else
	{
		//convert dec to hex
		unsigned int n=0;
		sscanf(text.GetBuffer(0),"%u",&n);
		return n;
	}
}

void CHexEdit::SetValue(DWORD _value,bool _bHex)
{
	CString text;
	if(!_bHex)
	{
		text.Format("%u",_value);
	}
	else
		text.Format("0x%x",_value);
	this->SetWindowText(text);
}
