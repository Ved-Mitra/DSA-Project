#pragma once // this line tells the compiler to only inlcude this file once

// #include <bits/stdc++.h>
#include <iostream>       // std::cout, std::cerr, std::endl
#include <fstream>        // std::ifstream, std::ofstream
#include <sstream>        // std::stringstream, std::getline(stream, string)
#include <string>         // std::string, std::stoi, std::stod
#include <vector>         // std::vector
#include <unordered_map>  // std::unordered_map
#include <unordered_set>  // std::unordered_set
#include <queue>          // std::queue, std::priority_queue
#include <stack>          // std::stack
#include <algorithm>      // std::sort, std::greater
#include <functional>     // std::greater (comparator)
#include <utility>        // std::pair
#include <iomanip>        // std::fixed, std::setprecision
#include <climits>// INT_MAX
#include "Constant.h"

struct NodeData
{
    std::string id;
    int domain;
    double strength;

    //used in detecting community and computing betweenness centrality
    double normalized_bc=0.0;
    int communityID=-1;

    NodeData() {} //if value is found i.e. node already exist but needs updating not necessary (as this condition would never be encountered) but for compilation reason added

    NodeData(std::string _id, int _domain, double _strength)
    {
        id = _id;
        domain = _domain;
        strength = _strength;
    }
};

struct AdjListNode
{
    std::string adjacentNode;
    int weight;

    AdjListNode(std::string v, int wt)
    {
        adjacentNode = v;
        weight = wt;
    }
};

class Graph
{
private:
    std::unordered_map<std::string, NodeData> NodeMap;
    std::unordered_map<std::string, std::vector<AdjListNode>> AdjList;
    std::vector<std::vector<double>> domainComplimentarity;
    std::vector<std::vector<std::string>> communityArr;
    int numberOfCommunity;

    void loadEdgesData() // call loadNodesData() first
    {
        std::ifstream edgesData(EDGESFILEPATH);
        long long int lineCnt = 0;
        if (edgesData.is_open())
        {
            std::cout << CYAN << "Starting to Load Edges data" << RESET << '\n';
            std::string line;
            std::getline(edgesData, line); // for skipping header
            while (std::getline(edgesData, line))
            {
                std::stringstream ss(line);
                std::string u, v, wt;
                std::getline(ss, u, ',');
                std::getline(ss, v, ',');
                std::getline(ss, wt, ',');
                lineCnt++;
                if (u.empty() || v.empty() || wt.empty())
                {
                    std::cerr << RED << "Cannot read at line " << lineCnt << RESET << '\n';
                    return;
                }
                if (NodeMap.find(u) == NodeMap.end() || NodeMap.find(v) == NodeMap.end())
                {
                    // only adding the nodes if both exist
                    continue;
                }
                int weight = std::stoi(wt);
                AdjListNode newNode(v, weight);
                AdjList[u].push_back(newNode);
            }
            edgesData.close(); // closing the file pointer to edges.csv
            std::cout << GREEN << "Succesfully Loaded Edges" << RESET << '\n';
        }
        else
        {
            std::cout << RED << "Loading Edges Failed" << RESET << std::endl;
        }
    }

    void loadNodesData()
    {
        std::ifstream NodesData(NODESFILEPATH);
        long long int lineCnt = 0;
        if (NodesData.is_open())
        {
            std::string line;
            std::getline(NodesData, line); // skipping header
            while (std::getline(NodesData, line))
            {
                std::stringstream ss(line);
                std::string id, domain, strength;
                std::getline(ss, id, ',');
                std::getline(ss, domain, ',');
                std::getline(ss, strength, ',');
                lineCnt++;
                if (id.empty() || domain.empty() || strength.empty())
                {
                    std::cerr << RED << "Cannot read at line " << lineCnt << RESET << '\n';
                }
                int domain_int = std::stoi(domain);
                double strength_double = std::stod(strength);
                NodeData newNode(id, domain_int, strength_double);
                NodeMap[id] = newNode;
            }
            NodesData.close(); // closing the file pointer
            std::cout << GREEN << "Successfully loaded Nodes data" << RESET << '\n';
        }
        else
        {
            std::cout << RED << "Cannot load Nodes data" << '\n';
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

    void DFS_detectCommunities(std::vector<std::string> &community, std::string node, std::unordered_map<std::string, bool> &visited)
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
        std::ofstream communities(COMMUNITIES);
        if (!communities.is_open())
        {
            std::cout << RED << "communities file cannot be opened" << RESET << std::endl;
            return;
        }
        communities << "ID" << "," << "communityID" << '\n'; // header for communities.csv
        std::unordered_map<std::string, bool> visited;
        for (auto &x : NodeMap)
        { // storing all id as false
            visited[x.first] = false;
        }
        int communityId = 1; // to detect number of communities
        for (auto &x : visited)
        {
            if (!x.second)
            {
                std::vector<std::string> community;
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
        std::cout << GREEN << "Successfully identified Communities" << RESET << std::endl;
    }

    void betweennessCentrality()
    {
        //USED BRANDES ALGORITHM
        std::cout << CYAN << "Starting betweenness centrality process" << RESET << std::endl;
        std::unordered_map<std::string, double> bc_scores;//to store raw score
        for (auto const &x: NodeMap)
        {
            bc_scores[x.first] = 0.0;
        }

        int node_count = 0;
        for (auto const &x: NodeMap)//for every node
        {
            std::stack<std::string> st; // Order of nodes for backpropagation i.e. departure time (TOPO SORT)
            std::unordered_map<std::string, std::vector<std::string>> predecessors;
            std::unordered_map<std::string, double> sigma; // Number of shortest paths from x.first
            std::unordered_map<std::string, int> distance;//distance of each node from x.first

            for (auto const &x : NodeMap)
            {
                distance[x.first] = -1; //-1 implies at infinity distance
                sigma[x.first] = 0.0;
            }
            distance[x.first] = 0;
            sigma[x.first] = 1.0;
            std::queue<std::string> q;
            q.push(x.first);

            while (!q.empty())
            {
                std::string node = q.front();
                q.pop();
                st.push(node); // Push into stack for topo sort type linear ordering

                if (AdjList.find(node) != AdjList.end())
                {//here considering edges weight to be 1
                    for (auto &neighbor : AdjList[node])
                    {
                        std::string w = neighbor.adjacentNode;
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

            std::unordered_map<std::string, double> dependency;//calculate credit
            for (auto const &x: NodeMap)
            {
                dependency[x.first] = 0.0;
            }
            while (!st.empty())//in backpropagation order
            {
                std::string w = st.top();
                st.pop();

                for (const std::string &v : predecessors[w])
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
                std::cout << YELLOW << "Processed " << node_count << "/" << NodeMap.size() <<  RESET << std::endl;
            }
        }

        std::cout << CYAN << "Raw BC scores computed. NOW Normalizing" << RESET << std::endl;

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

        std::ofstream centrality_file(BETWEENNESS_CENTRALITY);//saving the score
        if (!centrality_file.is_open())
        {
            std::cout << RED << "Could not open " << BETWEENNESS_CENTRALITY << RESET << std::endl;
            return;
        }
        centrality_file << "ID,normalized_bc\n"; //header for the .csv file
        for (auto const &x : bc_scores)
        {
            if (max_bc > 0)
            {
                double normalized_score = x.second / max_bc;
                centrality_file << x.first << "," << std::fixed << std::setprecision(8) << normalized_score << "\n";

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
        std::cout << GREEN << "Successfully computed and saved Betweenness Centrality." << RESET << std::endl;
    }

    std::unordered_map<std::string,double> DijsktraAlgorithm(std::string startNode)
    {
        //running Dijsktra using priority_queue
        std::priority_queue<std::pair<double,std::string>,std::vector<std::pair<double,std::string>>,std::greater<std::pair<double,std::string>>> pq;
        std::unordered_map<std::string,double> dist;
        for(auto &x:NodeMap)
            dist[x.first]=(double)INT_MAX;
        dist[startNode]=0.0;
        pq.push({0.0,startNode});
        while(!pq.empty())
        {
            std::string node=pq.top().second;
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

    std::pair<std::string,std::string> GlobalBridgeRecommedation(int community1,int community2)
    {
        //to get the nodes with max Global recommedation score in among the communities 
        std::pair<std::string,std::string> bridgerecommed={"_","_"}; 
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

    // Recommedation and BridgeRecommedation functions come after this
    void Recommedation(std::string startNode)
    {
        if(NodeMap.find(startNode)==NodeMap.end())
        {
            std::cout << RED << "No node with this id" << RESET << std::endl;
            return;
        }
        std::unordered_map<std::string,double> effectiveDistance=DijsktraAlgorithm(startNode);
        
        std::cout << CYAN << "Starting recommedation Engine " << RESET << std::endl;
        std::vector<std::pair<double,std::string>> recommedation;
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

        std::sort(recommedation.begin(),recommedation.end(),std::greater<std::pair<double,std::string>>());
        std::ofstream recommedate(RECOMMEDATION);
        if(!recommedate.is_open())
        {
            std::cout << RED << "Cannot write in recommedation.csv" << RESET << std::endl;
        }

        std::cout << GREEN << "Recommadation generated successfully" << RESET << std::endl;

        std::cout << CYAN << "Top 5 Recommedation for " << startNode << " are:" << std::endl;
        std::cout << YELLOW << "ID   Domain   Strength   BC_Score   Final Score" << RESET << std::endl;
        recommedate << "ID,Domain,Strength,Betweenness,Final Score" << '\n';

        int cnt=0;//to track the number of recommedations given
        for(int i=0;cnt<5 && i<recommedation.size();i++)//printing top 5 recommedation
        {
            std::string ID=recommedation[i].second;
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
            std::cout << recommedation[i].second << '\t' << NodeMap[recommedation[i].second].domain << '\t' << NodeMap[recommedation[i].second].strength << '\t' << NodeMap[recommedation[i].second].normalized_bc << '\t' << recommedation[i].first << '\n';
            //writing in recommedation.csv
            recommedate << recommedation[i].second << "," << NodeMap[recommedation[i].second].domain << "," << NodeMap[recommedation[i].second].strength << "," << NodeMap[recommedation[i].second].normalized_bc << "," << recommedation[i].first << '\n';
            cnt++;
        }
        recommedate.close();//closing file pointer
    }


    void BridgeRecommedation(std::string startNode)
    {
        if(NodeMap.find(startNode)==NodeMap.end())
        {
            std::cout << RED << "Node don't exist" << RESET << std::endl;
            return;
        }

        NodeData targetUser=NodeMap[startNode];
        std::cout << CYAN << "Starting Bridge Recommedation Engine for " << startNode << "(Community: " << targetUser.communityID << ")" << std::endl;

        //transversing by community
        std::vector<std::pair<double,std::string>> bridgeRecommedate(3);//store the highest score for bridge connection by from a community from the startNode community
        double TopScoreOfCommunity=0.0;
        int TopCommunity=0;

        std::unordered_map<std::string,double> dist_startNode_community=DijsktraAlgorithm(startNode);//to store shortest path from startNode in startNode's community

        for(int i=0;i<numberOfCommunity;i++)
        {
            if(i==targetUser.communityID-1)
                continue;

            //community1 node , community2 node
            std::pair<std::string,std::string> globalBridge=GlobalBridgeRecommedation(targetUser.communityID-1,i);
            //these nodes in globalBridge would act as connecting nodes for the two community and we would then direct a path from them from one communtiy to another to recommend bridge connections between communities
            if(globalBridge.first=="_" || globalBridge.second=="_")
                continue;
            
            std::vector<std::pair<double,std::string>> bridgeRecommedationCommunity;//to store the score for current Bridge recommedation of cummonity

            for(auto &x:communityArr[i])
            {
                std::unordered_map<std::string,double> dist_ith_community_from_Global_Bridge_Recommedation=DijsktraAlgorithm(globalBridge.second);//to store distance of all nodes from global recommedated node in second community

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
            std::sort(bridgeRecommedationCommunity.begin(),bridgeRecommedationCommunity.end(),std::greater<std::pair<double,std::string>>());

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
        std::cout << GREEN << "Successfully completed Bridge Recommedation" << RESET << std::endl;

        std::ofstream BridgeRecommedationFile(BRIDGE_RECOMMEDATION);
        if(!BridgeRecommedationFile.is_open())
        {
            std::cout << RED << "Cannot write Bridge Recommedation " << RESET << std::endl;
        }
        std::cout << YELLOW << "Top 3 Bridge Recommedation from " << startNode << "of community " << targetUser.communityID << " to community " << TopCommunity << RESET << std::endl;
        BridgeRecommedationFile << "ID,Domain,Strength,Betweenness,Final Score\n";
        std::cout << "ID\tDomain\tStrength\tBetweenness\tCommunityID\tFinal Score" << std::endl;
        for(int i=0;i<3;i++)
        {
            //writing recommedation in file .csv
            BridgeRecommedationFile << bridgeRecommedate[i].second << "," << NodeMap[bridgeRecommedate[i].second].domain << "," << NodeMap[bridgeRecommedate[i].second].strength << "," << NodeMap[bridgeRecommedate[i].second].normalized_bc << "," << bridgeRecommedate[i].first << '\n';
            //printing bridge recommedation
            std::cout << bridgeRecommedate[i].second << "\t" << NodeMap[bridgeRecommedate[i].second].domain << "\t" << NodeMap[bridgeRecommedate[i].second].strength << "\t" << NodeMap[bridgeRecommedate[i].second].normalized_bc << "\t" << TopCommunity << "\t" << bridgeRecommedate[i].first << '\n';
        }
        BridgeRecommedationFile.close();//closing the file pointer

        //for computing 
        std::pair<std::string,std::string> bridge=GlobalBridgeRecommedation(targetUser.communityID-1,TopCommunity-1);
        CompareCost(bridge);
    }


    double CostCalculation(std::string startNode)
    {
        if (NodeMap.find(startNode) == NodeMap.end())
        {
            std::cout << RED << "Invalid start node ID: " << startNode << RESET << std::endl;
            return -1;
        }
        // Total Cost function to track the total cost being spent
        double totalCost = 0.0;
        // Roundcount to keep the count of the rounds being required to spread the info
        int roundCount = 0;
        // informed set to keep the track of the nodes already aware of the info
        std::unordered_set<std::string> informed;
        // newly_informed set to keep the track of the nodes just aware of the info in the last round
        std::unordered_set<std::string> newly_informed;

        informed.insert(startNode);
        newly_informed.insert(startNode);

        while (!newly_informed.empty())
        {
            // nextRound set to store the nodes who will be conveyed the info in the next round
            std::unordered_set<std::string> nextRound;
            roundCount++;

            for (const std::string &u : newly_informed)
            {
                for (auto &neigh : AdjList[u])
                {
                    std::string v = neigh.adjacentNode;
                    int wt = neigh.weight;

                    if (informed.find(v) == informed.end())
                    {
                        double cost = BASE_COST / (double)wt; 
                        totalCost += cost;
                        // both nextRound and informed being updated
                        nextRound.insert(v);
                        informed.insert(v);
                    }
                }
            }
            // deleting the previously store nodes of newly_informed
            newly_informed = nextRound;
        }

        std::cout << GREEN << "Information reached " << informed.size() << " / "
                << NodeMap.size() << " nodes in " << roundCount << " rounds." << RESET << std::endl;

        std::cout << YELLOW << "Total cost of information spread from " << startNode
                << " = " << std::fixed << std::setprecision(6) << totalCost << RESET << std::endl;

        return totalCost;
    }


    void CompareCost(std::pair<std::string,std::string> bridge)
    {
        std::string u = bridge.first;
        std::string v = bridge.second;

        if (NodeMap.find(u) == NodeMap.end() || NodeMap.find(v) == NodeMap.end())
        {
            std::cout << RED << "Invalid bridge nodes provided." << RESET << std::endl;
            return;
        }

        std::cout << CYAN << "=== Comparing Information Spread Cost Before and After Bridging ===" << RESET << std::endl;
        std::cout << "Bridge: (" << u << ", " << v << ")" << std::endl;

        // Cost before adding bridge
        double costBeforeU= DEPLOY_COST+CostCalculation(u);
        double costBeforeV= DEPLOY_COST+CostCalculation(v);
        double totalCostBefore = costBeforeU + costBeforeV;

        // Add temporary bridge
        AdjList[u].push_back(AdjListNode(v, 1)); // neutral weight = 1
        AdjList[v].push_back(AdjListNode(u, 1));

        // Cost after adding bridge
        double costAfterU= DEPLOY_COST+CostCalculation(u);
        double totalCostAfter= costAfterU;

        // Restore original graph (remove temporary bridge)
        AdjList[u].pop_back();
        AdjList[v].pop_back();

        // Compute improvement %
        double improvement = ((totalCostBefore - totalCostAfter) / totalCostBefore) * 100.0;

        std::cout << CYAN << "=== COST COMPARISON RESULTS ===" << RESET << std::endl;
        std::cout << "Before Bridging : " << YELLOW << std::fixed << std::setprecision(6) << totalCostBefore << RESET << std::endl;
        std::cout << "After Bridging  : " << GREEN << std::fixed << std::setprecision(6) << totalCostAfter << RESET << std::endl;
        std::cout << "Improvement     : " << std::fixed << std::setprecision(2)
                << improvement << "% reduction in total cost" << std::endl;

        // Save results (append mode)
        std::ofstream resultsFile("InformationCostComparison.csv", std::ios::app);
        resultsFile << "u,v,Total Cost Before,Total Cost after,Improvement" << '\n';
        if (resultsFile.is_open())
        {
            resultsFile << u << "," << v << ","
                        << std::fixed << std::setprecision(6)
                        << totalCostBefore << "," << totalCostAfter << ","
                        << improvement << "\n";
            resultsFile.close();
        }
        else
        {
            std::cout << RED << "Could not write results to InformationCostComparison.csv" << RESET << std::endl;
        }
    }
};
