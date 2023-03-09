#include "game.h"

#include <iostream>

int main(){
    Game g = Game(7);
    g.MakeTile(1, 1, 0);
    g.PrintMap();
}