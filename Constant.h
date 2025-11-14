#pragma once

#include <iostream>
#include <string>

// File paths
const std::string EDGESFILEPATH= "Dataset/edges.csv";
const std::string NODESFILEPATH= "Dataset/nodes.csv";
const std::string COMMUNITIES="communities.csv";
const std::string BETWEENNESS_CENTRALITY="centrality.csv";
const std::string RECOMMEDATION="recommedation.csv";
const std::string BRIDGE_RECOMMEDATION="bridgeRecommedation.csv";
const std::string NETWORK_OPTIMIZATION_PROOF="InformationCostComparison.csv";

// Recommendation weight coefficients
const double ALPHA=0.5;
const double BETA=0.8;
const double GAMMA=0.7;
const double DELTA=0.6;

// Color ANSI code for terminal printing
const std::string RESET = "\033[0m"; // white color 
const std::string RED = "\033[31m";
const std::string GREEN = "\033[32m";
const std::string YELLOW = "\033[33m";
const std::string CYAN = "\033[36m";

// Graph type
const bool UNDIRECTED_GRAPH=true;

// Cost assumptions for Network Optimization proofs
// DEPLOY_COST is the cost required by the server to input the information to any of the nodes
const double DEPLOY_COST=2.0;
// BASE_COST is the cost required by one node to transmit the information to any of its adjacent nodes
const double BASE_COST=1.0;