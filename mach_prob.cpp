#include <iostream>
#include <fstream>
#include <sstream>
#include <ctime>
#include <vector>
using namespace std;

string temp, line;
string pField[7], cField[3], aField[5], mField[4], oField[99]; //pField - field for products; cField - field for cashier acc; aField - field for log activity; mField - member field; oField - order field
char choice1,choice2,choice3;
string aDate[2], aTime[2], auditLn;
string usn, pass;
int t = 0;
int success, mchoice, lmenu; //mga flags para sa back menu system. lmenu to enable log menu
int sale = 0;
char theDate[100];
char theTime[100];
int pdisc = 0;

void getTime(){
    time_t currentTime;
    tm* currTm;
    time(&currentTime);
    currTm = localtime(&currentTime);
    strftime(theDate, 50, "%m/%d/%Y", currTm);
    strftime(theTime, 100, "%X", currTm);
}

void logout(){
    //save log out time
    cout << "\nLogging out...\n";
    getTime();
    aDate[t] = theDate;
    aTime[t] = theTime;
    t++;

    if(t == 2)
        t = 0;

    auditLn += aTime[1] + "^" + aDate[1];

    ofstream auditS("logAct.txt", ios::app);
    auditS << auditLn + "\n";
    auditS.close();

    cout << "Saved log.\n";
}

void create_default_rec(){
    ofstream cashier_acc("cashierAcc.txt");
    cashier_acc << "lawrence^1234^!\n"; //username^password^[!]Active or [?]Inactive
    cashier_acc << "calilap^0520^!\n";
    cashier_acc << "velasco^2004^?\n";
    cashier_acc.close();

    ofstream items("products.txt");
    items << "1001^Safeguard^A^15^20^11^!\n"; //num^name^categ[A]Soap;[B]Shampoo;[C]Toothpaste^price^sprice^quan^stat
    items << "1002^Dove^A^15^20^11^!\n";
    items << "1003^Ivory^A^15^20^11^!\n";

    items << "2001^Pantene^B^15^20^11^!\n";
    items << "2002^Palmolive^B^15^20^11^!\n";

    items << "3001^Colgate^C^15^20^1^!\n";
    items << "3002^Closeup^C^15^20^11^!\n";
    items.close();

    ofstream audit("logAct.txt");
    audit << "05/06/2023^lawrence^08:20:04^08:50:00^05/06/2023\n"; //indate^user^intime^outtime^outdate
    audit << "05/07/2023^admin^10:15:09^12:50:12^05/07/2023\n";
    audit << "05/08/2023^admin^05:30:44^06:34:56^05/08/2023\n";
    audit.close();

    ofstream members("storeMembers.txt");
    members << "100001^Ali Bambang^10^!\n"; //num^name^points^status
    members << "100002^Sitc Ritsit^10^!\n";
    members.close();

    ofstream transacs("transactions.txt");
    transacs << "20004^1001^140^140^1003^100^200^1^10^!^05/11/2023\n"; //transID^id^sprice^sprice*quant^id^sprice^sprice*quant^id^sprice^sprice*quant^disc^sale^date^status
    transacs << "20005^3003^120^240^0^0^!^05/13/2023\n";
    transacs << "20006^3002^110^220^1001^140^280^10^0^!^05/13/2023\n";
    transacs.close();
}

int open_addAct(char ch){
    switch(ch){
        case 'C':{
            string inpuser;
            cout << "\n...Add Cashier...\n";
            cout << "Enter username: ";
            cin >> inpuser;
            cout << "Your default password: " << inpuser << "1234\n";
            ofstream addToList("cashierAcc.txt", ios::app);
            addToList << inpuser + "^" + inpuser + "1234^" + "!\n";
            addToList.close();
            cout << "Success!\n\n[1]Add another cashier\n[0]Return to Add People Menu\nChoice:";
            cin >> mchoice;
            while(mchoice != 1 && mchoice != 0){
                cout << "Invalid choice. Enter choice: ";
                cin >> mchoice;
            }
            break;
        }
        case 'S':{
            string name;
            int aChoice;
            cout << "\n...Store Member...\n";
            cout << "[1]Add new member\n[0]Archive/Restore member\nChoice:";
            cin >> aChoice;
            if(aChoice == 1){
                cout << "-\n-Add Member--\n";
                cout << "Enter name: ";
                cin.ignore();
                getline(cin, name);
                ifstream IDinc("storeMembers.txt");
                while(getline(IDinc, line)){
                    int f = 0;
                    stringstream SS(line);
                    while(getline(SS,temp,'^')){
                        mField[f] = temp;
                        f++;
                    }
                }
                int tempnum = stoi(mField[0]) + 1;
                IDinc.close();

                ofstream addMem("storeMembers.txt", ios::app);
                addMem << to_string(tempnum) + "^" + name + "^10^!\n";
                addMem.close();
            }
            else{
                string mID;
                cout << "\n--Archive/Restore--\n";
                cout << "Enter member ID: ";
                cin >> mID;
                ifstream arch("storeMembers.txt"); //archive/restore
                vector<string> templine;
                while(getline(arch, line)){
                    int f = 0;
                    stringstream SS(line);
                    while(getline(SS,temp,'^')){
                        mField[f] = temp;
                        f++;
                    }
                    if(mID == mField[0])
                        cout << "Member name: " << mField[1] << endl;
                    if(mID == mField[0] && mField[3] == "!"){
                        string repl = mField[0] + "^" + mField[1] + "^" + mField[2] + "^?";
                        templine.push_back(repl);
                        cout << "Successfully archived member.\n";
                    }
                    else if(mID == mField[0] && mField[3] == "?"){
                        string repl = mField[0] + "^" + mField[1] + "^" + mField[2] + "^!";
                        templine.push_back(repl);
                        cout << "Successfully restored member.\n";
                    }
                    else
                        templine.push_back(line);
                }
                arch.close();
                ofstream updMem("storeMembers.txt");
                for(int i = 0; i < templine.size(); i++)
                    updMem << templine.at(i) + "\n";
                updMem.close();
            }
            cout << "\n\n[1]Add/Archive/Restore another member\n[0]Return to People Menu\nChoice:";
            cin >> mchoice;
            while(mchoice != 1 && mchoice != 0){
                cout << "Invalid choice. Enter choice: ";
                cin >> mchoice;
            }
            break;
        }
        case 'B':{
            mchoice = 3;
            break;
        }
        case 'L':{
            mchoice = 2;
            break;
        }
    }
    return mchoice;
}

int open_posAct(char ch){
    switch(ch){
        case 'V':{
            string pNum;
            int fItem = 0; 
            cout << "\n---View Products---\n";
            cout << "[0]All Products\n[1]Available Products Only\n[2]Per Category\n\n[B]ack\n[L]og out\n";
            cout << "Enter Choice: ";
            cin >> choice2;
            choice2 = toupper(choice2);
            if(choice2 == '0'){
                cout << "\n--All Products--\n";
                cout << "[S]ingle Item\n[A]ll Available/Non-available Products\n";
                cout << "Enter Choice: ";
                cin >> choice3;
                choice3 = toupper(choice3);
                string aChoice = "";
                aChoice += choice3;
                //cout << aChoice;
                if(aChoice == "S"){
                    cout << "\n...Single Item...\n";
                    cout << "Enter product ID: ";
                    cin >> pNum;
                    ifstream searchItem("products.txt");
                    while(getline(searchItem, line)){
                        stringstream SS(line);
                        int f = 0;
                        while(getline(SS, temp, '^')){
                            pField[f] = temp;
                            f++;
                        }
                        if(pField[0] == pNum){
                            cout << "ID: " << pField[0] << " | " << pField[1];
                            if(pField[1].length() < 5)
                                cout << "\t\t| ";
                            else
                                cout << "\t| ";

                            if(pField[2] == "A")
                                cout << "Soap\t";
                            else if(pField[2] == "B")
                                cout << "Shampoo";
                            else
                                cout << "Toothpaste";
                            cout << "\t| Price: " << pField[3] << "\t| Selling Price: " << pField[4] << "\t| Stock: ";
                            cout << pField[5] << "\t| Status: ";
                            if(pField[6] == "!") cout << "Active" << endl;
                            else cout << "Archived" << endl;
                            fItem = 1; //found item
                        }
                    }
                    searchItem.close();

                    if(fItem){
                        cout << "\n[1]Search another product\n[0]Return to Cashier Menu\nChoice: ";
                        cin >> mchoice;
                        while(mchoice != 1 && mchoice != 0){
                            cout << "Invalid choice. Enter choice: ";
                            cin >> mchoice;
                        }
                    }
                    else{
                        cout << "Product does not exist.\n";
                        mchoice = 1;
                        open_posAct(choice1);
                    }
                }
                else if(aChoice == "A"){
                    cout << "...All Available/Non-available Products...\n";
                    ifstream searchItem("products.txt");
                    while(getline(searchItem, line)){
                        stringstream SS(line);
                        int f = 0;
                        while(getline(SS, temp, '^')){
                            pField[f] = temp;
                            f++;
                        }
                            cout << "ID: " << pField[0] << " | " << pField[1];
                            if(pField[1].length() < 5)
                                cout << "\t\t| ";
                            else
                                cout << "\t| ";

                            if(pField[2] == "A")
                                cout << "Soap\t";
                            else if(pField[2] == "B")
                                cout << "Shampoo";
                            else
                                cout << "Toothpaste";
                            cout << "\t| Price: " << pField[3] << "\t| Selling Price: " << pField[4] << "\t| Stock: ";
                            cout << pField[5] << "\t| Status: ";
                            if(pField[6] == "!") cout << "Active" << endl;
                            else cout << "Archived" << endl;
                            fItem = 1; //found item
                    }
                    searchItem.close();
                    if(fItem){
                        cout << "\n[1]View Another Product\n[0]Return to Cashier Menu\nChoice: ";
                        cin >> mchoice;
                        while(mchoice != 1 && mchoice != 0){
                            cout << "Invalid choice. Enter choice: ";
                            cin >> mchoice;
                        }
                    }
                    else{
                        cout << "No products yet.\n";
                        mchoice = 1;
                        open_posAct(choice1);
                    }
                }
            }
            else if(choice2 == '1'){
                cout << "\n--Available Products Only--\n";
                cout << "[S]ingle Item\n[A]ll Available Products\n";
                cout << "Enter Choice: ";
                cin >> choice3;
                choice3 = toupper(choice3);
                string aChoice = "";
                aChoice += choice3;
                //cout << aChoice;
                if(aChoice == "S"){
                    cout << "\n...Single Item...\n";
                    cout << "Enter product ID: ";
                    cin >> pNum;
                    ifstream searchItem("products.txt");
                    while(getline(searchItem, line)){
                        stringstream SS(line);
                        int f = 0;
                        while(getline(SS, temp, '^')){
                            pField[f] = temp;
                            f++;
                        }
                        if(pField[0] == pNum && pField[6] == "!"){
                            cout << "ID: " << pField[0] << " | " << pField[1];
                            if(pField[1].length() < 5)
                                cout << "\t\t| ";
                            else
                                cout << "\t| ";

                            if(pField[2] == "A")
                                cout << "Soap\t";
                            else if(pField[2] == "B")
                                cout << "Shampoo";
                            else
                                cout << "Toothpaste";
                            cout << "\t| Price: " << pField[3] << "\t| Selling Price: " << pField[4] << "\t| Stock: ";
                            cout << pField[5] << endl;
                            fItem = 1; //found item
                        }
                        else if(pField[0] == pNum && pField[6] == "?"){
                            cout << "Product inactive." << endl;
                            fItem = 1;
                        }
                    }
                    searchItem.close();

                    if(fItem){
                        cout << "\n[1]Search another product\n[0]Return to Cashier Menu\nChoice: ";
                        cin >> mchoice;
                        while(mchoice != 1 && mchoice != 0){
                            cout << "Invalid choice. Enter choice: ";
                            cin >> mchoice;
                        }
                    }
                    else{
                        cout << "Product does not exist.\n";
                        mchoice = 1;
                        open_posAct(choice1);
                    }
                }
                else if(aChoice == "A"){
                    cout << "...All Available Products...\n";
                    ifstream searchItem("products.txt");
                    while(getline(searchItem, line)){
                        stringstream SS(line);
                        int f = 0;
                        while(getline(SS, temp, '^')){
                            pField[f] = temp;
                            f++;
                        }
                        if(pField[6] == "!"){                            
                            cout << "ID: " << pField[0] << " | " << pField[1];
                            if(pField[1].length() < 5)
                                cout << "\t\t| ";
                            else
                                cout << "\t| ";

                            if(pField[2] == "A")
                                cout << "Soap\t";
                            else if(pField[2] == "B")
                                cout << "Shampoo";
                            else
                                cout << "Toothpaste";
                            cout << "\t| Price: " << pField[3] << "\t| Selling Price: " << pField[4] << "\t| Stock: ";
                            cout << pField[5] << endl;
                            fItem = 1; //found item
                        }
                    }
                    searchItem.close();
                    if(fItem){
                        cout << "\n[1]View Another Product\n[0]Return to Cashier Menu\nChoice: ";
                        cin >> mchoice;
                        while(mchoice != 1 && mchoice != 0){
                            cout << "Invalid choice. Enter choice: ";
                            cin >> mchoice;
                        }
                    }
                    else{
                        cout << "No products yet.\n";
                        mchoice = 1;
                        open_posAct(choice1);
                    }
                }
            }
            else if(choice2 == '2'){
                    string choice;
                    cout << "...Per Category...\n";
                    cout << "\nEnter category\n[A]Soap\n[B]Shampoo\n[C]Toothpaste\n\nChoice: ";
                    cin >> choice;
                    ifstream searchItem("products.txt");
                    while(getline(searchItem, line)){
                        stringstream SS(line);
                        int f = 0;
                        while(getline(SS, temp, '^')){
                            pField[f] = temp;
                            f++;
                        }
                        if(pField[6] == "!" && pField[2] == choice){                            
                            cout << "ID: " << pField[0] << " | " << pField[1];
                            if(pField[1].length() < 5)
                                cout << "\t\t| ";
                            else
                                cout << "\t| ";
                            cout << "\t| Price: " << pField[3] << "\t| Selling Price: " << pField[4] << "\t| Stock: ";
                            cout << pField[5] << endl;
                            fItem = 1; //found item
                        }
                    }
                    searchItem.close();
                    if(fItem){
                        cout << "\n[1]View Another Product\n[0]Return to Cashier Menu\nChoice: ";
                        cin >> mchoice;
                        while(mchoice != 1 && mchoice != 0){
                            cout << "Invalid choice. Enter choice: ";
                            cin >> mchoice;
                        }
                    }
                    else{
                        cout << "No products yet.\n";
                        mchoice = 1;
                        open_posAct(choice1);
                    }
            }
            break;
        }
        case 'T':{
            cout << "\n---Transaction---\n";
            string tLine, tLineRec, mName; //Transaction line record
            int disc = 0;
            int tchoice, tchoice1, tchoice2, add = 1;
            int mPoints = 0;

            cout << "A store member?: \n[1]Yes\n[0]No\nChoice: ";
            cin >> tchoice;
            if(usn == "admin" && tchoice == 0){
                cout << "\nAvail membership? \n[1]Yes\n[0]No\nChoice: ";
                cin >> tchoice1;
                if(tchoice1 == 1){
                    cout << "\n...Add Store Member...\n";
                    cout << "Enter name: ";
                    cin.ignore();
                    getline(cin, mName);
                    ifstream IDinc("storeMembers.txt");
                    while(getline(IDinc, line)){
                        int f = 0;
                        stringstream SS(line);
                        while(getline(SS,temp,'^')){
                            mField[f] = temp;
                            f++;
                        }
                    }
                    int tempnum = stoi(mField[0]) + 1;
                    IDinc.close();

                    ofstream addMem("storeMembers.txt", ios::app);
                    addMem << to_string(tempnum) + "^" + mName + "^10^!\n";
                    addMem.close();
                    disc++;
                }
            }
            if(tchoice==1){
                string mNum;
                int fMem = 0; 
                cout << "Enter member ID: ";
                cin >> mNum;
                ifstream searchMember("storeMembers.txt");
                    while(getline(searchMember, line)){
                        stringstream SS(line);
                        int f = 0;
                        while(getline(SS, temp, '^')){
                            mField[f] = temp;
                            f++;
                        }
                        if(mField[0] == mNum && mField[3] == "!"){                            
                            cout << "ID: " << mField[0] << " | " << mField[1];
                            if(mField[1].length() < 5)
                                cout << "\t\t| ";
                            else
                                cout << "\t| ";
                            cout << "Points: " << mField[2];
                            fMem = 1; //found member
                            mName = mField[1];
                        }
                        else if(mField[0] == mNum && mField[3] == "?"){
                            fMem = 2;
                        }
                    }
                    searchMember.close();
                if(fMem == 0){
                    cout << "No existing member.\n";
                    mchoice = 1;
                    open_posAct(choice1);
                }
                else if(fMem == 1){
                    disc++;
                }
                else if(fMem == 2){
                    cout << "Member inactive." << endl;
                }
            }

            cout << "\n\n[0]View available products\n[1]Add to order\nChoice: ";
            cin >> tchoice2;
            if(tchoice2 == 0){
                cout << "...All Available Products...\n";
                ifstream searchItem("products.txt");
                while(getline(searchItem, line)){
                    stringstream SS(line);
                    int f = 0;
                    while(getline(SS, temp, '^')){
                        pField[f] = temp;
                        f++;
                    }
                    if(pField[6] == "!"){                            
                        cout << "ID: " << pField[0] << " | " << pField[1];
                        if(pField[1].length() < 5)
                                cout << "\t\t| ";
                        else
                            cout << "\t| ";

                        if(pField[2] == "A")
                            cout << "Soap\t";
                        else if(pField[2] == "B")
                            cout << "Shampoo";
                        else
                            cout << "Toothpaste";
                        cout << "\t| Price: " << pField[3] << "\t| Selling Price: " << pField[4] << "\t| Stock: ";
                        cout << pField[5] << endl;
                    }
                }
            }
            else if(tchoice2 == 1){
                string pNum;
                int pQuant, l = 0;
                int fQuant = 0; //found quantity
                cout << "\n...Add to order...\n";
                while(add == 1){
                    vector<string> transLine; //transaction line
                    cout << "Enter product ID: ";
                    cin >> pNum;
                    ifstream searchItem("products.txt");
                    while(getline(searchItem, line)){
                        stringstream SS(line);
                        int f = 0;
                        while(getline(SS, temp, '^')){
                            pField[f] = temp;
                            f++;
                        }
                        if(pField[0] == pNum && pField[6] == "!"){ 
                            cout << "Product name: " << pField[1] << " | Product selling price: " << pField[4] << " | Available Stock: " << pField[5] << endl;
                            cout << "Enter quantity: ";
                            cin >> pQuant;
                            while(pQuant > stoi(pField[5])){
                                cout << "Insufficient Stock. Enter another quantity: ";
                                cin >> pQuant;
                                fQuant = 1;
                            }
                            cout << "Total: " << to_string(stoi(pField[4])*pQuant) << endl;
                            if(fQuant == 0){
                                string repl = pField[0] + "^" + pField[1] + "^" + pField[2] + "^" + pField[3] + "^" + pField[4] + "^" +  to_string(stoi(pField[5]) - pQuant) + "^" +  pField[6];
                                transLine.push_back(repl);
                            }
                            tLine += "^" + pField[0] + "^" + pField[1] + "^" + pField[4] + "^" + to_string(stoi(pField[4])*pQuant);
                            tLineRec += "^" + pField[0] + "^" + pField[4] +;
                            cout << "\n[0]View available products\n[1]Add to order\n[2]Add and Proceed to checkout\nChoice: ";
                            cin >> add;
                            if(add == 0){
                                cout << "...All Available Products...\n";
                                ifstream searchItem("products.txt");
                                while(getline(searchItem, line)){
                                    stringstream SS(line);
                                    int f = 0;
                                    while(getline(SS, temp, '^')){
                                        pField[f] = temp;
                                        f++;
                                    }
                                    if(pField[6] == "!"){                            
                                        cout << "ID: " << pField[0] << " | " << pField[1];
                                        if(pField[1].length() < 5)
                                                cout << "\t\t| ";
                                        else
                                            cout << "\t| ";

                                        if(pField[2] == "A")
                                            cout << "Soap\t";
                                        else if(pField[2] == "B")
                                            cout << "Shampoo";
                                        else
                                            cout << "Toothpaste";
                                        cout << "\t| Price: " << pField[3] << "\t| Selling Price: " << pField[4] << "\t| Stock: ";
                                        cout << pField[5] << endl;
                                    }
                                }
                            }
                        }
                        else if(pField[0] == pNum && pField[6] == "?"){ 
                            cout << "Product inactive.\n";
                            transLine.push_back(line);
                        }
                        else
                            transLine.push_back(line);
                    }
                    searchItem.close();
                    if(add == 1 || add == 2){
                        ofstream reloadItem("products.txt");
                        for(int i = 0; i < transLine.size(); i++){
                            reloadItem << transLine.at(i) + "\n";
                        }
                        reloadItem.close();
                    }
                } 
            }
                if(add = 2){
                    int o = 0;
                    float total = 0;
                    tLine += "^" + to_string(disc);
                    //cout << tLine;
                    int tempID;
                    ifstream IDinc("transactions.txt");
                    while(getline(IDinc,line)){
                        int f = 0;
                        stringstream SS(line);
                        while(getline(SS,temp,'^')){
                            oField[f] = temp;
                            f++;
                        }
                    }
                    tempID = stoi(oField[0])+1;
                    IDinc.close();
                    int cash;
                    cout << "Enter cash: ";
                    cin >> cash;


                    cout << "\n\t     ...Checkout...\n";
                    cout << "______________________________________\n";
                    cout << "\t       OUR STORE\n";
                    cout << "\nTransaction number: " << tempID << endl;
                    cout << "Date: ";
                    getTime();
                    cout << theDate << " " << theTime << endl << endl;
                    cout << "ID   | QTY ITEM\t\tPRICE\tTOTAL\n";

                    float tdisc = 0;
                    stringstream SS(tLine);
                    while(getline(SS,temp,'^')){
                        oField[o] = temp;
                        o++;
                        if(o == 5){
                            o = 1;
                            cout << oField[1] << " | " << stoi(oField[4])/stoi(oField[3]) << "x " << oField[2] << "\t";
                            if(oField[2].size() < 6)
                                cout << "\t";
                            cout << oField[3]  << "\t" << stoi(oField[4]) - stoi(oField[4])*(pdisc*0.01);  
                            cout << endl;
                            total += stoi(oField[4]) - stoi(oField[4])*(pdisc*0.01);
                            tdisc += stoi(oField[4])*(pdisc*0.01);
                            mPoints += stoi(oField[4])/stoi(oField[3]);
                        }
                    }
                    
                 //total, discounted, 12% VAT
                    cout << "--------------------------------------\n";
                    cout << "Total\t\t\t\t" << total << endl;
                    cout << "\nCash\t\t\t\t" << cash;
                    float dTotal;

                    if(disc > 0){
                        dTotal = total - (total*(disc*0.01));
                        float change = total - cash;
                        cout << "\nDiscount\t\t\t-" << total*(disc*0.01) << endl; 
                        cout << "Discounted Total\t\t" << dTotal << endl;
                        cout << "Change\t\t\t\t" << change;
                        cout << "\nVAT\t\t\t\t" << dTotal * 0.12 << endl;
                    }
                    else if(sale){
                        cout << "\nStore Sale Discounted\t\t-" << tdisc << endl;
                        float change = tdisc - cash;
                        cout << "Change\t\t\t\t" << change;
                        cout << "\nVAT\t\t\t\t" << total * 0.12 << endl;                       
                    }
                    else{
                        float change = total - cash;
                        cout << "Change\t\t\t\t" << change;
                        cout << "\nVAT\t\t\t\t" << total * 0.12 << endl;
                    }
                    cout << "______________________________________\n";
                
                    ifstream addPoints("storeMembers.txt"); //transaction ID increment
                    vector<string> templine;
                    while(getline(addPoints, line)){
                        int f = 0;
                        stringstream SS(line);
                        while(getline(SS,temp,'^')){
                            mField[f] = temp;
                            f++;
                        }
                        if(mName == mField[1] && mField[3] == "!"){
                            string repl = mField[0] + "^" + mField[1] + "^" + to_string(stoi(mField[2])+mPoints) + "^!";
                            templine.push_back(repl);
                        }
                        else
                            templine.push_back(line);
                    }
                    addPoints.close();
                    ofstream addMem("storeMembers.txt");
                    for(int i = 0; i < templine.size(); i++)
                        addMem << templine.at(i) + "\n";
                    addMem.close();

                    ofstream tRec("transactions.txt", ios::app);
                    tRec << to_string(tempID) + tLineRec + "^" +to_string(total) + "^" + to_string(int(disc)) + "^" + to_string(pdisc) + "^!^" + theDate + "\n";
                    tRec.close();
                }
            mchoice = 0;
            break;
        }
        case 'R':{
            string tID;

            vector<string> tempL;
            cout << "\n...Void/Refund Transaction...\n";
            cout << "Enter transaction ID: ";
            cin >> tID;
            int tFound = 0;
            ifstream voidTr("transactions.txt");
            while(getline(voidTr,line)){
                vector<string> tempField;
                stringstream SS(line);
                while(getline(SS,temp,'^')){
                    tempField.push_back(temp);
                }
                string repl;
                if(tempField.at(0) == tID && tempField.at(tempField.size()-2) == "!"){
                    for(int i = 0; i < tempField.size(); i++){
                        if(tempField.at(i) == "!"){
                            tempField.at(i) = "?^";
                            repl+=tempField.at(i);
                        }
                        else if(tempField.at(i) == "?"){
                            tempField.at(i) = "!^";
                            repl+=tempField.at(i);
                        }
                        else
                            repl+=tempField.at(i)+"^";
                    }
                    tempL.push_back(repl);

                    for(int j = 1; j < tempField.size()-4; j+=3){
                        vector<string> templine; //dito dineclare para mareset;
                        string pNum = tempField.at(j), rLine;
                        int rQuant = (stoi(tempField.at(j+2))/stoi(tempField.at(j+1))), fItem = 0; 
                        ifstream reloadItem("products.txt");
                        while(getline(reloadItem, line)){
                            stringstream SS(line);
                            int f = 0;
                            while(getline(SS, temp, '^')){
                                pField[f] = temp;
                                f++;
                            }
                            if(pField[0] == pNum){
                                pField[5] = to_string(stoi(pField[5]) + rQuant);
                                fItem = 1;
                                string repl = pField[0] + "^" + pField[1] + "^" + pField[2] + "^" + pField[3] + "^" + pField[4] + "^" +  pField[5] + "^" +  pField[6];
                                templine.push_back(repl);
                            }
                            else{
                                templine.push_back(line);
                            }
                        }
                        reloadItem.close();

                        if(fItem){
                            ofstream reloadItem("products.txt");
                            for(int i = 0; i < templine.size(); i++){
                                reloadItem << templine.at(i) + "\n";
                            }
                            reloadItem.close();
                        }
                    }
                    tFound = 1;
                }
                else{
                    tempL.push_back(line);
                }
            }
            voidTr.close();

            if(tFound){
                ofstream voidT("transactions.txt");
                for(int k = 0; k < tempL.size(); k++){
                    voidT << tempL.at(k) + "\n";
                }
                voidT.close();
                    ifstream IDinc("transactions.txt");
                    while(getline(IDinc,line)){
                        int f = 0;
                        stringstream SS(line);
                        while(getline(SS,temp,'^')){
                            oField[f] = temp;
                            f++;
                        }
                    }
                    IDinc.close();

                cout << "Successfully refunded transaction.\n\n[1]Refund another transaction\n[0]Return to Product Menu\nChoice: ";
                cin >> mchoice;
                while(mchoice != 1 && mchoice != 0){
                    cout << "Invalid choice. Enter choice: ";
                    cin >> mchoice;
                }
            }
            else if(tFound == 0){
                cout << "Transaction ID not found. \n";
                open_posAct(choice2);
            }
            break;
        }
        case 'A':{
            vector<string> templine;
            string opass, npass1, npass2;
            int fAcc = 0;
            cout << "\n---Account Settings---\n";
            cout << "Change password: \n";
            cout << "Enter old password: ";
            cin >> opass;
            
            while(opass != pass){
                cout << "Wrong password\nTry again: ";
                cin >> opass;
            }
                cout << "Enter new password: ";
                cin >> npass1;
                cout << "Confirm new password: ";
                cin >> npass2;
                while(npass1 != npass2){
                    cout << "Password does not match.\n";
                    cout << "Enter new password: ";
                    cin >> npass1;
                    cout << "Confirm new password: ";
                    cin >> npass2;
                }
                cout << "\nPassword changed.\n";
                ifstream changePass("cashierAcc.txt");
                while(getline(changePass, line)){
                    int f = 0;
                    stringstream SS(line);
                    while(getline(SS,temp,'^')){
                        cField[f] = temp;
                        f++;
                    }
                    if(cField[0] == usn){
                        string repl = usn + "^" + npass1 + "^!";
                        templine.push_back(repl);
                        fAcc = 1;
                    }
                    else
                        templine.push_back(line);
                }
                changePass.close();
                if(fAcc){
                    ofstream changeP("cashierAcc.txt");
                    for(int i = 0; i < templine.size(); i++){
                        changeP << templine.at(i) + "\n";
                    }
                    changeP.close();
                }

            break;
        }
        case 'B':{
            mchoice = 3;
            break;
        }
        case 'L':{
            mchoice = 2;
            break;
        }
    }
    return mchoice;
    
}

int show_posMenu(){
    cout << "\n-------------\n";
    cout << "Cashier Tools\n";
    cout << "-------------\n";
    cout << "[V]iew Products\n[T]ransaction\n[R]eturn Product\n";
    if(usn != "admin")
        cout << "[A]ccount Settings\n\n";
    if(usn == "admin"){
        cout << "\n\n[B]ack\n";
    }
    cout << "[L]og out\n";
    cout << "Enter Choice: ";
    cin >> choice1;
    choice1 = toupper(choice1);
    success = open_posAct(choice1);
    while(success == 1){
        success = open_posAct(choice1);
    }
    if(success == 0)
        show_posMenu();
    lmenu = 1;
    return success;
}

int open_prodAct(char ch){ //open product actions
    switch(ch){
        case 'A':{
            int pNum;
            string pName, pPrice, pSPrice, pQuant;
            char pCat;
            cout << "\n...Add Item...\n";
            cout << "Enter category ([A]Soap [B]Shampoo [C]Toothpaste): ";
            cin >> pCat;
            pCat = toupper(pCat);
            while(pCat != 'A' && pCat != 'B' && pCat != 'C'){
                cout << "Invalid Category\n Enter category ([A]Soap [B]Shampoo [C]Toothpaste): ";
                cin >> pCat;
                pCat = toupper(pCat);
            }
            cout << "Enter product name: ";
            cin.ignore();
            getline(cin, pName);
            cout << "Enter price: ";
            cin >> pPrice;
            cout << "Enter selling price: ";
            cin >> pSPrice;
            cout << "Enter product quantity: ";
            cin >> pQuant;

            int tempnum;
            ifstream addItem("products.txt");
            while(getline(addItem, line)){
                stringstream SS(line);
                int f = 0;
                while(getline(SS, temp, '^')){
                    pField[f] = temp;
                    f++;
                }
                if(pCat == 'A'){
                    tempnum = stoi(pField[0]);
                    if(tempnum < 2000 && tempnum > 1000)
                        pNum = tempnum + 1;
                }
                else if(pCat == 'B'){
                    tempnum = stoi(pField[0]);
                    if(tempnum < 3000 && tempnum > 2000)
                        pNum = tempnum + 1;
                }
                else{
                    tempnum = stoi(pField[0]);
                    if(tempnum < 4000 && tempnum > 3000)
                        pNum = tempnum + 1;
                }
            }
            addItem.close();

            ofstream addToList("products.txt", ios::app);
            addToList << to_string(pNum) + "^" + pName + "^" + pCat + "^" + pPrice << "^" + pSPrice + "^" + pQuant + "^!\n";
            addToList.close();
            cout << "Success!\n\n[1]Add another product\n[0]Return to Product Menu\nChoice:";
            cin >> mchoice;
            while(mchoice != 1 && mchoice != 0){
                cout << "Invalid choice. Enter choice: ";
                cin >> mchoice;
            }
            break;
        }
        case 'R':{
            vector<string> templine; //dito dineclare para mareset;
            string pNum, rLine;
            int rQuant, fItem = 0; 
            cout << "\n...Restock Item...\n";
            cout << "Enter product ID: ";
            cin >> pNum;
            ifstream reloadItem("products.txt");
            while(getline(reloadItem, line)){
                stringstream SS(line);
                int f = 0;
                while(getline(SS, temp, '^')){
                    pField[f] = temp;
                    f++;
                }
                if(pField[0] == pNum){
                    cout << "Product name: " << pField[1] << "\nCurrent stock: " << pField[5] << endl;
                    cout << "Enter quantity: ";
                    cin >> rQuant;
                    pField[5] = to_string(stoi(pField[5]) + rQuant);
                    fItem = 1;
                    string repl = pField[0] + "^" + pField[1] + "^" + pField[2] + "^" + pField[3] + "^" + pField[4] + "^" +  pField[5] + "^" +  pField[6];
                    cout << "Successfully restocked.\nNew Stock: " << pField[5];
                    templine.push_back(repl);
                }
                else{
                    templine.push_back(line);
                }
            }
            reloadItem.close();

            if(fItem){
                ofstream reloadItem("products.txt");
                for(int i = 0; i < templine.size(); i++){
                    reloadItem << templine.at(i) + "\n";
                }
                reloadItem.close();
                cout << "\n\n[1]Restock another product\n[0]Return to Product Menu\nChoice: ";
                cin >> mchoice;
                while(mchoice != 1 && mchoice != 0){
                    cout << "Invalid choice. Enter choice: ";
                    cin >> mchoice;
                    //choice3 = 'R';
                }
            }
            else{
                cout << "Product does not exist.\n";
                open_prodAct(choice2);
            }
            break;
        }
        case 'U':{ //KULANG NG KUNG ANO YUNG IUUPDATE. ARCHIVE TOH EH
            vector<string> templine; //dito dineclare para mareset;
            string pNum, rLine;
            int fItem = 0; 
            cout << "\n...Update Item...\n";
            cout << "[P]rice\n[S]elling\n[A]rchive/Restore\n\nChoice: ";
            cin >> choice3;
            choice3 = toupper(choice3);
            cout << "Enter product ID: ";
            cin >> pNum;
            ifstream updateItem("products.txt");
            while(getline(updateItem, line)){
                stringstream SS(line);
                int f = 0;
                while(getline(SS, temp, '^')){
                    pField[f] = temp;
                    f++;
                }
                if(pField[0] == pNum){
                    cout << "Product name: " << pField[1] << endl;
                    if(choice3 == 'P'){
                        cout << "Current Price: " << pField[3] << endl;
                        cout << "Enter new price: ";
                        cin >> pField[3];
                        cout << "Successfully updated price.\n";
                    }
                    else if(choice3 == 'S'){
                        cout << "Current Selling Price: " << pField[4] << endl;
                        cout << "Enter new selling price: ";
                        cin >> pField[4];
                        cout << "Successfully updated selling price.\n";
                    }
                    else if(choice3 == 'A'){
                        if(pField[6] == "!"){
                            pField[6] = "?";
                            cout << "Successfully archived.\n";
                        }
                        else{
                            pField[6] = "!";
                            cout << "Successfully restored.\n";
                        }
                    }
                    string repl = pField[0] + "^" + pField[1] + "^" + pField[2] + "^" + pField[3] + "^" + pField[4] + "^" +  pField[5] + "^" +  pField[6];
                    templine.push_back(repl);
                    fItem = 1; //found item
                }
                else{
                    templine.push_back(line);
                }
            }
            updateItem.close();

            if(fItem){
                ofstream updateItem("products.txt");
                for(int i = 0; i < templine.size(); i++){
                    updateItem << templine.at(i) + "\n";
                }
                updateItem.close();
                cout << "\n[1]Update another product\n[0]Return to Product Menu\nChoice: ";
                cin >> mchoice;
                while(mchoice != 1 && mchoice != 0){
                    cout << "Invalid choice. Enter choice: ";
                    cin >> mchoice;
                }
            }
            else{
                cout << "Product does not exist.\n";
                mchoice = 1;
                open_prodAct(choice2);
            }
            break;
        }
        case 'S':{
            cout << "\n...Store Sale...\n";
            cout << "Default percent disount: " << 10 << "%";
            cout << "\n[0]Enable/Disable\n[1]Change percent discount\n[2]Back\n\nChoice: ";
            cin >> choice3;
            if(choice3 == '0' && sale == 0){
                sale = 1;
                pdisc = 10;
                cout << "\nStore sale enabled.";
            }
            else if(choice3 == '0' && sale == 1){
                sale = 0;
                pdisc = 0;
                cout << "\nStore sale disabled.";
            }
            else{
                cout << "\nEnter new percent discount: ";
                cin >> pdisc;
            }
            cout << "\n[1]Store Sale\n[0]Return to Product Menu\nChoice: ";
            cin >> mchoice;
            while(mchoice != 1 && mchoice != 0){
                cout << "Invalid choice. Enter choice: ";
                cin >> mchoice;
            }
            break;
        }
        case 'B':{
            mchoice = 3;
            break;
        }
        case 'L':{
            mchoice = 2;
            break;
        }
    }
    return mchoice;
}

int open_repAct(char ch){
    switch(ch){
        case 'S':{
            char aChoice;
            string inpDate;
            cout << "\n--Store Sales--\n";
                cout << "Enter date: ";
                cin >> inpDate;
                int fDate = 0;
                int totalsalepday = 0;
                string line1, temp1;
                ifstream trans("transactions.txt");
                cout << endl;
                while(getline(trans, line)){
                    vector<string> transact; // para mareset
                    stringstream SS(line);
                    while(getline(SS, temp, '^')){
                        transact.push_back(temp);
                    }
                    if(transact.back() == inpDate && transact.at(transact.size()-2) == "!"){
                        cout << "Transaction ID: " << transact.front() << " | Sales: ";
                        int i = 3, saleperday = 0;
                        while(i < transact.size()-2){
                            saleperday += stod(transact.at(i));
                            i+=3;
                        }
                        cout << saleperday << endl;
                        totalsalepday += saleperday;
                        fDate = 1; //found item
                    }
                }
                trans.close();
                if(fDate != 1)
                    cout << "No existing log activity in this date.";
                else
                    cout << "\nTotal sale of " << inpDate << " : " << totalsalepday << endl;
                    

                cout << "\n\n[1]View another date\n[0]Return to Report Menu\nChoice: ";
                cin >> mchoice;
                while(mchoice != 1 && mchoice != 0){
                    cout << "Invalid choice. Enter choice: ";
                    cin >> mchoice;
                }                

            break;
        }
        case 'C':{
            int fItem = 0;
            cout << "\n--Critical Items--\n";
            ifstream crit("products.txt");
            while(getline(crit, line)){
                stringstream SS(line);
                int f = 0;
                while(getline(SS, temp, '^')){
                    pField[f] = temp;
                    f++;
                }
                if(stoi(pField[5]) < 10){
                    cout << "ID: " << pField[0] << " | " << pField[1];
                    if(pField[1].length() < 5)
                        cout << "\t\t| ";
                    else
                        cout << "\t| ";

                    if(pField[2] == "A")
                        cout << "Soap\t";
                    else if(pField[2] == "B")
                        cout << "Shampoo";
                    else
                        cout << "Toothpaste";
                    cout << "\t| Stock: " << pField[5] << endl;
                    fItem = 1;
                }
            }

            if(fItem == 0)
                cout << "No products with critical stocks yet.\n";
            cout << "\n[0]Return to Reports Menu\nChoice: ";
            cin >> mchoice;
            while(mchoice != 0){
                cout << "Invalid choice. Enter choice: ";
                cin >> mchoice;
            }
            break;
        }
        case 'A':{
            char aChoice;
            string inpDate, inpDate2;
            cout << "\n--Audit Trail--\n";

                cout << "Enter date: ";
                cin >> inpDate;
                int fDate = 0;

                ifstream audit("logAct.txt");
                while(getline(audit, line)){
                    stringstream SS(line);
                    int f = 0;
                    while(getline(SS, temp, '^')){
                        aField[f] = temp;
                        f++;
                    }
                    if(aField[0] == inpDate){
                        cout << "USER: " << aField[1] << " | LOG IN: " << aField[0] << ", " << aField[2] << " - LOG OUT: " << aField[4] << ", " << aField[3] << endl;
                        fDate = 1; //found item
                    }
                }
                audit.close();

                if(fDate != 1)
                    cout << "No existing log activity in this date.";

                cout << "\n\n[1]View another date\n[0]Return to Report Menu\nChoice: ";
                cin >> mchoice;
                while(mchoice != 1 && mchoice != 0){
                    cout << "Invalid choice. Enter choice: ";
                    cin >> mchoice;
                }                
                cout << "\n\n[1]View another date\n[0]Return to Report Menu\nChoice: ";
                cin >> mchoice;
                while(mchoice != 1 && mchoice != 0){
                    cout << "Invalid choice. Enter choice: ";
                    cin >> mchoice;
                }  
            break;
        }
        case 'M':{
            cout << "\n--Store Members--\n"; 
            ifstream sMembers("storeMembers.txt");
            while(getline(sMembers, line)){
                int f = 0;
                stringstream SS(line);
                while(getline(SS,temp,'^')){
                    mField[f] = temp;
                    f++;
                }
                cout << "Member ID: " << mField[0] << " | Member name: " << mField[1] << " | Points: " << mField[2] << " | Status: ";
                if(mField[3] == "!")
                    cout << "Active.\n";
                else
                    cout << "Inactive.\n";
            }
            sMembers.close();
            break;
        }
        case 'B':{
            mchoice = 3;
            break;
        }
        case 'L':{
            mchoice = 2;
            break;
        }
    return mchoice;
    }
}

int show_adAct(char ch){
    switch(ch){
        case 'P':{
            success = show_posMenu();
            break;
        } 
        case 'I':{
            cout << "\n--Product Inventory--\n";
            cout << "[A]dd Items\n[R]estock Items\n[U]pdate Items\n";
            cout << "[S]tore Sale\n\n[B]ack\n[L]og out\n";
            cout << "Enter Choice: ";
            cin >> choice2;
            choice2 = toupper(choice2);
            success = open_prodAct(choice2);
            while(success == 1){
                success = open_prodAct(choice2);
            }
            if(success == 0)
                show_adAct(choice1);
            break;
        }
        case 'R':{
            cout << "\n--Reports--\n";
            cout << "[S]tore Sales\n[C]ritical Sales\n[A]udit Trail\n[M]embers\n\n[B]ack\n[L]og out\n";
            cout << "Enter Choice: ";
            cin >> choice2;
            choice2 = toupper(choice2);
            success = open_repAct(choice2);
            while(success == 1){
                success = open_repAct(choice2);
            }
            if(success == 0)
                show_adAct(choice1);
            break;
        }
        case 'A':{
            cout << "\n--People--\n";
            cout << "[C]ashier\n[S]tore Members\n\n[B]ack\n[L]og out\n";
            cout << "Enter Choice: ";
            cin >> choice2;
            choice2 = toupper(choice2);
            success = open_addAct(choice2);
            while(success == 1){
                success = open_addAct(choice2);
            }
            if(success == 0)
                show_adAct(choice1);
            if(success == 2)
                lmenu = 1;
            break;
        }
        case 'L':{
            success = 2;
            break;
        }
    }
    return success;
}

int show_adminMenu(){
    if(usn == "admin"){
        cout << "\n-------------------\n";
        cout << "Administrator Tools\n";
        cout << "-------------------\n";
        cout << "[P]oint of Sale\n[I]nventory\n";
        cout << "[R]eports\n[A]dd People\n\n[L]og out\n";
        cout << "Enter Choice: ";
        cin >> choice1;
    }
    choice1 = toupper(choice1);
    int log = show_adAct(choice1);
    if(log == 3) //back sa main menu
        show_adminMenu();
    else if(log == 2){
        return log;
    }
}

void show_log_menu(){
    lmenu = 0;
    string mode;
    cout << "\n=========\n";
    cout << "OUR STORE\n";
    cout << "=========\n";

    cout << "[L]og in\n[E]xit\nChoice: ";
    cin >> mode;
    while(mode != "L" && mode != "E" && mode != "l" && mode != "e"){
        cout << "Invalid choice. Enter another: ";
        cin >> mode;
    }

    if(mode == "L" || mode == "l"){
        cout << "Enter username: ";
        cin >> usn;
        cout << "Enter password: ";
        cin >> pass;

        int fAcc = 0; //found account
        if(usn == "admin" && pass == "123"){
            fAcc = 1;
            getTime();
            aDate[t] = theDate;
            aTime[t] = theTime;
            t++;
            auditLn = aDate[0] + "^admin^" + aTime[0] + "^";
        }
        else{
            ifstream cashierFile("cashierAcc.txt");
            while(getline(cashierFile, line)){
                stringstream SS(line);
                int f = 0;
                while(getline(SS, temp, '^')){
                    cField[f] = temp;
                    f++;
                }
                if(cField[0] == usn && cField[1] == pass && cField[2] == "!"){
                    fAcc = 2;
                    getTime();
                    aDate[t] = theDate;
                    aTime[t] = theTime;
                    t++;
                    auditLn = aDate[0] + "^" + usn + "^" + aTime[0] + "^";
                }
            }
            cashierFile.close();
        }
        if(fAcc == 1 || lmenu == 1){
            success = show_adminMenu();
            if(success == 2){
                logout();
                show_log_menu();
            }
        }
        else if(fAcc == 2){
            choice1 = 'P';
            success = show_adminMenu();
            if(success == 2){
                logout();
                show_log_menu();
            }
        }
        else{
            cout << "Username does not exist or incorrect password\n\n";
            show_log_menu();
        }
    }
    else{
        cout << "Leaving...";
        exit;
    }
}

int main(){
    //create_default_rec();
    show_log_menu();

    return 0;
}