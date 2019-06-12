#include "stdafx.h"

#include "FileEdit.h"
#include <afxwin.h>
#include <vector>
#include <algorithm>

void ReadFileValues(CString FileName, std::vector<FileValues> &values)
{
    CStdioFile Inputfile;
	CFileException FileExc;
	CString line;

	if (!Inputfile.Open(FileName, CFile::modeRead, &FileExc)) {
		FileExc.ReportError();
	}

    while (Inputfile.ReadString(line)) {
        if (line.GetLength() > 0) {
            int indexOfSpace = line.ReverseFind(L' ');
           
            if (indexOfSpace > 0) {
                std::vector<FileValues>::iterator item = values.insert(values.end(), FileValues());
                item->m_sName = line.Left(indexOfSpace);
                item->m_sValue = line.Right(line.GetLength() - (1 + indexOfSpace));
            }
        }
    }

	Inputfile.Close();
}

void ReplaceValueWith(std::vector<FileValues> &values, const CString &itemName, const CString &newItemName, const CString &newItemValue)
{
    std::vector<FileValues>::iterator item = std::find_if(values.begin(), values.end(), PredicateNameEquals(itemName));
   
    if (item != values.end()) {
        item->m_sName = newItemName;
        item->m_sValue = newItemValue;
    } else {
		// not found. Add item
		std::vector<FileValues>::iterator item = values.insert(values.end(), FileValues());
		item->m_sName = newItemName;
		item->m_sValue = newItemValue;
	}
}

void SaveFileValues(CString FileName, const std::vector<FileValues> &values)
{
    CStdioFile Inputfile;
	CFileException FileExc;

	if (!Inputfile.Open(FileName, CFile::modeWrite, &FileExc)) {
		FileExc.ReportError();
	}

    std::for_each(
        values.begin(),
        values.end(),
        FunctorWriteToFile(Inputfile) );
	
	Inputfile.Close();
}

