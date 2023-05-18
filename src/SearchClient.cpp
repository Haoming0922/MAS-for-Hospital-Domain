# include <iostream>
# include <fstream>
# include <algorithm>
# include <string>
# include <vector>
# include <cstring>
# include <stdexcept>
# include <memory>
# include "Search.h"

std::vector<std::shared_ptr<LowLevel>> parseLevel();

int main(int argc, char *argv[]){
    
    // parse input, get low levels
    std::vector<std::shared_ptr<LowLevel>> lowLevels = parseLevel();
    
    // use CBS to search for plan
    std::vector<std::vector<ActionEnum>> plan;
    try{
        plan = conflictBasedSearch(lowLevels);
        if(!plan.empty()) std::cerr << "Found solution of length " << plan.size() << std::endl;
        else std::cerr << "Unable to find solution" << std::endl;
    } catch(std::bad_alloc& e)
    {
        std::cerr << "Error: " << e.what() << " - Out of memory! Unable to solve level." << std::endl;
        plan = {};
    }

    // print the result to server
    if(!plan.empty()){
        for(auto jointaction : plan){
            std::cout << action[jointaction[0]].name;
            if(jointaction.size()>1){
                for(size_t i=1;i<jointaction.size();i++){
                    std::cout << '|' << action[jointaction[i]].name;
                }
            }
            std::cout << std::endl;
            // We must read the server's response to not fill up the stdin buffer and block the server.
            std::string line;
            std::cin >> line;
        }
    }

    return 0;
}




std::vector<std::shared_ptr<LowLevel>> parseLevel(){
    // Send client name to server
    std::cout << "SearchClient" << std::endl;
    
    std::vector<std::shared_ptr<LowLevel>> lowLevels(10);
    
    std::string line;
    
    getline(std::cin, line); // #domain
    getline(std::cin, line); // hospital
    getline(std::cin, line); // #levelname
    getline(std::cin, line); // <name>
    
    // Parse color
    getline(std::cin, line); // #colors
    std::vector<Color> agentColors(10);
    std::vector<Color> boxColors(26);
    size_t pos = 0;
    int numAgent = 0, numBox = 0;
    bool isAgent = false;
    getline(std::cin, line);
    while (line[0]!='#') {
        line.erase(std::remove_if(line.begin(), line.end(), ::isspace), line.end());
        pos = line.find(":");
        Color color = ColorMap.at(line.substr(0,pos));
        line.erase(0, pos + 1);
        while (true) {
            if(line.find(",") != std::string::npos) pos = line.find(",");
            else pos = line.size();
            std::string token = line.substr(0, pos);
            
            if(token[0] >= '0' && token[0] <= '9') { agentColors[token[0] - '0'] = color; numAgent++; isAgent = true;}
            if(token[0] >= 'A' && token[0] <= 'Z') { boxColors[token[0] - 'A'] = color; numBox++;}
            
            if(isAgent){
                std::shared_ptr<LowLevel> l(new LowLevel);
                l->agentID = token[0] - '0';
                l->color = color;
                lowLevels[l->agentID] = l;
                isAgent = false;
            }
            
            line.erase(0, pos + 1);
            if(line.size()==0) break;
        }
        getline(std::cin, line);
    }
    agentColors.erase(agentColors.begin() + numAgent, agentColors.end());
    boxColors.erase(boxColors.begin() + numBox, boxColors.end());
    lowLevels.erase(lowLevels.begin() + numAgent, lowLevels.end());
    
    int boxID = 0, agentID = 0;
    while(boxID < numBox){
        if(boxColors[boxID] == lowLevels[agentID % numAgent]->color){
            lowLevels[agentID % numAgent]->boxIDs.push_back(boxID++);
        }
        agentID++;
    }
    
    for(auto lowLevel : lowLevels){
        lowLevel->boxRows.resize(lowLevel->boxIDs.size());
        lowLevel->boxCols.resize(lowLevel->boxIDs.size());
        lowLevel->boxRowsLast.resize(lowLevel->boxIDs.size());
        lowLevel->boxColsLast.resize(lowLevel->boxIDs.size());
    }
    
    // Parse initial state, line is at #initial now
    int numRows = 0, numCols = 0;
    std::vector<std::string> initial;
    getline(std::cin, line);
    numCols = line.length();
    while(line[0]!='#'){
        initial.push_back(line);
        getline(std::cin,line);
    }
    numRows = initial.size();
    
    std::vector<int> agentRows(10);
    std::vector<int> agentCols(10);
    std::vector<std::vector<bool>> walls(numRows, std::vector<bool>(numCols));
    std::vector<std::vector<char>> boxes(numRows, std::vector<char>(numCols));
    for(int row = 0; row < numRows; row++){
        for(int col = 0; col < numCols; col++){
            walls[row][col] = false;
            boxes[row][col] = 0;
        }
    }
    for(int row = 0; row < numRows; row++){
        line = initial[row];
        for(int col = 0; col < numCols; col++){
            char c = line[col];
            if(c=='+') walls[row][col] = true;
            if(c>='0' && c<='9'){
                agentRows[c-'0'] = row;
                agentCols[c-'0'] = col;
                lowLevels[c-'0']->agentRow = row;
                lowLevels[c-'0']->agentRowLast = row;
                lowLevels[c-'0']->agentCol = col;
                lowLevels[c-'0']->agentColLast = col;
            }
            if(c>='A' && c<='Z'){
                boxes[row][col] = c;
                for(auto lowLevel : lowLevels){
                    auto it = std::find(lowLevel->boxIDs.begin(), lowLevel->boxIDs.end(), c-'A');
                    if(it != lowLevel->boxIDs.end()){
                        int idx = std::distance(lowLevel->boxIDs.begin(), it);
                        lowLevel->boxRows[idx].push_back(row);
                        lowLevel->boxRowsLast[idx].push_back(row);
                        lowLevel->boxCols[idx].push_back(col);
                        lowLevel->boxColsLast[idx].push_back(col);
                        break;
                    }
                }
            }
        }
    }
    agentRows.erase(agentRows.begin() + numAgent, agentRows.end());
    agentCols.erase(agentCols.begin() + numAgent, agentCols.end());
    
    // Parse goal state, line at #goal now
    std::vector<std::vector<char>> goals(numRows, std::vector<char>(numCols));
    getline(std::cin,line);
    int row = 0;
    while(line[0]!='#'){
        for(int col = 0; col < numCols; col++){
            char c = line[col];
            if((c>='A' && c<='Z') || (c>='0' && c<='9')) goals[row][col] = c;
        }
        row++;
        getline(std::cin,line);
    }
    
    // end, line at #end now
    
    // std::cerr << "numRows:" << numRows << "\nnumCols:" << numCols
    // << "\nagentRows:" << agentRows.size() << "\nagentCols:" << agentCols.size()
    // << "\nagentColors:" << agentColors.size()
    // << "\nboxColors:" << boxColors.size() << "\nwalls:" << walls.size()
    // << "\nboxes:" << boxes.size() << "\ngoals:" << goals.size() << std::endl;

    std::shared_ptr<InitialState> initialState(new InitialState(numRows,numCols,numAgent,numBox,agentRows,agentCols,boxes,agentColors,boxColors,walls,goals));
    for(auto l:lowLevels) {
        l->initialState = initialState;
        int count = 0;
        for(size_t i = 0; i < l->boxRows.size(); i++){
            count += l->boxRows[i].size();
        }
        l->setBoxNum(count);
        count = 0;
    }
    return lowLevels;
}






// // select search strategy
// FrontierBFS frontier;
// if(argc>1){
//     if(strcmp(argv[1], "-bfs") == 0) FrontierBFS frontier;
//     if(strcmp(argv[1], "-dfs") == 0) FrontierDFS frontier;
//     if(strcmp(argv[1], "-astar") == 0) { HeuristicAStar h = HeuristicAStar(Manhattan, &initialState); FrontierBestFS frontier(h);}
//     if(strcmp(argv[1], "-wastar") == 0) { HeuristicWeightedAStar h = HeuristicWeightedAStar(Manhattan, &initialState); FrontierBestFS frontier(h);}
//     if(strcmp(argv[1], "-greedy") == 0) { HeuristicGreedy h = HeuristicGreedy(Manhattan, &initialState); FrontierBestFS frontier(h);}
// }