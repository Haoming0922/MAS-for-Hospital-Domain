#ifndef SEARCH_H
#define SEARCH_H

# include <chrono>
# include <vector>
# include <iostream>
# include "State.h"
# include "Frontier.hpp"


void printSearchStatus(Frontier& f, std::chrono::high_resolution_clock::time_point startTime);

std::vector<std::vector<actionEnum>> GraphSearch(InitialState& initialState, Frontier& f);


#endif
