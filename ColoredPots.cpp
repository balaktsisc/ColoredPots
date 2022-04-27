#include "ColoredPots.h"
#include <cstdlib>
#include <iostream>
#include <algorithm>


// Randomly initialized pots constructor
ColoredPots::ColoredPots(int N) {
    if (N >= 20 || N < 3) exit(1);
    this->pots = new Pot[N];
    this->numOfPots = N;

    char list[N - 2];

    for (int i = 0; i < N - 2; i++)
        list[i] = 'a' + i;

    for (int i = 0; i < N; i++) {
        this->pots[i].potColors = new char[4];
        this->pots[i].top = 0;
    }

    for (int i = 0; i < N - 2; i++) {
        char color = list[i];
        for (int j = 0; j < 4; j++) {
            RAND_INDEX:
                int index = rand() % (N - 2);
            if (this->pots[index].top < 4)
                this->pots[index].potColors[this->pots[index].top++] = color;
            else
                goto RAND_INDEX;
        }
    }
}

// Initialized pot constructor
ColoredPots::ColoredPots(int N, char colors[][4]) {
    if (N >= 20 || N < 3) exit(1);
    this->numOfPots = N;
    this->pots = new Pot[this->numOfPots];
    for (int i = 0; i < this->numOfPots; i++)
        this->pots[i].potColors = new char[4];
    for (int i = 0; i < this->numOfPots - 2; i++)
        setPot(i,colors[i]);
    this->pots[this->numOfPots-1].top = 0;
    this->pots[this->numOfPots-2].top = 0;
}

// Copy constructor
ColoredPots::ColoredPots(const ColoredPots &coloredPots) {
    this->numOfPots = coloredPots.numOfPots;
    this->pots = new Pot[coloredPots.numOfPots];
    for (int i = 0; i < this->numOfPots; i++)
        this->pots[i].potColors = new char[4];
    for (int i = 0; i < coloredPots.numOfPots; i++) {
        for (int j = 0; j < 4 && j < coloredPots.pots[i].top; j++)
            this->pots[i].potColors[j] = coloredPots.pots[i].potColors[j];
        this->pots[i].top = coloredPots.pots[i].top;
    }
    for (const auto &action: coloredPots.actions)
        this->actions.push_back(action);
}

ColoredPots::~ColoredPots() {
    for (int i = 0; i < this->numOfPots; i++)
        delete[] this->pots[i].potColors;
    delete[] this->pots;
}

// Move colour from the top of the first pot to the second pot.
bool ColoredPots::fillPot(Pot &from, Pot &to) {
    if (from.top == 0 || to.top == 4) return false;
    if (to.top != 0 && to.potColors[to.top-1] != from.potColors[from.top-1]) return false;
    while (to.top == 0 || (to.potColors[to.top-1] == from.potColors[from.top-1] && to.top < 4 && from.top > 0))
            to.potColors[to.top++] = from.potColors[--from.top];
    return true;
}

// Search tree state expansion
vector<ColoredPots *> ColoredPots::expand() {
    vector<ColoredPots *> children;
    ColoredPots *child;
    string step;

    // For-each pot, check if it can pour its top color to every other pot and then add this scenario as a child state.
    for (int i = 0; i < this->numOfPots; i++)
        for (int j = 0; j < this->numOfPots; j++) {
            child = new ColoredPots(*this);
            step = to_string(i) + " to " + to_string(j) + "\n";
            child->actions.push_back(step);
            if (i != j && fillPot(child->pots[i], child->pots[j]))
                children.push_back(child);
            else
                delete child;
        }
    return children;
}

// Set the color stack of a pot.
void ColoredPots::setPot(int potIndex, const char* colors) {
    for(int i = 0; i < 4; i++)
        this->pots[potIndex].potColors[i] = colors[i];
    this->pots[potIndex].top = 4;
}

// Prints out a graphic representation of the set of pots with the color order in each one.
void ColoredPots::printCurrentGameState () {
    ColoredPots game(*this);
    std::cout << std::endl;
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < game.numOfPots; j++) {
            if (j/10 > 0) std::cout << " ";
            std::cout << (game.pots[j].potColors[i] >= 'a' && game.pots[j].potColors[i] <= 'a' + 18
                          ? game.pots[j].potColors[i] : '|') << " ";

        }
        std::cout << std::endl;
        if (i == 3) {
            for (int j = 0; j < game.numOfPots; j++)
                std::cout << j << " ";
        }
    }
    std::cout << std::endl;
}

// Checks if all the pots contain at most one type of color.
bool ColoredPots::isSolved() {
    ColoredPots game(*this);
    for (int i = 0; i < game.numOfPots; i++) {
        int levels = 0;
        while(game.pots[i].top != 0) {
            char top = game.pots[i].potColors[--game.pots[i].top];
            if(game.pots[i].top != 0 && game.pots[i].potColors[game.pots[i].top-1] != top) return false;
            levels++;
        }
        if (levels!=4 && levels!=0) return false;
    }
    return true;
}

/* Creates a hashing key that represents the state of each pot. Different sets of pots 
 * with the same color order in each pot and different order of the pots inside the set
 * return the same hash key, as they physically are the same sets.
*/
string ColoredPots::getKey() const {
    string keys[this->numOfPots], key;
    for (int i = 0; i < this->numOfPots; i++) {
        int top = this->pots[i].top;
        for (int j = 0; j < 4; j++) {
            if (top != 0) {
                keys[i] += this->pots[i].potColors[j];
                top--;
            }
            else
                keys[i] += "-";
        }
    }
    sort(keys, keys + this->numOfPots);
    for (int i = 0; i < this->numOfPots; i++)
        key += keys[i];
    return key;
}

ColoredPots & ColoredPots::operator=(ColoredPots const &o) {
    for (int i = 0; i < this->numOfPots; i++) {
        for (int j = 0; j < 4; j++)
            this->pots[i].potColors[j] = o.pots[i].potColors[j];
        this->pots[i].top = o.pots[i].top;
    }
    this->numOfPots = o.numOfPots;
    this->actions = o.actions;
    return *this;
}

bool ColoredPots::operator==(const ColoredPots &o) const {
    for (int i = 0; i < this->numOfPots; i++) {
        if (this->pots[i].top != o.pots[i].top) return false;
        for (int j = 0; j < 4 && j < this->pots[i].top; j++)
            if (this->pots[i].potColors[j] != o.pots[i].potColors[j])
                return false;
    }
    return true;
}

bool operator<(const ColoredPots &a, const ColoredPots &b) {
    return a.getKey() < b.getKey();
}
