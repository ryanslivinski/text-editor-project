#include "ECTextDocument.h"
#include "ECKeyInput.h"
#include <iostream>
#include <cctype>
using namespace std;

// **********************************************************
// COMMANDS

ECInsertTextCommand :: ECInsertTextCommand(ECTextDocument *doc, int row, int pos, const std::string &listCharsToIns) 
    : _doc(doc), _row(row), _pos(pos), _listChars(listCharsToIns){}

void ECInsertTextCommand :: Execute()
{
    int count = _pos;
    for(int i=0; i<_listChars.size(); i++){
        _doc->InsertCharAt(_row, count, _listChars[i]);
        count++;
    }
}

void ECInsertTextCommand :: UnExecute()
{
    for(int i=0; i<_listChars.size(); i++){
        _doc->RemoveCharAt(_row, _pos);
    }
}

ECRemoveTextCommand :: ECRemoveTextCommand(ECTextDocument *doc, int row, int pos, int lenToRemove) 
  : _doc(doc), _row(row), _pos(pos), _lenToRemove(lenToRemove) {}

void ECRemoveTextCommand :: Execute()
{
    for(int i=0; i<_lenToRemove; i++){
        _deletedChars.push_back(_doc->GetCharAt(_row, _pos));
        _doc->RemoveCharAt(_row, _pos);
    }
}

void ECRemoveTextCommand :: UnExecute()
{
    int count = _pos;
    for(int i=0; i<_lenToRemove; i++){
        _doc->InsertCharAt(_row, count, _deletedChars[i]);
        count++;
    }
}

ECReplaceTextCommand :: ECReplaceTextCommand(ECTextDocument *doc, const std::string &strOldIn, const std::string &strNewIn)
    : _doc(doc), _strOld(strOldIn), _strNew(strNewIn) {}

void ECReplaceTextCommand :: Execute()
{
    _doc->ReplaceText(_strOld, _strNew);
}

void ECReplaceTextCommand :: UnExecute()
{
    _doc->ReplaceText(_strNew, _strOld);
}

ECCapTextCommand :: ECCapTextCommand(ECTextDocument *doc, int row, int pos, int lenToCap) 
    : _doc(doc), _row(row), _pos(pos), _lenToCap(lenToCap) {}

void ECCapTextCommand :: Execute()
{  
    if(_pos+_lenToCap > _doc->GetDocLen() || _pos < 0 || _lenToCap < 0)
        return;

    int count = _pos;

    for(int i=0; i<_lenToCap; i++){
        _doc->CapCharAt(_row, count);
        count++;
    }
}

void ECCapTextCommand :: UnExecute()
{
    int count = _pos;
    for(int i=0; i<_lenToCap; i++){
        _doc->LowerCharAt(_row, count);
        count++;
    }
}

ECLowerTextCommand :: ECLowerTextCommand(ECTextDocument *doc, int row, int pos, int lenToLower) 
    : _doc(doc), _row(row), _pos(pos), _lenToLower(lenToLower) {}

void ECLowerTextCommand :: Execute()
{
    int count = _pos;
    for(int i=0; i<_lenToLower; i++){
        _doc->LowerCharAt(_row, count);
        count++;
    }
}

void ECLowerTextCommand :: UnExecute()
{  
    int count = _pos;
    for(int i=0; i<_lenToLower; i++){
        _doc->CapCharAt(_row, count);
        count++;
    }
}

ECLineBreakCommand :: ECLineBreakCommand(ECTextDocument *doc, int row, int pos) 
    : _doc(doc), _row(row), _pos(pos) {}

void ECLineBreakCommand :: Execute()
{
    std::string _temp;
    /*
        ** Before enter **
        Ryan Slivinski (cursor before the "S")
    */
    for(int i=0; i < _pos; i++){
        _temp.push_back(_doc->GetCharAt(_row, i));
    }

    // _temp = "Ryan"
    _doc->NewLine(_row, _temp);

    /*
        ** After NewLine() **
        Ryan Slivinski
        Ryan Slivinski
    */

    for(int i=0; i < _pos; i++){
        _doc->RemoveCharAt(_row + 1, 0);
    }

    // _doc->InsertLine();

    /*
        ** After RemoveCharAt() **
        Ryan
        Slivinski
    */
}

void ECLineBreakCommand :: UnExecute()
{
    std::string _temp;
    for(int i=0; i<_doc->GetRowSizeDoc(_row+1); i++){
        _temp.push_back(_doc->GetCharAt(_row+1, i));
    }

    int count = _pos;
    for(int i=0; i<_temp.size(); i++){
        _doc->InsertCharAt(_row, count, _temp[i]);
        count++;
    }

    for(int i=0; i<_doc->GetRowSizeDoc(_row); i++){
        _doc->RemoveCharAt(_row+1, 0);
    }
}

ECBackspaceCommand :: ECBackspaceCommand(ECTextDocument *doc, int row, int pos)
    : _doc(doc), _row(row), _pos(pos) {}

void ECBackspaceCommand :: Execute()
{
    _temp = _doc->GetCharAt(_row, _pos);
    _doc->RemoveCharAt(_row, _pos);
}

void ECBackspaceCommand :: UnExecute()
{
    _doc->InsertCharAt(_row, _pos, _temp);
}

ECCombineRowsCommand :: ECCombineRowsCommand(ECTextDocument *doc, int row)
    : _doc(doc), _row(row) {}

void ECCombineRowsCommand :: Execute()
{
    for(int i=0; i < _doc->GetRowSizeDoc(_row); i++){
        _doc->InsertCharAt(_row - 1, _doc->GetRowSizeDoc(_row - 1), _doc->GetCharAt(_row, i));
        _temp.push_back(_doc->GetCharAt(_row, i));
    }

    int temp = _doc->GetRowSizeDoc(_row);
    for(int i=0; i<temp; i++){
        _doc->RemoveCharAt(_row, 0);
    }
}

void ECCombineRowsCommand :: UnExecute()
{
    for(int i=0; i<_temp.size(); i++){
        _doc->InsertCharAt(_row, i, _temp[i]);
    }

    int temp = _doc->GetRowSizeDoc(_row-1) - _temp.size();
    for(int i=0; i<_temp.size(); i++){
        _doc->RemoveCharAt(_row-1, temp);
    }
}

// **********************************************************
// CONTROLLER

ECTextDocumentCtrl :: ECTextDocumentCtrl(ECTextDocument &docIn) : doc(docIn)
{
}

ECTextDocumentCtrl :: ~ECTextDocumentCtrl()
{
}

/* Document Functions */
void ECTextDocumentCtrl :: InsertTextAt(int row, int pos, const std::string &listCharsToIns)
{
    ECCommand *InsertText = new ECInsertTextCommand(&doc, row, pos, listCharsToIns);
    histCmds.ExecuteCmd(InsertText);
    doc.UpdateViewDoc();
}

void ECTextDocumentCtrl :: RemoveTextAt(int row, int pos, int lenToRemove)
{
    ECCommand *RemoveText = new ECRemoveTextCommand(&doc, row, pos, lenToRemove);
    histCmds.ExecuteCmd(RemoveText);
    doc.UpdateViewDoc();
}

void ECTextDocumentCtrl :: ReplaceTextAt(const std::string &strOldIn, const std::string &strNewIn)
{
    ECCommand *ReplaceText = new ECReplaceTextCommand(&doc, strOldIn, strNewIn);
    histCmds.ExecuteCmd(ReplaceText);
    doc.UpdateViewDoc();
}

void ECTextDocumentCtrl :: CapTextAt(int row, int pos, int lenToCap)
{
    ECCommand *CapText = new ECCapTextCommand(&doc, row, pos, lenToCap);
    histCmds.ExecuteCmd(CapText);
    doc.UpdateViewDoc();
}

void ECTextDocumentCtrl :: LowerTextAt(int row, int pos, int lenToLower)
{
    ECCommand *LowText = new ECLowerTextCommand(&doc, row, pos, lenToLower);
    histCmds.ExecuteCmd(LowText);
    doc.UpdateViewDoc();
}
 
void ECTextDocumentCtrl :: Undo()
{
    histCmds.Undo();
    doc.UpdateViewDoc();
}

void ECTextDocumentCtrl :: Redo()
{
    histCmds.Redo();
    doc.UpdateViewDoc();
}

void ECTextDocumentCtrl :: LineBreak(int row, int pos)
{
    ECCommand *LineB = new ECLineBreakCommand(&doc, row, pos);
    histCmds.ExecuteCmd(LineB);
    doc.UpdateViewDoc();
}

void ECTextDocumentCtrl :: Backspace(int row, int pos)
{
    ECCommand *Back = new ECBackspaceCommand(&doc, row, pos);
    histCmds.ExecuteCmd(Back);
    doc.UpdateViewDoc();
}

void ECTextDocumentCtrl :: CombineRows(int row)
{
    ECCommand *Combine = new ECCombineRowsCommand(&doc, row);
    histCmds.ExecuteCmd(Combine);
    doc.UpdateViewDoc();
}

void ECTextDocumentCtrl :: AddPage()
{
    doc.AddPageDoc();
}

void ECTextDocumentCtrl :: AddTotalPage()
{
    doc.AddTotalPageDoc();
}

void ECTextDocumentCtrl :: InsertLine()
{
    doc.InsertLineDoc();
}

void ECTextDocumentCtrl :: InsertPage()
{
    doc.InsertPageDoc();
}

void ECTextDocumentCtrl :: DeletePage()
{
    doc.DeletePageDoc();
}

void ECTextDocumentCtrl :: UpdateView()
{
    doc.UpdateViewDoc();
}

void ECTextDocumentCtrl :: ClearRow(int row)
{
    doc.ClearRowDoc(row);
}

int ECTextDocumentCtrl :: IsSubstring(const std::string &strIn1, const std::string &strIn2)
{
    return doc.IsSubstringDoc(strIn1, strIn2);
}

/* Getters */
int ECTextDocumentCtrl :: GetRowSize(int row) const
{
    return doc.GetRowSizeDoc(row);
}

int ECTextDocumentCtrl :: GetNumRows() const
{
    return doc.GetNumRowsDoc();
}

int ECTextDocumentCtrl :: GetPage() const
{
    return doc.GetPageDoc();
}

int ECTextDocumentCtrl :: GetTotalPages() const
{
    return doc.GetTotalPagesDoc();
}

int ECTextDocumentCtrl :: GetDocSize() const
{
    return doc.GetDocSizeDoc();
}

int ECTextDocumentCtrl :: GetMode()
{
    return _mode.GetMode();
}

std::vector<std::vector<std::string>> ECTextDocumentCtrl :: GetPages() const
{
    return doc.GetPagesDoc();
}

/* Setters */
void ECTextDocumentCtrl :: SetMode(int modeIn)
{
    _mode.SetMode(modeIn);
}

void ECTextDocumentCtrl :: SetNewDoc(const std::vector<std::vector<std::string>> &docIn)
{
    doc.SetNewDoc(docIn);
}

/* File I/O */
void ECTextDocumentCtrl :: UpdateFile()
{
    doc.UpdateFileDoc();
}

// **********************************************************
// MODEL (text document)
ECTextDocument :: ECTextDocument() : _docCtrl(*this)
{
    _page = 0, _totalPages = 0;
    listChars.push_back({""});
    _docView = ECTextViewImp();
    ECObserver *KeyInput = new ECKeyListener(this);
    _docView.Attach(KeyInput);
    UpdateViewDoc();
    KeyInput->Update();
    _docView.SetCursorX(0);
}

ECTextDocument :: ~ECTextDocument()
{
    _docCtrl.~ECTextDocumentCtrl();
    _docView.~ECTextViewImp();
}

ECTextDocumentCtrl & ECTextDocument :: GetCtrl()
{
    return _docCtrl;
}

ECTextViewImp & ECTextDocument :: GetView()
{ 
    return _docView;
}

/* Document Functions */
void ECTextDocument :: InsertCharAt(int row, int pos, char ch)
{
    listChars[_page][row].insert(listChars[_page][row].begin()+pos, ch);
}

void ECTextDocument :: RemoveCharAt(int row, int pos)
{
    listChars[_page][row].erase(listChars[_page][row].begin()+pos);
}

void ECTextDocument :: CapCharAt(int row, int pos)
{
    listChars[_page][row][pos] = toupper(listChars[_page][row][pos]);  
}

void ECTextDocument :: LowerCharAt(int row, int pos)
{
    listChars[_page][row][pos] = tolower(listChars[_page][row][pos]);
}

void ECTextDocument :: UpdateViewDoc()
{
    _docView.InitRows();

    for(auto it = listChars[_page].begin(); it!= listChars[_page].end(); it++){
        _docView.AddRow(*it);
    }

    _docView.Refresh();
}

void ECTextDocument :: NewLine(int row, std::string str)
{
    listChars[_page].insert(listChars[_page].begin() + row, str);
}

void ECTextDocument :: AddPageDoc()
{
    _page += 1;
}

void ECTextDocument :: AddTotalPageDoc()
{
    _totalPages += 1;
}

void ECTextDocument :: InsertLineDoc()
{
    listChars[_page].push_back({""});
}

void ECTextDocument :: InsertPageDoc()
{
    listChars.push_back({""});
}

void ECTextDocument :: DeletePageDoc()
{
    _page -= 1;
}

void ECTextDocument :: ClearRowDoc(int row){
    listChars[_page].erase(listChars[_page].begin() + row);
}

int ECTextDocument :: IsSubstringDoc(const std::string &strIn1, const std::string &strIn2)
{
    int x = strIn1.length();
    int y = strIn2.length();
 
    for (int i = 0; i <= y - x; i++) {
        int j;
 
        for (j = 0; j < x; j++)
            if (strIn2[i + j] != strIn1[j])
                break;
 
        if (j == x)
            return i;
    }
    
    return -1;
}

void ECTextDocument :: ReplaceText(const std::string &strOld, const std::string &strNew)
{   
    for(int i=0; i<listChars[_page].size(); i++){
        size_t pos = listChars[_page][i].find(strOld);
        while(pos != std::string::npos){
            listChars[_page][i].replace(pos, strOld.length(), strNew);
            pos = listChars[_page][i].find(strOld, pos + strNew.length());
        }
    }
}

/* Getters */
char ECTextDocument :: GetCharAt(int row, int pos) const
{
    return listChars[_page][row][pos];
}

int ECTextDocument :: GetRowSizeDoc(int row) const{
    return listChars[_page][row].size();
}

/* Setters */
void ECTextDocument :: SetDocName(const std::string &nameIn)
{
    _docName = nameIn;
}

void ECTextDocument :: SetNewDoc(const std::vector<std::vector<std::string>> &docIn)
{
    listChars = docIn;
}

/* File I/O */
void ECTextDocument :: CreateDocument(const std::string &nameIn)
{
    ofstream myFile(nameIn);
    myFile.close();
}

void ECTextDocument :: LoadDocument()
{
    string myText;

    ifstream myFile(_docName);

    std::vector<std::string> temp;

    int row = 0;
    while(getline(myFile, myText)){
        listChars[_page].insert(listChars[_page].begin() + row, myText);
        row++;
    }
    _docView.Refresh();
}

void ECTextDocument :: UpdateFileDoc()
{  
    ofstream myFile(_docName);

    for(int page=0; page<listChars.size(); page++){
        for(int row=0; row<listChars[page].size(); row++){
        myFile << listChars[page][row] << "\n";
        }
    }
    myFile.close();
}