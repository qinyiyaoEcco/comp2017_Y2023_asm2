//
// Created by zhang on 2023/3/21.
//
#include "golbal.h"
#ifndef C_LIST_H
#define C_LIST_H
typedef struct node{
    int start;
    int end;
    unsigned int size;
    unsigned char* address;
    struct node* next;
}Node;
Node* list=NULL;
Node* createList();
Node* list_get_node(Node* head,int start);
void list_free(Node* head);
int list_get_end(Node* head,int start);
unsigned int list_get_value(Node* head,int start,int index,int bitsize);
void list_set_value(Node* head, int start, int index, int bitsize, unsigned int value);
void list_add(Node* head,int start,unsigned int size);
void list_remove(Node* head,int start);

Node* createList(){
    Node* lst=(Node*)calloc(1,sizeof(Node));
    lst->start=-1;
    lst->end=-1;
    lst->size=0;
    lst->address=NULL;
    lst->next=NULL;
    return lst;
}
Node* list_get_node(Node* head,int start){
    if(head==NULL) return NULL;
    Node* tmp=head->next;
    while(tmp!=NULL){
        if(tmp->start<=start&&tmp->end>start)
            return tmp;
        tmp=tmp->next;
    }
    return NULL;
}
int list_get_end(Node* head,int start){
    Node* tmp= list_get_node(head,start);
    if(tmp!=NULL)
        return tmp->end;
    else
        return -1;
}

unsigned int list_get_value(Node* head,int start,int index,int bitsize){
    unsigned int value=0;
    Node* node= list_get_node(head,start);
    if(node==NULL){
        if(TEST_MODE) printf("没有申请该内存块\n");
        end2();
    }
    unsigned int blockindex=(start-node->start)*64 + index;
    if(blockindex + bitsize / 8 > node->size){
        if(TEST_MODE) printf("内存读超出边界\n");
        end2();
    }else{
        switch(bitsize){
            case 32:
                value|=((unsigned int)(node->address[blockindex+2]))<<16;
                value|=((unsigned int)(node->address[blockindex+3]))<<24;
            case 16:
                value|=((unsigned int)(node->address[blockindex + 1])) << 8;
            case 8:
                value|=node->address[blockindex];
                break;
            default:
                value=0;
        }
    }
    return value;
}
void list_set_value(Node* head, int start, int index, int bitsize, unsigned int value){
    Node* node= list_get_node(head,start);
    if(node==NULL){
        if(TEST_MODE) printf("未找到该空间\n");
        end2();
    }
    unsigned int blockindex=(start-node->start)*64 + index;
    if(blockindex + bitsize / 8 > node->size){
        if(TEST_MODE) printf("内存写超出边界\n");
        end2();
    }else{

        switch(bitsize){
            case 32:
                node->address[blockindex+2]=(value>>16)&0xff;
                node->address[blockindex+3]=(value>>24)&0xff;
            case 16:
                node->address[blockindex + 1]= (value >> 8) & 0xff;
            case 8:
                node->address[blockindex]= value & 0xff;
                break;
            default:
                value=0;
        }
    }
}



void list_add(Node* head,int start,unsigned int size){
    Node* lst=(Node*)calloc(1,sizeof(Node));
    lst->start=start;
    lst->size=size;
    lst->end=(size+63)/64;
    lst->next=head->next;
    lst->address= (unsigned char*)calloc(size,sizeof(unsigned char));
    head->next=lst;
}
void list_remove(Node* head,int start){
    Node* tmp=head;
    while(tmp->next!=NULL){
        if(tmp->next->start==start){
            Node* t=tmp->next;
            tmp->next=t->next;
            free(t->address);
            free(t);
            return;
        }
        tmp=tmp->next;
    }
}
void list_free(Node* head){
    Node* tmp=head;

    while(tmp!=NULL){
        Node* t=tmp->next;
        free(tmp->address);
        free(tmp);
        tmp=t;
    }
}

#endif //C_LIST_H
