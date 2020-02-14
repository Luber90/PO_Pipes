#include <ncurses.h>
#include <string>
#include <string.h>
#include "lista.hpp"
#include "pipes.hpp"
#include "nano.hpp"
#include "nice-like.hpp"

using namespace std;


int main(){
    int num = 0;
    Pipes mytool(num);
    Nano nano;
    nano.tool = &mytool;
	mytool.backend = &nano;
    //nano.bind("<EDITION>", [](){printw("k"); refresh();}, "help");
    //nano.bind("<DEL>%New entry!Write in your entry${E}", [](){printw("J"); refresh();}, "help");
    mytool.init();
    nano.start();
    return 0;
}