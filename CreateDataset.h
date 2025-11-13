#pragma once //this line tells the compiler to only inlcude this file once

#include <bits/stdc++.h>
#include "Constant.h"
using namespace std;

//this code creates edge.csv ({U,V,Weight})
//this code creates nodes.csv ({id,domain,strenght})


//strength -- >indicator of strength in there skill
//domain --> domain of there work

class CreateData
{
    public:
    CreateData (const string FIlePath)
    {
        createEdgesCSV(FIlePath);
        createNodesCSV(FIlePath);
    }

    private:
    void createEdgesCSV(const string filePath)
    {
        ifstream data(filePath);
        ofstream edges(EDGESFILEPATH);
        string line;//to store line by line from data filePath
        //here we have not used rand() and seed(), the old way in c because the rand() has predictive patterns
        random_device rd;//seeding random number
        mt19937 gen(rd());//psedo-random engine assigning
        uniform_int_distribution<> distribute(1,4);//maps the engines output to [1,4] both inclusive for random weight described in later part of code
        if(!edges.is_open())
        {
            cout << RED << "cannot open edges.txt file " << "Try Again" << RESET << "\n";
            return;
        }

        if(data.is_open())
        {
            cout << CYAN << "Reading from " << filePath << " for Edges" << RESET << endl;
            cout << CYAN << "-----------------" << RESET << endl;
            edges << "Node1" << "," << "Node2" << "," << "Weight" << '\n';//adding header
            while(getline(data,line))
            {
                //now assigning random weights to edges according to the below chart
                /*1 = LinkedIn / Acquaintance (Weak tie)
                2 = Colleague / Business Partner (Moderate tie)
                3 = Friend (Strong tie)
                4 = Close Friend / Confidant (Very strong tie)*/
                int random_Weight=distribute(gen);
                stringstream ss(line);//for readin U and V
                string u,v;
                ss >> u >> v;
                if(u.empty() || v.empty())
                    continue;
                edges << u << "," << v << "," << random_Weight << "\n";
                //undirected using this line
                if(UNDIRECTED_GRAPH)
                    edges << v << "," << u << "," << random_Weight << '\n';
            }
            //closing the file pointers
            edges.close();
            data.close();
            cout << GREEN  <<  "Succesfully wrote the edges.txt file" << RESET << endl;
            //renaming edges.txt to edges.csv for better readability
        //     int success=rename("Dataset\\edges.txt","Dataset\\edges.csv");
        //     if(success==0)
        //         cout << GREEN << "Succesfully renamed edges.txt to edges.csv" <<  RESET << endl;
        //     else
        //         cout << RED << "Renaming Failed" <<  RESET << endl;
        }
        else
            cout << RED << filePath << " not opened" << RESET << endl;
    }

    void countUniqueIds(const string FilePath,set<string> &UniqueID)
    {
        cout << CYAN << "Starting the counting of nodes" << RESET << endl;
        long long linesProcessed=0;//to count on what line the program is now 
        ifstream data(FilePath);
        if(data.is_open())
        {
            string line;//to store line by line string from filePath
            string node1,node2;//to store the nodes read in egdes
            while(getline(data,line))//to get line by line from filePath
            {
                stringstream ss(line);
                ss >> node1 >> node2;//to get node1 and node2
                //storing the node1 and node2
                if(!node1.empty())//just a check to avoid any sort of errors
                    UniqueID.insert(node1);
                if(!node2.empty())
                    UniqueID.insert(node2);
                linesProcessed++;
                if(linesProcessed%10000 == 0)
                {//just for checking progress at each point of time
                    cout << YELLOW << "Processed ..." << linesProcessed << " lines" << RESET << "\n";
                }
            }
            cout << GREEN << "Finished counting nodes" << RESET << '\n';
            data.close();
        }
        else
            cout << RED << "Counting of unique IDS failed" << RESET <<"\n";
    }

    void createNodesCSV(const string filePath)
    {
        ofstream nodes(NODESFILEPATH);
        string line;//to store line by line from data filePath
        //here we have not used rand() and seed(), the old way in c because the rand() has predictive patterns
        random_device rd;//seeding random number
        mt19937 gen(rd());//psedo-random engine assigning
        uniform_int_distribution<> distribute_domain(1,4);//maps the engines output to [1,4] both inclusive for random domains described in later part of code
        uniform_real_distribution<> distribute_strength(0.0,10.0);//maps the engines output to [0.0,10.0] both inclusive for random strength 

        set<string> UniqueID;//to store the Unique ID i.e nodes
        countUniqueIds(filePath,UniqueID);

        if(nodes.is_open())
        {
            cout << CYAN << "Reading from " << filePath << " for Nodes" << RESET << endl;
            cout << CYAN  <<  "-----------------" << RESET << endl;
            nodes << "ID" << "," << "Domain" << "," << "Strength/skill expertise" << '\n';
            while(!UniqueID.empty())
            {
                //now assigning random weights to edges according to the below chart
                /*
                1 = Finance
                2 = Health
                3 = AI / ML
                4 = Academia*/
                int random_domain=distribute_domain(gen);
                double random_strength=distribute_strength(gen);
                string ID= *UniqueID.begin();
                UniqueID.erase(UniqueID.begin());

                nodes << ID << "," << random_domain << "," << setprecision(2) << random_strength << "\n";
            }
            //closing the file pointers
            nodes.close();
            cout <<  GREEN << "Succesfully wrote the nodes.txt file" << RESET << endl;
            //renaming edges.txt to edges.csv for better readability
        //     int success=rename("Dataset\\nodes.txt","Dataset\\nodes.csv");
        //     if(success==0)
        //         cout << GREEN << "Succesfully renamed nodes.txt to nodes.csv" << RESET << endl;
        //     else
        //         cout << RED << "Renaming Failed" << RESET << endl;
        }
        else
            cout << RED << "Dataset/nodes.txt cannot be" << " not opened" << RESET << endl;
    }
};