#pragma once //this line tells the compiler to only inlcude this file once

#include <bits/stdc++.h>
using namespace std;


class NodeData
{
    string id;
    int domain;
    double strength;

    NodeData(string _id,int _domain,double _strength)
    {
        id=_id;
        domain=_domain;
        strength=_strength;
    }
};


class AdjList
{
    string adjacentNode;
    int weight;

    AdjList(int v,int wt)
    {
        adjacentNode=v;
        weight=wt;
    }
};


class Graph
{
    //add detect communities
    private:
    unordered_map<string,NodeData> NodeMap;
    unordered_map<string,vector<AdjList>> AdjList;

    
};