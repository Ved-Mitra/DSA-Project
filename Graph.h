#pragma once //this line tells the compiler to only inlcude this file once

#include <bits/stdc++.h>
using namespace std;

const string EDGESFILEPATH= "Dataset\\edges.csv";
const string NODESFILEPATH= "Dataset\\nodes.csv";
//color ANSI code for terminal printing
const string RESET = "\033[0m";//white color 
const string RED = "\033[31m";
const string GREEN = "\033[32m";
const string YELLOW = "\033[33m";
const string CYAN = "\033[36m";


struct NodeData
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


struct AdjListNode
{
    string adjacentNode;
    int weight;

    AdjListNode(string v,int wt)
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
    unordered_map<string,vector<AdjListNode>> AdjList;

    void loadEdgesData()//call loadNodesData() first
    {
        ifstream edgesData(EDGESFILEPATH);
        long long int lineCnt=0;
        if(edgesData.is_open())
        {
            cout << CYAN << "Starting to Load Edges data" << RESET << '\n';
            string line;
            getline(edgesData,line);//for skipping header
            while(getline(edgesData,line))
            {
                stringstream ss(line);
                string u,v,wt;
                getline(ss,u,',');
                getline(ss,v,',');
                getline(ss,wt,',');
                lineCnt++;
                if(u.empty() || v.empty() || wt.empty())
                {
                    cerr << RED << "Cannot read at line " << lineCnt << RESET << '\n';
                    return;
                }
                if(NodeMap.find(u)==NodeMap.end() || NodeMap.find(v)==NodeMap.end())
                {
                    //only adding the nodes if both exist
                    continue;
                }
                int weight=stoi(wt);
                AdjListNode newNode(v,weight);
                AdjList[u].push_back(newNode);
            }
            edgesData.close();//closing the file pointer to edges.csv
            cout << GREEN << "Succesfully Loaded Edges" << RESET << '\n';
        }
        else
        {
            cout << RED << "Loading Edges Failed" << RESET << endl;
        }
    }

    void loadNodesData()
    {
        ifstream NodesData(NODESFILEPATH);
        long long int lineCnt=0;
        if(NodesData.is_open())
        {
            string line;
            getline(NodesData,line);//skipping header
            while(getline(NodesData,line))
            {
                stringstream ss(line);
                string id,domain,strength;
                getline(ss,id,',');
                getline(ss,domain,',');
                getline(ss,strength,',');
                lineCnt++;
                if(id.empty() || domain.empty() || strength.empty())
                {
                    cerr << RED << "Cannot read at line " << lineCnt << RESET << '\n';
                }
                int domain_int=stoi(domain);
                double strength_double=stod(strength);
                NodeData newNode(id,domain_int,strength_double);
                NodeMap[id]=newNode;
            }
            NodesData.close();//closing the file pointer
            cout << GREEN << "Successfully loaded Nodes data" << RESET << '\n';
        }
        else
        {
            cout << RED << "Cannot load Nodes data" << '\n';
        }
    }

    void DFS_detectCommunities(vector<string> &communities,string node,unordered_map<string,bool> &visited)
    {
        visited[node]=true;
        communities.push_back(node);
        for(AdjListNode &x:AdjList[node])
        {
            if(!visited[x.adjacentNode])
                DFS_detectCommunities(communities,x.adjacentNode,visited);
        }
    }

    vector<vector<string>> detectCommunities()
    {
        
    }
};