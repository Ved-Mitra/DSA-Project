# Social Network Intelligence Engine (SNIE)

The Social Network Intelligence Engine (SNIE) is a C++-based analytics framework that models a social network as a graph and performs deep structural intelligence. It detects communities, identifies influential nodes, generates personalized recommendations, and suggests cross-community bridge connections to improve global network connectivity.

This project applies the concepts of Graph Theory, Shortest Paths, Centrality Algorithms, and Semantic Scoring — making it a strong advanced DSA-based project.


# 📚 Table of Contents
[Project Overview](#project-overview)

[Goal & Objective](#goals--objectives)

[Algorithm & Complexity Table](#algorithm-complexity-table)

[Scoring Formula](#scoring-formulas)

[Requirements](#requirements)

[Installation](#installation)


## Project Overview
The Social Network Intelligence Engine (SNIE) is an analytical system designed to understand, interpret, and optimize connectivity within social, professional, or collaborative networks. Nodes represent individuals or entities, while edges represent relationships with varying strength.
Key Features:

● Offline Structural Intelligence: Community detection and Betweenness Centrality (BC)

● Real-Time Local Intelligence: Weighted Dijkstra paths and semantic scoring

● Global Optimization: Cross-community bridge recommendations

● Visualization: Communities, influence, weighted ties, and bridge simulations

● Information Spread

## Goals & Objectives
● Detect natural communities and disconnected clusters.

● Identify influential individuals using Betweenness Centrality.

● Recommend meaningful local and cross-community connections.

● Suggest high-impact global bridges that unify separated communities.

● Use semantic edge weights to capture relationship strength.

● Simulate and compare information spreading cost before and after bridging.

● Visualize the entire network along with recommended optimizations.

## Algorithm Complexity Table

| Algorithm / Operation                     | Complexity              | Notes                                   |
|-------------------------------------------|--------------------------|------------------------------------------|
| Community Detection                       | O(V + E)                | DFS / Union-Find                         |
| Weighted Dijkstra                         | O((V + E) log V)        | Shortest path with weighted edges        |
| Betweenness Centrality                    | O(V × E)                | Brandes Algorithm                        |
| Bridge Pairing                            | O(k²)                   | BC × BC heuristic                        |
| Information Spread Cost Calculation (BFS) | O(V + E)                | Each node & edge processed once          |
| **CompareCost (Before/After Bridging)**   | O(V + E)                | Runs `CostCalculation()` 3×              |

## Scoring Formulas
`Local Recommendation Score:`
Score=α⋅DomainSim+β⋅Proximity+γ⋅StrengthFactor+δ⋅NormalizedBC_score

`Bridge Recommendation Score:`
BridgeScore=α⋅(1−DomainSim)+β⋅Proximity+γ⋅StrengthBalance+δ⋅NormalizedBC_score

`Global Bridge Score (Heuristic):`
GlobalScore=BCA(i)×BCB(j)×DomainComplementarity×StrengthBalance


# Requirements

1. Python 3.8
2. Manin
3. Networkx
4. numpy
5. pandas
6. Mingw-32bit


# Installation

1. To install Networkx pandas numpy
``` Bash
pip install networkx pandas numpy
```
2. Install manim
``` Bash
pip install manim
```
3. Install ffmpeg
install ffmpeg from https://www.gyan.dev/ffmpeg/builds/
then paste the bin path of ffmpeg in system environmental variables
then verify that ffmpeg is installed
```bash
ffmpeg -version
```
4. Git clone
``` Bash
git clone https://github.com/Ved-Mitra/DSA-Project.git
```

5. Exceute
```Bash
g++ main.cpp -o snie
```
6. Execute Animation
```Bash
python animation.py
manim -pql animation.py SNIEAnimation

```

