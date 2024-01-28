#include "ECCommand.h"
using namespace std;
#include <iostream>

// ******************************************************
// Command history

ECCommandHistory :: ECCommandHistory() 
{
  idx = -1;
}

ECCommandHistory :: ~ECCommandHistory()
{
  for(int i=0; i<cmdList.size(); i++){
    delete cmdList[i];
  }
  cmdList.clear();
}

void ECCommandHistory :: CheckPtrs()
{
  if(idx >= 0 && (idx < cmdList.size())){
    for(int i=idx+1; i<cmdList.size(); i++){
      delete cmdList[i];
    }
    cmdList.erase(cmdList.begin()+(idx+1), cmdList.end());
  }
}

void ECCommandHistory :: Undo()
{
  if(idx > -1){
    cmdList[idx]->UnExecute();
    idx--;
  }
}

void ECCommandHistory :: Redo()
{
  int n = cmdList.size();
  if(idx < n-1){
    idx++;
    cmdList[idx]->Execute();
  }
}

void ECCommandHistory :: ExecuteCmd(ECCommand *pCmd)
{
  CheckPtrs();
  pCmd->Execute();
  cmdList.push_back(pCmd);
  idx++;
}