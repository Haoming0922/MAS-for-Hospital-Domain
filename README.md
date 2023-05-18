# Intelligent Multi‑Agent System for Hospital Domain


## Feature
- Implemented **conflict-based search**. It consists of high level node for conflict resolving and low level search for single agent planning.
- For low level search, use **astar heuristic search**: *h = goalCounts * 100 - sum of shortest distance between boxes to goals - the minimal distance between agent and its boxes*


## Run
- `java -jar server.jar -c "src/SearchClient.exe -bfs" -l "levels/MAsimple2.lvl" -g`

<p float="left">
<img width='270' height='180' src="image/2023-04-23193606.png">
<img width='270' height='180' src="image/2023-04-23193555.png">
</p>

- `java -jar server.jar -c "src/SearchClient.exe -bfs" -l "levels/AgentWayz.lvl" -g`

<p float="left">
<img width='270' height='180' src="image/2023-05-18162804.png">
<img width='270' height='180' src="image/2023-05-18162848.png">
</p>
