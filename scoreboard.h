//  Class: CMSC 411
//  Authors: Alex Gomes and Pranav Papali
//  Project: Scoreboarding Algorithm
//  Filename: scoreboard.h

#ifndef _SCOREBOARD_H_
#define _SCOREBOARD_H_

#include <iostream> 
#include <string>
#include <fstream>
#include <vector>
#include <cstdbool>
#include <algorithm>

using namespace std;

class instruction{
    private:
        string instr; // label indicating what kind of instruction
        string r1; //string label for source 1 register
        string r2; //string label for source 2 register
        string destReg; //string label for destination register
        int address; //memory address for loads and stores
        float dest; //value in destination register
        float source1; //value in source1 register
        float source2; //value in source2 register

        //pipeline stages
        int issue, read, ex, write;
    public:
        //deafult constructor
        instruction(string i, string destR, string reg1 = " ", string reg2 = " ", float d = 0, int add = 0, float s1 = 0, float s2 = 0){
            instr = i;
            destReg = destR;
            r1 = reg1;
            r2 = reg2;
            dest = d;
            address = add;
            source1 = s1;
            source2 = s2;
            issue = 0; 
            read = 0;
            ex = 0;
            write = 0;
        }

        //setters
        void setDest(float d){
            dest = d;
        }
        void setS1(float s1){
            source1 = s1;
        }
        void setS2(float s2){
            source2 = s2;
        }
        void setIssue(int i){
            issue = i;
        }
         void setRead(int r){
            read = r;
        }
         void setEx(int e){
            ex = e;
        }
         void setWrite(int w){
            write = w;
        }

        //getters
        string getInstr(){
            return instr;
        }
        string getDestReg(){
            return destReg;
        }
        float getDest(){
            return dest;
        }
        int getAddress(){
            return address;
        }
        float getSource1(){
            return source1;
        }
        float getSource2(){
            return source2;
        }
        string getS1(){
            return r1;
        }
        string getS2(){
            return r2;
        }
        int getIssue(){
            return issue;
        }
        int getRead(){
            return read;
        }
        int getEx(){
            return ex;
        }
        int getWriteBack(){
            return write;
        }
};

int returnAddress(int offset, int address); //combines the offset and address
void floatOperation(string instruction); //floating point arithmetic
void intOperation(string instr, string dest, string s1, string s2); //integer arithmetic
void setRegister(string destReg, int dest); //store
int loadValue(int address); //load
void readFile(string userFile); //reading the user file

string removal(string s); //helper function to remove the first character
string removeSpaces(string str); //helper to elimnate spaces in a string

//pipeline related functions
void pipeline();
void issue(int index);
void read(int index);
void execute(int index);
void writeback(int index);

void printScoreBoard();

#endif 