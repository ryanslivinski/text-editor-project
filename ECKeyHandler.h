#ifndef ECKeyHandler_h
#define ECKeyHandler_h

#include "ECTextViewImp.h"
#include "ECTextDocument.h"


class KeyHandler{

public:
    virtual ~KeyHandler();
    virtual void Handle(int key) = 0;
};

class ArrowHandler : public KeyHandler{

public:
    ArrowHandler(ECTextViewImp *viewIn, ECTextDocumentCtrl docCtrlIn) : _view(viewIn), _docCtrl(docCtrlIn) {}
    void Handle(int key){
        if(key == ARROW_RIGHT){
            std::vector<std::string> vec = _docCtrl.GetVec();
            if(_view->GetCursorX() < vec[_view->GetCursorY()].size())
                _view->SetCursorX(_view->GetCursorX() + 1);
        }
        else if(key == ARROW_LEFT){
            if(_view->GetCursorX() > 0)
                _view->SetCursorX(_view->GetCursorX() - 1);
        }
        else if(key == ARROW_UP){
            if(_view->GetCursorX() > 0)
                _view->SetCursorY(_view->GetCursorY() - 1);
        }
        else if(key == ARROW_DOWN){
            if(_view->GetCursorX() < row)
                _view->SetCursorY(_view->GetCursorX() + 1);
        }
        else{
            std::vector<std::string> vec = _docCtrl.GetVec();
            char keyPressed = _view->GetPressedKey();
            std::string s = std::string(1, keyPressed);
            listStringsToIns[row] += s;
            // _docCtrl.InsertTextAt(row, listStringsToIns);

            _view->SetCursorX(_view->GetCursorX() + 1);
            _view->InitRows();
            for(int i=0; i<listStringsToIns.size(); i++){
                _view->AddRow(listStringsToIns[i]);
            }
        }
    }
private:
    ECTextDocumentCtrl _docCtrl;
    ECTextViewImp _view;
};

class CharHandler : public KeyHandler{

public:
    CharHandler(ECTextDocumentCtrl docCtrlIn) : _docCtrl(docCtrlIn) {}
    void Handle(int key){
        if(key != ARROW_UP && key != ARROW_DOWN && key != ARROW_LEFT && key != ARROW_RIGHT){
            char keyPressed = _view->GetPressedKey();
            std::string s = std::string(1, keyPressed);
            listStringsToIns[row] += s;
            // _docCtrl.InsertTextAt(row, listStringsToIns);

            _view->SetCursorX(_view->GetCursorX() + 1);
            _view->InitRows();
            for(int i=0; i<listStringsToIns.size(); i++){
                _view->AddRow(listStringsToIns[i]);
            }
        }
    }

private:
    ECTextDocumentCtrl _docCtrl;
};

#endif