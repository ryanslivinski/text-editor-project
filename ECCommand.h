#ifndef ECCommand_h
#define ECCommand_h

#include <vector>
#include <string>

// ******************************************************
// Command interface

class ECCommand
{
public:
    virtual ~ECCommand() {}
    virtual void Execute() = 0;
    virtual void UnExecute() = 0;
};

// ******************************************************
// Command history

class ECCommandHistory
{
public:
    ECCommandHistory();
    virtual ~ECCommandHistory();
    void Undo();
    void Redo();
    void ExecuteCmd( ECCommand *pCmd );
    void CheckPtrs();
    void UpdateView();
    int GetIndex() { return idx; }
    int GetCmdListSize() { return cmdList.size(); }
    
private:
    std::vector<ECCommand*> cmdList;
    std::vector<std::pair<int, int> > coords;
    int idx;
};

#endif /* ECCommand_h */
