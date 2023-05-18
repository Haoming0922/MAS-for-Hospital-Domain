#ifndef STATE_H
#define STATE_H

# include <iostream>
# include <vector>
# include <unordered_set>
# include <functional>  // For std::hash
# include <stdexcept>
# include "Color.h"
# include "Action.h"

class State{
    public:
        int rowNum;
        int colNum;
        int agentNum;
        int boxNum;
        
        // agentRows[i] = r means agent i at row r
        std::vector<int> agentRows;
        std::vector<int> agentCols;
        // box[r][c]= char or 0
        std::vector<std::vector<char>> boxes; 
        
        State* parent;
        State* initialState;
        int depth;
        
        State(int rowNum, int colNum, int agentNum, int boxNum, std::vector<int> agentRows, std::vector<int> agentCols, 
              std::vector<std::vector<char>> boxes);
        State(State* parent, std::vector<ActionEnum> jointAction);
        
        std::vector<std::vector<ActionEnum>> extractPlan();
        bool isGoalState();
        std::vector<State*> getExpandState();     
        int coordEncode(int x, int y) {return x*colNum + y;};
        
    private:
        std::vector<ActionEnum> jointAction;
   
        std::vector<ActionEnum> getApplicableAction(int agentIdx);
        bool isConflict(std::vector<ActionEnum> jointAction);
        bool isFree(int row, int col);
        bool isNear(int agentIdx, ActionField a);
};


// initial state: constains all information of the level (walls, goals)
class InitialState : public State{
    public:
        InitialState(int rowNum, int colNum, int agentNum, int boxNum, std::vector<int> agentRows, std::vector<int> agentCols, 
        std::vector<std::vector<char>> boxes, std::vector<Color> agentColors, std::vector<Color> boxColor,
        std::vector<std::vector<bool>> walls, std::vector<std::vector<char>> goals);
        // agentColors[i] = color means agent i is color
        std::vector<Color> agentColors;
        // box[char-'0'] = color
        std::vector<Color> boxColor;
        // same as box[][]
        std::vector<std::vector<bool>> walls;
        std::vector<std::vector<char>> goals;
        
        std::vector<std::vector<int>> goalIDs;
};


struct StateHash{
    std::size_t operator()(const State* s) const;
    std::size_t myHash_int(const std::vector<int>& v) const;
    std::size_t myHash_char(const std::vector<char>& v) const;
};

struct StateEqual{
    bool operator()(const State* lhs, const State* rhs) const;
};


typedef std::unordered_set<State*,StateHash,StateEqual> MyStateSet;

#endif