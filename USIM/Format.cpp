// Format.cpp: implementation of the Format class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "usim.h"
#include "Format.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

Format::Format()
{
#define FORCE_ANSI      0x10000
#define FORCE_UNICODE   0x20000

// formatting (using wsprintf style formatting)
void Format(CString& str, LPCTSTR lpszFormat, ...)
{
     ASSERT(AfxIsValidString(lpszFormat, FALSE));

     va_list argList;
     va_start(argList, lpszFormat);

     // make a guess at the maximum length of the resulting string
     int nMaxLen = 0;
     for (LPCTSTR lpsz = lpszFormat; *lpsz != '\0';
          lpsz = _tcsinc(lpsz))
     {
          // handle '%' character, but watch out for '%%'
          if (*lpsz != '%' || *(lpsz = _tcsinc(lpsz)) == '%')
          {
               nMaxLen += _tclen(lpsz);
               continue;
          }

          int nItemLen = 0;

          // handle '%' character with format
          int nWidth = 0;
          for (; *lpsz != '\0'; lpsz = _tcsinc(lpsz))
          {
               // check for valid flags
               if (*lpsz == '#')
                    nMaxLen += 2;   // for '0x'
               else if (*lpsz == '*')
                    nWidth = va_arg(argList, int);
               else if (*lpsz == '-' || *lpsz == '+' || *lpsz == '0'
                        || *lpsz == ' ')
                           ;
               else // hit non-flag character
                          break;
          }
          // get width and skip it
          if (nWidth == 0)
          {
               // width indicated by
               nWidth = _ttoi(lpsz);
               for (; *lpsz != '\0' && _istdigit(*lpsz);
                        lpsz = _tcsinc(lpsz))
                     ;
          }
          ASSERT(nWidth >= 0);

          int nPrecision = 0;
          if (*lpsz == '.')
          {
               // skip past '.' separator (width.precision)
               lpsz = _tcsinc(lpsz);

               // get precision and skip it
               if (*lpsz == '*')
               {
                    nPrecision = va_arg(argList, int);
                    lpsz = _tcsinc(lpsz);
               }
               else
               {
                    nPrecision = _ttoi(lpsz);
                    for (; *lpsz != '\0' && _istdigit(*lpsz);
                    lpsz = _tcsinc(lpsz))
                            ;
               }
               ASSERT(nPrecision >= 0);
          }

          // should be on type modifier or specifier
          int nModifier = 0;
          switch (*lpsz)
          {
          // modifiers that affect size
          case 'h':
               nModifier = FORCE_ANSI;
               lpsz = _tcsinc(lpsz);
               break;
          case 'l':
               nModifier = FORCE_UNICODE;
               lpsz = _tcsinc(lpsz);
               break;

          // modifiers that do not affect size
          case 'F':
          case 'N':
          case 'L':
               lpsz = _tcsinc(lpsz);
               break;
          }

          // now should be on specifier
          switch (*lpsz | nModifier)
          {
          // single characters
          case 'c':
          case 'C':
               nItemLen = 2;
               va_arg(argList, TCHAR);
               break;
          case 'c'|FORCE_ANSI:
          case 'C'|FORCE_ANSI:
               nItemLen = 2;
               va_arg(argList, char);
               break;
          case 'c'|FORCE_UNICODE:
          case 'C'|FORCE_UNICODE:
               nItemLen = 2;
               va_arg(argList, WCHAR);
               break;

          // strings
          case 's':
          case 'S':
               nItemLen = lstrlen(va_arg(argList, LPCTSTR));
               nItemLen = max(1, nItemLen);
               break;
          case 's'|FORCE_ANSI:
          case 'S'|FORCE_ANSI:
               nItemLen = lstrlenA(va_arg(argList, LPCSTR));
               nItemLen = max(1, nItemLen);
               break;
#ifndef _MAC
          case 's'|FORCE_UNICODE:
          case 'S'|FORCE_UNICODE:
               nItemLen = wcslen(va_arg(argList, LPWSTR));
               nItemLen = max(1, nItemLen);
               break;
#endif
          }

          // adjust nItemLen for strings
          if (nItemLen != 0)
          {
               nItemLen = max(nItemLen, nWidth);
               if (nPrecision != 0)
               nItemLen = min(nItemLen, nPrecision);
          }
          else
          {
               switch (*lpsz)
               {
               // integers
               case 'd':
               case 'i':
               case 'u':
               case 'x':
               case 'X':
               case 'o':
                    va_arg(argList, int);
                    nItemLen = 32;
                    nItemLen = max(nItemLen, nWidth+nPrecision);
                    break;

               case 'e':
               case 'f':
               case 'g':
               case 'G':
                    va_arg(argList, _AFX_DOUBLE);
                    nItemLen = 32;
                    nItemLen = max(nItemLen, nWidth+nPrecision);
                    break;

               case 'p':
                    va_arg(argList, void*);
                    nItemLen = 32;
                    nItemLen = max(nItemLen, nWidth+nPrecision);
                    break;

               // no output
               case 'n':
                    va_arg(argList, int*);
                    break;

               default:
                    ASSERT(FALSE);  // unknown formatting option
               }
          }

          // adjust nMaxLen for output nItemLen
          nMaxLen += nItemLen;
     }
     va_end(argList);

     // finally, set the buffer length and format the string
     va_start(argList, lpszFormat);  // restart the arg list
     str.GetBuffer(nMaxLen);
     VERIFY(_vstprintf(str.GetBuffer(1), lpszFormat, argList) <= nMaxLen);
     str.ReleaseBuffer();
     va_end(argList);
}

Format::~Format()
{

}
