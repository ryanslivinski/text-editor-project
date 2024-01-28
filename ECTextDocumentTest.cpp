//
//  ECTextDocumentTest.cpp
//  
//
//  Created by Yufeng Wu on 2/26/20.
//
//

#include <iostream>
#include "ECTextDocument.h"

using namespace  std;

// static void CompTestB(){
//     ECTextDocument doc;
//     ECTextDocumentCtrl &docCtrl = doc.GetCtrl();
//     vector<char> listText1;

//     listText1.push_back('a');
//     listText1.push_back('b');
//     listText1.push_back('c');
//     listText1.push_back('d');
//     listText1.push_back('e');
//     cout << "Here" << endl;
//     docCtrl.InsertTextAt(0, listText1);
//     vector<char> listText2;
//     listText2.push_back('f');
//     listText2.push_back('g');
//     listText2.push_back('h');
//     int posIns = 2;
//     docCtrl.InsertTextAt(posIns, listText2);
    
//     // cout << "ORIGINAL" << endl;
//     // for(int i=0; i<doc.GetDocLen(); i++){
//     //     cout << doc.GetCharAt(i) << endl;
//     // }
    
//     // cout << endl;
//     // cout << endl;

//     int posDel = 1;
//     int lenDel = 1;
//     docCtrl.RemoveTextAt( posDel, lenDel );
//     docCtrl.CapTextAt(2, 3);
//     docCtrl.LowerTextAt(3,1);

//     // cout << "Removing at 1 w/ length 1" << endl;
//     // cout << "Upper at 2 w/ length 3" << endl;
//     // cout << "Lower at 3 w/ length 1" << endl;
//     // for(int i=0; i<doc.GetDocLen(); i++){
//     //     cout << doc.GetCharAt(i) << endl;
//     // }
    
//     // cout << endl;
//     // cout << endl;
    
//     cout << "Undo lower" << endl;
//     cout << "Undo cap" << endl;
//     docCtrl.Undo();
//     docCtrl.Undo();
//     cout << doc.GetDocLen() << endl; // 7
//     cout << doc.GetCharAt(1) << endl;  // f
//     cout << doc.GetCharAt(2) << endl;  // g
//     cout << doc.GetCharAt(3) << endl;  // h
//     cout << doc.GetCharAt(4) << endl;  // c
//     // for(int i=0; i<doc.GetDocLen(); i++){
//     //     cout << doc.GetCharAt(i) << endl;
//     // }

//     cout << endl;
//     cout << endl;

//     cout << "Redo cap" << endl;
//     docCtrl.Redo();
//     cout << doc.GetDocLen() << endl; // 7
//     cout << doc.GetCharAt(2) << endl;  // G  
//     cout << doc.GetCharAt(3) << endl;  // H
//     // for(int i=0; i<doc.GetDocLen(); i++){
//     //     cout << doc.GetCharAt(i) << endl;
//     // }
    
//     cout << endl;
//     cout << endl;

//     cout << "Redo lower" << endl;
//     docCtrl.Redo();  
//     cout << doc.GetDocLen() << endl; // 7
//     cout << doc.GetCharAt(1) << endl;  // f
//     cout << doc.GetCharAt(2) << endl;  // G
//     cout << doc.GetCharAt(3) << endl;  // h
//     // for(int i=0; i<doc.GetDocLen(); i++){
//     //     cout << doc.GetCharAt(i) << endl;
//     // }    

// }

// static void UndoTest(){

//     ECTextDocument doc;
//     ECTextDocumentCtrl &docCtrl = doc.GetCtrl();
//     vector<char> listText1;

//     listText1.push_back('a');
//     listText1.push_back('b');
//     listText1.push_back('c');
//     listText1.push_back('d');
//     listText1.push_back('e');
//     docCtrl.InsertTextAt(0, listText1);
//     // docCtrl.printStatus();
//     vector<char> listText2;
//     listText2.push_back('f');
//     listText2.push_back('g');
//     listText2.push_back('h');
//     int posIns = 2;
//     docCtrl.InsertTextAt(posIns, listText2);
//     // docCtrl.printStatus();
    

//     cout << doc.GetDocLen() << endl;    // 8
//     cout << doc.GetCharAt(0) << endl;   // a 
//     cout << doc.GetCharAt(1) << endl;   // b
//     cout << doc.GetCharAt(2) << endl;   // f
//     cout << doc.GetCharAt(3) << endl;   // g
//     cout << doc.GetCharAt(4) << endl;   // h
//     cout << doc.GetCharAt(5) << endl;   // c
//     cout << doc.GetCharAt(6) << endl;   // d
//     cout << doc.GetCharAt(7) << endl;   // e
    
//     cout << endl;
//     cout << endl;

//     cout << "Undo" << endl;
//     docCtrl.Undo();
//     cout << doc.GetDocLen() << endl;    // 5
//     cout << doc.GetCharAt(0) << endl;   // a 
//     cout << doc.GetCharAt(1) << endl;   // b
//     cout << doc.GetCharAt(2) << endl;   // c
//     cout << doc.GetCharAt(3) << endl;   // d
//     cout << doc.GetCharAt(4) << endl;   // e
    
//     cout << endl;
//     cout << endl;

//     cout << "Redo" << endl;
//     docCtrl.Redo();
//     cout << doc.GetDocLen() << endl;    // 8
//     cout << doc.GetCharAt(0) << endl;   // a 
//     cout << doc.GetCharAt(1) << endl;   // b
//     cout << doc.GetCharAt(2) << endl;   // f
//     cout << doc.GetCharAt(3) << endl;   // g
//     cout << doc.GetCharAt(4) << endl;   // h
//     cout << doc.GetCharAt(5) << endl;   // c
//     cout << doc.GetCharAt(6) << endl;   // d
//     cout << doc.GetCharAt(7) << endl;   // e

//     cout << endl;
//     cout << endl;

//     cout << "Undo x2" << endl;
//     docCtrl.Undo();
//     docCtrl.Undo();
//     cout << doc.GetDocLen() << endl;    // 0

//     cout << endl;
//     cout << endl;

//     cout << "Redo" << endl;
//     docCtrl.Redo();
//     cout << doc.GetDocLen() << endl;    // 5
//     cout << doc.GetCharAt(0) << endl;   // a 
//     cout << doc.GetCharAt(1) << endl;   // b
//     cout << doc.GetCharAt(2) << endl;   // c
//     cout << doc.GetCharAt(3) << endl;   // d
//     cout << doc.GetCharAt(4) << endl;   // e

//     cout << endl;
//     cout << endl;

//     cout << "Redo" << endl;
//     docCtrl.Redo();
//     cout << doc.GetDocLen() << endl;    // 8
//     cout << doc.GetCharAt(0) << endl;   // a 
//     cout << doc.GetCharAt(1) << endl;   // b
//     cout << doc.GetCharAt(2) << endl;   // f
//     cout << doc.GetCharAt(3) << endl;   // g
//     cout << doc.GetCharAt(4) << endl;   // h
//     cout << doc.GetCharAt(5) << endl;   // c
//     cout << doc.GetCharAt(6) << endl;   // d
//     cout << doc.GetCharAt(7) << endl;   // e

// }

void StringTest(){
    // ECTextDocument doc;
    // ECTextDocumentCtrl &docCtrl = doc.GetCtrl();
    // vector<char> listText1;

    // int row = 0;

    // listText1.push_back('r');
    // listText1.push_back('y');
    // listText1.push_back('a');
    // listText1.push_back('n');
    // docCtrl.InsertTextAt(0, 0, listText1);

    // cout << docCtrl.GetLine(row) << endl; // ryan

    // docCtrl.RemoveTextAt(row, 1, 2);

    // cout << docCtrl.GetLine(row) << endl; // rn

    // docCtrl.Undo();

    // cout << docCtrl.GetLine(row) << endl; // ryan

    
}

int main()
{
    // test code
    // CompTestB();
    // UndoTest();
    // StringTest();
    
    return 0;
}

