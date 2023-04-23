# include "Action.h"

std::map<actionEnum,struct actionField> action = {

{actionEnum::OpNo, {"NoOp", NoOp, 0,0,0,0}},

{actionEnum::MoveN, {"Move(N)", Move, -1,0,0,0}},
{actionEnum::MoveS, {"Move(S)", Move, 1,0,0,0}},
{actionEnum::MoveW, {"Move(W)", Move, 0,-1,0,0}},
{actionEnum::MoveE, {"Move(E)", Move, 0,1,0,0}},

{actionEnum::PushNN, {"Push(N,N)", Push, -1,0,-1,0}},
{actionEnum::PushNE, {"Push(N,E)", Push, -1,0,0,1}},
{actionEnum::PushNW, {"Push(N,W)", Push, -1,0,0,-1}},
{actionEnum::PushSS, {"Push(S,S)", Push, 1,0,1,0}},
{actionEnum::PushSW, {"Push(S,W)", Push, 1,0,0,-1}},
{actionEnum::PushSE, {"Push(S,E)", Push, 1,0,0,1}},
{actionEnum::PushWW, {"Push(W,W)", Push, 0,-1,0,-1}},
{actionEnum::PushWN, {"Push(W,N)", Push, 0,-1,-1,0}},
{actionEnum::PushWS, {"Push(W,S)", Push, 0,-1,1,0}},
{actionEnum::PushEE, {"Push(E,E)", Push, 0,1,0,1}},
{actionEnum::PushEN, {"Push(E,N)", Push, 0,1,-1,0}},
{actionEnum::PushES, {"Push(E,S)", Push, 0,1,1,0}},

{actionEnum::PullNN, {"Pull(N,N)", Pull, -1,0,-1,0}},
{actionEnum::PullNE, {"Pull(N,E)", Pull, -1,0,0,1}},
{actionEnum::PullNW, {"Pull(N,W)", Pull, -1,0,0,-1}},
{actionEnum::PullSS, {"Pull(S,S)", Pull, 1,0,1,0}},
{actionEnum::PullSW, {"Pull(S,W)", Pull, 1,0,0,-1}},
{actionEnum::PullSE, {"Pull(S,E)", Pull, 1,0,0,1}},
{actionEnum::PullWW, {"Pull(W,W)", Pull, 0,-1,0,-1}},
{actionEnum::PullWN, {"Pull(W,N)", Pull, 0,-1,-1,0}},
{actionEnum::PullWS, {"Pull(W,S)", Pull, 0,-1,1,0}},
{actionEnum::PullEE, {"Pull(E,E)", Pull, 0,1,0,1}},
{actionEnum::PullEN, {"Pull(E,N)", Pull, 0,1,-1,0}},
{actionEnum::PullES, {"Pull(E,S)", Pull, 0,1,1,0}}

};