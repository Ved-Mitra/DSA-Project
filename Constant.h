#pragma once

#include <bits/stdc++.h>
using namespace std;


const string EDGESFILEPATH= "Dataset/edges.csv";
const string NODESFILEPATH= "Dataset/nodes.csv";
const string COMMUNITIES="communities.csv";
const string BETWEENNESS_CENTRALITY="centrality.csv";
const string RECOMMEDATION="recommedation.csv";
const string BRIDGE_RECOMMEDATION="bridgeRecommedation.csv";
const string NETWORK_OPTIMIZATION_PROOF="InformationCostComparison.csv";

//to be decided
const double ALPHA=0.5;
const double BETA=0.8;
const double GAMMA=0.7;
const double DELTA=0.6;

//color ANSI code for terminal printing
const string RESET = "\033[0m";//white color 
const string RED = "\033[31m";
const string GREEN = "\033[32m";
const string YELLOW = "\033[33m";
const string CYAN = "\033[36m";

//for undirected or directed graph
const bool UNDIRECTED_GRAPH=true;
//Cost assumptions for Network Optimization proofs
// DEPLOY_COST is the cost required by the server to input the information to any of the nodes
const double DEPLOY_COST=2.0;
// BASE_COST is the cost required by one node to transmit the information to any of its adjacent nodes
const double BASE_COST=1.0;
