#ifndef STATE_H
#define STATE_H

# include <iostream>
# include <vector>
# include <unordered_set>
#include <functional>  // For std::hash
# include <stdexcept>
# include "Color.h"
# include "Action.h"

class State{
    public:
        int RowNum;
        int ColNum;
        int depth;
        // agentRows[i] = r means agent i at row r
        std::vector<int> agentRows;
        std::vector<int> agentCols;
        // box[r][c]= char or 0
        std::vector<std::vector<char>> boxes; 
        
        State* parent;
        State* initialState;
        
        std::vector<actionEnum> jointAction;
        
        State(int RowNum, int ColNum, std::vector<int> agentRows, std::vector<int> agentCols, 
              std::vector<std::vector<char>> boxes);
        State(State* parent, std::vector<actionEnum> jointAction);
        std::vector<std::vector<actionEnum>> extractPlan();
        int getDepth(){return this->depth;};
        bool isGoalState();
        std::vector<State*> getExpandState();
        bool isConflict(std::vector<actionEnum> jointAction);
        std::vector<actionEnum> getApplicableAction(int agentIdx);
        bool isFree(int row, int col);
        bool isNear(int agentIdx, actionField a);
        
    private:
        int coordEncode(int x, int y) {return x*ColNum + y;};
};


// initial state
class InitialState : public State{
    public:
        InitialState(int RowNum, int ColNum, std::vector<int> agentRows, std::vector<int> agentCols, 
        std::vector<std::vector<char>> boxes, std::vector<Color> agentColors, std::vector<Color> boxColor,
        std::vector<std::vector<bool>> walls, std::vector<std::vector<char>> goals);
        // agentColors[i] = color means agent i is color
        std::vector<Color> agentColors;
        // box[char-'0'] = color
        std::vector<Color> boxColor;
        // same as box[][]
        std::vector<std::vector<bool>> walls;
        std::vector<std::vector<char>> goals;
};


struct StateHash{
    std::size_t operator()(const State* s) const;
    std::size_t myHash_int(const std::vector<int>& v) const;
    std::size_t myHash_char(const std::vector<char>& v) const;
};

struct StateEqual{
    bool operator()(const State* lhs, const State* rhs) const;
};


typedef std::unordered_set<State*,StateHash,StateEqual> MySet;


#endif