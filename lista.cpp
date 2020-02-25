#include <string>
#include <ncurses.h>
#include "lista.hpp"

using namespace std;

//class NoElement{};

//class NoSpace{};



Box::Box(int xx, int yy, string comma){
    x = xx;
    y = yy;
    b = comma.length();
    comm = comma;
}
int Box::getX(){
    return x;
}
int Box::getY(){
    return y;
}
string Box::getComm(){
    return comm;
}
int Box::length(){
    return b;
}
bool Box::isIn(int xx, int yy){
    if((xx>=x)&&(xx<=(x+1+b)&&(yy>=y)&&(yy<=y+2))) return true;
    else return false;
}
bool Box::isOverlap(int xx, int yy, int len){
    int alx = x, arx = x+b+1, ary=y, aly=y+2, blx=xx, brx=xx+len, bry=yy, bly=yy+2;

    if (alx > brx || blx > arx) 
        return false; 
  
    if (aly < bry || bly < ary) 
        return false; 
  
    return true;
}
void Box::addArr(int xx, int yy, int c){
    if(arrcount == 11){
        return;
    }
    arrows[arrcount][0] = xx;
    arrows[arrcount][1] = yy;
    arrows[arrcount][2] = c;
    arrcount++;
}

int Box::getArrC(){
    return arrcount;
}





ElementB::ElementB(Box *a){
    value = a;
    next = nullptr;
}

ElementB::~ElementB(){
    delete value;
    delete next;
}

Box* ElementB::getV(){
    return value;
}

ElementB* ElementB::getNext(){
    return next;
}

void ElementB::setNext(ElementB * n){
    next = n;
}



Lista::Lista(){
        first = nullptr;
        length = 0;
}

void Lista::reset(){
    first = nullptr;
    length = 0;
}

int Lista::len(){
    return length;
}

Lista::~Lista(){
    delete first;
}

void Lista::add(ElementB *n){
    if(!first){
        first = n;
        length++;
        return;
    }
    int x = n->getV()->getX(), y = n->getV()->getY(), len = n->getV()->length();
    ElementB *curr = first;
    while (curr->getNext()){
        if(curr->getV()->isOverlap(x, y, len)){
            throw NoSpace();
        }
        curr = curr->getNext();
    }
    if(curr->getV()->isOverlap(x, y, len)){
        throw NoSpace();
    }
    curr->setNext(n);
    length++;
}

bool Lista::del(int x, int y){
    if(length>0){
        if(first->getV()->isIn(x, y)){
            ElementB * tmp = first->getNext();
            //delete first;
            first = tmp;
            length--;
            return true;
        }
        ElementB *curr = first;
        while(curr->getNext()){
            if(curr->getNext()->getV()->isIn(x, y)){
                ElementB* tmp = curr->getNext();
                curr->setNext(tmp->getNext());
                length--;
                return true;
                //delete tmp;
            }
            curr = curr->getNext();
            if (!curr){
                return false;
            }
        }
    }
    return false;
}

void Box::chngComm(string c){
    b = c.length();
    comm = c;
}

void Lista::editB(int x, int y, string c){
    for(int i = 0; i < length; i++){
        if(((*this)[i])->getV()->isIn(x, y)){
            ((*this)[i])->getV()->chngComm(c);
            return;
        }
    }
}

ElementB* Lista::operator[](int i){
    if((length>0)&&(i<length)){
        ElementB* curr = first;
        for(int j = 0; j < i; j++){
            if(curr->getNext()){
                curr = curr->getNext();
            }
        }
        return curr;
    }
    else throw NoElement();
}

void Lista::delA(int xx, int yy){
    for(int i = 0; i < length; i++){
        for(int j = 0; j < 12; j++){
            if(((*this)[i])->getV()->arrows[j][0]==xx&&((*this)[i])->getV()->arrows[j][1]==yy){
                ((*this)[i])->getV()->arrows[j][0] = 0;
                ((*this)[i])->getV()->arrows[j][1] = 0;
            }
        }
    }
}

