#include <bits/stdc++.h>
using namespace std;

#include "CreateDataset.h"
#include "Graph.h"

const string filePath="Dataset\\facebook_combined(UG).txt";

int main()
{
    CreateData data(filePath);//creating dataset

    Graph sniegraph;
    sniegraph.detectCommunities();//detecting communities
    sniegraph.betweennessCentrality();//calculating betweenesscentrality

    sniegraph.Recommedation("0");
}