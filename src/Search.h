#ifndef SEARCH_H
#define SEARCH_H

# include <chrono>
# include <vector>
# include <memory>
# include <iostream>
# include "State.h"
# include "Frontier.hpp"
# include "CBS.h"


void printSearchStatus(FrontierHighLevel& f, std::chrono::high_resolution_clock::time_point startTime);

std::vector<std::shared_ptr<LowLevel>> lowLevelSearch(std::shared_ptr<LowLevel> ll, HeuristicAStar& h);

std::vector<std::vector<ActionEnum>> conflictBasedSearch(std::vector<std::shared_ptr<LowLevel>>& lowLevels);

#endif
