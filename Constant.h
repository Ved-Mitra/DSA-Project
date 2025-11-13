#pragma once

#include <bits/stdc++.h>

const std::string EDGESFILEPATH= "Dataset/edges.csv";
const std::string NODESFILEPATH= "Dataset/nodes.csv";
const std::string COMMUNITIES="communities.csv";
const std::string BETWEENNESS_CENTRALITY="centrality.csv";
const std::string RECOMMEDATION="recommedation.csv";
const std::string BRIDGE_RECOMMEDATION="bridgeRecommedation.csv";

//to be decided
const double ALPHA=0.5;
const double BETA=0.8;
const double GAMMA=0.7;
const double DELTA=0.6;

//color ANSI code for terminal printing
const std::string RESET = "\033[0m";//white color 
const std::string RED = "\033[31m";
const std::string GREEN = "\033[32m";
const std::string YELLOW = "\033[33m";
const std::string CYAN = "\033[36m";

//for undirected or directed graph
const bool UNDIRECTED_GRAPH=true;
