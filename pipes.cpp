#include <ncurses.h>
#include <string>
#include <cmath>
#include <string.h>
#include "lista.hpp"
#include "pipes.hpp"
#include <iostream>
#include <fstream>
#include <sstream>
#include <sys/stat.h>
#include <sys/types.h>




int tonum(string s){
    int num = 0;
    stringstream a(s);
    a >> num;
    return num;
}

void split(string s, string *tab){
    istringstream a(s);
    int counter = 0;
    do { 
        string word; 
        a >> word; 
        if(word!="\n"){
            tab[counter] = word;
            counter++; 
        }
    } while (a);
}

Pipes::Pipes(int numm){
    tryb = "Adding";
    num = numm;
    pos[0] = 2;
    pos[1] = 1;
    adding = false;
    is_saved = "YES";
    file_name = "";
    path = "";
    //move(pos[0], pos[1]);
    mode = 0;

}

void Pipes::moveleft(){
    if(pos[1] > 1) pos[1]--;
    this->redraw();
}

void Pipes::moveright(){
    if(pos[1] < cols - 2) pos[1]++;
    this->redraw();
}

void Pipes::moveup(){
    if(pos[0] > 2)pos[0]--;
    this->redraw();
}

void Pipes::movedown(){
    if(pos[0] < rows - 4)pos[0]++;
    this->redraw();
}

void Pipes::addB(){
    move(pos[0], pos[1]);
    //echo();
    //scanw("%s", tmp);
    buff = namebuff;
    if(this->fileExist(buff)){
        try{
            boxlist.add(new ElementB(new Box(pos[1]-1, pos[0]-1, buff)));
            move(rows-2, 0);
            clrtoeol();
        }
        catch(NoSpace){
            move(rows-2, 0);
            printw("Brak miejsca!");
        }
        //noecho();
        is_saved = "NO";
        this->redraw();
    }
    else{
        move(rows-2, 0);
        printw("Nie istnieje podany plik!");
    }
}

void Pipes::delB(){
    boxlist.del(pos[1], pos[0]);
    is_saved="NO";
    this->redraw();
}

void Pipes::editB(){
    move(rows-2, 0);
    buff = namebuff;
    if(this->fileExist(buff)){
        move(rows-2, 0);
        clrtoeol();
        move(pos[0], pos[1]);
        boxlist.editB(pos[1], pos[0], buff);
        is_saved = "NO";
        this->redraw();
    }
    else{
        move(rows-2, 0);
        printw("Nie istnieje podany plik!");
    }
}

void Pipes::addA(){
    if(!adding){
    for(z=0;z<boxlist.len();z++){
        if(boxlist[z]->getV()->isIn(pos[1], pos[0])){
            backend->re_bind2("F2", "F2-Add/delete connection!Type the type of arrow:${PRIO}", [&](){addA();});
            adding = true;
            tmpbox = boxlist[z]->getV();
            move(rows-2, 0);
            printw("Wybieranie");
            break;
            }
        }   
    }
    else{
        int c;
        move(rows-2, 0);
        //printw("Podaj rodzaj:");
        //echo();
        //scanw("%d", &c);
        //noecho();
        c = prior;
        backend->re_bind2("F2", "F2-Add/delete connection", [&](){addA();});
        tmpbox->addArr(pos[1], pos[0], c);
        adding = false;
        move(rows-2, 0);
        clrtoeol();
    }
    is_saved="NO";
    this->redraw();
}

void Pipes::clean(){
    for(int i=1; i<rows-2; i++){
        move(i, 0);
        clrtoeol();
    }
}

void Pipes::show(Box *a){
    int pose[2] = {a->getY(), a->getX()};
    attron(A_BOLD);
    move(pose[0], pose[1]);
    for(i = 0; i < a->length() + 2; i++){
        printw("*");
    }
    move(pose[0]+1, pose[1]);
    printw("*%s*", a->getComm().c_str());
    move(pose[0]+2, pose[1]);
    for(i = 0; i<a->length()+2; i++){
        printw("*");
    }
    attroff(A_BOLD);
    int k;
    for(int j = 0; j < 12; j++){
        if(a->arrows[j][0] > 0&&a->arrows[j][1] > 1){
            if(a->arrows[j][2]==0){
                attron(A_REVERSE);
            }
            else if(a->arrows[j][2]==1){
                attron(A_DIM);
            }
            int *tmp = this->findP(a, a->arrows[j][0], a->arrows[j][1]);
            pose[0] = tmp[0];
            pose[1] = tmp[1];
            if(pose[1]==a->arrows[j][0]){
                if (pose[0] - a->arrows[j][1] >= 0){
                    move(pos[0]+1, pos[1]);
                    for(k = 1; k <= pose[0] - a->arrows[j][1] - 2; k++ ){
                        move(pose[0] - k, pose[1] + (a->arrows[j][0] - pose[1])/2);
                        printw("|");
                    }
                    move(a->arrows[j][1]+1, a->arrows[j][0]);
                    printw("^");
                }
                else{
                    move(pos[0]-1, pos[1]);
                    for(k = 1; k < a->arrows[j][1] - pose[0] - 1; k++ ){
                        move(pose[0] + k, pose[1] + (a->arrows[j][0] - (pose[1]) - 1)/2);
                        printw("|");
                    }
                    move(a->arrows[j][1]-1, a->arrows[j][0]);
                    printw("v");
                }
            }
            else if(pose[1] - a->arrows[j][0] < 0) {
                move(pose[0], pose[1]+1);
                hline('-', (a->arrows[j][0] - pose[1])/2 - 1);
                if (pose[0] - a->arrows[j][1] >= 0){
                    for(k = 1; k <= pose[0] - a->arrows[j][1]; k++ ){
                        move(pose[0] - k, pose[1] + (a->arrows[j][0] - pose[1])/2);
                        printw("|");
                    }
                }
                else{
                    for(k = 1; k < a->arrows[j][1] - pose[0]; k++ ){
                        move(pose[0] + k, pose[1] + (a->arrows[j][0] - (pose[1]) - 1)/2);
                        printw("|");
                    }
                }
                move(a->arrows[j][1], pose[1] + (a->arrows[j][0] - (pose[1]) + 1)/2);
                hline('-', (a->arrows[j][0] - pose[1]) - ((a->arrows[j][0] - pose[1])/2 - 1) - 2);
                move(a->arrows[j][1], a->arrows[j][0]- 1);
                printw(">");
            }
            else{
                move(a->arrows[j][1], a->arrows[j][0] + 1);
                hline('-', (pose[1] - a->arrows[j][0])/2);
                if (pose[0] - a->arrows[j][1] >= 0){
                    for(k = 2; k <= pose[0] - a->arrows[j][1]; k++ ){
                        move(a->arrows[j][1] + k - 1, a->arrows[j][0] + (pose[1] - a->arrows[j][0])/2);
                        printw("|");
                    }
                }
                else{
                    for(k = 1; k < a->arrows[j][1] - pose[0]; k++ ){
                        move(a->arrows[j][1] - k, a->arrows[j][0]  + (pose[1] - a->arrows[j][0])/2);
                        printw("|");
                    }
                }
                move(pose[0], a->arrows[j][0] + (pose[1] - a->arrows[j][0])/2 + 1);
                hline('-', (pose[1] - a->arrows[j][0]) - ((pose[1] - a->arrows[j][0])/2 - 1) - 2);
                move(a->arrows[j][1], a->arrows[j][0]+ 1);
                printw("<");
            }
            if(a->arrows[j][2]==0){
                attroff(A_REVERSE);
            }
            else if(a->arrows[j][2]==1){
                attroff(A_DIM);
            }

        }
    }
        
}

void Pipes::redraw(){
    getmaxyx(stdscr, rows, cols);
    move(pos[0], pos[1]);
    this->clean();
    for(z=0;z<boxlist.len();z++){
        this->show(boxlist[z]->getV());
    }
    move(rows-2, 30);
    printw("                      ");
    move(rows-2, 30);
    printw("Mode: %s", tryb.c_str());
    move(pos[0], pos[1]);
    refresh();
}

int* Pipes::findP(Box* a, int xx, int yy){
    int x = a->getX(), y = a->getY(), *best = new int[2];
    best[0] = y;
    best[1] = x;
    double dist = sqrt(pow(x-xx,2)+pow(y-yy,2));
    for(int k = 0; k < (2*a->length() + 2*3) - 1; k++){
        if((x<a->getX()+a->length()+1)&&y==a->getY()){
            x++;
        }
        else if((x==a->getX()+a->length()+1)&&y<a->getY()+2){
            y++;
        }
        else if(x>a->getX()&&y==a->getY()+2){
            x--;
        }
        else{
            y--;
        }

        if(sqrt(pow(x-xx,2)+pow(y-yy,2)) < dist){
            dist = sqrt(pow(x-xx,2)+pow(y-yy,2));
            best[0] = y;
            best[1] = x;
        }
    }
    return best;
}

void Pipes::changeMode(){
    switch(mode){
        case 0:
            mode = 1;
            tryb = "Deleting";
            backend->re_bind2("F1", "F1-Delete box", [&](){delB();});
            backend->re_bind2("F2", "F2-Delete connection", [&](){delA();});
            break;
        case 1:
            mode = 2;
            tryb = "Editing";
            backend->re_bind2("F1", "F1-Edit box!Type new name of file:${NAME_BUFF}", [&](){editB();});
            break;
        case 2:
            mode = 0;
            backend->re_bind2("F1", "F1-Add box!Type the name of exe or datafile:${NAME_BUFF}", [&](){addB();});
            backend->re_bind2("F2", "F2-Add connection", [&](){addA();});
            tryb = "Adding";
            break;
    }
    this->redraw();
    move(pos[0], pos[1]);
}

int Pipes::getMode(){ return mode;}

void Pipes::delA(){
    boxlist.delA(pos[1], pos[0]);
    is_saved="NO";
    this->redraw();
}

void Pipes::savef(){
    string nazwa;
    nazwa = file_name;
    if (nazwa == ""){
        if(!(path=="")){
            nazwa = path;
        }
        else{
            throw(NoSpace());
        }
    }
    ofstream file(nazwa + ".pipes");
    for(i = 0; i < boxlist.len(); i++){
        file << boxlist[i]->getV()->getX() << " " << boxlist[i]->getV()->getY() << " " << boxlist[i]->getV()->getComm() << " " <<  boxlist[i]->getV()->getArrC() << " ";
        for(int j = 0; j < 12; j++){
            file << boxlist[i]->getV()->arrows[j][0] << " " << boxlist[i]->getV()->arrows[j][1] << " " << boxlist[i]->getV()->arrows[j][2] <<" ";
        }
        file << "\n";
    }
    file.close();
    is_saved="YES";
    this->setPath(nazwa);
    this->redraw();
}

void Pipes::openf(){
    //move(rows-2, 0);
    //printw("Podaj nazwe:");
    string nazwa, buffor, tab[41];
    //char tmp[100];
    //echo();
    //scanw("%s", tmp);
    //noecho();
    //move(rows-2, 0);
    //clrtoeol();
    nazwa = file_name;
    boxlist.reset();
    ifstream file(nazwa + ".pipes");
    int counter = 0;
    while (getline (file, buffor)) {
        split(buffor, tab);
        boxlist.add(new ElementB(new Box(tonum(tab[0]), tonum(tab[1]), tab[2])));
        for(int i = 0; i < 12; i++){
            if(tonum(tab[4 + i*3])>0&&tonum(tab[5 + i*3])>0){
                boxlist[counter]->getV()->addArr(tonum(tab[4 + i*3]), tonum(tab[5 + i*3]), tonum(tab[6 + i*3]));
            }
        }
        counter++;
    }
    file.close();
    is_saved="YES";
    this->redraw();
}


void Pipes::setEntry(string field, string value){
    if(field == "KEY"){
        key = value;
    }
    else if(field == "FILE_NAME"){
        file_name = value;
    }
    else if(field == "NAME_BUFF"){
        namebuff = value;
    }
    else if(field == "PRIO"){
        prior = tonum(value);
    }
}

string Pipes::getEntry(string field){
    if(field == "IS_SAVED"){
        return is_saved;
    }
    return NULL;
}

void Pipes::setPath(string namee){
    path = namee;
}

bool Pipes::fileExist(string namee){
    struct stat b;   
    return (stat (namee.c_str(), &b) == 0);
}


void Pipes::init(){
    switch(num){
    case 0:
        backend->bind("#nano#<LARROW>%Move left",[&](){moveleft();}, "Go left");
        backend->bind("#nano#<RARROW>%Move right",[&](){moveright();}, "Go right");
        backend->bind("#nano#<UARROW>%Move up",[&](){moveup();}, "Go up");
        backend->bind("#nano#<DARROW>%Move left",[&](){movedown();}, "Go down");
        backend->bind("#nano#<F1>%Add box!Type the name of exe or datafile:${NAME_BUFF}",[&](){addB();}, "Add/delete/edit box");
        backend->bind("#nano#<F2>%Add connection",[&](){addA();}, "Add/delete connection");
        backend->bind("#nano#<F3>%Change edition mode",[&](){changeMode();}, "Change edition mode");
        backend->bind("#nano#<F5>%Save!Type a filename:${FILE_NAME}", [&](){savef();}, "Save file");
        backend->bind("#nano#<F6>%Open!Type a filename:${FILE_NAME}", [&](){openf();}, "Open file");
        break;
    case 1:
        backend->bind(".1.prawo", [&](){addB();}, "Go left");
        
        break;
    }
}