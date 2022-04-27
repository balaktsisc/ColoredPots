#ifndef COLORED_POTS_H
#define COLORED_POTS_H
#include <stack>
#include <string>
#include <vector>

using namespace std;

typedef struct pot {
  char *potColors;
  int top;
} Pot;

class ColoredPots {
private:
    int numOfPots;
    Pot *pots;
    vector<string> actions;
public:
    ColoredPots(int);
    ColoredPots(int, char colors[][4]);
    ColoredPots(const ColoredPots &);
    ~ColoredPots();
    static bool fillPot(Pot &, Pot &);
    void setPot(int, const char *);
    void printCurrentGameState();
    vector<ColoredPots *> expand();
    bool isSolved();
    vector<string> getActions()const {
        return actions;
    }
    ColoredPots & operator= (ColoredPots const &);
    bool operator== (const ColoredPots &) const;
    string getKey() const;
    friend bool operator< (const ColoredPots &,const ColoredPots &);
};
#endif //COLORED_POTS_H
