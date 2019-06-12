// Document modified at : Saturday, July 20, 2002 07:35:44 PM , by user : Pavel Sokolov , from computer : CREA7OR
// CEAdvButton.cpp : implementation file
#include "stdafx.h"
#include "CEAdvButton.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

CEAdvButton::CEAdvButton()
{
}

CEAdvButton::~CEAdvButton()
{
}

BEGIN_MESSAGE_MAP(CEAdvButton, CButton)
	//{{AFX_MSG_MAP(CEAdvButton)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

void CEAdvButton::AddControlToList( int l_intID )
{
	m_caIDs.Add( l_intID );
}

void CEAdvButton::ClearList()
{
	m_caIDs.RemoveAll();
}

void CEAdvButton::SetCheck( int l_intCheck )
{
	if ( l_intCheck == 0 )
	{
		EnableList( FALSE );
	}
	else
	{
		EnableList( TRUE );
	}
	CButton::SetCheck( l_intCheck );
}

void CEAdvButton::SetEnable( int l_intCheck )
{
	if ( l_intCheck == 0 )
	{
		EnableList( FALSE );
	}
	else
	{
		EnableList( TRUE );
	}
}

void CEAdvButton::EnableList( BOOL l_boolEnable )
{
	int l_intSize = m_caIDs.GetSize();
	for ( int l_intCnt = 0 ; l_intCnt< l_intSize; l_intCnt++ )
	{
		GetParent()->GetDlgItem( m_caIDs.GetAt( l_intCnt ))->EnableWindow( l_boolEnable );
	}
}
void CEAdvButton::OnClicked() 
{
	SetCheck( GetCheck() );
}
