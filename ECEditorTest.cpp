// Test code for editor
// #include "ECTextViewImp.h" 
// #include "ECKeyInput.h"
#include "ECTextDocument.h"
#include <iostream>

using namespace  std;
  
int myCounter = 0; 
      
int main(int argc, char *argv[])
{
    if(argc < 2){
        cout << "Not enough arguments. Try again." << endl;
        exit(EXIT_FAILURE);
    }
    
    ECTextDocument *doc = new ECTextDocument();

    ifstream myFile(argv[1]);

    // file exists, set doc name, load to editor
    if(myFile.good()){
        doc->SetDocName(argv[1]);
        doc->LoadDocument();
    }

    // does not exist, set doc name and create blank txt file
    else{
        doc->SetDocName(argv[1]);
        doc->CreateDocument(argv[1]);
    }

    doc->UpdateViewDoc();
    doc->GetView().Show();
    
    return 0;
} 
 
  