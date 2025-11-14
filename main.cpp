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
    cout << "\n";
    cout << "\n";
    sniegraph.betweennessCentrality();//calculating betweenesscentrality
    cout << "\n";
    cout << "\n";

    sniegraph.Recommedation("34");
    cout << "\n";
    cout << "\n";
    sniegraph.BridgeRecommedation("40");
}
