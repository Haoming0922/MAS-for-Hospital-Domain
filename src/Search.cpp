# include "Search.h"


void printSearchStatus(FrontierHighLevel& f, std::chrono::high_resolution_clock::time_point startTime){
    auto Time = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(Time - startTime).count();
    std::cerr << "#Generated: " << f.total_size() << "  Time: " << duration/1000.0 << "s" << std::endl;
    // "  #Frontier: " << f.size() 
}


// LowLevel astar search, using shortest path heuristic
std::vector<std::shared_ptr<LowLevel>> lowLevelSearch(std::shared_ptr<LowLevel> ll, HeuristicAStar& h, std::vector<Constrain>& constrains){
    FrontierLowLevel f(h);
    f.add(ll);
    int minimalTime = ll->getMaxConstrainTime(constrains);
    while(!f.ifEmpty()){
        std::shared_ptr<LowLevel> lowLevel = f.pop();
        if(lowLevel->goalCount() == lowLevel->getBoxNum() && lowLevel->time >= minimalTime){
            return lowLevel->getTrack();
        }
        std::vector<std::shared_ptr<LowLevel>> lowLevelExpands = lowLevel->getExpandLowLevel(constrains);
        for(auto lowLevelExpand : lowLevelExpands){
            if(lowLevelExpand->goalCount() == lowLevel->getBoxNum() && lowLevel->time >= minimalTime){
                return lowLevelExpand->getTrack();
            }
            if(lowLevelExpand->goalCount() > lowLevel->goalCount()) {
                f.clear();
                f.add(lowLevelExpand);
                break;
            }
            if(!f.contains(lowLevelExpand) && f.findVisited(lowLevelExpand) == nullptr){
                f.add(lowLevelExpand);
            }
            else if(f.findVisited(lowLevelExpand) != nullptr){
                if(lowLevelExpand->time >= f.findVisited(lowLevelExpand)->time) continue;
                else f.add(lowLevelExpand);
            }
        }
    }
    return {};
}



// high level CBS search
std::vector<std::vector<ActionEnum>> conflictBasedSearch(std::vector<std::shared_ptr<LowLevel>>& lowLevels){
    auto startTime = std::chrono::high_resolution_clock::now();
    int iteration = 0;
    FrontierHighLevel f;
    std::shared_ptr<HighLevelNode> root(new HighLevelNode());
    HeuristicAStar h = HeuristicAStar(shortest_distance, lowLevels[0]->initialState);
    for(auto lowLevel:lowLevels){
        root->lowLevelTracks.emplace_back(lowLevelSearch(lowLevel, h, root->constrains));
    }
    root->setCost();
    f.add(root);
    
    while(!f.ifEmpty()){
        if(++iteration % 1000 == 0) printSearchStatus(f,startTime);
        std::shared_ptr<HighLevelNode> node = f.pop();
        std::vector<Constrain> firstConstrains = node->findFirstConflict();
        if(firstConstrains.empty()){
            printSearchStatus(f,startTime);
            return node->getPlan();
        }
        for(auto constrain : firstConstrains){
            std::shared_ptr<HighLevelNode> nodeCopy(new HighLevelNode(*node));
            nodeCopy->constrains.emplace_back(constrain);
            
            std::vector<std::shared_ptr<LowLevel>> res = lowLevelSearch(lowLevels[constrain.lowLevelID], h, nodeCopy->constrains);        
            if(!res.empty()){
                nodeCopy->lowLevelTracks[constrain.lowLevelID] = res;
                nodeCopy->setCost();
                f.add(nodeCopy);
                if(nodeCopy->findFirstConflict().empty()){
                    printSearchStatus(f,startTime);
                    return nodeCopy->getPlan();
                }
            }
        }
    }
    return {};
}




// std::vector<std::vector<ActionEnum>> GraphSearch(InitialState& initialState, Frontier& f){
//     auto startTime = std::chrono::high_resolution_clock::now();
//     f.add(&initialState);
//     int iteration = 0;
//     while(!f.ifEmpty()){
//         if(++iteration % 10000 == 0) printSearchStatus(f,startTime);
//         State* s = f.pop();
//         if(s->isGoalState()){
//             printSearchStatus(f,startTime);
//             return s->extractPlan();
//         }
//         std::vector<State*> s_expand = s->getExpandState();
//         for(auto s:s_expand){
//             if(s->isGoalState()){
//                 printSearchStatus(f,startTime);
//                 return s->extractPlan();
//             }
//             if(!f.contains(s) && !f.ifVisited(s)){
//                 f.add(s);
//             }
//         }
//     }
//     return {}; 
// }