#ifndef ECKeyInput_h
#define ECKeyInput_h

#include "ECTextViewImp.h"
#include "ECTextDocument.h"
#include "ECObserver.h"
#include <string>

using namespace std;

class ECKeyListener : public ECObserver{

public:

    ~ECKeyListener() {}
    ECKeyListener(ECTextDocument *docIn){
        _doc = docIn;
        _view = &_doc->GetView();
        _docCtrl = &_doc->GetCtrl();
    }

    void Update(){

        switch (_view->GetPressedKey())
        {
        case ARROW_RIGHT:
            ArrowRightKey();
            break;

        case ARROW_LEFT:
            ArrowLeftKey();
            break;

        case ARROW_UP:
            ArrowUpKey();
            break;

        case ARROW_DOWN:
            ArrowDownKey();
            break;
        
        case BACKSPACE:
            BackspaceKey();
            break;

        case ENTER:
            EnterKey();
            break;

        case CTRL_Z: // undo
            CtrlZKey();
            break;

        case CTRL_Y: // redo
            CtrlYKey();
            break;

        case CTRL_F: // search & replace
            _searchString = "";
            _replaceString = "";
            _docCtrl->SetMode(SEARCH_MODE);
            break;

        case ESC:
            _docCtrl->SetMode(EDIT_MODE);
            ResetColor();
            break;

        default:
            if(_docCtrl->GetMode() == EDIT_MODE)
            {
                char temp = _view->GetPressedKey();
                std::string keyPressed = { temp };

                if(_docCtrl->GetNumRows() >= _view->GetRowNumInView()){
                    int temp = _docCtrl->GetPage() + 1;
                    while(temp > _docCtrl->GetDocSize() - 1){
                        _docCtrl->InsertPage();
                    }
                    _docCtrl->AddPage();
                    _docCtrl->AddTotalPage();
                    _view->SetCursorY(0);
                }
                _docCtrl->InsertTextAt(_view->GetCursorY(), _view->GetCursorX(), keyPressed);
                _view->SetCursorX(_view->GetCursorX() + 1);
                _docCtrl->UpdateFile();
                UpdateStatusBar();
                break;
            }

            else if(_docCtrl->GetMode() == SEARCH_MODE)
            {
                char temp = _view->GetPressedKey();
                if(temp == '/'){
                    _docCtrl->SetMode(REPLACE_MODE);
                }
                
                else{
                    std::string keyPressed = { temp };
                    _searchString += keyPressed;
                    _view->ClearStatusRows();
                    _view->AddStatusRow("Search: " + _searchString, "", true);
                }

                break;
            }

            else if(_docCtrl->GetMode() == REPLACE_MODE)
            {
                char temp = _view->GetPressedKey();
                std::string keyPressed = { temp };
                _replaceString += keyPressed;
                _view->ClearStatusRows();
                _view->AddStatusRow("Replace: " + _replaceString, "", true);
                break;
            }
        }

        if(_docCtrl->GetMode() == EDIT_MODE){
            UpdateStatusBar();
        }

        else if(_docCtrl->GetMode() == SEARCH_MODE){
            UpdateSearchStatusBar();
        }

        else if(_docCtrl->GetMode() == REPLACE_MODE){
            UpdateReplaceStatusBar();
        }

        _view->Refresh();
    }

    void SearchDocument(std::string &strIn)
    {
        std::vector<std::vector<std::string>> temp = _docCtrl->GetPages();
        for(int i=0; i<temp.size(); i++){
            for(int j=0; j<temp[i].size(); j++){
                std::string tempStr = temp[i][j];
                int cur = 0;
                while(tempStr.length() >= strIn.length()){
                    int subStr = _docCtrl->IsSubstring(strIn, tempStr);
                    if(subStr != -1){
                        _view->SetColor(j, cur + subStr, cur + subStr + strIn.size()-1, TEXT_COLOR_RED);
                        tempStr = tempStr.substr(subStr + strIn.length(), tempStr.length()-1);
                        cur += strIn.length() + subStr;
                    }
                    else
                        break;
                }
            }
        }
    }

    // **********************************************************
    // Arrow Keys
    void ArrowRightKey()
    {
        if(_view->GetCursorX() < _docCtrl->GetRowSize(_view->GetCursorY()))
                _view->SetCursorX(_view->GetCursorX() + 1);
        
        else if(_docCtrl->GetRowSize(_view->GetCursorY()) == _view->GetCursorX() && _view->GetCursorY() == _docCtrl->GetNumRows() - 1 && _docCtrl->GetPage() < _docCtrl->GetTotalPages()){
            int temp = _docCtrl->GetPage() + 1;
            while(temp > _docCtrl->GetDocSize() - 1){
                _docCtrl->InsertPage();
            }
            _docCtrl->AddPage();
            _view->SetCursorY(0);
        }

        else if(_docCtrl->GetRowSize(_view->GetCursorY()) == _view->GetCursorX() && _view->GetCursorY() < _docCtrl->GetNumRows() - 1){
            _view->SetCursorY(_view->GetCursorY() + 1);
            _view->SetCursorX(0);
        }

        if(_view->GetCursorX() > _docCtrl->GetRowSize(_view->GetCursorY()))
            _view->SetCursorX(_docCtrl->GetRowSize(_view->GetCursorY()));

        _docCtrl->UpdateView();
    }

    void ArrowLeftKey()
    {
        if(_view->GetCursorX() > 0)
            _view->SetCursorX(_view->GetCursorX() - 1);
            
        else if(_view->GetCursorX() == 0 && _view->GetCursorY() == 0 && _docCtrl->GetPage() > 0){
            int temp = _docCtrl->GetPage() - 1;
            while(temp > _docCtrl->GetDocSize() - 1){
                _docCtrl->InsertPage();
            }
            _docCtrl->DeletePage();
            _view->SetCursorY(_docCtrl->GetNumRows() - 1);
        }

        else if(_view->GetCursorX() == 0 && _view->GetCursorY() > 0){
            _view->SetCursorY(_view->GetCursorY() - 1);
            _view->SetCursorX(_docCtrl->GetRowSize(_view->GetCursorY()));
        }

        if(_view->GetCursorX() > _docCtrl->GetRowSize(_view->GetCursorY()))
            _view->SetCursorX(_docCtrl->GetRowSize(_view->GetCursorY()));

        _docCtrl->UpdateView();
    }

    void ArrowUpKey()
    {
        if(_view->GetCursorY() == 0 && _docCtrl->GetPage() > 0){
            int temp = _docCtrl->GetPage() - 1;
            while(temp > _docCtrl->GetDocSize() - 1){
                _docCtrl->InsertPage();
            }
            _docCtrl->DeletePage();
            _view->SetCursorY(_docCtrl->GetNumRows() - 1);
        }

        else if(_view->GetCursorY() > 0)
            _view->SetCursorY(_view->GetCursorY() - 1);
        
        if(_view->GetCursorX() > _docCtrl->GetRowSize(_view->GetCursorY()))
            _view->SetCursorX(_docCtrl->GetRowSize(_view->GetCursorY()));

        _docCtrl->UpdateView();
    }
    void ArrowDownKey()
    {
        if(_view->GetCursorY() == _docCtrl->GetNumRows() - 1 && _docCtrl->GetPage() < _docCtrl->GetTotalPages()){
            int temp = _docCtrl->GetPage() + 1;
            while(temp > _docCtrl->GetDocSize() - 1){
                _docCtrl->InsertPage();
            }
            _docCtrl->AddPage();
            _view->SetCursorY(0);
        }
        else if(_view->GetCursorY() < _docCtrl->GetNumRows() - 1)
            _view->SetCursorY(_view->GetCursorY() + 1);

        if(_view->GetCursorX() > _docCtrl->GetRowSize(_view->GetCursorY()))
            _view->SetCursorX(_docCtrl->GetRowSize(_view->GetCursorY()));

        _docCtrl->UpdateView();
    }

    // **********************************************************
    // Soft function keys
    void BackspaceKey()
    {
        if(_docCtrl->GetMode() == EDIT_MODE){
            if(_view->GetCursorX() == 0 && _view->GetCursorY() > 0){
                int curCursorX = _docCtrl->GetRowSize(_view->GetCursorY() - 1);

                _docCtrl->CombineRows(_view->GetCursorY());
                _docCtrl->ClearRow(_view->GetCursorY());

                _view->SetCursorX(curCursorX);
                _view->SetCursorY(_view->GetCursorY() - 1);
                _docCtrl->UpdateView();
            }
            else if(_view->GetCursorX() > 0){
                _view->SetCursorX(_view->GetCursorX() - 1);
                _docCtrl->Backspace(_view->GetCursorY(), _view->GetCursorX());
            }
        }

        else if(_docCtrl->GetMode() == SEARCH_MODE){
            _searchString = _searchString.substr(0, _searchString.length()-1);
        }

        else if(_docCtrl->GetMode() == REPLACE_MODE){
            _replaceString = _replaceString.substr(0, _replaceString.length()-1);
        }

        _docCtrl->UpdateFile();
    }

    void EnterKey()
    {
        if(_docCtrl->GetMode() == EDIT_MODE){
            if(_docCtrl->GetNumRows() >= _view->GetRowNumInView()){
                int temp = _docCtrl->GetPage() + 1;
                while(temp > _docCtrl->GetDocSize() - 1){
                    _docCtrl->InsertPage();
                }
                _docCtrl->AddPage();
                _docCtrl->AddTotalPage();
                _view->SetCursorY(0);
            }
            _docCtrl->LineBreak(_view->GetCursorY(), _view->GetCursorX());
            _view->SetCursorY(_view->GetCursorY() + 1);
            _view->SetCursorX(0);
        }

        else if(_docCtrl->GetMode() == SEARCH_MODE){
            ResetColor();
            SearchDocument(_searchString);
        }

        else if(_docCtrl->GetMode() == REPLACE_MODE){
            _docCtrl->ReplaceTextAt(_searchString, _replaceString);
            ResetColor();
            _searchString = _replaceString;
            _replaceString = "";
            SearchDocument(_searchString);
            _docCtrl->SetMode(SEARCH_MODE);
        }

        _docCtrl->UpdateFile();
    }
    // **********************************************************
    // Undo/Redo

    void CtrlZKey()
    {
        if(_docCtrl->GetCmdIndex() > -1){
            _docCtrl->Undo();
            if(_view->GetCursorX() > 0)
                _view->SetCursorX(_docCtrl->GetRowSize(_view->GetCursorY()));
            
            else if(_view->GetCursorX() == 0 && _view->GetCursorY() > 0){
                _view->SetCursorY(_view->GetCursorY() - 1);
                _view->SetCursorX(_docCtrl->GetRowSize(_view->GetCursorY()));
            }
        }

        _docCtrl->UpdateFile();
    }

    void CtrlYKey()
    {
        if(_docCtrl->GetCmdIndex() < _docCtrl->GetCmdSize() - 1){
            _docCtrl->Redo();
            if(_view->GetCursorX() < _docCtrl->GetRowSize(_view->GetCursorY()))
                _view->SetCursorX(_docCtrl->GetRowSize(_view->GetCursorY()));
            
            else if(_docCtrl->GetRowSize(_view->GetCursorY()) == _view->GetCursorX() && _view->GetCursorY() < _docCtrl->GetNumRows() - 1){
                _view->SetCursorY(_view->GetCursorY() + 1);
                _view->SetCursorX(0);
            }
        }

        _docCtrl->UpdateFile();
    }
    // **********************************************************
    // Status bar functions
    
    void UpdateStatusBar()
    {
        _view->ClearStatusRows();
        _view->AddStatusRow("Row: " + to_string(_view->GetCursorY()) + " Col: " + to_string(_view->GetCursorX()), "Page: " + to_string(_docCtrl->GetPage()), true);
    }

    void UpdateSearchStatusBar()
    {
        _view->ClearStatusRows();
        _view->AddStatusRow("Search: " + _searchString, "", true);
    }

    void UpdateReplaceStatusBar()
    {
        _view->ClearStatusRows();
        _view->AddStatusRow("Replace: " + _replaceString, "", true);
    }

    void ResetColor()
    {
        for(int i=0; i<_view->GetRowNumInView(); i++){
            for(int j=0; j<_view->GetColNumInView(); j++){
                _view->SetColor(i, j, j, TEXT_COLOR_DEF);
            }
        }
    }

private:
    ECTextViewImp *_view;
    ECTextDocumentCtrl *_docCtrl;
    ECTextDocument *_doc;
    std::string _searchString, _replaceString;
};

#endif