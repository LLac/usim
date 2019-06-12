//////////////////////////////////////////////////////////////////////
// CPathSplit.cpp - Kochise 2002-2004
//

#include "stdafx.h"
#include "CPathSplit.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

/////////////////////////////////////////////////////////////////////////////
// CSkinProgress

// g_Global      : g_ = Global
// i_Input       : i_ = Input
// no z...
// m_Member      : m_ = Member
// o_Output      : o_ = Output
// l_Local       : l_ = Local
// ...           : use your imagination

// aArray        : a  = Array    (array)
// bBool         : b  = Boolean  (boolean, true/false)
// cConstant     : c  = Constant (constant, whatever...)
// dDefine       : d  = Define   (simple define or defined value)
// eEnum         : e  = Enum     (enum list element)
// nNumber       : n  = Number   (char, long, int, whatever...)
// oObject       : o  = Object   (C++ class)
// pPointer      : p  = Pointer  (typicaly a 32 bits ulong address)
// sStruct       : s  = Struct   (structure)
// tTemplate     : t  = Template (template)
// uUnion        : u  = Union    (join two or more values of the same size under a common name)

// poRemoteClass : po = Pointer on Object
// cdConstantDef : cd = Constant Define, typicaly a constant defined value
// usUnionStruct : us = Union of Structures
// ...           : use your imagination

// o_psBitmap    : CBitmap::GetBitmap((BITMAP*) o_psBitmap); // s = BITMAP, p = *, o_ means it's an output

// <Value>
// [Function]

CPathSplit::CPathSplit
(
)
{

  // Init

  ErasePointers();

  // Process
}

CPathSplit::CPathSplit
( CString const& i_poStrPath
)
{

  // Init

  ErasePointers();

  // Process

  Split(i_poStrPath);
}

BOOL CPathSplit::Split
( CString const& i_poStrPath
)
{
  int l_nLenPath;

  // Init

  ErasePointers(); // Erase pointers

  m_oStrPath = i_poStrPath;
  m_oStrPath.TrimLeft();
  m_oStrPath.TrimRight();

  // Process

  l_nLenPath = m_oStrPath.GetLength();

  if(l_nLenPath > 0)
  { // If the path is not empty
    m_anPathFirst[ePATHEL_DIRECTORY] = m_oStrPath.Find('\\');

    if(m_anPathFirst[ePATHEL_DIRECTORY] != (-1))
    { // If first directory found
      m_anPathCount[ePATHEL_DRIVE] = m_anPathFirst[ePATHEL_DIRECTORY] - m_anPathFirst[ePATHEL_DRIVE];

      if(m_oStrPath.Mid(m_anPathFirst[ePATHEL_DIRECTORY], 2) == "\\\\")
      { // If Network
        m_anPathFirst[ePATHEL_DIRECTORY] += 1;
      }else{}

      m_anPathFirst[ePATHEL_FILENAME] = m_oStrPath.ReverseFind('\\');

      if(m_anPathFirst[ePATHEL_FILENAME] != (-1))
      { // If last directory found (typicaly the file name, but may be empty also)
        m_anPathFirst[ePATHEL_FILENAME] += 1; // Include the trailing '\' in the directory
        m_anPathCount[ePATHEL_DIRECTORY] = m_anPathFirst[ePATHEL_FILENAME] - m_anPathFirst[ePATHEL_DIRECTORY];

        m_anPathFirst[ePATHEL_EXTENSION] = m_oStrPath.ReverseFind('.');

        if
        (
             (m_anPathFirst[ePATHEL_EXTENSION] != (-1))
          && (m_anPathFirst[ePATHEL_EXTENSION]  > m_anPathFirst[ePATHEL_FILENAME])
        )
        { // If a extension was found AFTER the last directory
          m_anPathCount[ePATHEL_FILENAME]  = m_anPathFirst[ePATHEL_EXTENSION] - m_anPathFirst[ePATHEL_FILENAME];
          m_anPathCount[ePATHEL_EXTENSION] = l_nLenPath - m_anPathFirst[ePATHEL_EXTENSION];
        }
        else
        {
          m_anPathCount[ePATHEL_FILENAME]  = l_nLenPath - m_anPathFirst[ePATHEL_FILENAME];
          m_anPathFirst[ePATHEL_EXTENSION] = 0;
          // m_anPathCount[ePATHEL_EXTENSION] = 0; // Already to 0 from ErasePointers()
        }
      }else{}
    }
    else
    {
      m_anPathCount[ePATHEL_DRIVE] = l_nLenPath;
    }

    return true;
  }else{}

  return false;
}

CString CPathSplit::GetPath
( int      i_nEnd
, CString* i_poStrQueue
, int      i_nStart
)
{
  CString l_oStrTempo;

  //

  if(i_nEnd < 0)
  { // In case...
    i_nEnd = 0;
  }else{}

  if(i_nStart < 0)
  { // ...of bad values
    i_nStart = 0;
  }else{}

  if(i_nEnd >= ePATHEL__END)
  { // Clip to the maximum number of path elements currently supported
    i_nEnd = ePATHEL__END - 1;
  }else{}

  if(i_nStart > i_nEnd)
  { // Clip the start
    i_nStart = i_nEnd;
  }else{}

  l_oStrTempo
  = m_oStrPath.Mid
    ( m_anPathFirst[i_nStart] // From...
    ,    (m_anPathFirst[i_nEnd] - m_anPathFirst[i_nStart]) // Size of the Start TO end
       + m_anPathCount[i_nEnd] // Includes size of end
    )
  ;

  if(i_poStrQueue != NULL)
  { // Add a queue of needed
    l_oStrTempo += *i_poStrQueue;
  }else{}

  return l_oStrTempo;
}

void CPathSplit::ErasePointers
(
)
{
  int l_nLoop;

  // Init

  for
  ( l_nLoop = 0
  ; l_nLoop < ePATHEL__END
  ; l_nLoop += 1
  )
  { // Erase pointers
    m_anPathFirst[l_nLoop] = 0;
    m_anPathCount[l_nLoop] = 0;
  }

  // Process
}
