#ifndef LISTA_HPP_INCLUDED_
#define LISTA_HPP_INCLUDED_

using namespace std;

class NoElement{};

class NoSpace{};

class Box{
public:
    Box(int xx, int yy, string comma);
    int getX();
    int getY();
    string getComm();
    void chngComm(string c);
    int getArrC();
    int length();
    bool isIn(int xx, int yy);
    bool isOverlap(int xx, int yy, int len);
    void addArr(int xx, int yy, int c);
    int arrows[12][3]={0};
private:
    int x, y, b, arrcount = 0;
    string comm;
};

class ElementB{
public:
    ElementB(Box *a);
    ~ElementB();
    Box* getV();
    ElementB* getNext();
    void setNext(ElementB * n);
private:
    Box* value;
    ElementB* next;
};

class Lista{
public:
    Lista();
    ~Lista();
    void add(ElementB *n);
    void del(int x, int y);
    int len();
    void delA(int xx, int yy);
    void editB(int x, int y, string c);
    ElementB* operator[](int i);
    void reset();
private:
    ElementB *first;
    int length;

};


#endif