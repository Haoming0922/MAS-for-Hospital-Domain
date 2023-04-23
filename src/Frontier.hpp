#ifndef FRONTIER_H
#define FRONTIER_H

# include <string>
# include <queue>
# include <stack>
# include <unordered_set>

# include "State.h"
# include "Heuristic.h"

// interface
class Frontier{
    public:
        virtual void add(State* state) = 0;
        virtual State* pop() = 0;
        virtual int size() = 0;
        virtual int total_size() = 0;
        virtual bool ifEmpty() = 0;
        virtual bool contains(State* state) = 0;
        virtual bool ifVisited(State* state) = 0;
        virtual std::string getName() = 0;
};


class FrontierBFS : public Frontier{
    public:
        void add(State* s){
            queue.push(s);
            queue_set.emplace(s);
            whole_set.emplace(s);
        }
        State* pop(){
            State* s = queue.front();
            queue.pop();
            queue_set.erase(s);
            return s;
        }
        int size(){
            return queue.size();
        }
        int total_size(){
            return whole_set.size();
        }
        bool ifEmpty(){
            return queue.empty();
        }
        bool contains(State* s){
            return queue_set.count(s);
        }
        bool ifVisited(State* s){
            return whole_set.count(s);
        }
        std::string getName(){
            return "Breadth First Search";
        }
        
    private:
        std::queue<State*> queue;
        MySet queue_set;
        MySet whole_set;
};


class FrontierDFS : public Frontier{
    public:
        void add(State* s){
            stack.push(s);
            stack_set.emplace(s);
        }
        State* pop(){
            State* s = stack.top();
            stack.pop();
            stack_set.erase(s);
            return s;
        }
        int size(){
            return stack.size();
        }
        int total_size(){
            return whole_set.size();
        }
        bool ifEmpty(){
            return stack.empty();
        }
        bool contains(State* s){
            return stack_set.count(s);
        }
        bool ifVisited(State* s){
            return whole_set.count(s);
        }
        std::string getName(){
            return "Depth First Search";
        }
        
    private:
        std::stack<State*> stack;
        MySet stack_set;
        MySet whole_set;
};


class FrontierBestFS : public Frontier{
    public:
    
        FrontierBestFS(Heuristic& heuristic): heuristic(heuristic), pq(Heuristic::HeuristicCompare(heuristic)) {}
        
        void add(State* s){
            pq.push(s);
            pq_set.emplace(s);
        }
        State* pop(){
            State* s = pq.top();
            pq.pop();
            pq_set.erase(s);
            return s;
        }
        int size(){
            return pq.size();
        }
        int total_size(){
            return whole_set.size();
        }
        bool ifEmpty(){
            return pq.empty();
        }
        bool contains(State* s){
            return pq_set.count(s);
        }
        bool ifVisited(State* s){
            return whole_set.count(s);
        }
        
        std::string getName(){
            return "Best First Search using " + this->getName();
        }
        
    private:
        Heuristic& heuristic;
        std::priority_queue<State*,std::vector<State*>, Heuristic::HeuristicCompare> pq;
        MySet pq_set;
        MySet whole_set;
};

#endif