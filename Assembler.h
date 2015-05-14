#pragma once
#include <iostream>
#include <fstream>
#include <string>
#include <bitset>
using namespace std;

class Assembler
{
private:
    static const int LineSize = 80;
    static const int LabelSize = 9;
    static const int OpcodeSize = 7;
    static const int OperandSize = 19;
    static const int OthersSize = 80;
    static const int AddrSize = 5;

    char line[LineSize];
    char addr[AddrSize];
    char label[LabelSize];
    char opcode[OpcodeSize];
    char operand[OperandSize];
    char others[OthersSize];

    char* cptr = NULL;
    int lc = 0;

    char* symbolStr[100];
    int symbolAddr[100];
    int symbolNum = 0;
    
public:
    Assembler();
    ~Assembler();

    void FirstPass();
    void SecondPass();

private:
    // string을 0으로 초기화
    void InitString(char* pStr, int n);
    // string의 앞 공백을 제거
    void DeleteSpace(char* pStr, int n);
    // strArr에서 pStr을 찾아 인덱스를 반환
    int FindStr(char* pStr, char** strArr, int n);
};

