#pragma once // this line tells the compiler to only inlcude this file once

#include <bits/stdc++.h>
#include "Constanst.h"
using namespace std;

struct NodeData
{
    string id;
    int domain;
    double strength;

    NodeData() {}

    NodeData(string _id, int _domain, double _strength)
    {
        id = _id;
        domain = _domain;
        strength = _strength;
    }
};

struct AdjListNode
{
    string adjacentNode;
    int weight;

    AdjListNode(string v, int wt)
    {
        adjacentNode = v;
        weight = wt;
    }
};

class Graph
{
    // add detect communities
private:
    unordered_map<string, NodeData> NodeMap;
    unordered_map<string, vector<AdjListNode>> AdjList;

    void loadEdgesData() // call loadNodesData() first
    {
        ifstream edgesData(EDGESFILEPATH);
        long long int lineCnt = 0;
        if (edgesData.is_open())
        {
            cout << CYAN << "Starting to Load Edges data" << RESET << '\n';
            string line;
            getline(edgesData, line); // for skipping header
            while (getline(edgesData, line))
            {
                stringstream ss(line);
                string u, v, wt;
                getline(ss, u, ',');
                getline(ss, v, ',');
                getline(ss, wt, ',');
                lineCnt++;
                if (u.empty() || v.empty() || wt.empty())
                {
                    cerr << RED << "Cannot read at line " << lineCnt << RESET << '\n';
                    return;
                }
                if (NodeMap.find(u) == NodeMap.end() || NodeMap.find(v) == NodeMap.end())
                {
                    // only adding the nodes if both exist
                    continue;
                }
                int weight = stoi(wt);
                AdjListNode newNode(v, weight);
                AdjList[u].push_back(newNode);
            }
            edgesData.close(); // closing the file pointer to edges.csv
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
        long long int lineCnt = 0;
        if (NodesData.is_open())
        {
            string line;
            getline(NodesData, line); // skipping header
            while (getline(NodesData, line))
            {
                stringstream ss(line);
                string id, domain, strength;
                getline(ss, id, ',');
                getline(ss, domain, ',');
                getline(ss, strength, ',');
                lineCnt++;
                if (id.empty() || domain.empty() || strength.empty())
                {
                    cerr << RED << "Cannot read at line " << lineCnt << RESET << '\n';
                }
                int domain_int = stoi(domain);
                double strength_double = stod(strength);
                NodeData newNode(id, domain_int, strength_double);
                NodeMap[id] = newNode;
            }
            NodesData.close(); // closing the file pointer
            cout << GREEN << "Successfully loaded Nodes data" << RESET << '\n';
        }
        else
        {
            cout << RED << "Cannot load Nodes data" << '\n';
        }
    }

public:
    Graph()
    {
        loadNodesData(); // calling loadNodesData() first as loadEdgesData() uses NodeMap which is filled by loadNodesData()
        loadEdgesData();
    }

    void DFS_detectCommunities(vector<string> &community, string node, unordered_map<string, bool> &visited)
    {
        visited[node] = true;
        community.push_back(node);
        for (AdjListNode &x : AdjList[node])
        {
            if (!visited[x.adjacentNode])
                DFS_detectCommunities(community, x.adjacentNode, visited);
        }
    }

    void detectCommunities()
    {
        ofstream communities(COMMUNITIES);
        if (!communities.is_open())
        {
            cout << RED << "communities file cannot be opened" << RESET << endl;
            return;
        }
        communities << "ID" << "," << "communityID" << '\n'; // header for communities.csv
        unordered_map<string, bool> visited;
        for (auto &x : NodeMap)
        { // storing all id as false
            visited[x.first] = false;
        }
        int communityId = 1; // to detect number of communities
        for (auto &x : visited)
        {
            if (!x.second)
            {
                vector<string> community;
                DFS_detectCommunities(community, x.first, visited);
                for (auto &x : community)
                {
                    // writing the communities in communities.txt
                    communities << x << "," << communityId << '\n';
                }
                communityId++;
            }
        }
        //cout << communityId << endl;
        communities.close(); // just closing the file pointer
        cout << GREEN << "Successfully identified Communities" << RESET << endl;
        int rename_success = rename("communities.txt", "communities.csv"); // renaming the file from .txt to .csv
        if (rename_success == 0)
        {
            cout << GREEN << "Successfully renamed communities file " << RESET << endl;
        }
        else
        {
            cout << RED << "Renaming for communities failed" << RESET << endl;
        }
    }

    void betweennessCentrality()
    {
        //USED BRANDES ALGORITHM
        cout << CYAN << "Starting betweenness centrality process" << RESET << endl;
        unordered_map<string, double> bc_scores;//to store raw score
        for (auto const &x: NodeMap)
        {
            bc_scores[x.first] = 0.0;
        }

        int node_count = 0;
        for (auto const &x: NodeMap)//for every node
        {
            stack<string> st; // Order of nodes for backpropagation i.e. departure time (TOPO SORT)
            unordered_map<string, vector<string>> predecessors;
            unordered_map<string, double> sigma; // Number of shortest paths from x.first
            unordered_map<string, int> distance;//distance of each node from x.first

            for (auto const &x : NodeMap)
            {
                distance[x.first] = -1; //-1 implies at infinity distance
                sigma[x.first] = 0.0;
            }
            distance[x.first] = 0;
            sigma[x.first] = 1.0;
            queue<string> q;
            q.push(x.first);

            while (!q.empty())
            {
                string node = q.front();
                q.pop();
                st.push(node); // Push into stack for topo sort type linear ordering

                if (AdjList.find(node) != AdjList.end())
                {//here considering edges weight to be 1
                    for (auto &neighbor : AdjList[node])
                    {
                        string w = neighbor.adjacentNode;
                        if (distance[w] < 0)///newNode
                        {
                            distance[w] = distance[node] + 1;
                            q.push(w);
                        }
                        if (distance[w] == distance[node] + 1)//check for shortest path
                        {
                            sigma[w] += sigma[node];
                            predecessors[w].push_back(node);
                        }
                    }
                }
            }

            unordered_map<string, double> dependency;//calculate credit
            for (auto const &x: NodeMap)
            {
                dependency[x.first] = 0.0;
            }
            while (!st.empty())//in backpropagation order
            {
                string w = st.top();
                st.pop();

                for (const string &v : predecessors[w])
                {
                    double credit = (sigma[v] / sigma[w]) * (1.0 + dependency[w]);//calculating credit for v
                    dependency[v] += credit;
                }
                if (w != x.first)
                {
                    bc_scores[w] += dependency[w];//adding score
                }
            }
            node_count++;
            if (node_count % 1000 == 0)
            {
                cout << YELLOW << "Processed " << node_count << RESET << endl;
            }
        }

        cout << CYAN << "Raw BC scores computed. NOW Normalizing" << RESET << endl;

        //Normalizing the betweenness centrality score to ontain between 0.0 - 1.0
        double max_bc = 0.0;
        int denominator= UNDIRECTED_GRAPH?2.0:1.0;//BRANDES ALGORITHM calculates twice for undirected graph just getting real score
        for (auto const &x : bc_scores)
        {
            double corrected_score = x.second/denominator;
            bc_scores[x.first] = corrected_score;
            if (corrected_score > max_bc)
            {
                max_bc = corrected_score;
            }
        }

        ofstream centrality_file(BETWEENNESS_CENTRALITY);//saving the score
        if (!centrality_file.is_open())
        {
            cout << RED << "Could not open " << BETWEENNESS_CENTRALITY << RESET << endl;
            return;
        }
        centrality_file << "ID,normalized_bc\n"; //header for the .csv file
        for (auto const &x : bc_scores)
        {
            if (max_bc > 0)
            {
                double normalized_score = x.second / max_bc;
                centrality_file << x.first << "," << fixed << setprecision(8) << normalized_score << "\n";
            }
            else
            {
                //only in case max_bc is zero
                centrality_file << x.first << ",0.0\n";
            }
        }
        centrality_file.close();//closing the file pointer
        cout << GREEN << "Successfully computed and saved Betweenness Centrality." << RESET << endl;
    }
};