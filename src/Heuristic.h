#ifndef HEURISTIC_H
#define HEURISTIC_H

# include <cmath>
# include "State.h"


// list of heuristic function
enum HeuType{
    Manhattan
};


// base class 
class Heuristic{
    public:
        Heuristic(HeuType t, InitialState* s);
        int h(State* s);
        virtual int f(State* s)=0;
        virtual std::string getName()=0;
        struct HeuristicCompare{
            Heuristic& heuristic;
            HeuristicCompare(Heuristic& h) : heuristic(h) {}
            bool operator()(State* l, State* r) const{
                return heuristic.f(l) < heuristic.f(r);
            };
        };
    private:
        HeuType type;
        std::map<char, std::vector<std::vector<int>>> manhattan_tables;
        const InitialState* initialState;
};






// three type of heuristic
class HeuristicAStar : public Heuristic {
    public:
        HeuristicAStar(HeuType t, InitialState* s):Heuristic(t,s) {};
        int f(State* s) override {return s->getDepth() + this->h(s);};
        std::string getName() override {return "Heuristic AStar";};
};

class HeuristicWeightedAStar : public Heuristic {
    public:
        HeuristicWeightedAStar(HeuType t, InitialState* s):Heuristic(t,s) {this->w = 5;};
        int f(State* s) override {return s->getDepth() + this->h(s) + this->w;};
        std::string getName() override {return "Heuristic Weighted AStar";};
    private:
        int w;
};

class HeuristicGreedy : public Heuristic {
    public:
        HeuristicGreedy(HeuType t, InitialState* s):Heuristic(t,s) {};
        int f(State* s) override {return this->h(s);};
        std::string getName() override {return "Heuristic Greedy";};
};

#endif