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

int cal_dec_number(const char* expr, int size){
    int result=0;
    for(int i=0;i<size;i++){
        int n=(*expr++-'0');
        result+=result*10+n;
    }
    return result;
}

bool spot_dec_number(const char *expr, int* value, int *number_size){
    int state=0;
    int size=0;
    while(*expr!='\0'&&('0'<=*expr&&*expr<='9')){
        int ca;
        if(*expr=='0') ca=0;
        else ca=1;
        switch(ca){
            case 0:
                if(state==0){size++;if(*(expr+1)<'0'||*(expr+1)>'9'){return 1;} return 0;}
                if(state==1){state=1;size++;break;}
                else return 0;
            case 1:
                if(state==0){state=1;size++;break;}
                if(state==1){state=1;size++;break;}
                else return 0;
            default:
                return 0;
        }
        expr++;
    }
    expr-=(size);
    *value=cal_dec_number(expr,size);
    *number_size=size;
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

bool spot_hex_number(const char *expr, int *value, int *number_size){
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
    *number_size=size+2;
    return true;
}




int next(const char* expr,int* size){
    *size=0;
    while(*expr==' ') {(*size)++; expr++;}

    *size+=3;

    if(*expr=='e'&&*(expr+1)=='i'&&*(expr+2)=='p') return REG + R_EIP;

    if(*expr=='e'&&*(expr+1)=='a'&&*(expr+2)=='x') return REG + R_EAX;
    if(*expr=='e'&&*(expr+1)=='c'&&*(expr+2)=='x') return REG + R_ECX;
    if(*expr=='e'&&*(expr+1)=='d'&&*(expr+2)=='x') return REG + R_EDX;
    if(*expr=='e'&&*(expr+1)=='b'&&*(expr+2)=='x') return REG + R_EBX;
    if(*expr=='e'&&*(expr+1)=='s'&&*(expr+2)=='p') return REG + R_ESP;
    if(*expr=='e'&&*(expr+1)=='b'&&*(expr+2)=='p') return REG + R_EBP;
    if(*expr=='e'&&*(expr+1)=='s'&&*(expr+2)=='i') return REG + R_ESI;
    if(*expr=='e'&&*(expr+1)=='d'&&*(expr+2)=='i') return REG + R_EDI;

    *size-=3;

    *size+=2;

    if(*expr=='a'&&*(expr+1)=='x') return 2*REG + R_AX;
    if(*expr=='c'&&*(expr+1)=='x') return 2*REG + R_CX;
    if(*expr=='d'&&*(expr+1)=='x') return 2*REG + R_DX;
    if(*expr=='b'&&*(expr+1)=='x') return 2*REG + R_BX;
    if(*expr=='s'&&*(expr+1)=='p') return 2*REG + R_SP;
    if(*expr=='b'&&*(expr+1)=='p') return 2*REG + R_BP;
    if(*expr=='s'&&*(expr+1)=='i') return 2*REG + R_SI;
    if(*expr=='d'&&*(expr+1)=='i') return 2*REG + R_DI;

    if(*expr=='a'&&*(expr+1)=='l') return 3*REG + R_AL;
    if(*expr=='c'&&*(expr+1)=='l') return 3*REG + R_CL;
    if(*expr=='d'&&*(expr+1)=='l') return 3*REG + R_DL;
    if(*expr=='b'&&*(expr+1)=='l') return 3*REG + R_BL;
    if(*expr=='a'&&*(expr+1)=='h') return 3*REG + R_AH;
    if(*expr=='c'&&*(expr+1)=='h') return 3*REG + R_BH;
    if(*expr=='d'&&*(expr+1)=='h') return 3*REG + R_DH;
    if(*expr=='b'&&*(expr+1)=='h') return 3*REG + R_BH;

    *size-=2;

    *size+=1;

    if(*expr=='+') return ADD;
    if(*expr=='-') return SUB;
    if(*expr=='*') return MUL;
    if(*expr=='/') return DIV;

    if(*expr=='(') return LB;
    if(*expr==')') return RB;

    *size-=1;

    int value=0;
    if(spot_hex_number(expr,&value,size)) return value+1000;
    if(spot_dec_number(expr,&value,size)) return value+1000;

    *size=0;

    return -1;
}

bool add_expr(const char** expr,int* val);
bool mul_expr(const char** expr,int* val);
bool add_expr1(const char** expr,int* val, int *op_type);
bool mul_expr1(const char** expr,int* val, int *op_type);

bool real_expr(const char** expr,int* val){
    int size=0;
    int re;
    if(next(*expr, &size)==LB){
        *expr+=1;
        add_expr(expr,val);
        *expr+=1;
    } else if((re=next(*expr,&size))>1000){
        *val=re-1000;
        *expr+=size;
    } else if((re=next(*expr, &size))>=REG){
        if(re>3*REG){
            printf("%d\n",re-3*REG);
            // *val=reg_b((re-3*REG));
        } else if(re>2*REG){
            *val=reg_w((re-2*REG));
        } else {
            if(re-REG==R_EIP) *val=cpu.eip;
            else *val=reg_l((re-1*REG));
        }
        *expr+=size;
    } else{
        return false;
    }
    return true;
}

bool mul_expr1(const char** expr,int* val, int *op_type){
    int size;
    int type;
    if((type=next(*expr, &size))==MUL||(type=next(*expr, &size))==DIV){
        *expr+=size;
        int v1,v2,op=-1;
        real_expr(expr, &v1);
        mul_expr1(expr, &v2, &op);
        if(op==-1) *val=v1;
        else if(op==MUL) *val=v1*v2;
        else *val=v1/v2;
        *op_type=type;
    }
    return true;
}

bool mul_expr(const char** expr,int* val){
    int v1,v2;
    int op=-1;
   if(real_expr(expr, &v1)&&mul_expr1(expr, &v2, &op)){
       if(op==-1) *val=v1;
       else if(op==MUL) *val=v1*v2;
       else *val=v1/v2;
       return true;
   }
   return false;
}

bool add_expr1(const char** expr,int* val, int *op_type){
    int size;
    int type;
    if((type=next(*expr, &size))==ADD||(type=next(*expr, &size))==SUB){
        *expr+=size;
        int v1,v2,op=-1;
        mul_expr(expr, &v1);
        add_expr1(expr, &v2, &op);
        if(op==-1) *val=v1;
        else if(op==ADD) *val=v1+v2;
        else *val=v1-v2;
        *op_type=type;
    }
    return true;
}

bool add_expr(const char** expr,int* val){
    int v1,v2;
    int op=-1;
   if(mul_expr(expr, &v1)&&add_expr1(expr, &v2, &op)){
       if(op==-1) *val=v1;
       else if(op==ADD) *val=v1+v2;
       else *val=v1-v2;
       return true;
   }
   return false;
}

int cal_expr(const char* expr){
    int val=0;
    add_expr(&expr, &val);
    return val;
}