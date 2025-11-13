#pragma once // this line tells the compiler to only inlcude this file once

#include <bits/stdc++.h>
#include "Constant.h"
using namespace std;

struct NodeData
{
    string id;
    int domain;
    double strength;

    //used in detecting community and computing betweenness centrality
    double normalized_bc=0.0;
    int communityID=-1;

    NodeData() {}//if value is found i.e. node already exist but needs updating not necessary (as this condition would never be encountered) but for compilation reason added

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
    vector<vector<double>> domainComplimentarity;
    vector<vector<string>> communityArr;
    int numberOfCommunity;

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
        domainComplimentarity.push_back({0.6,0.2,0.5,0.55});
        domainComplimentarity.push_back({0.1,0.5,0.3,0.45});
        domainComplimentarity.push_back({0.55,0.1,0.45,0.35});
        domainComplimentarity.push_back({0.6,0.4,0.45,0.65});
        numberOfCommunity=0;
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

                    //allcating communityID
                    NodeMap[x].communityID=communityId;
                }
                communityArr.push_back(community);
                communityId++;
            }
        }
        numberOfCommunity=communityId-1;
        //cout << communityId << endl;
        communities.close(); // just closing the file pointer
        cout << GREEN << "Successfully identified Communities" << RESET << endl;
        // int rename_success = rename("communities.txt", "communities.csv"); // renaming the file from .txt to .csv
        // if (rename_success == 0)
        // {
        //     cout << GREEN << "Successfully renamed communities file " << RESET << endl;
        // }
        // else
        // {
        //     cout << RED << "Renaming for communities failed" << RESET << endl;
        // }
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
                cout << YELLOW << "Processed " << node_count << "/" << NodeMap.size() <<  RESET << endl;
            }
        }

        cout << CYAN << "Raw BC scores computed. NOW Normalizing" << RESET << endl;

        //Normalizing the betweenness centrality score to ontain between 0.0 - 1.0
        double max_bc = 0.0;
        double denominator= UNDIRECTED_GRAPH?2.0:1.0;//BRANDES ALGORITHM calculates twice for undirected graph just getting real score
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

                //storing the normalized_bc score for each node
                NodeMap[x.first].normalized_bc=normalized_score;
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

    unordered_map<string,double> DijsktraAlgorithm(string startNode)
    {
        //running Dijsktra using priority_queue
        priority_queue<pair<double,string>,vector<pair<double,string>>,greater<pair<double,string>>> pq;
        unordered_map<string,double> dist;
        for(auto &x:NodeMap)
            dist[x.first]=(double)INT_MAX;
        dist[startNode]=0.0;
        pq.push({0.0,startNode});
        while(!pq.empty())
        {
            string node=pq.top().second;
            double dNode=pq.top().first;
            pq.pop();
            for(auto &x:AdjList[node])
            {
                //more is weight the closer is the relation
                double effectiveDistance=1/(double)x.weight;
                double newDist=dNode+effectiveDistance;
                if(dist[x.adjacentNode]>newDist)
                {
                    dist[x.adjacentNode]=newDist;
                    pq.push({newDist,x.adjacentNode});
                }
            }
        }
        return dist;
    }

    void Recommedation(string startNode)
    {
        if(NodeMap.find(startNode)==NodeMap.end())
        {
            cout << RED << "No node with this id" << RESET << endl;
            return;
        }
        unordered_map<string,double> effectiveDistance=DijsktraAlgorithm(startNode);
        
        cout << CYAN << "Starting recommedation Engine " << RESET << endl;
        vector<pair<double,string>> recommedation;
        NodeData targetUser=NodeMap[startNode];
        for(auto &x:NodeMap)
        {
            if(x.second.id==targetUser.id)
                continue;
            
            int domainSimilarity=(x.second.domain==targetUser.domain)?1:0;
            double distance=(effectiveDistance[x.second.id]==(double)INT_MAX)?0.0:effectiveDistance[x.second.id];
            double proximity=1.0/(1.0+distance);
            double strengthFactor=(targetUser.strength+x.second.strength)/20.0; //dividing by 2 to get average to avoid a high skill person to connect to low skill person and dividing by 10 to normalize the strengthFactor
            double bc_score=x.second.normalized_bc;//normalized betweenness centrality score
            
            //calculating recommedation score
            double score= ALPHA*domainSimilarity + BETA*proximity + GAMMA*strengthFactor + DELTA*bc_score;
            recommedation.push_back({score,x.second.id});
        }

        sort(recommedation.begin(),recommedation.end(),greater<pair<double,string>>());
        ofstream recommedate(RECOMMEDATION);
        if(!recommedate.is_open())
        {
            cout << RED << "Cannot write in recommedation.csv" << RESET << endl;
        }

        cout << GREEN << "Recommadation generated successfully" << RESET << endl;

        cout << CYAN << "Top 5 Recommedation for " << startNode << " are:" << endl;
        cout << YELLOW << "ID   Domain   Strength   BC_Score   Final Score" << RESET << endl;
        recommedate << "ID,Domain,Strength,Betweenness,Final Score" << '\n';

        int cnt=0;//to track the number of recommedations given
        for(int i=0;cnt<5 && i<recommedation.size();i++)//printing top 5 recommedation
        {
            string ID=recommedation[i].second;
            bool already_connected=false;
            for(auto &x:AdjList[startNode])
            {
                //to avoid recommedation to adjacent nodes
                if(ID==x.adjacentNode)
                {
                    already_connected=true;
                    break;
                }                
            }
            if(already_connected)
                continue;

            //printing
            cout << recommedation[i].second << '\t' << NodeMap[recommedation[i].second].domain << '\t' << NodeMap[recommedation[i].second].strength << '\t' << NodeMap[recommedation[i].second].normalized_bc << '\t' << recommedation[i].first << '\n';
            //writing in recommedation.csv
            recommedate << recommedation[i].second << "," << NodeMap[recommedation[i].second].domain << "," << NodeMap[recommedation[i].second].strength << "," << NodeMap[recommedation[i].second].normalized_bc << "," << recommedation[i].first << '\n';
            cnt++;
        }
        recommedate.close();//closing file pointer
    }

    pair<string,string> GlobalBridgeRecommedation(int community1,int community2)
    {
        //to get the nodes with max Global recommedation score in among the communities 
        pair<string,string> bridgerecommed={"_","_"};
        double maxScore=0.0; 
        for(auto &u:communityArr[community1])
        {
            for(auto &v:communityArr[community2])
            {
                if(u==v)
                    continue;
                double StrengthFactor=(NodeMap[u].strength+NodeMap[v].strength)/20.0;

                //Global Bridge Score = BC_A(i) × BC_B(j) × DomainComplementarity × StrengthBalance
                double score=NodeMap[u].normalized_bc*NodeMap[v].normalized_bc*(domainComplimentarity[NodeMap[u].domain-1][NodeMap[v].domain-1])*StrengthFactor;
                if(score>maxScore)
                {
                    bridgerecommed.first=u;
                    bridgerecommed.second=v;
                }
            }
        }
        return bridgerecommed;
    }

    void BridgeRecommedation(string startNode)
    {
        if(NodeMap.find(startNode)==NodeMap.end())
        {
            cout << RED << "Node don't exist" << RESET << endl;
            return;
        }

        NodeData targetUser=NodeMap[startNode];
        cout << CYAN << "Starting Bridge Recommedation Engine for " << startNode << "(Community: " << targetUser.communityID << ")" << endl;

        //transversing by community
        vector<pair<double,string>> bridgeRecommedate(3);//store the highest score for bridge connection by from a community from the startNode community
        double TopScoreOfCommunity=0.0;
        int TopCommunity=0;

        unordered_map<string,double> dist_startNode_community=DijsktraAlgorithm(startNode);//to store shortest path from startNode in startNode's community

        for(int i=0;i<numberOfCommunity;i++)
        {
            if(i==targetUser.communityID-1)
                continue;

            //community1 node , community2 node
            pair<string,string> globalBridge=GlobalBridgeRecommedation(targetUser.communityID-1,i);
            //these nodes in globalBridge would act as connecting nodes for the two community and we would then direct a path from them from one communtiy to another to recommend bridge connections between communities
            if(globalBridge.first=="_" || globalBridge.second=="_")
                continue;
            
            vector<pair<double,string>> bridgeRecommedationCommunity;//to store the score for current Bridge recommedation of cummonity

            for(auto &x:communityArr[i])
            {
                unordered_map<string,double> dist_ith_community_from_Global_Bridge_Recommedation=DijsktraAlgorithm(globalBridge.second);//to store distance of all nodes from global recommedated node in second community

                double domainSimilarity=(targetUser.domain==NodeMap[x].domain)?0.0:1.0;
                double total_distance=dist_startNode_community[globalBridge.first] + 1 + dist_ith_community_from_Global_Bridge_Recommedation[x];//sum of distance from startNode to global bridge recommedation node + 1 + distance from global node recommedation in 2nd community to x node
                double proximity=1.0/(1.0+total_distance);
                double strengthBalance=(targetUser.strength+NodeMap[x].strength)/20.0;//dividing by 20.0 to normalize strength(10.0) and also take average(2.0)
                double normalized_BC_score=(NodeMap[targetUser.id].normalized_bc+NodeMap[x].normalized_bc)/2.0;//to take average of betweenness centrality

                //calculating final score
                double score=ALPHA*(1-domainSimilarity) + BETA*(proximity) + GAMMA*strengthBalance + DELTA*normalized_BC_score;

                bridgeRecommedationCommunity.push_back({score,x});
            }

            //sorting bridge recommedation for this community
            sort(bridgeRecommedationCommunity.begin(),bridgeRecommedationCommunity.end(),greater<pair<double,string>>());

            double topScore_in_ith_Community=0.0;
            for(int j=0;j<3;j++)
            {
                //taking top3 nodes score and taking average
                topScore_in_ith_Community+=bridgeRecommedationCommunity[j].first;
            }
            
            if(topScore_in_ith_Community>TopScoreOfCommunity)
            {
                //storing the community with top score
                TopScoreOfCommunity=topScore_in_ith_Community;
                for(int j=0;j<3;j++)
                {
                    bridgeRecommedate[j].second=bridgeRecommedationCommunity[j].second;
                    bridgeRecommedate[j].first=bridgeRecommedationCommunity[j].first;
                }
                TopCommunity=i+1;//as communityID is 1-indexed based
            }            
        }
        cout << GREEN << "Successfully completed Bridge Recommedation" << RESET << endl;

        ofstream BridgeRecommedationFile(BRIDGE_RECOMMEDATION);
        if(!BridgeRecommedationFile.is_open())
        {
            cout << RED << "Cannot write Bridge Recommedation " << RESET << endl;
        }
        cout << YELLOW << "Top 3 Bridge Recommedation from " << startNode << "of community " << targetUser.communityID << " to community " << TopCommunity << RESET << endl;
        BridgeRecommedationFile << "ID,Domain,Strength,Betweenness,Final Score\n";
        cout << "ID\tDomain\tStrength\tBetweenness\tCommunityID\tFinal Score" << endl;
        for(int i=0;i<3;i++)
        {
            //writing recommedation in file .csv
            BridgeRecommedationFile << bridgeRecommedate[i].second << "," << NodeMap[bridgeRecommedate[i].second].domain << "," << NodeMap[bridgeRecommedate[i].second].strength << "," << NodeMap[bridgeRecommedate[i].second].normalized_bc << "," << bridgeRecommedate[i].first << '\n';
            //printing bridge recommedation
            cout << bridgeRecommedate[i].second << "\t" << NodeMap[bridgeRecommedate[i].second].domain << "\t" << NodeMap[bridgeRecommedate[i].second].strength << "\t" << NodeMap[bridgeRecommedate[i].second].normalized_bc << "\t" << TopCommunity << "\t" << bridgeRecommedate[i].first << '\n';
        }
        BridgeRecommedationFile.close();//closing the file pointer
    }
const double BASE_COST=1.0;
const double DEPLOY_COST=2.0;
    double CostCalculation(string startNode)
{
    if (NodeMap.find(startNode) == NodeMap.end())
    {
        cout << RED << "Invalid start node ID: " << startNode << RESET << endl;
        return -1;
    }

    double totalCost = 0.0;
    int roundCount = 0;

    unordered_set<string> informed;
    unordered_set<string> newly_informed;

    informed.insert(startNode);
    newly_informed.insert(startNode);

    while (!newly_informed.empty())
    {
        unordered_set<string> nextRound;
        roundCount++;

        for (const string &u : newly_informed)
        {
            for (auto &neigh : AdjList[u])
            {
                string v = neigh.adjacentNode;
                int wt = neigh.weight;

                if (informed.find(v) == informed.end())
                {
                    double cost = BASE_COST / (double)wt; 
                    totalCost += cost;
                    nextRound.insert(v);
                    informed.insert(v);
                }
            }
        }

        newly_informed = nextRound;
    }

    cout << GREEN << "Information reached " << informed.size() << " / "
         << NodeMap.size() << " nodes in " << roundCount << " rounds." << RESET << endl;

    cout << YELLOW << "Total cost of information spread from " << startNode
         << " = " << fixed << setprecision(6) << totalCost << RESET << endl;

    return totalCost;
}




};
