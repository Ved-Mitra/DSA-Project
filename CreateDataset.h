#pragma once //this line tells the compiler to only inlcude this file once

#include <bits/stdc++.h>
#include "Constant.h"


//this code creates edge.csv ({U,V,Weight})
//this code creates nodes.csv ({id,domain,strenght})


//strength -- >indicator of strength in there skill
//domain --> domain of there work

class CreateData
{
    public:
    CreateData (const std::string FIlePath)
    {
        createEdgesCSV(FIlePath);
        createNodesCSV(FIlePath);
    }

    private:
    void createEdgesCSV(const std::string filePath)
    {
        std::ifstream data(filePath);
        std::ofstream edges(EDGESFILEPATH);
        std::string line;//to store line by line from data filePath
        //here we have not used rand() and seed(), the old way in c because the rand() has predictive patterns
        std::random_device rd;//seeding random number
        std::mt19937 gen(rd());//psedo-random engine assigning
        std::uniform_int_distribution<> distribute(1,4);//maps the engines output to [1,4] both inclusive for random weight described in later part of code
        if(!edges.is_open())
        {
            std::cout << RED << "cannot open edges.txt file " << "Try Again" << RESET << "\n";
            return;
        }

        if(data.is_open())
        {
            std::cout << CYAN << "Reading from " << filePath << " for Edges" << RESET << std::endl;
            std::cout << CYAN << "-----------------" << RESET << std::endl;
            edges << "u" << "," << "v" << "," << "weight" << '\n';//adding header
            while(std::getline(data,line))
            {
                //now assigning random weights to edges according to the below chart
                /*1 = LinkedIn / Acquaintance (Weak tie)
                2 = Colleague / Business Partner (Moderate tie)
                3 = Friend (Strong tie)
                4 = Close Friend / Confidant (Very strong tie)*/
                int random_Weight=distribute(gen);
                std::stringstream ss(line);//for readin U and V
                std::string u,v;
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
            std::cout << GREEN  << "Succesfully wrote the edges.txt file" << RESET << std::endl;
            //renaming edges.txt to edges.csv for better readability
        //    int success=rename("Dataset\\edges.txt","Dataset\\edges.csv");
        //    if(success==0)
        //        cout << GREEN << "Succesfully renamed edges.txt to edges.csv" <<  RESET << endl;
        //    else
        //        cout << RED << "Renaming Failed" <<  RESET << endl;
        }
        else
            std::cout << RED << filePath << " not opened" << RESET << std::endl;
    }

    void countUniqueIds(const std::string FilePath,std::set<std::string> &UniqueID)
    {
        std::cout << CYAN << "Starting the counting of nodes" << RESET << std::endl;
        long long linesProcessed=0;//to count on what line the program is now 
        std::ifstream data(FilePath);
        if(data.is_open())
        {
            std::string line;//to store line by line string from filePath
            std::string node1,node2;//to store the nodes read in egdes
            while(std::getline(data,line))//to get line by line from filePath
            {
                std::stringstream ss(line);
                ss >> node1 >> node2;//to get node1 and node2
                //storing the node1 and node2
                if(!node1.empty())//just a check to avoid any sort of errors
                    UniqueID.insert(node1);
                if(!node2.empty())
                    UniqueID.insert(node2);
                linesProcessed++;
                if(linesProcessed%10000 == 0)
                {//just for checking progress at each point of time
                    std::cout << YELLOW << "Processed ..." << linesProcessed << " lines" << RESET << "\n";
                }
            }
            std::cout << GREEN << "Finished counting nodes" << RESET << '\n';
            data.close();
        }
        else
            std::cout << RED << "Counting of unique IDS failed" << RESET <<"\n";
    }

    void createNodesCSV(const std::string filePath)
    {
        std::ofstream nodes(NODESFILEPATH);
        std::string line;//to store line by line from data filePath
        //here we have not used rand() and seed(), the old way in c because the rand() has predictive patterns
        std::random_device rd;//seeding random number
        std::mt19937 gen(rd());//psedo-random engine assigning
        std::uniform_int_distribution<> distribute_domain(1,4);//maps the engines output to [1,4] both inclusive for random domains described in later part of code
        std::uniform_real_distribution<> distribute_strength(0.0,10.0);//maps the engines output to [0.0,10.0] both inclusive for random strength 

        std::set<std::string> UniqueID;//to store the Unique ID i.e nodes
        countUniqueIds(filePath,UniqueID);

        if(nodes.is_open())
        {
            std::cout << CYAN << "Reading from " << filePath << " for Nodes" << RESET << std::endl;
            std::cout << CYAN  << "-----------------" << RESET << std::endl;
            nodes << "id" << "," << "domain" << "," << "strength" << '\n';
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
                std::string ID= *UniqueID.begin();
                UniqueID.erase(UniqueID.begin());

                nodes << ID << "," << random_domain << "," << std::fixed << std::setprecision(2) << random_strength << "\n";
            }
            //closing the file pointers
            nodes.close();
            std::cout << GREEN << "Succesfully wrote the nodes.txt file" << RESET << std::endl;
            //renaming edges.txt to edges.csv for better readability
        //    int success=rename("Dataset\\nodes.txt","Dataset\\nodes.csv");
        //    if(success==0)
        //        cout << GREEN << "Succesfully renamed nodes.txt to nodes.csv" << RESET << endl;
        //    else
        //        cout << RED << "Renaming Failed" << RESET << endl;
        }
        else
            std::cout << RED << "Dataset/nodes.txt cannot be" << " not opened" << RESET << std::endl;
    }
};
