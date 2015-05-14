#include "Assembler.h"


Assembler::Assembler()
{
}


Assembler::~Assembler()
{
}


void Assembler::FirstPass()
{
    ifstream srcfile;
    ofstream astfile;
    ofstream symtabfile;

    srcfile.open("srcfile");
    astfile.open("astfile");
    symtabfile.open("symtabfile");

    lc = 0;

    while (!srcfile.eof())
    {
        // init by 0
        InitString(line, LineSize);
        InitString(label, LabelSize);
        InitString(opcode, OpcodeSize);
        InitString(operand, OperandSize);
        InitString(others, OthersSize);

        // Scan next line of code
        srcfile.getline(line, LineSize);
        cptr = line;
        // label, opcode, operand 추출
        if (*cptr != ' ' && *cptr != '\n'){ // 라벨이 존재하면 라벨을 label에 저장
            cptr = strtok(line, " ");
            strcpy(label, cptr);
            cptr += (int)strlen(label) + 1; // 포인터 cptr이 가리키는 위치를 label 이후로 지정.
        }
        strcpy(others, cptr); // 라벨을 제외한 나머지 문자들을 others에 저장
        DeleteSpace(others, OthersSize); // others의 앞의 공백 제거

        cptr = strtok(others, " ");
        strcpy(opcode, cptr); // OP코드를 opcode에 저장
        cptr += (int)strlen(opcode) + 1; // 포인터 cptr이 가리키는 위치를 opcode 이후로 지정.
        strcpy(others, cptr); // 나머지 문자를 others에 저장
        DeleteSpace(others, OthersSize); // others의 앞의 공백 제거

        cptr = strtok(others, " ");
        cptr = strtok(cptr, "\r"); //comment가 없을 경우.
        if (cptr != NULL)
        {
            strcpy(operand, cptr); // 오퍼랜드를 operand에 저장
            cptr += (int)strlen(operand) + 1; // 포인터 cptr이 가리키는 위치를 operand 이후로 지정.
            strcpy(others, cptr); // 나머지 문자를 others에 저장
            DeleteSpace(others, OthersSize); // others의 앞의 공백 제거
        }

        // Label?
        if (strcmp(label, "") != 0)
        {
            symbolStr[symbolNum] = new char[LabelSize];
            InitString(symbolStr[symbolNum], LabelSize);
            // Store symbol in AST together with LC
            symtabfile << label << "," << lc << endl;
            strcpy(symbolStr[symbolNum], label);
            symbolAddr[symbolNum++] = lc;
        }
        else
        {
            // ORG?
            if (strcmp(opcode, "ORG") == 0)
            {
                // Set LC
                lc = atoi(operand);
                astfile << lc << "  " << "ORG" << " " << lc << endl;
                continue;
            }
            else
            {
                // END?
                if (strcmp(opcode, "END") == 0)
                {
                    // Go to Second Pass
                    astfile << lc << "  " << "END" << endl;
                    return;
                }
            }
        }
        // Generates Output of 1st Pass. Address Symbol Table
        astfile << lc << " " << label << " " << opcode << " " << operand << endl;

        // Increment LC
        lc++;
    }

    srcfile.close();
    astfile.close();
    symtabfile.close();
}


void Assembler::SecondPass()
{
    char* pi[4] = { "ORG", "END", "DEC", "HEX" };
    char* mri[7] = { "AND", "ADD", "LDA", "STA", "BUN", "BSA", "ISZ" };
    char* opStr[18] = { "CLA", "CLE", "CMA", "CME", "CIR", "CIL", "INC", "SPA", "SNA", "SZA", "SZE", "HLT",
        "INP", "OUT", "SKI", "SKO", "ION", "IOF" };
    char* opHex[18] = { "7800", "7400", "7200", "7100", "7080", "7040", "7020", "7010", "7008", "7004", "7002", "7001",
        "F800", "F400", "F200", "F100", "F080", "F040" };

    ifstream astfile;
    ifstream symtabfile;
    ofstream objfile;

    astfile.open("astfile");
    symtabfile.open("symtabfile");
    objfile.open("objfile");

    lc = 0;

    while (!astfile.eof())
    {
        // init by 0
        InitString(addr, AddrSize);
        InitString(line, LineSize);
        InitString(label, LabelSize);
        InitString(opcode, OpcodeSize);
        InitString(operand, OperandSize);
        InitString(others, OthersSize);

        // Scan next line of code
        astfile.getline(line, LineSize);
        cptr = line;
        // lc, label, opcode, operand 추출
        cptr = strtok(line, " ");
        strcpy(addr, cptr); // lc를 addr에 저장
        cptr += (int)strlen(addr) + 1; // 포인터 cptr이 가리키는 위치를 addr 이후로 지정.
        strcpy(others, cptr); // 나머지 문자를 others에 저장
        lc = atoi(addr);

        if (*cptr != ' ' && *cptr != '\n'){ // 라벨이 존재하면 라벨을 label에 저장
            cptr = strtok(others, " ");
            strcpy(label, cptr);
            cptr += (int)strlen(label) + 1; // 포인터 cptr이 가리키는 위치를 label 이후로 지정.
        }
        strcpy(others, cptr); // 라벨을 제외한 나머지 문자들을 others에 저장
        DeleteSpace(others, OthersSize); // others의 앞의 공백 제거

        cptr = strtok(others, " ");
        strcpy(opcode, cptr); // OP코드를 opcode에 저장
        cptr += (int)strlen(opcode) + 1; // 포인터 cptr이 가리키는 위치를 opcode 이후로 지정.
        strcpy(others, cptr); // 나머지 문자를 others에 저장
        DeleteSpace(others, OthersSize); // others의 앞의 공백 제거

        cptr = strtok(others, " ");
        cptr = strtok(cptr, "\r"); //comment가 없을 경우.
        if (cptr != NULL)
        {
            strcpy(operand, cptr); // 오퍼랜드를 operand에 저장
            cptr += (int)strlen(operand) + 1; // 포인터 cptr이 가리키는 위치를 operand 이후로 지정.
            strcpy(others, cptr); // 나머지 문자를 others에 저장
            DeleteSpace(others, OthersSize); // others의 앞의 공백 제거
        }

        // PI?
        int piIdx;
        if ((piIdx = FindStr(opcode, pi, 4)) >= 0)
        {
            // ORG?
            if (strcmp(opcode, "ORG") == 0)
            {
                lc = atoi(operand);
                continue;
            }
            else
            {
                // END?
                if (strcmp(opcode, "END") == 0)
                {
                    break;
                }
                else
                {
                    // Convert operand to binary & store in location given by LC
                    if (strcmp(opcode, "DEC") == 0)
                    {
                        objfile << bitset<16>(atoi(operand)) << endl;
                    }
                    else if (strcmp(opcode, "HEX") == 0)
                    {
                        objfile << bitset<16>((int)strtol(operand, NULL, 16)) << endl;
                    }
                }
            }
        }
        else
        {
            // MRI?
            int mriIdx;
            if ((mriIdx = FindStr(opcode, mri, 7)) >= 0)
            {
                // Get opcode & set bits 2-4
                objfile << bitset<4>(mriIdx);

                // Search address-symbol table for binary equivalent of symbolic address
                // and set bits 5-16
                // symbol table에서 각 자리수를 가져옴
                int symbolIdx = FindStr(operand, symbolStr, symbolNum);
                int symbolCipher[3];
                symbolCipher[2] = symbolAddr[symbolIdx] % 10;
                symbolCipher[1] = symbolAddr[symbolIdx] / 10 % 10;
                symbolCipher[0] = symbolAddr[symbolIdx] / 100 % 10;

                objfile << bitset<4>(symbolCipher[0]) << bitset<4>(symbolCipher[1]) << bitset<4>(symbolCipher[2]) << endl;
            }
            else
            {
                // Valid non-MRI Instrction
                int opIdx;
                if ((opIdx = FindStr(opcode, opStr, 18)) >= 0)
                {
                    // Store binary equivalent of instruction in location given by LC
                    char cipher[2];
                    cipher[1] = '\0';
                    cipher[0] = opHex[opIdx][0];
                    objfile << bitset<4>((int)strtol(cipher, NULL, 16));
                    cipher[0] = opHex[opIdx][1];
                    objfile << bitset<4>((int)strtol(cipher, NULL, 16));
                    cipher[0] = opHex[opIdx][2];
                    objfile << bitset<4>((int)strtol(cipher, NULL, 16));
                    cipher[0] = opHex[opIdx][3];
                    objfile << bitset<4>((int)strtol(cipher, NULL, 16)) << endl;
                }
                else
                {
                    // Error
                    cout << "Invalid operand code" << endl;
                    cout << "Invalid non-MRI Instruction" << endl;
                    break;
                }
            }

        }
        // Increment LC
        lc++;
    }

    astfile.close();
    symtabfile.close();
    objfile.close();
}


void Assembler::InitString(char* pStr, int n)
{
    int i;
    for (i = 0; i<n; i++)
    {
        pStr[i] = 0;
    }
}


void Assembler::DeleteSpace(char* pStr, int n)
{
    int i, j;
    for (i = 0; i<n; i++)
    {
        if (pStr[i] != ' ') // 공백이 아닌 부분을 만나면
        {
            for (j = 0; j<n - i; j++)
            {
                pStr[j] = pStr[i + j]; // 그 부분부터 첫칸으로 땡김
            }
            return;
        }
    }
}


int Assembler::FindStr(char* pStr, char** strArr, int n)
{
    for (int i = 0; i < n; i++)
    {
        if (strcmp(pStr, strArr[i]) == 0)
        {
            return i;
        }
    }
    return -1;
}