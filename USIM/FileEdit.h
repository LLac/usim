#if !defined(FILEEDIT_H__24C2AEA9_B40D_11D4_BFA7_00C0DF034AED__INCLUDED_)
#define FILEEDIT_H__24C2AEA9_B40D_11D4_BFA7_00C0DF034AED__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <vector>

struct FileValues
{
    CString m_sName;
    CString m_sValue;
};

class PredicateNameEquals   
{
public:
    PredicateNameEquals(const CString &name)
        : m_sName(name)
    {
    }
   
    bool operator() ( const FileValues &value ) const
    {
        return value.m_sName == m_sName;
    }
   
private:
    const CString &m_sName;   
} ;

class FunctorWriteToFile
{
public:
    FunctorWriteToFile(CStdioFile &file)
        : m_file(file)
    {
    }
   
    void operator() (const FileValues &values) const
    {
        m_file.WriteString(values.m_sName);
        m_file.WriteString(" ");
        m_file.WriteString(values.m_sValue);
        m_file.WriteString("\n");
    }
   
private:
    CStdioFile &m_file;
};

void ReadFileValues(CString FileName, std::vector<FileValues> &values);
void ReplaceValueWith(std::vector<FileValues> &values, const CString &itemName, const CString &newItemName, const CString &newItemValue);
void SaveFileValues(CString FileName, const std::vector<FileValues> &values);

#endif // !defined(FILEEDIT_H__24C2AEA9_B40D_11D4_BFA7_00C0DF034AED__INCLUDED_)