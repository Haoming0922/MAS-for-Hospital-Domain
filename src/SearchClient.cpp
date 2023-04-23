# include <iostream>
# include <fstream>
# include <algorithm>
# include <string>
# include <vector>
# include <cstring>
# include <stdexcept>
# include "Search.h"

InitialState parseLevel();

int main(int argc, char *argv[]){
    
    // parse input, get initial state
    InitialState initialState = parseLevel();
    
    // select search strategy
    FrontierBFS frontier; // default BFS
    if(argc>1){
        if(strcmp(argv[1], "-bfs") == 0) FrontierBFS frontier;
        if(strcmp(argv[1], "-dfs") == 0) FrontierDFS frontier;
        if(strcmp(argv[1], "-astar") == 0) { HeuristicAStar h = HeuristicAStar(Manhattan, &initialState); FrontierBestFS frontier(h);}
        if(strcmp(argv[1], "-wastar") == 0) { HeuristicWeightedAStar h = HeuristicWeightedAStar(Manhattan, &initialState); FrontierBestFS frontier(h);}
        if(strcmp(argv[1], "-greedy") == 0) { HeuristicGreedy h = HeuristicGreedy(Manhattan, &initialState); FrontierBestFS frontier(h);}
    }
    
    // search for plan
    std::vector<std::vector<actionEnum>> plan;
    try{
        plan = GraphSearch(initialState, frontier);
        std::cerr << "Found solution of length " << plan.size() << std::endl;
    } catch(std::bad_alloc& e)
    {
        std::cerr << "Error: " << e.what() << " - Out of memory! Unable to solve level." << std::endl;
        plan = {};
    }

    // print to server
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


InitialState parseLevel(){
    // Send client name to server
    std::cout << "SearchClient" << std::endl;
    
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
            if(token[0] >= '0' && token[0] <= '9') { agentColors[token[0] - '0'] = color; numAgent++;}
            if(token[0] >= 'A' && token[0] <= 'Z') { boxColors[token[0] - 'A'] = color; numBox++;}
            line.erase(0, pos + 1);
            if(line.size()==0) break;
        }
        getline(std::cin, line);
    }
    agentColors.erase(agentColors.begin() + numAgent, agentColors.end());
    boxColors.erase(boxColors.begin() + numBox, boxColors.end());
    
    // Parse initial state, line is at #initial now
    int numRows = 0, numCols = 0;
    std::vector<std::string> initial;
    getline(std::cin,line);
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
            }
            if(c>='A' && c<='Z'){
                boxes[row][col] = c;
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

    InitialState initialState(numRows,numCols,agentRows,agentCols,boxes,agentColors,boxColors,walls,goals);
    return initialState;
}