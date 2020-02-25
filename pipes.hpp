#ifndef PIPES_HPP_INCLUDED_
#define PIPES_HPP_INCLUDED_
#include "shared.hpp"
#include "lista.hpp"

using namespace std;


int tonum(string s);

void split(string s, string* tab);

class NoName{};

class Pipes : public Tool{
public:
    void setEntry(string field, string value);
    string getEntry(string field);
    void init();
    void redraw();
    Backend* backend;
    Pipes(int numm);
    void moveright();
    void moveleft();
    void moveup();
    void movedown();
    void addB();
    void editB();
    void addA();
    void delB();
    void delA();
    void clean();
    void show(Box*a);
    int* findP(Box* a, int xx, int yy);
    void savef();
    void openf();
    void setPath(string namee);
    bool fileExist(string namee);
private:
    int pos[2], i, z, rows, cols, mode, num, prior;
    char tmp[100];
    bool adding;
    string buff, val, is_saved, key, file_name, path, tryb, namebuff, exepath;
    Lista boxlist;
    Box* tmpbox;
    int getMode();
    void changeMode();
};

#endif