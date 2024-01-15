// name: 
// unikey: 
// SID:
#include "golbal.h"
#include "command.h"


int main(int argc,char* argv[]){
    setbuf(stdout, 0);
    //FILE *fp=fopen("C:\\Users\\zhang\\Desktop\\C2\\mul.mi","rb");
    //FILE *fp=fopen("input/test1.mi","rb");
    FILE *fp=fopen(argv[1],"rb");
    if(fp==NULL)printf("read file faild\n");
    fread(command,sizeof(unsigned char),0x400,fp);
    fread(Date_memory,sizeof(unsigned char),0x400,fp);
    fclose(fp);

    for(;PC<0x400;){
        unsigned int rs1=0,rs2=0,rd=0,fun3=0,fun7=0;
        current_com= data_get_value(command,PC,32);
        int imm=0;
        if(TEST_MODE)
            printf("%2x: 0x%08x\t", PC, current_com);
        if(current_com != 0) {
            R[0] = 0;
            switch (current_com & 0x7f) {
                case 0x33: {
                    decodeR(current_com, &fun7, &rs1, &rs2, &fun3, &rd);
                    if (fun3 == 0 && fun7 == 0) add(R, rd, rs1, rs2);
                    else if (fun3 == 0 && fun7 == 0x20) sub(R, rd, rs1, rs2);
                    else if (fun3 == 4 && fun7 == 0) xor(R, rd, rs1, rs2);
                    else if (fun3 == 6 && fun7 == 0) or(R, rd, rs1, rs2);
                    else if (fun3 == 7 && fun7 == 0) and(R, rd, rs1, rs2);
                    else if (fun3 == 1 && fun7 == 0) sll(R, rd, rs1, rs2);
                    else if (fun3 == 5 && fun7 == 0) srl(R, rd, rs1, rs2);
                    else if (fun3 == 5 && fun7 == 0x20) sra(R, rd, rs1, rs2);
                    else if (fun3 == 2 && fun7 == 0) slt(R, rd, rs1, rs2);
                    else if (fun3 == 3 && fun7 == 0) sltu(R, rd, rs1, rs2);
                    else
                        end1();
                    break;
                }
                case 0x13: {
                    decodeI(current_com, &imm, &rs1, &fun3, &rd);
                    if (fun3 == 0) addi(R, rd, rs1, imm);
                    else if (fun3 == 4) xori(R, rd, rs1, imm);
                    else if (fun3 == 6) ori(R, rd, rs1, imm);
                    else if (fun3 == 7) andi(R, rd, rs1, imm);
                    else if (fun3 == 2) slti(R, rd, rs1, imm);
                    else if (fun3 == 3) sltiu(R, rd, rs1, imm);
                    else
                        end1();
                    break;
                }
                case 0x37: {
                    decodeU(current_com, &imm, &rd);
                    lui(R, rd, imm);
                    break;
                }
                case 0x3: {
                    decodeI(current_com, &imm, &rs1, &fun3, &rd);
                    if (fun3 == 0) lb(R, rd, rs1, imm);
                    else if (fun3 == 1) lh(R, rd, rs1, imm);
                    else if (fun3 == 2) lw(R, rd, rs1, imm);
                    else if (fun3 == 4) lbu(R, rd, rs1, imm);
                    else if (fun3 == 5) lhu(R, rd, rs1, imm);
                    else
                        end1();
                    break;
                }
                case 0x23: {
                    decodeS(current_com, &imm, &rs1, &rs2, &fun3);
                    if (fun3 == 0) sb(R, rs1, rs2, imm);
                    else if (fun3 == 1) sh(R, rs1, rs2, imm);
                    else if (fun3 == 2) sw(R, rs1, rs2, imm);
                    else
                        end1();
                    break;
                }
                case 0x63: {
                    decodeSB(current_com, &imm, &rs1, &rs2, &fun3);
                    if (fun3 == 0) beq(R, &PC, rs1, rs2, imm);
                    else if (fun3 == 1) bne(R, &PC, rs1, rs2, imm);
                    else if (fun3 == 4) blt(R, &PC, rs1, rs2, imm);
                    else if (fun3 == 6) bltu(R, &PC, rs1, rs2, imm);
                    else if (fun3 == 5) bge(R, &PC, rs1, rs2, imm);
                    else if (fun3 == 7) bgeu(R, &PC, rs1, rs2, imm);
                    else
                        end1();
                    break;
                }
                case 0x67: {
                    decodeI(current_com, &imm, &rs1, &fun3, &rd);
                    if (fun3 == 0) jalr(R, &PC, rd, rs1, imm);
                    else
                        end1();
                    break;
                }
                case 0x6f: {
                    decodeUJ(current_com, &imm, &rd);
                    jal(R, &PC, rd, imm);
                    break;

                }
                default:
                    end1();
            }
        }
        PC+=4;
        if(TEST_MODE) showR();

        //printf("0x%08x\n",current_com&0x0000007f);
    }
    list_free(list);
}

void end1(){
    printf("Instruction Not Implemented: 0x%08x\n", current_com);
    showR();
    end();
}
void end2(){
    printf("Illegal Operation: 0x%08x\n", current_com);
    showR();
    end();
}
void end(){
    free(list);
    exit(1);
}
void showR(){
    if(TEST_MODE) printf("---------------\n");
    printf("PC = 0x%08x;\n",PC);
    for(int i=0;i<32;i++){
        if((!TEST_MODE)||R[i]!=0)
            printf("R[%d] = 0x%08x;\n",i,R[i]);
    }
    if(TEST_MODE) printf("---------------\n\n");
}


