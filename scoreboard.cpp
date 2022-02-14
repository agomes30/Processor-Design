//  Class: CMSC 411
//  Authors: Alex Gomes and Pranav Papali
//  Project: Scoreboarding Algorithm
//  Filename: scoreboard.cpp

#include "scoreboard.h"

using namespace std;

//vector for storing objects
vector<instruction> myInstance;

//array for mem location/values
int memArray[19] = {45, 12, 0, 0, 10, 135, 254, 127, 18, 4, 55, 8, 2, 98, 13, 5, 233, 158, 167}; //index is the mem location, content is the value
float floatArr[32] = {0}; //floating point register array
int intArr[32] = {0}; //integer register array

//constant for execution length (number of cycles)
const int INT_UNIT = 1;
const int FP_ADDER = 2;
const int FP_MULT = 10;
const int FP_DIV = 40;

int returnAddress(int offset, int address){

    int index = address; 
    index += offset; 
    //circular indexing
    if (index > 18){
        index = index - 19;
    }
    //returns index of memArr, which is recomputed address
    return index;
}

string removal(string s){

    string ns;
    //eliminates first element in a string
    for (int i = 1; i < s.length(); i++){
        ns = ns + s.at(i);
    }
    return ns;
}

int loadValue(int address){
    //returns value from memory
    return memArray[address];
}

void setRegister(string destReg, int dest){

    //sets floating point register indexed by global array
    if (destReg[0] == 'F'){
        destReg = removal(destReg);
        int intDestReg = stoi(destReg);
        floatArr[intDestReg] = ((float)dest); 
       
    }
    //sets integer register indexed by global array
    else if (destReg[0] == '$'){
        destReg = removal(destReg);
        int intDestReg = stoi(destReg);
        intArr[intDestReg] = dest;  
    }
}

void floatOperation(string instr, string dest, string s1, string s2){

    //erasing the first char of the string 
    dest = removal(dest);
    s1 = removal(s1);
    s2 = removal(s2);

    //casting the string to integer
    int intDest = stoi(dest);
    int intS1 = stoi(s1);
    int intS2 = stoi(s2);
    
    //accessing the register value from the array
    float floatDest = 0;      
    float floatS1 = floatArr[intS1]; 
    float floatS2 = floatArr[intS2]; 

    //performs respective arithmetic operations to load destination register with correct value
    if (instr == "ADD.D") {
        floatDest = floatS1 + floatS2;
    }
    else if (instr == "SUB.D") {
        floatDest = floatS1 - floatS2;
    }
    else if (instr == "MUL.D") {
        floatDest = floatS1 * floatS2;
    }
    else if (instr == "DIV.D") {
        floatDest = floatS1 / floatS2;
    }

    floatArr[intDest] = floatDest; 
   
    //updating the instance object
    myInstance.back().setDest(floatDest);
    myInstance.back().setS1(floatS1);
    myInstance.back().setS2(floatS2);
}

void intOperation(string instr, string dest, string s1, string s2){

    int destIndex = 0;
    int intDest = 0;
    int intS1 = 0;
    int intS2 = 0;

    //erasing the first char of the string 
    dest = removal(dest);
    s1 = removal(s1);

    if (s2[0] == '$') {
        s2 = removal(s2);
        intS2 = stoi(s2);
        intS2 = intArr[intS2];
    }
    else {
        intS2 = stoi(s2);
    }

    //casting the string to integer
    destIndex = stoi(dest);
    intDest = 0;
    intS1 = stoi(s1);

    intS1 = intArr[intS1]; 

    //performs respective arithmetic operations to load destination register with correct value
    if ((instr == "ADDI") || (instr == "ADD")) {
        intDest = intS1 + intS2;
    }
    else if ((instr == "SUBI") || (instr == "SUB")) {
        intDest = intS1 - intS2;
    }
   
    intArr[destIndex] = intDest;

    //updating the instance object, stored as float but later printed as int
    myInstance.back().setDest(float(intDest));
    myInstance.back().setS1(float(intS1));
    myInstance.back().setS2(float(intS2));
}

void readFile(string userFile){
    
    string instr, dest, source1, source2, address, offset;
    int off, add, intDestReg;
    float s1, s2, d;
    
    ifstream sampleFile(userFile);

    if (sampleFile.is_open()){

        //while the end of the file is not reached
        while(!sampleFile.eof()){
  
            getline(sampleFile, instr, ' ');

            if ((instr == "L.D") || (instr == "S.D")){

                getline(sampleFile, dest, ' ');
                getline(sampleFile, offset, '(');
                getline(sampleFile, address);

                dest.pop_back();

                int size = address.size();
                if (address[size - 1] == '\n'){   
                    address.pop_back();
                    address.pop_back();
                }
                else {
                    address.pop_back();
                }

                off = stoi(offset);
                add = stoi(address);
                //recalculated address (offset+address)
                add = returnAddress(off, add);

                if (instr == "L.D"){
                    d = loadValue(add); //content of the recalculated address
                    myInstance.push_back(instruction(instr, dest," ", " ", d, add));
                    setRegister(dest, d); //passing the destination reg label and the value
                }
                else if (instr == "S.D"){

                    if (dest[0] == 'F'){
                        dest = removal(dest);
                        intDestReg = stoi(dest);
                        memArray[add] = floatArr[intDestReg]; 
                    
                    }
                    else if (dest[0] == '$'){
                        dest = removal(dest);
                        intDestReg = stoi(dest);
                        memArray[add] = intArr[intDestReg];  
                    }

                    myInstance.push_back(instruction(instr, dest," ", " ", memArray[add], add));                    
                    memArray[add] = myInstance.back().getDest();
                }
            }
            else if ((instr == "ADD.D") || (instr == "SUB.D") || (instr == "MUL.D") || (instr == "DIV.D")){

                getline(sampleFile, dest, ' ');
                getline(sampleFile, source1, ' ');
                getline(sampleFile, source2);
                
                dest.pop_back();
                source1.pop_back();
                cout << "Source2 is _" << source2 << "_" << endl;
                cout << "Source2 length is _" << source2.length() << endl;
                source2.pop_back();
                source2.erase(remove(source2.begin(), source2.end(), ' '), source2.end());
                cout << "new Source2 is _" << source2 << "_" << endl;
                cout << "new length is _" << source2.length() << endl;
                myInstance.push_back(instruction(instr, dest, source1, source2));
                
                floatOperation(instr, dest, source1, source2);
            }
            else if ((instr == "ADD") || (instr == "SUB")){

                getline(sampleFile, dest, ' ');
                getline(sampleFile, source1, ' ');
                getline(sampleFile, source2);
                
                //source2 = removeSpaces(source2);
                dest.pop_back();
                source1.pop_back();
                source2.pop_back();

                myInstance.push_back(instruction(instr, dest, source1, source2));
                
                intOperation(instr, dest, source1, source2);
            } 
            else if ((instr == "ADDI") || (instr == "SUBI")){

                getline(sampleFile, dest, ' ');
                getline(sampleFile, source1, ' ');
                getline(sampleFile, source2);

                //source2 = removeSpaces(source2);
                dest.pop_back();
                source1.pop_back();
                source2.pop_back();

                s2 = stof(source2);

                myInstance.push_back(instruction(instr, dest, source1, " ", 0, 0, 0, s2));

                intOperation(instr, dest, source1, source2);
            }            
        }
    }
    else{
        cout << "File not found!" << endl;
    }
    
    sampleFile.close();
}

string removeSpaces(string str)  
{ 
    //removes all spaces from a given string 
    str.erase(remove(str.begin(), str.end(), ' '), str.end()); 
    return str; 
} 

void issue(int index){
    int wb1 = 0; //write after write
    int wb2 = 0; //unit busy
    int issue3 = 0; //no hazards
    int finalWb = 0; //stores the greatest of the two writebacks
    string currDestReg = myInstance[index].getDestReg();

    if ((myInstance[index].getInstr() == "L.D") || (myInstance[index].getInstr() == "ADDI") ||
        (myInstance[index].getInstr() == "SUBI") || (myInstance[index].getInstr() == "ADD") ||
        (myInstance[index].getInstr() == "SUB")) {

        if (index == 0){
            //updating the issue
            myInstance[index].setIssue(1);
        }
        else {
            for (int i = 0; i < index; i++){

                if ((myInstance[i].getInstr() == "L.D") || (myInstance[i].getInstr() == "S.D") ||
                    (myInstance[i].getInstr() == "ADDI") || (myInstance[i].getInstr() == "SUBI") ||
                    (myInstance[index].getInstr() == "ADD") || (myInstance[index].getInstr() == "SUB")) {
                    
                    if (wb1 < myInstance[i].getWriteBack()){
                        wb1 = myInstance[i].getWriteBack();
                    }
                    if (myInstance[i].getDestReg() == currDestReg){
                        if (wb2 < myInstance[i].getWriteBack()){
                            wb2 = myInstance[i].getWriteBack();
                        }
                    }
                }
                issue3 = myInstance[i].getIssue(); //getting the last immediate instance issue
            }
            int cycleArr[3] = {issue3, wb1, wb2}; //array of cycles 
            //to find the greatest cycle value
            for (int j = 0; j < 3; j++){
                if (cycleArr[j] > finalWb){
                    finalWb = cycleArr[j];
                }
            }
            //updating the issue with the greater of the wb + 1
            myInstance[index].setIssue(finalWb + 1);
        }
    }
    else if (myInstance[index].getInstr() == "S.D"){
        //store,load, int add, int sub
        if (index == 0){
            //updating the issue
            myInstance[index].setIssue(1);
        }
        else {
            for (int i = 0; i < index; i++){

                if ((myInstance[i].getInstr() == "L.D") || (myInstance[i].getInstr() == "S.D") ||
                    (myInstance[i].getInstr() == "ADDI") || (myInstance[i].getInstr() == "SUBI") ||
                    (myInstance[index].getInstr() == "ADD") || (myInstance[index].getInstr() == "SUB")) {

                    if (finalWb < myInstance[i].getWriteBack()){
                        finalWb = myInstance[i].getWriteBack();
                    }
                }
            }
            myInstance[index].setIssue(finalWb + 1);  
        }
    }
    else if ((myInstance[index].getInstr() == "ADD.D") || (myInstance[index].getInstr() == "SUB.D")){
        if (index == 0){
            //updating the issue
            myInstance[index].setIssue(1);
        }
        else {
            for (int i = 0; i < index; i++){
        
                if ((myInstance[i].getInstr() == "ADD.D") || (myInstance[i].getInstr() == "SUB.D")){
                    
                    if (wb1 < myInstance[i].getWriteBack()){
                        wb1 = myInstance[i].getWriteBack();
                    }
                }
                
                if (myInstance[i].getDestReg() == currDestReg){
                    if (wb2 < myInstance[i].getWriteBack()){
                        wb2 = myInstance[i].getWriteBack();                        
                    }
                }
                issue3 = myInstance[i].getIssue(); //getting the last immediate instance issue
            }
            int cycleArr[3] = {issue3, wb1, wb2}; //array of cycles 
            //to find the greatest cycle value
            for (int j = 0; j < 3; j++){
                if (cycleArr[j] > finalWb){
                    finalWb = cycleArr[j];
                }
            }

            myInstance[index].setIssue(finalWb + 1);
        }
    }
    else if (myInstance[index].getInstr() == "MUL.D"){
        if (index == 0){
            //updating the issue
            myInstance[index].setIssue(1);
        }
        else {
            for (int i = 0; i < index; i++){
                if (myInstance[i].getInstr() == "MUL.D"){
                    
                    if (wb1 < myInstance[i].getWriteBack()){
                        wb1 = myInstance[i].getWriteBack();
                    }
                    if (myInstance[i].getDestReg() == currDestReg){
                        if (wb2 < myInstance[i].getWriteBack()){
                            wb2 = myInstance[i].getWriteBack();
                        }
                    }
                }
                issue3 = myInstance[i].getIssue(); //getting the last immediate instance issue
            }
            int cycleArr[3] = {issue3, wb1, wb2}; //array of cycles 
            //to find the greatest cycle value
            for (int j = 0; j < 3; j++){
                if (cycleArr[j] > finalWb){
                    finalWb = cycleArr[j];
                }
            }
            //updating the issue with the greater of the wb + 1
            myInstance[index].setIssue(finalWb + 1);
        }
    }
    else if (myInstance[index].getInstr() == "DIV.D"){
        if (index == 0){
            //updating the issue
            myInstance[index].setIssue(1);
        }
        else {
            for (int i = 0; i < index; i++){
                if (myInstance[i].getInstr() == "DIV.D"){
                    
                    if (wb1 < myInstance[i].getWriteBack()){
                        wb1 = myInstance[i].getWriteBack();
                    }
                    if (myInstance[i].getDestReg() == currDestReg){
                        if (wb2 < myInstance[i].getWriteBack()){
                            wb2 = myInstance[i].getWriteBack();
                        }
                    }
                }
                issue3 = myInstance[i].getIssue(); //getting the last immediate instance issue
            }
            int cycleArr[3] = {issue3, wb1, wb2}; //array of cycles 
            //to find the greatest cycle value
            for (int j = 0; j < 3; j++){
                if (cycleArr[j] > finalWb){
                    finalWb = cycleArr[j];
                }
            }
            //updating the issue with the greater of the wb + 1
            myInstance[index].setIssue(finalWb + 1);
        }
    }
}

void read(int index){
    
    int cycle1 = 0;
    int cycle2 = 0;
    int cycle3 = 0;
    int finalCycle = 0; //the greatest write back value
    int wb = 0;

    string source1 = myInstance[index].getS1();
    string source2 = myInstance[index].getS2();
    source2 = removeSpaces(source2);

    cycle3 = myInstance[index].getIssue(); //current instance issue

    for (int i = 0; i < index; i++){
        wb = myInstance[i].getWriteBack();

        //if there is a match in the registers for current source1 and previous destination
        if (source1 == myInstance[i].getDestReg()){ 
            cycle1 = wb;
        }
        //if there is a match in the registers for current source2 and previous destination
        if (source2 == myInstance[i].getDestReg()){
            cycle2 = wb;
        }
    }
    int cycleArr[3] = {cycle1, cycle2, cycle3}; //array of cycles 
    //to find the greatest cycle value
    for (int i = 0; i < 3; i++){
        if (cycleArr[i] > finalCycle){
            finalCycle = cycleArr[i];
        }
    }
    //cycle for read is 1 + greatest cycle value
    myInstance[index].setRead(finalCycle + 1);
}

void execute(int index){

    int read = 0;
    string stringInstr;
    read = myInstance[index].getRead();
    stringInstr = myInstance[index].getInstr();

    //depending on instruction, the respective increment will be added to the number of cycles
    //Integer Unit
    if ((stringInstr == "L.D") || (stringInstr == "S.D") || (stringInstr == "ADDI") || 
        (stringInstr == "SUBI") || (stringInstr == "ADD") || (stringInstr == "SUB")) {
        myInstance[index].setEx(read + INT_UNIT);
    }
    //Floating Point Adder
    else if ((stringInstr == "ADD.D") || (stringInstr == "SUB.D")){
        myInstance[index].setEx(read + FP_ADDER);
    }
    //Floating Point Multiplier
    else if (stringInstr == "MUL.D"){
        myInstance[index].setEx(read + FP_MULT);
    }
    //Floating Point Divider
    else if (stringInstr == "DIV.D"){
        myInstance[index].setEx(read + FP_DIV);
    }
}

void writeback(int index){
    int cycle1 = 0;
    int cycle2 = 0;
    int cycle3 = 0;
    int finalCycle = 0; //the greatest write back value

    cycle3 = myInstance[index].getEx(); //current instance issue

    for (int i = 0; i < index; i++){
        //if there is a match in the registers for current source1 and previous destination
        if (myInstance[index].getDestReg() == myInstance[i].getS1()){ 
            cycle1 = myInstance[i].getRead();
        }
        //if there is a match in the registers for current source2 and previous destination
        if (myInstance[index].getDestReg() == myInstance[i].getS2()){
            cycle2 = myInstance[i].getRead();
        }
    }

    int cycleArr[3] = {cycle1, cycle2, cycle3}; //array of cycles 
    //to find the greatest cycle value
    for (int i = 0; i < 3; i++){
        if (cycleArr[i] > finalCycle){
            finalCycle = cycleArr[i];
        }
    }
    //cycle for read is 1 + greatest cycle value
    myInstance[index].setWrite(finalCycle + 1);
}

void pipeline(){
    //the loop performs the pipeline for each instruction (tracking what cycle each stage is completed)
    for (int j = 0; j < myInstance.size(); j++){
        issue(j);
        read(j);
        execute(j);
        writeback(j);
    }
}
void printScoreBoard(){

    cout << "__________________________________________________________" << endl;
    cout << "Issue\t\t" << "Read Operand\t" << "Execute\t\t" << "Write Back" << endl;

    //prints the cycles at which the pipeline stages were completed for each instruction
    for (int i = 0; i < myInstance.size(); i++){
        cout << myInstance[i].getIssue() << "\t\t" << myInstance[i].getRead() << "\t\t" << 
        myInstance[i].getEx() << "\t\t" << myInstance[i].getWriteBack() << endl;
    }
    cout << "__________________________________________________________" << endl;
}

void printFloatRegisters() {

    //loops through float register array printing values stored in each index
    for (int i = 0; i < 32; i++) {
        if (i % 4 == 0) {
            cout << endl;
        }

        //formatted so it is easy to see
        if (i < 31)
            cout << "F" << i << ": " << floatArr[i] << ", ";
        else
            cout << "F" << i << ": " << floatArr[i];
    }
    cout << endl;
}

void printIntRegisters() {

    //loops through int register array printing values stored in each index
    for (int i = 0; i < 32; i++) {
        if (i % 4 == 0) {
            cout << endl;
        }
        
        //formatted so it is easy to see
        if (i < 31)
            cout << "$" << i << ": " << intArr[i] << ", ";
        else
            cout << "$" << i << ": " << intArr[i];
    }
    cout << endl;
}

int main(){
    cout << "\nThe scoreboard for chosen text file: " << endl;
    
    //if your text file is named differently, please type the appropriate text file name into the quotations below
    readFile("sampleFile.txt"); // <<< Enter specified text file name here
    pipeline();
    printScoreBoard();

    cout << "\nThe final values stored in the 32 floating point registers:" << endl;
    printFloatRegisters();

    cout << "\nThe final values stored in the 32 integer registers:" << endl;
    printIntRegisters();
    
    return 0;
}