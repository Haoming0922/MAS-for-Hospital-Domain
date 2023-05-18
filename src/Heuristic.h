#ifndef HEURISTIC_H
#define HEURISTIC_H

# include <cmath>
# include "State.h"
# include "CBS.h"

#define MAXOPT 10000

// list of heuristic function
enum HeuType{
    Manhattan,
    shortest_distance
};



// Heuristic Function = goalCounts * 100 - shortest distance between boxes to goals - minimal distance between agent and box

// position (row, col) is encoded as row * ColNum + col



// base class
class Heuristic{
    public:
        Heuristic(HeuType t, std::shared_ptr<InitialState> s);
        int h(std::shared_ptr<LowLevel> ll);
        // int h(State* s);
        virtual int f(std::shared_ptr<LowLevel> ll)=0;
        virtual std::string getName()=0;
        struct HeuristicCompare{
            Heuristic& heuristic;
            HeuristicCompare(Heuristic& h) : heuristic(h) {};
            bool operator()(std::shared_ptr<LowLevel> l, std::shared_ptr<LowLevel> r) const{
                return heuristic.f(l) < heuristic.f(r);
            };
        };
    private:
        HeuType type;
        // std::map<char, std::vector<std::vector<int>>> manhattan_tables;
        std::vector<std::vector<std::vector<int>>> shortestDistance; // shortestDistance[i][j][k] = shortest distance for bix j of type i at position k to its goal 
        std::shared_ptr<std::vector<std::vector<int>>> opt; // opt[i][j] = shortest distance from position i to position j
        std::shared_ptr<InitialState> initialState;                                     
};



// three type of heuristic
class HeuristicAStar : public Heuristic {
    public:
        HeuristicAStar(HeuType t, std::shared_ptr<InitialState> s):Heuristic(t,s) {};
        int f(std::shared_ptr<LowLevel> ll) override {return ll->time + this->h(ll);};
        std::string getName() override {return "Heuristic AStar";};
};

class HeuristicWeightedAStar : public Heuristic {
    public:
        HeuristicWeightedAStar(HeuType t, std::shared_ptr<InitialState> s):Heuristic(t,s) {this->w = 5;};
        int f(std::shared_ptr<LowLevel> ll) override {return ll->time + this->h(ll) * this->w;};
        std::string getName() override {return "Heuristic Weighted AStar";};
    private:
        int w;
};

class HeuristicGreedy : public Heuristic {
    public:
        HeuristicGreedy(HeuType t, std::shared_ptr<InitialState> s):Heuristic(t,s) {};
        int f(std::shared_ptr<LowLevel> ll) override {return this->h(ll);};
        std::string getName() override {return "Heuristic Greedy";};
};

#endif