#include <iostream>
#include <queue>
#include <unordered_map>
#include <algorithm>
#include "ColoredPots.h"
#include "ColorList.h"
#include <chrono>

using namespace std;


ColoredPots *BFS(ColoredPots *initial, long long &examined, long long &mem) {
    queue<ColoredPots *> agenda;
    unordered_map <string, ColoredPots *> closed;

    agenda.push(initial);
    examined = 0;
    mem = 1;

    while (!agenda.empty()) {
        if (agenda.size() + closed.size() > mem)
            mem = agenda.size() + closed.size();
        ColoredPots *s = agenda.front();
        agenda.pop();
        if (closed.count(s->getKey()) == 0) {
            examined++;
            if (s->isSolved())
                return s;
            pair<string,ColoredPots *> k (s->getKey(),s);
            closed.insert(k);
            vector<ColoredPots *> children = s->expand();
            for (auto & i : children)
                if (closed.count(i->getKey()) == 0)
                    agenda.push(i);
        }
    }
    return nullptr;
}

void solveGame(ColoredPots &game) {
    long long ex, mem;
    ColoredPots *initial = &game;

    chrono::steady_clock::time_point begin = chrono::steady_clock::now();
    ColoredPots *solved = BFS(initial,ex, mem);
    chrono::steady_clock::time_point end = chrono::steady_clock::now();

    if (solved != nullptr) {
        for (const string &s: solved->getActions())
            cout << s;
        cout << "Examined: " << ex << "\tMemory: " << mem << "\tTime: " << chrono::duration_cast<chrono::milliseconds> (end - begin).count() << " ms";
        //solved->printCurrentGameState();
        cout << endl;
    } else
        cout << "Problem unsolvable!" << endl;
}

int main() {
    //N=18
    ColoredPots gameRandom(18);
    solveGame(gameRandom);

    //N=15
    ColoredPots gameRandom0(15);
    solveGame(gameRandom0);

    //N=11
    char colors[9][4] = {{GREY,PURPLE,TURQUOISE,ORANGE},
                         {RED,PINK,PINK,PINK},
                         {CYAN,TURQUOISE,PURPLE,TURQUOISE},
                         {BLUE,CYAN,ORANGE,GREY},
                         {GREY,CYAN,PURPLE,BLUE},
                         {BLUE,CYAN,GREEN,PINK},
                         {ORANGE,RED,PURPLE,TURQUOISE},
                         {RED,GREEN,GREY,GREEN},
                         {RED,BLUE,ORANGE,GREEN}};
    ColoredPots game(11, colors);
    solveGame(game);

    //N=9
    char colors0[7][4] = { {PURPLE,GREEN,PURPLE,TURQUOISE},
                           {GREEN,GREEN,CYAN,TURQUOISE},
                           {PINK,PINK,ORANGE,PINK},
                           {TURQUOISE,YELLOW,GREEN,PINK},
                           {PURPLE,CYAN,YELLOW,ORANGE},
                           {ORANGE,PURPLE,YELLOW,YELLOW},
                           {ORANGE,TURQUOISE,CYAN,CYAN}};
    ColoredPots game0(9, colors0);
    solveGame(game0);

    //N=8
    char colors1[6][4] = { {RED,BLUE,YELLOW,YELLOW},
                           {BLUE,GREEN,GREEN,PINK},
                           {GREEN,GREEN,RED,YELLOW},
                           {PINK,RED,RED,PINK},
                           {BLUE,PINK,YELLOW,ORANGE},
                           {ORANGE,ORANGE,BLUE,ORANGE}};
    ColoredPots game1(8, colors1);
    solveGame(game1);

    //N=7
    char colors2[5][4] = { {RED,BLUE,YELLOW,YELLOW},
                           {BLUE,GREEN,GREEN,PINK},
                           {GREEN,GREEN,RED,YELLOW},
                           {PINK,RED,RED,PINK},
                           {BLUE,PINK,YELLOW,BLUE}};
    ColoredPots game2(7, colors2);
    solveGame(game2);

    return 0;
}
