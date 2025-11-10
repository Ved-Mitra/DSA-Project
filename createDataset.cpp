#include <bits/stdc++.h>
using namespace std;

//this code creates edge.csv ({U,V,Weight})
//this code creates nodes.csv ({id,domain,strenght})


//strength -- >indicator of strength in there skill
//domain --> domain of there work

void createEdgesCSV(const string filePath)
{
    ifstream data(filePath);
    const string edgesPath="Dataset\\edges.txt";
    ofstream edges(edgesPath);
    string line;//to store line by line from data filePath
    //here we have not used rand() and seed(), the old way in c because the rand() has predictive patterns
    random_device rd;//seeding random number
    mt19937 gen(rd());//psedo-random engine assigning
    uniform_int_distribution<> distribute(1,4);//maps the engines output to [1,4] both inclusive for random weight described in later part of code
    if(!edges.is_open())
    {
        cout << "cannot open edges.txt file " << "Try Again" <<  "\n";
        return;
    }

    if(data.is_open())
    {
        cout << "Reading from " << filePath << " for Edges" << endl;
        cout << "-----------------" << endl;
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
        }
        //closing the file pointers
        edges.close();
        data.close();
        cout << "Succesfully wrote the edges.txt file";
        //renaming edges.txt to edges.csv for better readability
        int success=rename("Dataset\\edges.txt","Dataset\\edges.csv");
        if(success==0)
            cout << "Succesfully renamed edges.txt to edges.csv" << endl;
        else
            cout << "Renaming Failed" << endl;
    }
    else
        cout << filePath << " not opened" << endl;
}

void countUniqueIds(const string FilePath,set<string> &UniqueID)
{
    cout << "Starting the counting of nodes" << endl;
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
                cout << "Processed ..." << linesProcessed << " lines" << "\n";
            }
        }
        cout << "Finished counting nodes" << '\n';
        data.close();
    }
    else
        cout << "Counting of unique IDS failed" << "\n";
}

void createNodesCSV(const string filePath)
{
    ofstream nodes("Dataset\\nodes.txt");
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
        cout << "Reading from " << filePath << " for Nodes" << endl;
        cout << "-----------------" << endl;
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
        cout << "Succesfully wrote the nodes.txt file";
        //renaming edges.txt to edges.csv for better readability
        int success=rename("Dataset\\nodes.txt","Dataset\\nodes.csv");
        if(success==0)
            cout << "Succesfully renamed nodes.txt to nodes.csv" << endl;
        else
            cout << "Renaming Failed" << endl;
    }
    else
        cout << "Dataset/nodes.txt cannot be" << " not opened" << endl;
}

int main()
{
    //filePath examples are:
    // 1.   "Dataset\\facebook_combined(UG).txt"
    // 2.   "Dataset\\twitter_combined(DG).txt"
}