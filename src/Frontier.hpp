#ifndef FRONTIER_H
#define FRONTIER_H

# include <string>
# include <queue>
# include <stack>
# include <unordered_set>
# include "CBS.h"
# include "State.h"
# include "Heuristic.h"


class FrontierLowLevel{
    public:
        FrontierLowLevel(Heuristic& heuristic): heuristic(heuristic), pq(Heuristic::HeuristicCompare(heuristic)) {}
        
        void add(std::shared_ptr<LowLevel> ll){
            pq.push(ll);
            pq_set.emplace(ll);
            whole_set.emplace(ll);
        }
        
        std::shared_ptr<LowLevel> pop(){
            std::shared_ptr<LowLevel> ll = pq.top();
            pq.pop();
            pq_set.erase(ll);
            return ll;
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
        
        bool contains(std::shared_ptr<LowLevel> ll){
            return pq_set.count(ll);
        }
        
        std::shared_ptr<LowLevel> findVisited(std::shared_ptr<LowLevel> ll) {
            if(ll->actionCBS.a == OpNo) return nullptr;
            auto it = std::find(whole_set.begin(), whole_set.end(), ll);
            if(it != whole_set.end()) return *it;
            else return nullptr;
        }

        // bool ifVisited(std::shared_ptr<LowLevel> ll){
        //     auto it = std::find(whole_set.begin(), whole_set.end(), ll);
        //     if(it != whole_set.end()){
        //         return *it;
        //     }
        //     else return 
        // }
        
        void clear(){
            while(pq.size() > 0) pq.pop();            
            pq_set.clear();
            whole_set.clear();
        }
        
        std::string getName(){
            return "Low Level Best First Search";
        }
        
    private:
        Heuristic& heuristic;
        std::priority_queue<std::shared_ptr<LowLevel>, std::vector<std::shared_ptr<LowLevel>>, Heuristic::HeuristicCompare> pq;
        MyLowLevelSet pq_set;
        MyLowLevelSet whole_set;
};




class FrontierHighLevel{
    public:
        FrontierHighLevel() : pq(HighLevelNode::HighLevelNodeCompare()), count(0) {}
        
        void add(std::shared_ptr<HighLevelNode> hln){
            pq.push(hln);
            count ++;
        }
        
        std::shared_ptr<HighLevelNode> pop(){
            std::shared_ptr<HighLevelNode> hln = pq.top();
            pq.pop();
            return hln;
        }
        
        bool ifEmpty(){
            return pq.empty();
        }
        
        int total_size(){
            return count;
        }
        
        std::string getName(){
            return "Conflict Based High Level Search";
        }
        
    private:
        std::priority_queue<std::shared_ptr<HighLevelNode>, std::vector<std::shared_ptr<HighLevelNode>>, HighLevelNode::HighLevelNodeCompare> pq;
        int count;
};


#endif




// // interface
// class Frontier{
//     public:
//         virtual void add(State* state) = 0;
//         virtual State* pop() = 0;
//         virtual int size() = 0;
//         virtual int total_size() = 0;
//         virtual bool ifEmpty() = 0;
//         virtual bool contains(State* state) = 0;
//         virtual bool ifVisited(State* state) = 0;
//         virtual std::string getName() = 0;
// };




// class FrontierBFS : public Frontier{
//     public:
//         void add(State* s){
//             queue.push(s);
//             queue_set.emplace(s);
//             whole_set.emplace(s);
//         }
//         State* pop(){
//             State* s = queue.front();
//             queue.pop();
//             queue_set.erase(s);
//             return s;
//         }
//         int size(){
//             return queue.size();
//         }
//         int total_size(){
//             return whole_set.size();
//         }
//         bool ifEmpty(){
//             return queue.empty();
//         }
//         bool contains(State* s){
//             return queue_set.count(s);
//         }
//         bool ifVisited(State* s){
//             return whole_set.count(s);
//         }
//         std::string getName(){
//             return "Breadth First Search";
//         }
        
//     private:
//         std::queue<State*> queue;
//         MyStateSet queue_set;
//         MyStateSet whole_set;
// };




// class FrontierDFS : public Frontier{
//     public:
//         void add(State* s){
//             stack.push(s);
//             stack_set.emplace(s);
//             whole_set.emplace(s);
//         }
//         State* pop(){
//             State* s = stack.top();
//             stack.pop();
//             stack_set.erase(s);
//             return s;
//         }
//         int size(){
//             return stack.size();
//         }
//         int total_size(){
//             return whole_set.size();
//         }
//         bool ifEmpty(){
//             return stack.empty();
//         }
//         bool contains(State* s){
//             return stack_set.count(s);
//         }
//         bool ifVisited(State* s){
//             return whole_set.count(s);
//         }
//         std::string getName(){
//             return "Depth First Search";
//         }
        
//     private:
//         std::stack<State*> stack;
//         MyStateSet stack_set;
//         MyStateSet whole_set;
// };




// class FrontierBestFS : public Frontier{
//     public:
    
//         FrontierBestFS(Heuristic& heuristic): heuristic(heuristic), pq(Heuristic::HeuristicCompare(heuristic)) {}
        
//         void add(State* s){
//             pq.push(s);
//             pq_set.emplace(s);
//             whole_set.emplace(s);
//         }
//         State* pop(){
//             State* s = pq.top();
//             pq.pop();
//             pq_set.erase(s);
//             return s;
//         }
//         int size(){
//             return pq.size();
//         }
//         int total_size(){
//             return whole_set.size();
//         }
//         bool ifEmpty(){
//             return pq.empty();
//         }
//         bool contains(State* s){
//             return pq_set.count(s);
//         }
//         bool ifVisited(State* s){
//             return whole_set.count(s);
//         }
        
//         std::string getName(){
//             return "Best First Search";
//         }
        
//     private:
//         Heuristic& heuristic;
//         std::priority_queue<State*,std::vector<State*>, Heuristic::HeuristicCompare> pq;
//         MyStateSet pq_set;
//         MyStateSet whole_set;
// };