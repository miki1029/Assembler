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
    // string�� 0���� �ʱ�ȭ
    void InitString(char* pStr, int n);
    // string�� �� ������ ����
    void DeleteSpace(char* pStr, int n);
    // strArr���� pStr�� ã�� �ε����� ��ȯ
    int FindStr(char* pStr, char** strArr, int n);
};

