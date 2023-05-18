#ifndef CBS_H
#define CBS_H


# include <iostream>
# include <vector>
# include <algorithm>
# include <iterator>
# include <unordered_set>
# include <memory>
# include <functional>  // For std::hash
# include <stdexcept>
# include "Color.h"
# include "Action.h"
# include "State.h"


enum ConstrainType{
    vertex, // bump into other's current position 
    follow, // bump into other's last position
    oneWayRoadConflict,
    oneWayRoadBlock
};


// a constrain means: In one lowlevel, at time t, a agent/box can not be at (row, col)
struct Constrain{
    ConstrainType type;
    int lowLevelID;
    int row;
    int col;
    int time;
};


// a lowlevel contains only one agent, and the corresponding boxes of it (ignore all other agents and boxes)
class LowLevel : public std::enable_shared_from_this<LowLevel>{
    public:        
        Color color;
        int agentID;
        std::vector<int> boxIDs; 
        
        // agent and box position at current time t
        int agentRow;
        int agentCol;
        std::vector<std::vector<int>> boxRows;
        std::vector<std::vector<int>> boxCols;    
        
        // agent and box position at last time step t-1
        int agentRowLast; 
        int agentColLast;
        std::vector<std::vector<int>> boxRowsLast;
        std::vector<std::vector<int>> boxColsLast;    
        
        // action applied from last time step t-1 to current time step t
        ActionCBS actionCBS; 
        
        // lowlevel at last time step t-1
        std::shared_ptr<LowLevel> parent;
        
        // current time step t
        int time;
        
        std::shared_ptr<InitialState> initialState;
        
        LowLevel(){parent = NULL; time = 0;};
        LowLevel(std::shared_ptr<LowLevel> ll, ActionEnum a);
        
        std::vector<std::shared_ptr<LowLevel>> getExpandLowLevel(std::vector<Constrain>& constrains);
        std::vector<std::shared_ptr<LowLevel>> getTrack(); // if reach goal state, get the lowlevels along the way
        int goalCount();
        int getMaxConstrainTime(std::vector<Constrain>& constrains); // get the maximal time of all its constrains
        std::shared_ptr<LowLevel> getptr() { return shared_from_this();}
        bool isOneWayRoad(int row, int col);
        void setBoxNum(int n) {this->boxNum = n;};
        int getBoxNum() { return this->boxNum;};
              
    private:
        std::vector<ActionEnum> getApplicableAction();
        int boxNum;
        
        // Is (row, col) free and within map ?
        bool isFree(int row, int col);
        
        // Is box at the correct position to be push/pull
        bool isNear(ActionField a);
        
        // Is the level meet all the constains?
        bool isConstrain(std::vector<Constrain>& constrains);
        
        bool isConstrainOpNoEnable(std::vector<Constrain>& constrains);      
};




// a high level node is used for merging all the lowlevels and solving the constrains
class HighLevelNode{
    public:
        HighLevelNode() {cost = 0;}
        
        std::vector<std::vector<std::shared_ptr<LowLevel>>> lowLevelTracks;
        std::vector<Constrain> constrains;
        int cost;
        
        void setCost();
        // bool isValid(); // check if all lowlevels have constains or not
        
        // get the first constrains, return nullptr or a vector of length 2
        std::vector<Constrain> findFirstConflict(); 
        
        // if success, get the plan
        std::vector<std::vector<ActionEnum>> getPlan(); 
        
        // used for priority queue sorting
        struct HighLevelNodeCompare{
            bool operator()(std::shared_ptr<HighLevelNode> l, std::shared_ptr<HighLevelNode> r) const{
                return l->cost > r->cost;
            };
        };
    
    private:
        std::vector<std::vector<std::shared_ptr<LowLevel>>> jointLowLevels;
        
        // transform the format from vector of lowlevels to vector of time
        void setJointLowLevels();
};


struct LowLevelHash{
    std::size_t operator()(const std::shared_ptr<LowLevel> ll) const;
};
struct LowLevelEqual{
    bool operator()(const std::shared_ptr<LowLevel> lhs, const std::shared_ptr<LowLevel> rhs) const;
};
typedef std::unordered_set<std::shared_ptr<LowLevel>, LowLevelHash, LowLevelEqual> MyLowLevelSet;


struct ConstrainHash{
    std::size_t operator()(const Constrain c) const;
};
struct ConstrainEqual{
    bool operator()(const Constrain lhs, const Constrain rhs) const;
};
typedef std::unordered_set<Constrain, ConstrainHash, ConstrainEqual> MyConstrainSet;



#endif