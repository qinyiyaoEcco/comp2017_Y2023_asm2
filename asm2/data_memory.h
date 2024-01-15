//
// Created by zhang on 2023-03-26.
//
#include "golbal.h"
#ifndef C_DATA_MEMORY_H
#define C_DATA_MEMORY_H

unsigned int data_get_value(unsigned char*arr,unsigned int index,int bitsize){
    unsigned int value=0;
    switch(bitsize){
        case 32:
            if(index+4>0x400) end1();
            value|=arr[index+3];
            value=value<<8;
            value|=arr[index+2];
            value=value<<8;
        case 16:
            if(index+2>0x400) end1();
            value|=arr[index+1];
            value=value<<8;
        case 8:
            if(index>=0x400) end1();
            value|=arr[index];
            return value;
        default:
            end1();
    }
    return 0;
}
void data_set_value(unsigned char*arr,unsigned int index,unsigned int value,int bitsize){
    switch(bitsize){
        case 32:
            if(index+4>0x400) end1();
            arr[index+3]=value>>24;
            arr[index+2]=(value&0xff0000)>>16;
        case 16:
            if(index+2>0x400) end1();
            arr[index+1]=(value&0xff00)>>8;
        case 8:
            if(index>=0x400) end1();
            arr[index]=value&0xff;
            break;
        default:
            end1();
    }

}

#endif //C_DATA_MEMORY_H
