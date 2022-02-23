#include <iostream>
#include <thread>
#include <mutex>
#include <vector>

using namespace std;
typedef struct Ticket{
    Ticket(int tk_No,int tk_price){
        No=tk_No;
        price=tk_price;
    }
    int No;
    int price;
}Ticket;

class Tickets{
public:
    Tickets(size_t total_num=1000,int price=500){
        ticket_num=ticket_total_num=total_num;
        ticket_price=price;
    }
    Ticket sale(){
        sale_lock.lock();
        if(ticket_num<=0){
            cout<<"sale out"<<endl;
            sale_out=1;
            sale_lock.unlock();
            return Ticket(-1,0);
        }
        ticket_num--;
        Ticket tk=Ticket(ticket_total_num-ticket_num,ticket_price);
        if(ticket_num!=1000&&ticket_num%100==0){
            ticket_price+=100;
        }
        sale_lock.unlock();
        return tk;
    }
    bool is_sale_out(){
        return sale_out;
    }
private:
    bool sale_out;
    size_t ticket_total_num;
    size_t ticket_num;
    mutex sale_lock;
    int ticket_price;
};

class Scalper{
public:
    Scalper(){
        self_tickets=nullptr;
        self_money=0;
    }
    Scalper(Tickets* tickets,int id){
        scalper_id=id;
        self_tickets=tickets;
        self_money=0;
    }
    int sale(){
        if(self_tickets==nullptr) return -1;
        else {
            Ticket tk=self_tickets->sale();
            if(tk.No>=0){
                self_money+=tk.price;
                sale_tickets.push_back(tk);
            }
        }
        return 0;
    }
   void print_self_info(){
       cout<<"scaler:"<<scalper_id<<endl;
       cout<<"sale:"<<self_money<<endl;
       cout<<"sale tickets:\n";
       for(int i=0;i<sale_tickets.size();i++){
           cout<<sale_tickets[i].No<<" ";
       }
       
   }
   bool sale_out(){
       return self_tickets->is_sale_out();
   }
private:
    int scalper_id;
    Tickets* self_tickets;
    int self_money;
    vector<Ticket> sale_tickets;
};

void purchase(Scalper *s){
    while(!s->sale_out()){
        s->sale();
    }
}

int main(){
    Tickets total_tk;
    Scalper s1(&total_tk,1),s2(&total_tk,2),s3(&total_tk,3);
    thread t2(purchase,&s1),t1(purchase,&s2),t3(purchase,&s3);
    t1.join();
    t2.join();
    t3.join();
    s1.print_self_info();
    s2.print_self_info();
    s3.print_self_info();
    return 0;
}