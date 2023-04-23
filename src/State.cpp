# include "State.h"

State::State(int RowNum, int ColNum, std::vector<int> agentRows, 
             std::vector<int> agentCols, std::vector<std::vector<char>> boxes){
    this->RowNum = RowNum;
    this->ColNum = ColNum; 
    this->agentRows = agentRows;
    this->agentCols = agentCols;
    this->boxes = boxes;
}

State::State(State* parent, std::vector<actionEnum> jointAction){
    // copy parent
    this->RowNum = parent->RowNum;
    this->ColNum = parent->ColNum;
    this->agentRows = parent->agentRows;
    this->agentCols = parent->agentCols;
    this->boxes = parent->boxes;
    
    // set own parameters
    this->parent = parent;
    this->depth = this->parent->depth + 1;
    this->jointAction = jointAction;
    this->initialState = parent->initialState;
    
    // apply action
    int agentIdx = 0, boxRow,boxCol;
    char box;
    for(auto a:jointAction){
        // std::cout << action.at(a).name << ' ' << std::endl;
        switch(action.at(a).type){
            case NoOp:
                break;
            case Move:
                // move agent
                agentRows[agentIdx] += action.at(a).agentRowDelta;
                agentCols[agentIdx] += action.at(a).agentColDelta;
                break;
            case Push:
                // move agent
                agentRows[agentIdx] += action.at(a).agentRowDelta;
                agentCols[agentIdx] += action.at(a).agentColDelta;
                
                // empty box old location
                boxRow = agentRows[agentIdx];
                boxCol = agentCols[agentIdx];
                box = boxes[boxRow][boxCol];
                boxes[boxRow][boxCol] = 0;
                
                // update box new location
                boxRow += action.at(a).boxRowDelta;
                boxCol += action.at(a).boxColDelta;
                boxes[boxRow][boxCol] = box;
                break;
            case Pull:
                // empty box old location
                boxRow = agentRows[agentIdx] - action.at(a).boxRowDelta;
                boxCol = agentCols[agentIdx] - action.at(a).boxColDelta;
                box = boxes[boxRow][boxCol];
                boxes[boxRow][boxCol] = 0;
                
                // update box new location
                boxRow = agentRows[agentIdx];
                boxCol = agentCols[agentIdx];
                boxes[boxRow][boxCol] = box;
                
                // move agent
                agentRows[agentIdx] += action.at(a).agentRowDelta;
                agentCols[agentIdx] += action.at(a).agentColDelta;
                break;
        }
        agentIdx++;
    }
}




InitialState::InitialState(int RowNum, int ColNum,std::vector<int> agentRows, std::vector<int> agentCols, 
        std::vector<std::vector<char>> boxes,std::vector<Color> agentColors, std::vector<Color> boxColor,
        std::vector<std::vector<bool>> walls, std::vector<std::vector<char>> goals) : State(RowNum,ColNum,agentRows,agentCols,boxes){
    this->agentColors = agentColors;
    this->boxColor = boxColor; 
    this->walls = walls;
    this->goals = goals;
    this->parent = NULL;
    this->initialState = this;
    this->depth = 0;
}




bool State::isGoalState(){
    for(int row = 0; row<RowNum; row++){
        for(int col = 0; col<ColNum; col++){
            // agent
            if(static_cast<InitialState*>(initialState)->goals[row][col] >= '0' && static_cast<InitialState*>(initialState)->goals[row][col] <= '9'){
                int agentIdx = static_cast<InitialState*>(initialState)->goals[row][col];
                if(agentRows[agentIdx] != row || agentRows[agentIdx] != col) return false;
            }
            // box
            if(static_cast<InitialState*>(initialState)->goals[row][col] >= 'A' && static_cast<InitialState*>(initialState)->goals[row][col] <= 'Z'){
                if(boxes[row][col] != static_cast<InitialState*>(initialState)->goals[row][col]) return false;
            }
        }
    }
    return true;
}



std::vector<State*> State::getExpandState(){

    std::vector<State*> result_state;
    
    // get applicable actions for each agent
    std::vector<std::vector<actionEnum>> applicableAction;
    for(size_t i=0;i<this->agentRows.size();i++) applicableAction.push_back(getApplicableAction(i));
    
    std::vector<size_t> actionPermutation(agentRows.size(),0);
    bool done = false;
    while(true){
        // join actions 
        std::vector<actionEnum> jointAction;
        for(size_t i=0;i<this->agentRows.size();i++){
            jointAction.emplace_back(applicableAction[i][actionPermutation[i]]);
        }
        
        // check conflicts
        if(!isConflict(jointAction)){
            State* newState = new State(this,jointAction);
            result_state.emplace_back(newState);
        }
        
        // enumerate all
        for(size_t i=0;i<this->agentRows.size();i++){
            if(actionPermutation[i] < applicableAction[i].size()-1){
                actionPermutation[i]++;
                break;
            }
            else{
                actionPermutation[i] = 0;
                if(i == agentRows.size()-1) done = true;
            }
        }
        if(done) break;
    }
    return result_state;
}



bool State::isConflict(std::vector<actionEnum> jointAction){
    if(jointAction.size()==1) return false;
    
    // check every two-action are conflict or not
    std::unordered_set<int> coord {}; // coordinate encode = i*#col + j
    for(size_t i=0;i<jointAction.size()-1;i++){
        for(size_t j=i+1;j<jointAction.size();j++){
            actionEnum a1 = jointAction[i];
            actionEnum a2 = jointAction[j];

            if(a1==OpNo || a2==OpNo) continue;
            
            int insert = 0, size1 = 0, size2 = 0;
            
            // agent and box coordinate of action a1
            coord.insert(coordEncode(agentRows[i],agentCols[i]));
            coord.insert(coordEncode(agentRows[i] + action[a1].agentRowDelta,agentCols[i] + action[a1].agentColDelta)); 
            if(action[a1].type==Push) coord.insert(coordEncode(agentRows[i] + action[a1].agentRowDelta + action[a1].boxRowDelta, agentCols[i] + action[a1].agentColDelta + action[a1].boxColDelta));
            if(action[a1].type==Pull) coord.insert(coordEncode(agentRows[i] - action[a1].boxRowDelta, agentCols[i] - action[a1].boxColDelta));
            size1 = coord.size();
            
            // agent and box coordinate of action a2
            coord.insert(coordEncode(agentRows[j],agentCols[j]));
            coord.insert(coordEncode(agentRows[j] + action[a2].agentRowDelta, agentCols[j] + action[a2].agentColDelta)); 
            if(action[a1].type==Push){
                coord.insert(coordEncode(agentRows[j] + action[a2].agentRowDelta + action[a2].boxRowDelta, agentCols[j] + action[a2].agentColDelta + action[a2].boxColDelta));
                insert++;
            }
            if(action[a1].type==Pull){
                coord.insert(coordEncode(agentRows[j] - action[a2].boxRowDelta, agentCols[j] - action[a2].boxColDelta));
                insert++;
            }
            size2 = coord.size();
            
            // if set increased size < inserting times of a2, => repeated coordinate => conflict
            if(size2-size1 < insert+2) return true;
            coord.clear();
        }
    }
    return false;
}


std::vector<actionEnum> State::getApplicableAction(int agentIdx){
    std::vector<actionEnum> result_action;
    for(auto candidate_action:action){
        switch(candidate_action.second.type){
            case(NoOp):
                result_action.push_back(candidate_action.first);
                break;
            case(Move):
                if(isFree(agentRows[agentIdx] + candidate_action.second.agentRowDelta,
                          agentCols[agentIdx] + candidate_action.second.agentColDelta))
                  result_action.push_back(candidate_action.first);
                break;
            case(Push):
                if(isFree(agentRows[agentIdx] + candidate_action.second.agentRowDelta + candidate_action.second.boxRowDelta,
                          agentCols[agentIdx] + candidate_action.second.agentColDelta + candidate_action.second.boxColDelta) && 
                   isNear(agentIdx,candidate_action.second)){
                  result_action.push_back(candidate_action.first);        
                    }
                break;
            case(Pull):
                if(isFree(agentRows[agentIdx] + candidate_action.second.agentRowDelta,
                          agentCols[agentIdx] + candidate_action.second.agentColDelta) && 
                   isNear(agentIdx,candidate_action.second))
                  result_action.push_back(candidate_action.first);
                break;
        }
    }
    return result_action;
}



bool State::isFree(int row, int col){
    // check if out of map
    if( row<0 || row>=RowNum || col<0 || col>=ColNum) return false;
    // check if an agent occupys
    for(size_t i=0; i<agentRows.size(); i++){
        if(row==agentRows[i] && col==agentCols[i]) return false;
    }
    // check if a wall occupys
    if(static_cast<InitialState*>(initialState)->walls[row][col]) return false;
    // check if a box occupys
    if(boxes[row][col]) return false;
    
    return true;
}



bool State::isNear(int agentIdx, actionField a){
    // box is at the correct position to be push/pull
    // box and agent are of the same color
    int boxCol, boxRow;
    switch(a.type){
        case(Push):
            boxCol = agentCols[agentIdx] + a.agentColDelta;
            boxRow = agentRows[agentIdx] + a.agentRowDelta;
            if(boxes[boxRow][boxCol] && static_cast<InitialState*>(initialState)->boxColor[boxes[boxRow][boxCol]-'A'] == static_cast<InitialState*>(initialState)->agentColors[agentIdx]) return true;
            break;
        case(Pull):
            boxCol = agentCols[agentIdx] - a.boxColDelta;
            boxRow = agentRows[agentIdx] - a.boxRowDelta;
            if(boxes[boxRow][boxCol] && static_cast<InitialState*>(initialState)->boxColor[boxes[boxRow][boxCol]-'A'] == static_cast<InitialState*>(initialState)->agentColors[agentIdx]) return true;
            break;
        case(NoOp):
            throw std::runtime_error("Error in isNear()");
        case(Move):
            throw std::runtime_error("Error in isNear()");
    }
    return false;
}


std::vector<std::vector<actionEnum>> State::extractPlan(){
    std::vector<std::vector<actionEnum>> plan;
    State* s = this;
    plan.resize(s->depth);
    while(s->depth != 0){
        plan[s->depth - 1] = s->jointAction;
        s = s->parent;
    }
    return plan;
}


size_t StateHash::operator()(const State* s) const{
    size_t hash = 0;
    hash ^= myHash_int(s->agentRows) + 0x9e3779b9 + (hash << 6) + (hash >> 2);
    hash ^= myHash_int(s->agentCols) + 0x9e3779b9 + (hash << 6) + (hash >> 2);
    for(size_t i=0;i<s->boxes.size();i++){
        hash ^= myHash_char(s->boxes[i]) + 0x9e3779b9 + (hash << 6) + (hash >> 2);
    }
    return hash;
}


bool StateEqual::operator()(const State* lhs, const State* rhs) const{
    bool equal = true;
    for(size_t i=0; i < lhs->boxes.size(); i++){
        equal = equal && (lhs->boxes[i] == rhs->boxes[i]);
    }
    return equal && (lhs->agentRows == rhs->agentRows) && (lhs->agentCols == rhs->agentCols);
}


size_t StateHash::myHash_int(const std::vector<int>& v) const{
    std::hash<int> hash_fn;
    size_t hash = 0;
    for (size_t i = 0; i < v.size(); ++i) {
        hash ^= hash_fn(v[i]) + 0x9e3779b9 + (hash << 6) + (hash >> 2);
    }
    return hash;
}


size_t StateHash::myHash_char(const std::vector<char>& v) const{
    std::hash<char> hash_fn;
    size_t hash = 0;
    for (size_t i = 0; i < v.size(); ++i) {
        hash ^= hash_fn(v[i]) + 0x9e3779b9 + (hash << 6) + (hash >> 2);
    }
    return hash;
}