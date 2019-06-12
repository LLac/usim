//////////////////////////////////////////////////////////////////////
// CPathSplit.h - Kochise Kochise 2002-2004
//

#ifndef __CPATHSPLIT_H__
#define __CPATHSPLIT_H__

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

typedef enum ePathElement
{ ePATHEL_DRIVE = 0
, ePATHEL_DIRECTORY
, ePATHEL_FILENAME
, ePATHEL_EXTENSION
, ePATHEL__END
};

class CPathSplit
{ 
  // Construction
  public: 
    CPathSplit
    (
    );

    CPathSplit
    ( CString const& i_poStrPath
    );

  // Operations
    // Split the path
    BOOL Split
    ( CString const& i_poStrPath
    );
    
    inline CString GetPath()     {return m_oStrPath;}

    CString GetPath
    ( int      i_nEnd
    , CString* i_poStrQueue = NULL          // No queue
    , int      i_nStart     = ePATHEL_DRIVE // From DRIVE
    );

    inline CString GetDrive()    {return GetPath(ePATHEL_DRIVE,     NULL ,ePATHEL_DRIVE);}
    inline CString GetDirectory(){return GetPath(ePATHEL_DIRECTORY, NULL, ePATHEL_DIRECTORY);}
    inline CString GetFileName() {return GetPath(ePATHEL_FILENAME,  NULL, ePATHEL_FILENAME);}
    inline CString GetExtension(){return GetPath(ePATHEL_EXTENSION, NULL, ePATHEL_EXTENSION);}

  // Attributes
  protected:
    CString m_oStrPath;

    int     m_anPathFirst[ePATHEL__END];
    int     m_anPathCount[ePATHEL__END];

    void ErasePointers
    (
    );
}; 

#endif // __CPATHSPLIT_H__
