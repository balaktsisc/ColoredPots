#include <iostream>
#include <queue>
#include <algorithm>
#include "ColoredPots.h"
#define N 4

using namespace std;


ColoredPots *BFS(ColoredPots *initial, long long &examined, long long &mem)
{
    queue<ColoredPots *> agenda;
    vector <ColoredPots> closed;

    agenda.push(initial);
    examined=0;
    mem=1;
    while (!agenda.empty()) {
        if (agenda.size() + closed.size() > mem)
            mem = agenda.size() + closed.size();
        ColoredPots *s = agenda.front();
        agenda.pop();
        if (find(closed.begin(), closed.end(), *s)==closed.end()) {
            examined++;
            if (s->isSolved())
                return s;
            closed.push_back(*s);
            vector<ColoredPots *> children =s->expand();
            for (auto & i : children) {
                if (find(closed.begin(), closed.end(), *i)==closed.end())
                    agenda.push(i);
            }
        }
    }
    return nullptr;
}

int main() {
    char colors[N-2][4] = {{'y','r','y','r'},
                           {'y','y','r','r'}};/*,
                           {'g','g','r','y'},
                           {'w','r','r','w'},
                           {'b','w','y','b'}};*/
    char list[N-2] = {'y','r'};//,'b','g','w'};
    ColoredPots game(N, colors, list);

    long long ex, mem;

    ColoredPots *initial = &game;
    ColoredPots *solved = BFS(initial,ex, mem);
    if (solved != nullptr) {
        for (const string &s: solved->getActions())
            cout << s;
        cout << "Examined: " << ex << "\tMemory: " << mem << endl;
    } else
        cout << "Problem unsolvable!" << endl;

    char colors0[3][4] = { {'r','b','y','y'},
                           {'b','r','r','b'},
                           {'y','r','b','y'}};
    char list0[3] = {'y','r','b'};
    ColoredPots game0(5, colors0, list0);

    long long ex0, mem0;

    ColoredPots *initial0 = &game0;
    ColoredPots *solved0 = BFS(initial0,ex0, mem0);
    if (solved0 != nullptr) {
        for (const string &s: solved0->getActions())
            cout << s;
        cout << "Examined: " << ex0 << "\tMemory: " << mem0 << endl;
    } else
        cout << "Problem unsolvable!" << endl;




    char colors2[5][4] = { {'r','b','y','y'},
                           {'b','g','g','w'},
                           {'g','g','r','y'},
                           {'w','r','r','w'},
                           {'b','w','y','b'}};
    char list2[5] = {'y','r','b','g','w'};
    ColoredPots game2(7, colors2, list2);

    long long ex2, mem2;

    ColoredPots *initial2 = &game2;
    ColoredPots *solved2 = BFS(initial2,ex2, mem2);
    if (solved2 != nullptr) {
        for (const string &s: solved2->getActions())
            cout << s;
        cout << "Examined: " << ex2 << "\tMemory: " << mem2 << endl;
    } else
        cout << "Problem unsolvable!" << endl;

    return 0;
}