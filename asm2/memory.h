//
// Created by zhang on 2023-03-19.
//
#include "golbal.h"

#ifndef C_MEMORY_H
#define C_MEMORY_H
#include "data_memory.h"
#include "List.h"

typedef unsigned long long ull;


unsigned char flag[128]={0};



int memorycheck(int start,int size){
    //-1 free    else buzy
    for(int i=0;i<size;i++){
        if(flag[start+i])
            return start+i;
    }
    return -1;
}
void memoryfree(int start){
    int end=list_get_end(list,start);
    if(end!=-1){
        for(int i=start;i<end;i++) flag[i]=0;
        list_remove(list,start);
    }

}
void memorymloc(unsigned int size){
    unsigned int block_size= (size + 63) / 64;
    for(int i=0;i< 129 - block_size;){
        int j= memorycheck(i, block_size);
        if(j==-1){
            R[28]=0xb700+i*64;
            for(int s=0;s<block_size;s++) flag[i+s]=1;

            if(list==NULL) list=createList();
            list_add(list,i,size);
            return;
        }
        else{
            i=j+1;
        }
    }
    R[28]=0;
}


void maloc(unsigned int value){
    memorymloc(value);
}
void fre(unsigned int value){
    int start=(int)((value-0xb700)>>6);
    memoryfree(start);
}


unsigned int memory_read_unsigned(unsigned int address,int bitsize){
    int value=0;
    if(address<0x400){
        return data_get_value(command,address,bitsize);
    }
    if(address<0x800)
        return data_get_value(Date_memory,(address-0x400),bitsize);

    if(address==0x0812) {if(TEST_MODE) printf("input a char: ");return(getchar());}
    if(address==0x0816) {if(TEST_MODE) printf("input a int: ");scanf("%d",&value);return value;}
    if(address>=0xb700)
        return list_get_value(list,(int)(address-0xb700)/64,(int)(address-0xb700)%64,bitsize);

    if(TEST_MODE) printf("内存读取范围错误： 0x%x",address);
    end1();
    return 0;


}
int memory_read_signed(unsigned int address,int bitsize){
    return (int)memory_read_unsigned(address, bitsize);
}

void memory_write(unsigned int address,unsigned int value,int bitsize){
    if(address==0x80C) {printf("CPU Halt Requested\n");
        exit(0);
    }
    else if(address==0x0800) {if(TEST_MODE) printf("output: ");printf("%c",value);}
    else if(address==0x0804) {if(TEST_MODE) printf("output: ");printf("%d",(int)value);}
    else if(address==0x0808) {if(TEST_MODE) printf("output: ");printf("%x",value);}
    else if(address==0x0824) showR();
    else if(address==0x0820) {if(TEST_MODE) printf("output: ");printf("0x%08x",PC);}
    else if(address==0x0828) {if(TEST_MODE) printf("output: ");printf("0x%08x",memory_read_unsigned(address,32));}
    else if(address==0x0830) {maloc(value);if(TEST_MODE) printf("尝试申了%u大小的空间\n",value);}
    else if(address==0x0834) {fre(value);if(TEST_MODE) printf("尝试释放空间\n");}
    else if(address>=0x400&&address<0x800){
        data_set_value(Date_memory,(address-0x400),value,bitsize);
    }
    else if(address>=0xb700) {
        list_set_value(list, (int) (address - 0xb700) / 64, (int) (address - 0xb700) % 64, bitsize, value);
        if(TEST_MODE) printf("存入address:%u\n",list_get_value(list, (int) (address - 0xb700) / 64, (int) (address - 0xb700) % 64, bitsize));
    }
    else{
        end1();
    }
}





#endif //C_MEMORY_H
