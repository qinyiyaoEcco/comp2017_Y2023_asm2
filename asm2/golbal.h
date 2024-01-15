//
// Created by zhang on 2023/3/27.
//

#ifndef C_GOLBAL_H
#define C_GOLBAL_H
#include <stdio.h>
#include <stdlib.h>
#define TEST_MODE 0
unsigned char command[0x400]={0};
unsigned char Date_memory[0x400]={0};
unsigned int R[32]={0};
unsigned int current_com;
unsigned int PC=0;

void end();
void end1();
void end2();
void showR();

#endif //C_GOLBAL_H
