#include "ColoredPots.h"
#include <cstdlib>
#include <iostream>
#include <algorithm>


// Randomly initialized pots constructor
ColoredPots::ColoredPots(int N) {
    this->prev = nullptr;
    this->pots = nullptr;
    listOfColors = nullptr;
    if (N >= 20 || N < 3) exit(1);
    this->numOfPots = N;
    initializeColorList();
    setPotsRandom();
}

// Initialized pot constructor
ColoredPots::ColoredPots(int N, char colors[][4], char list[]) {
    this->prev = nullptr;
    if (N >= 20 || N < 3) exit(1);
    this->numOfPots = N;
    listOfColors = new char[N-2];
    for (int i = 0; i < N-2; i++)
        listOfColors[i] = list[i];
    this->pots = new Pot[this->numOfPots];
    for(int i = 0; i < this->numOfPots - 2; i++)
        setPot(i,colors[i]);
}

// Initialize main color list.
void ColoredPots::initializeColorList() {
    listOfColors = new char[numOfPots - 2];
    for (int i = 0; i < numOfPots - 2; i++)
        listOfColors[i] = 'a' + i;
}

// Copy constructor
ColoredPots::ColoredPots(const ColoredPots &coloredPots) {
    this->numOfPots = coloredPots.numOfPots;
    this->prev = nullptr;
    this->pots = new Pot[coloredPots.numOfPots];
    for (int i = 0; i < coloredPots.numOfPots; i++) {
        stack<char> colors(coloredPots.pots[i].potColors);
        this->pots[i].potColors = colors;
    }
    for (const auto & action : coloredPots.actions)
        this->actions.push_back(action);
}

// Move colour from the top of the first pot to the second pot.
bool ColoredPots::fillPot(Pot &from, Pot &to) {
    if (from.potColors.empty() || to.potColors.size() == 4) return false;
    if (!to.potColors.empty() && to.potColors.top() != from.potColors.top()) return false;
    if (to.potColors.empty()) {
        do {
            to.potColors.push(from.potColors.top());
            from.potColors.pop();
        } while (to.potColors.top() == from.potColors.top() && to.potColors.size() < 4 && !from.potColors.empty());
    }
    else {
        while (to.potColors.top() == from.potColors.top() && to.potColors.size() < 4 && !from.potColors.empty()) {
            to.potColors.push(from.potColors.top());
            from.potColors.pop();
        }
    }
    return true;
}

// Search tree state expansion
vector<ColoredPots *> ColoredPots::expand() {
    vector<ColoredPots *> children;
    ColoredPots *child;
    string step;

    // For-each pot, check if it can pour its top color to every other pot and then add this scenario as a child state.
    for (int i = 0; i < this->numOfPots; i++) {
        for (int j = 0; j < this->numOfPots; j++) {
            child = new ColoredPots(*this);
            child->prev = this;
            step = to_string(i) + " to " + to_string(j) + "\n";
            child->actions.push_back(step);
            if (i != j && fillPot(child->pots[i], child->pots[j]))
                children.push_back(child);
            else
                delete child;
        }
    }
    return children;
}

// Set the color stack of a pot.
void ColoredPots::setPot(int potIndex, char* colors) {
    for(int i = 0; i < 4; i++)
        this->pots[potIndex].potColors.push(colors[i]);
}

// Initialize numOfPots-2 pots with randomly colored four levels.
void ColoredPots::setPotsRandom() {
    this->pots = new Pot[this->numOfPots];
    for (int i = 0; i < this->numOfPots - 2; i++)
        for (int j = 0; j < 4; j++)
            pots[i].potColors.push(listOfColors[rand() % (this->numOfPots - 2)]);
}

void ColoredPots::printCurrentGameState () {
    ColoredPots game(*this);
    std::cout << std::endl;
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < game.numOfPots; j++)
            if (!game.pots[j].potColors.empty()) {
                std::cout << game.pots[j].potColors.top() << " ";
                game.pots[j].potColors.pop();
            } else
                std::cout << "- ";
        std::cout << std::endl;
        if (i == 3) {
            for (int j = 1; j <= game.numOfPots; j++)
                std::cout << j << " ";
        }
    }
    std::cout << std::endl;
}

bool ColoredPots::isSolved() {
    ColoredPots game(*this);
    for (int i = 0; i < game.numOfPots; i++) {
        int levels = 0;
        while(!game.pots[i].potColors.empty()) {
            char top = game.pots[i].potColors.top();
            game.pots[i].potColors.pop();
            if(!game.pots[i].potColors.empty() && game.pots[i].potColors.top() != top) return false;
            levels++;
        }
        if (levels!=4 && levels!=0) return false;
    }
    return true;
}

ColoredPots & ColoredPots::operator=(ColoredPots o) {
    this->pots = o.pots;
    this->numOfPots = o.numOfPots;
    this->actions = o.actions;
    this->prev = o.prev;
    return *this;
}


bool isSameStack(stack<char> stack1, stack<char> stack2) {
    bool flag = true;
    if (stack1.size() != stack2.size()) {
        flag = false;
        return flag;
    }
    while (!stack1.empty()) {
        if (stack1.top() == stack2.top()) {
            // Pop top of both stacks
            stack1.pop();
            stack2.pop();
        }
        else {
            flag = false;
            break;
        }
    }
    return flag;
}

bool ColoredPots::operator==(const ColoredPots &o) const {
    for (int i = 0; i < this->numOfPots; i++)
        if (!isSameStack(this->pots[i].potColors, o.pots[i].potColors)) return false;
    return true;
}
