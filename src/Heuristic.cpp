# include "Heuristic.h"

Heuristic::Heuristic(HeuType t, std::shared_ptr<InitialState> s){
    this->type = t;
    this->initialState = s;
    switch(this->type){
        case(shortest_distance):
            // Floyd–Warshall algorithm
            opt = std::make_shared<std::vector<std::vector<int>>>(initialState->rowNum * initialState->colNum, std::vector<int>(initialState->rowNum * initialState->colNum, MAXOPT));
            for(int i = 0 ; i < initialState->rowNum * initialState->colNum ; i++){
                (*opt)[i][i] = 0;
                if(i % initialState->colNum != 0 && !initialState->walls[i / initialState->colNum][i % initialState->colNum - 1]) (*opt)[i][i-1] = 1;
                if((i+1) % initialState->colNum != 0 && !initialState->walls[i / initialState->colNum][i % initialState->colNum + 1]) (*opt)[i][i+1] = 1;
                if(i - initialState->colNum >= 0 && !initialState->walls[i / initialState->colNum - 1][i % initialState->colNum]) (*opt)[i][i - initialState->colNum] = 1;
                if(i + initialState->colNum < initialState->rowNum * initialState->colNum && !initialState->walls[i / initialState->colNum + 1][i % initialState->colNum]) (*opt)[i][i + initialState->colNum] = 1;
            }                
            for(int k = 0 ; k < initialState->rowNum * initialState->colNum ; k++){
                for(int i = 0 ; i < initialState->rowNum * initialState->colNum ; i++){
                    for(int j = 0 ; j < initialState->rowNum * initialState->colNum ; j++){
                        if((*opt)[i][j] > (*opt)[i][k] + (*opt)[k][j] ) (*opt)[i][j] = (*opt)[i][k] + (*opt)[k][j];
                    }
                }
            }
            
            std::vector<std::vector<char>> g = initialState->goals;
            initialState->goalIDs.resize(initialState->boxNum);
            shortestDistance.resize(initialState->boxNum);
            for(int i = 0 ; i < initialState->rowNum; i++){
                for(int j = 0 ; j < initialState->colNum; j++){
                    char goal = g[i][j];
                    if('A' <= goal && goal <= 'Z'){
                        int goalBoxIdx = initialState->coordEncode(i,j);
                        if(goal - 'A' >= initialState->goalIDs.size()){
                            initialState->goalIDs.resize(goal - 'A' + 1);
                        }
                        initialState->goalIDs[goal - 'A'].push_back(goalBoxIdx);
                        shortestDistance[goal-'A'].push_back((*opt)[goalBoxIdx]);
                    }
                }
            }
            
            break;
    }
}


int Heuristic::h(std::shared_ptr<LowLevel> ll){
    int shortestDis = 0;
    int minDistanceAgentBox = MAXOPT;
    switch(this->type){        
        case(shortest_distance):
            for(size_t i = 0; i < ll->boxIDs.size(); i++){
                for(size_t j = 0; j < ll->boxRows[i].size(); j++){
                   int dis = this->shortestDistance[ll->boxIDs[i]][j][ll->boxRows[i][j] * initialState->colNum + ll->boxCols[i][j]];
                   shortestDis += dis;
                   if(dis!=0) minDistanceAgentBox = std::min( minDistanceAgentBox, 
                                                    (*opt) [ll->boxRows[i][j] * initialState->colNum + ll->boxCols[i][j]] [ll->agentRow * initialState->colNum + ll->agentCol] );    
                }
            }
            break;
    } 
    // return shortestDis;
    return - shortestDis - minDistanceAgentBox + ll->goalCount() * 100;
}


// int Heuristic::h(State* s){
//     int res = 0;
//     switch(thiinitialState->type){
//         case(Manhattan):
//             for(size_t row=0; row<initialState->boxes.size(); row++){
//                 for(size_t col=0; col<initialState->boxes[0].size(); col++){
//                     char box = initialState->boxes[row][col];
//                     if('A'<=box && box<='Z'){
//                         res += thiinitialState->manhattan_tables.at(box)[row][col];
//                     }
//                 }
//             }
//             break;
//     } 
//     return res;
// }

// case(Manhattan):
//     std::vector<std::vector<char>> g = initialState->goals;
//     for(size_t row=0; row<g.size(); row++){
//         for(size_t col=0; col<g[0].size(); col++){
//             char goal = g[row][col];
//             if('A' <= goal && goal <= 'Z'){
//                 std::vector<std::vector<int>> heu_table;
//                 for(size_t r=0; r<g.size(); r++){
//                     for(size_t c=0; c<g[0].size(); c++){
//                         heu_table[r][c] = abs(r-row) + abs(c-col);
//                     }
//                 }
//                 manhattan_tables.emplace(goal,heu_table);
//             }
//         }
//     }
//     break;