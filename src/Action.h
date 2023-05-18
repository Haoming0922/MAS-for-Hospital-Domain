#ifndef ACTION_H
#define ACTION_H

# include <string>
# include <map>

/*
    List of possible actions. Each action has the following parameters, 
    taken in order from left to right:
    1. The name of the action as a string. This is the string sent to the server
    when the action is executed. Note that for Pull and Push actions the syntax is
    "Push(X,Y)" and "Pull(X,Y)" with no spaces.
    2. Action type: NoOp, Move, Push or Pull (only NoOp and Move initially supported)
    3. agentRowDelta: the vertical displacement of the agent (-1,0,+1)
    4. agentColDelta: the horisontal displacement of the agent (-1,0,+1)
    5. boxRowDelta: the vertical displacement of the box (-1,0,+1)
    6. boxColDelta: the horisontal discplacement of the box (-1,0,+1) 
    Note: Origo (0,0) is in the upper left corner. So +1 in the vertical direction is down (S) 
    and +1 in the horisontal direction is right (E).
*/

enum ActionEnum{
    OpNo,
    
    MoveN, MoveS, MoveW, MoveE,
    
    PushNN, PushNW, PushNE,
    PushSS, PushSW, PushSE,
    PushEN, PushES, PushEE,
    PushWN, PushWW, PushWS,
    
    PullNN, PullNW, PullNE,
    PullSS, PullSW, PullSE,
    PullEN, PullES, PullEE,
    PullWN, PullWW, PullWS
};
    
enum OpType {NoOp, Move, Push, Pull};

struct ActionCBS{
    ActionEnum a;
    int boxID;
};

struct ActionField{
    std::string name;
    OpType type;
    int agentRowDelta;
    int agentColDelta;
    int boxRowDelta;
    int boxColDelta;
};


extern std::map<ActionEnum,struct ActionField> action;

#endif