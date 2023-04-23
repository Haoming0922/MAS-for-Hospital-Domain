# include "Heuristic.h"

Heuristic::Heuristic(HeuType t, InitialState* s){
    this->type = t;
    this->initialState = s;
    switch(this->type){
        case(Manhattan):
            std::vector<std::vector<char>> g = initialState->goals;
            for(size_t row=0; row<g.size(); row++){
                for(size_t col=0; col<g[0].size(); col++){
                    char goal = g[row][col];
                    if('A' <= goal && goal <= 'Z'){
                        std::vector<std::vector<int>> heu_table;
                        for(size_t r=0; r<g.size(); r++){
                            for(size_t c=0; c<g[0].size(); c++){
                                heu_table[r][c] = abs(r-row) + abs(c-col);
                            }
                        }
                        manhattan_tables.emplace(goal,heu_table);
                    }
                }
            }
            break;
    }
}

int Heuristic::h(State* s){
    int res = 0;
    switch(this->type){
        case(Manhattan):
            for(size_t row=0; row<s->boxes.size(); row++){
                for(size_t col=0; col<s->boxes[0].size(); col++){
                    char box = s->boxes[row][col];
                    if('A'<=box && box<='Z'){
                        res += this->manhattan_tables.at(box)[row][col];
                    }
                }
            }
            break;
    }
    return res;
}