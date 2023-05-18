# include "CBS.h"

LowLevel::LowLevel(std::shared_ptr<LowLevel> ll, ActionEnum a){
    color = ll->color;
    
    agentID = ll->agentID;
    agentRowLast = ll->agentRow;
    agentColLast = ll->agentCol;
    agentRow = ll->agentRow + action[a].agentRowDelta;
    agentCol = ll->agentCol + action[a].agentColDelta;
    
    boxIDs = ll->boxIDs;
    boxRows = ll->boxRows;
    boxCols = ll->boxCols;
    boxRowsLast = ll->boxRows;
    boxColsLast = ll->boxCols;
    
    ActionCBS actionCBS;
    actionCBS.a = a;
    
    if(action[a].type == Push){
        for(size_t i = 0; i < boxRows.size(); i++){
            for(size_t j = 0; j < boxRows[i].size(); j++){
                if(agentRow==boxRows[i][j] && agentCol==boxCols[i][j]){
                    boxRows[i][j] += action[a].boxRowDelta;
                    boxCols[i][j] += action[a].boxColDelta;
                    actionCBS.boxID = i;
                    break;
                }
            } 
        }
    }
    else if(action[a].type == Pull){
        for(size_t i = 0; i < boxRows.size(); i++){
            for(size_t j = 0; j < boxRows[i].size(); j++){
                if(agentRow - action[a].agentRowDelta - action[a].boxRowDelta == boxRows[i][j] && agentCol - action[a].agentColDelta - action[a].boxColDelta == boxCols[i][j]){
                    boxRows[i][j] += action[a].boxRowDelta;
                    boxCols[i][j] += action[a].boxColDelta;
                    actionCBS.a = a;
                    break;
                } 
            }
        }
    }
    
    this->actionCBS = actionCBS;
    this->parent = ll;
    this->time = ll->time + 1;
    this->initialState = ll->initialState;
    this->boxNum = ll->getBoxNum();
}
              
              

std::vector<std::shared_ptr<LowLevel>> LowLevel::getTrack(){
    std::vector<std::shared_ptr<LowLevel>> lowLevelTrack;
    std::shared_ptr<LowLevel> ll = getptr();
    lowLevelTrack.resize(ll->time);
    while(ll->time != 0){
        lowLevelTrack[ll->time - 1] = ll;
        ll = ll->parent;
    }
    return lowLevelTrack;
}


int LowLevel::goalCount(){
    int goalCount = 0;
    for(size_t i = 0 ; i < boxRows.size() ; i++){
        for(size_t j = 0 ; j < boxRows[i].size(); j++){
            int boxPosition = boxRows[i][j] * initialState->colNum + boxCols[i][j];
            auto it = std::find(initialState->goalIDs[boxIDs[i]].begin(), initialState->goalIDs[boxIDs[i]].end(), boxPosition);
            if(it != initialState->goalIDs[boxIDs[i]].end()) goalCount++;
        }
    }
    return goalCount;
}



std::vector<std::shared_ptr<LowLevel>> LowLevel::getExpandLowLevel(std::vector<Constrain>& constrains){
    std::vector<std::shared_ptr<LowLevel>> result_lowLevels;
    std::vector<ActionEnum> applicableAction = getApplicableAction();
    for(auto a:applicableAction){
        std::shared_ptr<LowLevel> newLowLevel(new LowLevel(getptr(), a));
        if(!newLowLevel->isConstrain(constrains)) result_lowLevels.emplace_back(newLowLevel);
        else{
            std::shared_ptr<LowLevel> newLowLevel(new LowLevel(getptr(), OpNo));
            if(!newLowLevel->isConstrainOpNoEnable(constrains)){
                result_lowLevels.emplace_back(newLowLevel);
            }
        }
    }
    return result_lowLevels;
}



std::vector<ActionEnum> LowLevel::getApplicableAction(){
    std::vector<ActionEnum> result_action;
    for(auto candidate_action:action){
        switch(candidate_action.second.type){
            case(NoOp):
                // result_action.push_back(candidate_action.first);
                break;
            case(Move):
                if(isFree(agentRow + candidate_action.second.agentRowDelta,
                          agentCol + candidate_action.second.agentColDelta))
                    result_action.push_back(candidate_action.first);
                break;
            case(Push):
                if(isFree(agentRow + candidate_action.second.agentRowDelta + candidate_action.second.boxRowDelta,
                          agentCol + candidate_action.second.agentColDelta + candidate_action.second.boxColDelta) && 
                   isNear(candidate_action.second))
                    result_action.push_back(candidate_action.first);
                break;
            case(Pull):
                if(isFree(agentRow + candidate_action.second.agentRowDelta,
                          agentCol + candidate_action.second.agentColDelta) && 
                   isNear(candidate_action.second))
                    result_action.push_back(candidate_action.first);
                break;
        }
    }
    return result_action;
}




bool LowLevel::isFree(int row, int col){
    // check if out of map
    if( row<0 || row>=initialState->rowNum || col<0 || col>=initialState->colNum) return false;
    
    // check if a wall occupys
    if(initialState->walls[row][col]) return false;
    
    // check if a box occupys
    for(size_t i = 0 ; i < boxRows.size() ; i++){
        for(size_t j = 0; j < boxRows[i].size(); j++){
            if(row==boxRows[i][j] && col==boxCols[i][j]) return false;
        }
    }    
    
    
    return true;
}




bool LowLevel::isNear(ActionField a){
    int boxCol, boxRow;
    switch(a.type){
        case(Push):
            boxCol = agentCol + a.agentColDelta;
            boxRow = agentRow + a.agentRowDelta;
            for(size_t i = 0 ; i < boxRows.size() ; i++){
                for(size_t j = 0; j < boxRows[i].size(); j++){
                    if(boxRow==boxRows[i][j] && boxCol==boxCols[i][j])
                        return true;
                }
            }
            break;
        case(Pull):
            boxCol = agentCol - a.boxColDelta;
            boxRow = agentRow - a.boxRowDelta;
            for(size_t i = 0 ; i < boxRows.size() ; i++){
                for(size_t j = 0; j < boxRows[i].size(); j++){
                    if(boxRow==boxRows[i][j] && boxCol==boxCols[i][j])
                        return true;
                }
            }
            break;
        case(NoOp):
            throw std::runtime_error("Error in isNear()");
        case(Move):
            throw std::runtime_error("Error in isNear()");
    }
    return false;
}



bool LowLevel::isConstrain(std::vector<Constrain>& constrains){
    for(auto constrain : constrains){
        if(constrain.lowLevelID == this->agentID && constrain.type == oneWayRoadConflict && (this->time <= 10 && this->time <= constrain.time) ) return true;
        
        if(constrain.lowLevelID != this->agentID || constrain.time != this->time) continue;
        if(constrain.type == vertex && constrain.row == this->agentRow && constrain.col == this->agentCol) return true;
        if(constrain.type == follow && constrain.row == this->agentRow && constrain.col == this->agentCol) return true;
        for(size_t i = 0; i < this->boxRows.size(); i++){
            for(size_t j = 0; j < this->boxRows[i].size(); j++){
                if(constrain.type == vertex && constrain.row == this->boxRows[i][j] && constrain.col == this->boxCols[i][j]) return true;
                if(constrain.type == follow && constrain.row == this->boxRows[i][j] && constrain.col == this->boxCols[i][j]) return true;
            }
        }
    }
    return false;
    // case(box):
    // auto it = std::find(boxIDs.begin(), boxIDs.end(), constrain.ID);
    // int idx = std::distance(boxIDs.begin(), it);
    // for(size_t i = 0; i < boxRows[idx].size(); i++){
    //     if(constrain.lowLevelID == this->agentID && constrain.time == this->time &&
    //        constrain.row == boxRows[idx][i] && constrain.col == boxCols[idx][i]) return true;
    //     break;
    // }
}




bool LowLevel::isConstrainOpNoEnable(std::vector<Constrain>& constrains){
    for(auto constrain : constrains){        
        if(constrain.lowLevelID != this->agentID || constrain.time != this->time) continue;
        if(constrain.type == vertex && constrain.row == this->agentRow && constrain.col == this->agentCol) return true;
        if(constrain.type == follow && constrain.row == this->agentRow && constrain.col == this->agentCol) return true;
        for(size_t i = 0; i < this->boxRows.size(); i++){
            for(size_t j = 0; j < this->boxRows[i].size(); j++){
                if(constrain.type == vertex && constrain.row == this->boxRows[i][j] && constrain.col == this->boxCols[i][j]) return true;
                if(constrain.type == follow && constrain.row == this->boxRows[i][j] && constrain.col == this->boxCols[i][j]) return true;
            }
        }
    }
    return false;   
}




int LowLevel::getMaxConstrainTime(std::vector<Constrain>& constrains){
    int res = 0;
    for(auto c : constrains){
        if(c.lowLevelID == this->agentID && c.time >= res) res = c.time;
    }
    return res;
}


bool LowLevel::isOneWayRoad(int row, int col){
    if( initialState->walls[row][col-1] && initialState->walls[row][col+1] &&
        !initialState->walls[row-1][col] && !initialState->walls[row+1][col] ) return true;
    if( !initialState->walls[row][col-1] && !initialState->walls[row][col+1] &&
        initialState->walls[row-1][col] && initialState->walls[row+1][col] ) return true;
    return false;
}


void HighLevelNode::setCost(){
    int sum = 0;
    for(auto lowLevelTrack : lowLevelTracks){
        sum += lowLevelTrack.size();
    }
    this->cost = sum;
}



std::vector<Constrain> HighLevelNode::findFirstConflict(){
    setJointLowLevels();
    for(int time = 0; time < jointLowLevels.size(); time++){
        std::vector<std::shared_ptr<LowLevel>> jointLowLevel = jointLowLevels[time];
        
        // checks between every two low levels
        for(int i = 0; i < jointLowLevel.size()-1; i++){
            for(int j = i+1; j < jointLowLevel.size(); j++){
                LowLevel l1 = *jointLowLevel[i];
                LowLevel l2 = *jointLowLevel[j];
                                
                // get the constrain candidates
                MyConstrainSet s = {
                    Constrain{ vertex, i, l1.agentRow, l1.agentCol, time+1 },
                    Constrain{ vertex, j, l2.agentRow, l2.agentCol, time+1 },
                    Constrain{ vertex, i, l1.agentRowLast, l1.agentColLast, time },
                    Constrain{ vertex, j, l2.agentRowLast, l2.agentColLast, time }
                };
                for(int k = 0; k < l1.boxRows.size(); k++){
                    for(size_t l = 0; l < l1.boxRows[k].size(); l++){
                        s.emplace( Constrain{ vertex, i, l1.boxRows[k][l], l1.boxCols[k][l], time + 1 } );
                        s.emplace( Constrain{ vertex, i, l1.boxRowsLast[k][l], l1.boxColsLast[k][l], time } );
                    }                
                }
                for(int k = 0; k < l2.boxRows.size(); k++){
                    for(size_t l = 0; l < l2.boxRows[k].size(); l++){
                        s.emplace( Constrain{ vertex, j, l2.boxRows[k][l], l2.boxCols[k][l], time + 1 } );
                        s.emplace( Constrain{ vertex, j, l2.boxRowsLast[k][l], l2.boxColsLast[k][l], time } );
                    }
                }
                
                // check every two constrain candidates and return if exists
                for (auto it1 = s.begin(); it1 != s.end(); ++it1){
                    for (auto it2 = std::next(it1); it2 != s.end(); ++it2){
                        if((*it1).lowLevelID == (*it2).lowLevelID || (*it1).col != (*it2).col || (*it1).row != (*it2).row ) continue;
                        
                        // one-way road constrain
                        if(l1.isOneWayRoad((*it1).row, (*it1).col)){
                            std::vector<Constrain> result = { 
                                Constrain{ oneWayRoadConflict, (*it1).lowLevelID, (*it1).row, (*it1).col, (*it1).time},
                                Constrain{ oneWayRoadConflict, (*it2).lowLevelID, (*it2).row, (*it2).col, (*it2).time} };
                            return result;
                        }
                        
                        // vertex constrain
                        if((*it1).time == (*it2).time){
                            std::vector<Constrain> result = {*it1, *it2};
                            return result;
                        }
                
                        // follow constrain
                        if( (*it1).time == (*it2).time + 1 ){
                            std::vector<Constrain> result = { Constrain{ follow, (*it1).lowLevelID, (*it1).row, (*it1).col, (*it1).time} };
                            return result;
                        }
                        if( (*it1).time == (*it2).time - 1 ){
                            std::vector<Constrain> result = { Constrain{ follow, (*it2).lowLevelID, (*it2).row, (*it2).col, (*it2).time} };
                            return result;
                        }
                    }
                }
            }
        }
    }
    return {};
}


std::vector<std::vector<ActionEnum>> HighLevelNode::getPlan(){
    setJointLowLevels();
    std::vector<std::vector<ActionEnum>> res;
    for(auto jointLowLevel: jointLowLevels){
        std::vector<ActionEnum> jointAction;
        for(auto lowLevel: jointLowLevel){
            jointAction.emplace_back(lowLevel->actionCBS.a);
        }
        res.emplace_back(jointAction);
    }
    return res;
}


void HighLevelNode::setJointLowLevels(){
    jointLowLevels.clear();
    size_t maxTime = 0;
    for(auto lowLevelTrack : lowLevelTracks){
        if(lowLevelTrack.size() >= maxTime) maxTime = lowLevelTrack.size();
    }
    for(size_t i = 0; i < maxTime; i++){
        std::vector<std::shared_ptr<LowLevel>> jointLowLevel;
        for(size_t j = 0; j < lowLevelTracks.size(); j++){
            if( i < lowLevelTracks[j].size()){
                jointLowLevel.emplace_back(lowLevelTracks[j][i]);
            }
            else{
                int lastLevelIdx = lowLevelTracks[j].size() - 1;
                std::shared_ptr<LowLevel> lastLowLevel(new LowLevel(lowLevelTracks[j][lastLevelIdx], OpNo));
                jointLowLevel.emplace_back(lastLowLevel);
            }
        }
        jointLowLevels.emplace_back(std::move(jointLowLevel));
    }
}


std::size_t LowLevelHash::operator()(const std::shared_ptr<LowLevel> ll) const{
    size_t hash = 0;
    hash ^= ll->agentRow + 0x9e3779b9 + (hash << 6) + (hash >> 2);
    hash ^= ll->agentCol + 0x9e3779b9 + (hash << 6) + (hash >> 2);
    for(size_t i = 0 ; i < ll->boxRows.size() ; i++){
        for(size_t j = 0; j < ll->boxRows[i].size(); j++){
            hash ^= ll->boxRows[i][j] + 0x9e3779b9 + (hash << 6) + (hash >> 2);
            hash ^= ll->boxCols[i][j] + 0x9e3779b9 + (hash << 6) + (hash >> 2);
        }
    }
    return hash;
}


bool LowLevelEqual::operator()(const std::shared_ptr<LowLevel> lhs, const std::shared_ptr<LowLevel> rhs) const{
    if( (lhs->agentID != rhs->agentID) || 
        (lhs->agentRow != rhs->agentRow) || 
        (lhs->agentCol != rhs->agentCol) ) return false;
        
    bool equal = true;
    for(size_t i=0; i < lhs->boxRows.size(); i++){
        for(size_t j = 0; j < lhs->boxRows[i].size(); j++){
            equal = equal && (lhs->boxRows[i][j] == rhs->boxRows[i][j]);
            equal = equal && (lhs->boxCols[i][j] == rhs->boxCols[i][j]);
        }
    }
    return equal;
}


std::size_t ConstrainHash::operator()(const Constrain c) const{
    size_t hash = 0;
    hash ^= c.row + 0x9e3779b9 + (hash << 6) + (hash >> 2);
    hash ^= c.col + 0x9e3779b9 + (hash << 6) + (hash >> 2);
    hash ^= c.time + 0x9e3779b9 + (hash << 6) + (hash >> 2);
    return hash;
}


bool ConstrainEqual::operator()(const Constrain lhs, const Constrain rhs) const{
    return (lhs.lowLevelID == rhs.lowLevelID) && (lhs.type == rhs.type) && (lhs.time == rhs.time)
            && (lhs.row == rhs.row) && (lhs.col == rhs.col);
}
