#include "../cpu/reg.h"

#define NUMBER 0
#define ADD 1
#define SUB 2
#define MUL 3
#define DIV 4
#define LB 5
#define RB 6
#define PREFIX 7
#define REG 107

int cal_dec_number(char* expr, int size){
    int result=0;
    for(int i=0;i<size;i++){
        int n=(*expr++-'0');
        result+=result*10+n;
    }
    return result;
}

bool spot_dec_number(char *expr, int* value){
    int state=0;
    int number_size=0;
    while(*expr!='\0'&&('0'<=*expr&&*expr<='9')){
        int ca;
        if(*expr=='0') ca=0;
        else ca=1;
        switch(ca){
            case 0:
                if(state==0){number_size++;if(*(expr+1)<'0'||*(expr+1)>'9'){return 1;} return 0;}
                if(state==1){state=1;number_size++;break;}
                else return 0;
            case 1:
                if(state==0){state=1;number_size++;break;}
                if(state==1){state=1;number_size++;break;}
                else return 0;
            default:
                return 0;
        }
        expr++;
    }
    expr-=(number_size);
    *value=cal_dec_number(expr,number_size);
    return state==1;
}

int cal_hex_number(const char* expr,int size){
    int result=0;
    for(int i=0;i<size;i++){
        int n=(*expr++-'0');
        result+=result*16+n;
    }
    return result;
}

bool spot_hex_number(char *expr, int *value){
    int size=0;
    if(*expr=='0'&&(*(expr+1)=='x'||*(expr+1)=='X')){
        expr+=2;
    }
    else return false;
    while(*expr!='\0'&&((*expr>='0'&&*expr<='9')||(*expr>='A'&&*expr<='F')||(*expr>='a'&&*expr<='f'))){
        size++;
        expr++;
    }
    expr-=(size+2);
    *value=cal_hex_number(expr,size);
    return true;
}




int next(char* expr){
    while(*expr==' ') expr++;

    if(*expr=='e'&&*(expr+1)=='a'&&*(expr+2)=='x') return (expr+=3,REG + R_EAX);
    if(*expr=='e'&&*(expr+1)=='c'&&*(expr+2)=='x') return (expr+=3,REG + R_ECX);
    if(*expr=='e'&&*(expr+1)=='d'&&*(expr+2)=='x') return (expr+=3,REG + R_EDX);
    if(*expr=='e'&&*(expr+1)=='b'&&*(expr+2)=='x') return (expr+=3,REG + R_EBX);
    if(*expr=='e'&&*(expr+1)=='s'&&*(expr+2)=='p') return (expr+=3,REG + R_ESP);
    if(*expr=='e'&&*(expr+1)=='b'&&*(expr+2)=='p') return (expr+=3,REG + R_EBP);
    if(*expr=='e'&&*(expr+1)=='s'&&*(expr+2)=='i') return (expr+=3,REG + R_ESI);
    if(*expr=='e'&&*(expr+1)=='d'&&*(expr+2)=='i') return (expr+=3,REG + R_EDI);

    if(*expr=='a'&&*(expr+1)=='x') return (expr+=2,2*REG + R_AX);
    if(*expr=='c'&&*(expr+1)=='x') return (expr+=2,2*REG + R_CX);
    if(*expr=='d'&&*(expr+1)=='x') return (expr+=2,2*REG + R_DX);
    if(*expr=='b'&&*(expr+1)=='x') return (expr+=2,2*REG + R_BX);
    if(*expr=='s'&&*(expr+1)=='p') return (expr+=2,2*REG + R_SP);
    if(*expr=='b'&&*(expr+1)=='p') return (expr+=2,2*REG + R_BP);
    if(*expr=='s'&&*(expr+1)=='i') return (expr+=2,2*REG + R_SI);
    if(*expr=='d'&&*(expr+1)=='i') return (expr+=2,2*REG + R_DI);

    if(*expr=='a'&&*(expr+1)=='l') return (expr+=2,3*REG + R_AL);
    if(*expr=='c'&&*(expr+1)=='l') return (expr+=2,3*REG + R_CL);
    if(*expr=='d'&&*(expr+1)=='l') return (expr+=2,3*REG + R_DL);
    if(*expr=='b'&&*(expr+1)=='l') return (expr+=2,3*REG + R_BL);
    if(*expr=='a'&&*(expr+1)=='h') return (expr+=2,3*REG + R_AH);
    if(*expr=='c'&&*(expr+1)=='h') return (expr+=2,3*REG + R_BH);
    if(*expr=='d'&&*(expr+1)=='h') return (expr+=2,3*REG + R_DH);
    if(*expr=='b'&&*(expr+1)=='h') return (expr+=2,3*REG + R_BH);

    if(*expr=='+') return (expr+=1, ADD);
    if(*expr=='-') return (expr+=1, SUB);
    if(*expr=='*') return (expr+=1, MUL);
    if(*expr=='/') return (expr+=1, DIV);

    if(*expr=='(') return (expr+=1, LB);
    if(*expr==')') return (expr+=1, RB);
}