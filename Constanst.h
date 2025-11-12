#pragma once

#include <bits/stdc++.h>
using namespace std;


const string EDGESFILEPATH= "Dataset\\edges.csv";
const string NODESFILEPATH= "Dataset\\nodes.csv";
const string COMMUNITIES="communities.txt";
const string BETWEENNESS_CENTRALITY="centrality.csv";

//to be decided
const double ALPHA=0.0;
const double BETA=0.0;
const double GAMMA=0.0;
const double DELTA=0.0;

//color ANSI code for terminal printing
const string RESET = "\033[0m";//white color 
const string RED = "\033[31m";
const string GREEN = "\033[32m";
const string YELLOW = "\033[33m";
const string CYAN = "\033[36m";

//for undirected or directed graph
const bool UNDIRECTED_GRAPH=true;