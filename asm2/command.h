//
// Created by zhang on 2023-03-19.
//
#include "golbal.h"
#ifndef C_COMMAND_H
#define C_COMMAND_H
#include "memory.h"
void decodeR(unsigned int tmp,unsigned int *func7, unsigned int *rs1, unsigned int *rs2, unsigned int *fun3,unsigned int *rd){
    *func7=(tmp>>25)&0x7f;
    *rs2=(tmp>>20)&0x1f;
    *rs1=(tmp>>15)&0x1f;
    *fun3=(tmp>>12)&0x7;
    *rd=(tmp>>7)&0x1f;
}
void decodeI(unsigned int tmp,int *imm, unsigned int *rs1, unsigned int *fun3, unsigned int *rd){
    *rs1=(tmp>>15)&0x1f;
    *fun3=(tmp>>12)&0x7;
    *rd=(tmp>>7)&0x1f;
    *imm=((int)tmp)>>20;
}
void decodeS(unsigned int tmp,int *imm, unsigned int *rs1, unsigned int *rs2, unsigned int *fun3){
    unsigned int immt=0;
    *rs2=(tmp>>20)&0x1f;
    *rs1=(tmp>>15)&0x1f;
    *fun3=(tmp>>12)&0x7;

    immt=((tmp&0x7e000000)>>25)<<5;
    immt=immt|((tmp>>7)&0x1f);
    immt=immt<<20;
    immt|=tmp&0x80000000;
    *imm=(int)immt;
    *imm=(*imm)>>20;
}
void decodeSB(unsigned int tmp,int *imm, unsigned int *rs1, unsigned int *rs2, unsigned int *fun3){
    unsigned int immt=0;
    *rs2=(tmp>>20)&0x1f;
    *rs1=(tmp>>15)&0x1f;
    *fun3=(tmp>>12)&0x7;
    
    immt=((tmp>>25)&0x3f)<<5;
    immt|=((tmp>>7)&0x1e);
    immt|=((tmp>>7)&1)<<11;
    immt=(tmp&0x80000000)|(immt<<19);
    *imm=((int)immt)>>19;
}
void decodeU(unsigned int tmp,int *imm,unsigned int *rd){
    *imm=(int)(tmp&0xfffff000);
    *rd=(tmp>>7)&0x1f;
}
void decodeUJ(unsigned int tmp,int *imm,unsigned int *rd){
    unsigned immt=0;
    *rd=(tmp>>7)&0x1f;

    immt=tmp&0x000ff000;
    immt|=((tmp>>20)&1)<<11;
    immt|=((tmp>>21)&0x3ff)<<1;

    immt=immt<<11;
    immt=(tmp&0x80000000)|immt;
    *imm=((int)immt)>>11;
}

void add(unsigned int* R,unsigned int rd,unsigned int rs1,unsigned int rs2){
    if(TEST_MODE) printf("add R[%d]=R[%d]+R[%d]\n",rd,rs1,rs2);
    R[rd]=(int)R[rs1]+(int)R[rs2];
}
void addi(unsigned int* R,unsigned int rd,unsigned int rs1,int imm){
    if(TEST_MODE) printf("addi R[%d]=R[%d]%+d\n",rd,rs1,imm);
    R[rd]=(int)R[rs1]+imm;
}
void sub(unsigned int* R,unsigned int rd,unsigned int rs1,unsigned int rs2){
    if(TEST_MODE) printf("sub R[%d]=R[%d]-R[%d]\n",rd,rs1,rs2);
    R[rd]=(int)R[rs1]-(int)R[rs2];
}
void lui(unsigned int* R,unsigned int rd,int imm){
    if(TEST_MODE) printf("lui RH[%d]=0x%x\n",rd,imm&0xfffff000);
    R[rd]=imm&0xfffff000;
}
void xor(unsigned int* R,unsigned int rd,unsigned int rs1,unsigned int rs2){
    if(TEST_MODE) printf("xor R[%d]=R[%d]^R[%d]\n",rd,rs1,rs2);
    R[rd]=R[rs1]^R[rs2];
}
void xori(unsigned int* R,unsigned int rd,unsigned int rs1,int imm){
    if(TEST_MODE) printf("xori R[%d]=R[%d]^%d\n",rd,rs1,imm);
    R[rd]=R[rs1]^imm;
}
void or(unsigned int* R,unsigned int rd,unsigned int rs1,unsigned int rs2){
    if(TEST_MODE) printf("or R[%d]=R[%d]|R[%d]\n",rd,rs1,rs2);
    R[rd]=R[rs1]|R[rs2];
}
void ori(unsigned int* R,unsigned int rd,unsigned int rs1,int imm){
    if(TEST_MODE) printf("ori R[%d]=R[%d]|%d\n",rd,rs1,imm);
    R[rd]=R[rs1]|imm;
}
void and(unsigned int* R,unsigned int rd,unsigned int rs1,unsigned int rs2){
    if(TEST_MODE) printf("and R[%d]=R[%d]&R[%d]\n",rd,rs1,rs2);
    R[rd]=(int)R[rs1]&(int)R[rs2];
}
void andi(unsigned int* R,unsigned int rd,unsigned int rs1,int imm){
    if(TEST_MODE) printf("andi R[%d]=R[%d]&%d\n",rd,rs1,imm);
    R[rd]=(int)R[rs1]&imm;
}

void sll(unsigned int* R,unsigned int rd,unsigned int rs1,unsigned int rs2){
    if(TEST_MODE) printf("sll R[%d]=R[%d]<<R[%d]\n",rd,rs1,rs2);
    R[rd]=R[rs1]<<R[rs2];
}
void srl(unsigned int* R,unsigned int rd,unsigned int rs1,unsigned int rs2){
    if(TEST_MODE) printf("srl R[%d]=R[%d]>>R[%d]\n",rd,rs1,rs2);
    R[rd]=R[rs1]>>R[rs2];
}
void sra(unsigned int* R,unsigned int rd,unsigned int rs1,unsigned int rs2){
    if(TEST_MODE) printf("sra R[%d]=R[%d]>>R[%d]\n",rd,rs1,rs2);
    R[rd]=R[rs1]<<(32-R[rs2]);
    R[rd]=R[rd]|R[rs1]>>R[rs2];
}
void lb(unsigned int* R,unsigned int rd,unsigned int rs1,int imm){
    if(TEST_MODE) printf("lb R[%d]=M[R[%d]+%d]=M[%u]\n",rd,rs1,imm,R[rs1]+imm);
    R[rd]=memory_read_signed(R[rs1]+imm,8);
}
void lh(unsigned int* R,unsigned int rd,unsigned int rs1,int imm){
    if(TEST_MODE) printf("lh R[%d]=M[R[%d]+%d]=M[%u]\n",rd,rs1,imm,R[rs1]+imm);
    R[rd]=memory_read_signed(R[rs1]+imm,16);
}
void lw(unsigned int* R,unsigned int rd,unsigned int rs1,int imm){
    if(TEST_MODE) printf("lw R[%d]=M[R[%d]+%d]=M[%u]\n",rd,rs1,imm,R[rs1]+imm);
    R[rd]=memory_read_signed(R[rs1]+imm,32);
}
void lbu(unsigned int* R,unsigned int rd,unsigned int rs1,int imm){
    if(TEST_MODE) printf("lbu R[%d]=M[R[%d]+%d]=M[%u]\n",rd,rs1,imm,R[rs1]+imm);
    R[rd]=memory_read_unsigned(R[rs1]+imm,8);
}
void lhu(unsigned int* R,unsigned int rd,unsigned int rs1,int imm){
    if(TEST_MODE) printf("lhu R[%d]=M[R[%d]+%d]=M[%u]\n",rd,rs1,imm,R[rs1]+imm);
    R[rd]=memory_read_unsigned(R[rs1]+imm,16);
}
void sb(unsigned int* R,unsigned int rs1,unsigned int rs2,int imm){
    if(TEST_MODE) printf("sb M[R[%d]%+d]=R[%d]\n",rs1,imm,rs2);
    memory_write(R[rs1]+imm,R[rs2],8);
    if(TEST_MODE) if(R[rs1]+imm<0x800||R[rs1]+imm>=0xb700)
        printf("M[%u]=%u\n",R[rs1]+imm,memory_read_unsigned(R[rs1]+imm,8));
}
void sh(unsigned int* R,unsigned int rs1,unsigned int rs2,int imm){
    if(TEST_MODE) printf("sh M[R[%d]+%d]=R[%d]\n",rs1,imm,rs2);
    memory_write(R[rs1]+imm,R[rs2],16);
    if(TEST_MODE) if(R[rs1]+imm<0x800||R[rs1]+imm>=0xb700)
        printf("M[%u]=%u\n",R[rs1]+imm,memory_read_unsigned(R[rs1]+imm,16));
}
void sw(unsigned int* R,unsigned int rs1,unsigned int rs2,int imm){
    if(TEST_MODE) printf("sw M[R[%d]+%d]=R[%d]\n",rs1,imm,rs2);
    memory_write(R[rs1]+imm,R[rs2],32);
    if(TEST_MODE) if(R[rs1]+imm<0x800||R[rs1]+imm>=0xb700)
        printf("M[%u]=%u\n",R[rs1]+imm,memory_read_unsigned(R[rs1]+imm,32));
}
void slt(unsigned int* R,unsigned int rd,unsigned int rs1,unsigned int rs2){
    if(TEST_MODE) printf("slt R[%d]=R[%d]<R[%d]?1:0;\n",rd,rs1,rs2);
    R[rd]=(int)R[rs1]<(int)R[rs2];
}
void slti(unsigned int* R,unsigned int rd,unsigned int rs1,int imm){
    if(TEST_MODE) printf("slti R[%d]=R[%d]<%d?1:0;\n",rd,rs1,imm);
    R[rd]=(int)R[rs1]<(int)imm;
}
void sltu(unsigned int* R,unsigned int rd,unsigned int rs1,unsigned int rs2){
    if(TEST_MODE) printf("sltu R[%d]=R[%d]<R[%d]?1:0;\n",rd,rs1,rs2);
    R[rd]=R[rs1]<R[rs2];
}
void sltiu(unsigned int* R,unsigned int rd,unsigned int rs1,int imm){
    if(TEST_MODE) printf("sltiu R[%d]=R[%d]<%d?1:0;\n",rd,rs1,imm);
    R[rd]=R[rs1]<imm;
}
void beq(unsigned int* R,unsigned int* PC,unsigned int rs1,unsigned int rs2,int imm){
    if(TEST_MODE) printf("beq R[%d]=R[%d]?PC=PC+%d;\n",rs1,rs2,imm);
    if(!(R[rs1]^R[rs2])) {*PC+=imm;*PC-=4;}
}
void bne(unsigned int* R,unsigned int* PC,unsigned int rs1,unsigned int rs2,int imm){
    if(TEST_MODE) printf("bne R[%d]!=R[%d]?PC=PC+%d;\n",rs1,rs2,imm);
    if((R[rs1]^R[rs2])) {*PC+=imm;*PC-=4;}
}
void blt(unsigned int* R,unsigned int* PC,unsigned int rs1,unsigned int rs2,int imm){
    if(TEST_MODE) printf("blt R[%d]<R[%d]?PC=PC+%d:0;\n",rs1,rs2,imm);
    if(((int)R[rs1]<(int)R[rs2])) {*PC+=imm;*PC-=4;}
}
void bltu(unsigned int* R,unsigned int* PC,unsigned int rs1,unsigned int rs2,int imm){
    if(TEST_MODE) printf("bltu R[%d]<R[%d]?PC=PC+%u;\n",rs1,rs2,imm);
    if((R[rs1]<R[rs2])) {*PC+=imm;*PC-=4;}
}
void bge(unsigned int* R,unsigned int* PC,unsigned int rs1,unsigned int rs2,int imm){
    if(TEST_MODE) printf("bge R[%d]>=R[%d]?PC=PC+%d;\n",rs1,rs2,imm);
    if(((int)R[rs1]>=(int)R[rs2])) {*PC+=imm;*PC-=4;}
}
void bgeu(unsigned int* R,unsigned int* PC,unsigned int rs1,unsigned int rs2,int imm){
    if(TEST_MODE) printf("bgeu R[%d]>=R[%d]?PC=PC+%u;\n",rs1,rs2,imm);
    if(R[rs1]>=R[rs2]) {*PC+=imm;*PC-=4;}
}
void jal(unsigned int* R,unsigned int* PC,unsigned int rd,int imm){
    if(TEST_MODE) printf("jal R[%d]=PC+4\tPC=PC%+d;\n",rd,imm);
    R[rd]=*PC+4;
    *PC+=imm;
    *PC-=4;
}
void jalr(unsigned int* R,unsigned int* PC,unsigned int rd,unsigned int rs1,int imm){
    if(TEST_MODE) printf("jalr R[%d]=PC+4\tPC=R[%d]+%d;\n",rd,rs1,imm);
    R[rd]=*PC+4;
    *PC=R[rs1]+imm;
    *PC-=4;
}




#endif //C_COMMAND_H
