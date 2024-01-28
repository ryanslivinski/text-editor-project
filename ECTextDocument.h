#ifndef ECTextDocument_h
#define ECTextDocument_h

#include "ECCommand.h"
#include "ECTextViewImp.h"
#include "ECDocMode.h"
#include <vector>
#include <string>
#include <fstream>

class ECTextDocument;

// **********************************************************
// COMMANDS
class ECInsertTextCommand : public ECCommand{

public:
    ECInsertTextCommand(ECTextDocument *doc, int row, int pos, const std::string &listCharsToIns);
    virtual void Execute();
    virtual void UnExecute();

private:
    ECTextDocument *_doc;
    int _row, _pos;
    std::string _listChars;
};

class ECRemoveTextCommand : public ECCommand{

public:
    ECRemoveTextCommand(ECTextDocument *doc, int row, int pos, int lenToRemove);
    virtual void Execute();
    virtual void UnExecute();

private:
    ECTextDocument *_doc;
    int _row, _pos;
    int _lenToRemove;
    std::vector<char> _deletedChars;
};

class ECReplaceTextCommand : public ECCommand{

public:
    ECReplaceTextCommand(ECTextDocument *doc, const std::string &strOldIn, const std::string &strNewIn);
    virtual void Execute();
    virtual void UnExecute();

private:
    ECTextDocument *_doc;
    std::string _strOld, _strNew;
};

class ECCapTextCommand : public ECCommand{

public:
    ECCapTextCommand(ECTextDocument *doc, int row, int pos, int lenToCap);
    virtual void Execute();
    virtual void UnExecute();

private:
    ECTextDocument *_doc;
    int _row, _pos;
    int _lenToCap;
};

class ECLowerTextCommand : public ECCommand{

public:
    ECLowerTextCommand(ECTextDocument *doc, int row, int pos, int lenToLower);
    virtual void Execute();
    virtual void UnExecute();

private:
    ECTextDocument *_doc;
    int _row, _pos;
    int _lenToLower;
};

class ECLineBreakCommand : public ECCommand{

public:
    ECLineBreakCommand(ECTextDocument *doc, int row, int pos);
    virtual void Execute();
    virtual void UnExecute();

private:
    ECTextDocument *_doc;
    int _row, _pos;
};

class ECBackspaceCommand : public ECCommand{

public:
    ECBackspaceCommand(ECTextDocument *doc, int row, int pos);
    virtual void Execute();
    virtual void UnExecute();

private:
    ECTextDocument *_doc;
    int _row, _pos;
    char _temp;
};

class ECCombineRowsCommand : public ECCommand{

public:
    ECCombineRowsCommand(ECTextDocument *doc, int row);
    virtual void Execute();
    virtual void UnExecute();

private:
    ECTextDocument *_doc;
    int _row;
    std::string _temp;
};

class ECClearRowCommand : public ECCommand{

public: 
    ECClearRowCommand(ECTextDocument *doc, int row);
    virtual void Execute();
    virtual void UnExecute();

private:
    ECTextDocument *_doc;
    int _row;
    std::string _temp;
};

// **********************************************************
// CONTROLLER

class ECTextDocumentCtrl
{
public:
    ECTextDocumentCtrl(ECTextDocument &docIn);
    virtual ~ECTextDocumentCtrl();

    /* Document Functions */
    void InsertTextAt(int row, int pos, const std::string &listCharsToIns);
    void RemoveTextAt(int row, int pos, int lenToRemove);
    void ReplaceTextAt(const std::string &strOldIn, const std::string &strNewIn);
    void CapTextAt(int row, int pos, int lenToCap);
    void LowerTextAt(int row, int pos, int lenToLower);
    void Undo();
    void Redo();
    void LineBreak(int row, int pos);
    void Backspace(int row, int pos);
    void CombineRows(int row);
    void AddPage();
    void AddTotalPage();
    void InsertLine();
    void InsertPage();
    void DeletePage();
    void UpdateView();
    void ClearRow(int row);
    int IsSubstring(const std::string &strIn1, const std::string &strIn2);

    /* Getters */
    int GetRowSize(int row) const;
    int GetNumRows() const;
    int GetPage() const;
    int GetTotalPages() const;
    int GetDocSize() const;
    int GetCmdIndex() { return histCmds.GetIndex(); }
    int GetCmdSize() { return histCmds.GetCmdListSize(); }
    int GetMode();
    std::vector<std::vector<std::string>> GetPages() const;

    /* Setters */
    void SetMode(int modeIn);
    void SetNewDoc(const std::vector<std::vector<std::string>> &docIn);

    /* File I/O */
    void UpdateFile();
    
private:
    ECTextDocument &doc;
    ECCommandHistory histCmds;
    ECDocumentMode _mode;

};

// **********************************************************
// MODEL (text document)

class ECTextDocument
{
public:
    ECTextDocument();
    virtual ~ECTextDocument();
    ECTextDocumentCtrl &GetCtrl();
    ECTextViewImp &GetView();

    /* Document Functions */
    void InsertCharAt(int row, int pos, char ch);
    void RemoveCharAt(int row, int pos);
    void CapCharAt(int row, int pos);
    void LowerCharAt(int row, int pos);
    void UpdateViewDoc();
    void NewLine(int row, std::string str);
    void AddPageDoc();
    void AddTotalPageDoc();
    void InsertLineDoc();
    void InsertPageDoc();
    void DeletePageDoc();
    void ClearRowDoc(int row);
    int IsSubstringDoc(const std::string &strIn1, const std::string &strIn2);
    void ReplaceText(const std::string &strOld, const std::string &strNew);

    /* Getters */
    int GetDocLen() const { return listChars.size(); }
    char GetCharAt(int row, int pos) const;
    int GetRowSizeDoc(int row) const;
    int GetNumRowsDoc() const { return listChars[_page].size(); }
    int GetPageDoc() const { return _page; }
    int GetTotalPagesDoc() const { return _totalPages; }
    int GetDocSizeDoc() const { return listChars.size(); }
    std::vector<std::vector<std::string>> GetPagesDoc() const { return listChars; }

    /* Setters */
    void SetDocName(const std::string &nameIn);
    void SetNewDoc(const std::vector<std::vector<std::string>> &docIn);

    /* File I/O */
    void CreateDocument(const std::string &nameIn);
    void LoadDocument();
    void UpdateFileDoc();

private:
    ECTextDocumentCtrl _docCtrl;
    ECTextViewImp _docView;
    std::vector<std::vector<std::string>> listChars;
    int _page, _totalPages;
    std::string _docName;
};

#endif /* ECTextDocument_h */