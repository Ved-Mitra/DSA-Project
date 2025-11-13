#include <bits/stdc++.h>
using namespace std;

#include "CreateDataset.h"
#include "Graph.h"

const string filePath="Dataset/FinalDataset.txt";

int main()
{
    CreateData data(filePath);//creating dataset

    Graph sniegraph;
    sniegraph.detectCommunities();//detecting communities
    sniegraph.betweennessCentrality();//calculating betweenesscentrality

    sniegraph.Recommedation("34");
    sniegraph.BridgeRecommedation("40");
}