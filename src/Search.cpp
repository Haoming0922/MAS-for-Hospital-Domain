# include "Search.h"

void printSearchStatus(Frontier& f, std::chrono::high_resolution_clock::time_point startTime){
    //TODO: memory status
    auto Time = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(Time - startTime).count();
    std::cerr << "#Generated: " << f.total_size() << "  #Frontier: " << f.size() 
              << "  Time: " << duration/1000.0 << "s" << std::endl;
}

std::vector<std::vector<actionEnum>> GraphSearch(InitialState& initialState, Frontier& f){
    auto startTime = std::chrono::high_resolution_clock::now();
    f.add(&initialState);
    int iteration = 0;
    while(!f.ifEmpty()){
        if(++iteration % 10000 == 0) printSearchStatus(f,startTime);
        State* s = f.pop();
        if(s->isGoalState()){
            printSearchStatus(f,startTime);
            return s->extractPlan();
        }
        std::vector<State*> s_expand = s->getExpandState();
        for(auto s:s_expand){
            if(s->isGoalState()){
                printSearchStatus(f,startTime);
                return s->extractPlan();
            }
            if(!f.contains(s) && !f.ifVisited(s)){
                f.add(s);
            }
        }
    }
    return {}; 
}

