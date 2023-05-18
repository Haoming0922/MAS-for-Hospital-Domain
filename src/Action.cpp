# include "Action.h"

std::map<ActionEnum,struct ActionField> action = {

{ActionEnum::OpNo, {"NoOp", NoOp, 0,0,0,0}},

{ActionEnum::MoveN, {"Move(N)", Move, -1,0,0,0}},
{ActionEnum::MoveS, {"Move(S)", Move, 1,0,0,0}},
{ActionEnum::MoveW, {"Move(W)", Move, 0,-1,0,0}},
{ActionEnum::MoveE, {"Move(E)", Move, 0,1,0,0}},

{ActionEnum::PushNN, {"Push(N,N)", Push, -1,0,-1,0}},
{ActionEnum::PushNE, {"Push(N,E)", Push, -1,0,0,1}},
{ActionEnum::PushNW, {"Push(N,W)", Push, -1,0,0,-1}},
{ActionEnum::PushSS, {"Push(S,S)", Push, 1,0,1,0}},
{ActionEnum::PushSW, {"Push(S,W)", Push, 1,0,0,-1}},
{ActionEnum::PushSE, {"Push(S,E)", Push, 1,0,0,1}},
{ActionEnum::PushWW, {"Push(W,W)", Push, 0,-1,0,-1}},
{ActionEnum::PushWN, {"Push(W,N)", Push, 0,-1,-1,0}},
{ActionEnum::PushWS, {"Push(W,S)", Push, 0,-1,1,0}},
{ActionEnum::PushEE, {"Push(E,E)", Push, 0,1,0,1}},
{ActionEnum::PushEN, {"Push(E,N)", Push, 0,1,-1,0}},
{ActionEnum::PushES, {"Push(E,S)", Push, 0,1,1,0}},

{ActionEnum::PullNN, {"Pull(N,N)", Pull, -1,0,-1,0}},
{ActionEnum::PullNE, {"Pull(N,E)", Pull, -1,0,0,1}},
{ActionEnum::PullNW, {"Pull(N,W)", Pull, -1,0,0,-1}},
{ActionEnum::PullSS, {"Pull(S,S)", Pull, 1,0,1,0}},
{ActionEnum::PullSW, {"Pull(S,W)", Pull, 1,0,0,-1}},
{ActionEnum::PullSE, {"Pull(S,E)", Pull, 1,0,0,1}},
{ActionEnum::PullWW, {"Pull(W,W)", Pull, 0,-1,0,-1}},
{ActionEnum::PullWN, {"Pull(W,N)", Pull, 0,-1,-1,0}},
{ActionEnum::PullWS, {"Pull(W,S)", Pull, 0,-1,1,0}},
{ActionEnum::PullEE, {"Pull(E,E)", Pull, 0,1,0,1}},
{ActionEnum::PullEN, {"Pull(E,N)", Pull, 0,1,-1,0}},
{ActionEnum::PullES, {"Pull(E,S)", Pull, 0,1,1,0}}

};