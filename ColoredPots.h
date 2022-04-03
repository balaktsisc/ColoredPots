#ifndef COLORED_POTS_H
#define COLORED_POTS_H
#include <stack>
#include <string>
#include <vector>

using namespace std;

typedef struct pot {
  stack<char> potColors;
} Pot;

class ColoredPots {
private:
    int numOfPots;
    Pot *pots;
    char *listOfColors;
    vector<string> actions;
    ColoredPots *prev;
    void initializeColorList();
public:
    ColoredPots(int);
    ColoredPots(int, char[][4], char[]);
    ColoredPots(const ColoredPots &);
    static bool fillPot(Pot &, Pot &);
    void setPot(int, char *);
    void setPotsRandom();
    void printCurrentGameState();
    vector<ColoredPots *> expand();
    bool isSolved();
    vector<string> getActions()const {
        return actions;
    }
    ColoredPots *getPrevious()const {
        return prev;
    }
    ColoredPots & operator= (ColoredPots);
    bool operator== (const ColoredPots &) const;
   // friend bool operator< (const ColoredPots &,const ColoredPots &);
};
#endif //COLORED_POTS_H
