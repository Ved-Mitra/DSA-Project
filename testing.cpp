#include <bits/stdc++.h>
using namespace std;
void dfs(vector<vector<int>> &adj, int n, int node, vector<int> &vis, vector<int> &groups){
    vis[node]=1;
    groups.push_back(node);
    for(auto it:adj[node]){
        if(!vis[it]){
            dfs(adj, n, it, vis, groups);
        }
    }
}
void community_min(vector<int> &sumdist, vector<vector<int>> &communities, vector<pair<int,int>> &maxCentrality){
    for(auto &i : communities){
        int mindist = INT_MAX;
        int best_node=-1;
        for(int node : i){
            if(sumdist[node]<mindist){
                mindist=sumdist[node];
                best_node=node;
            }
        }
        maxCentrality.push_back({best_node, mindist});
    }
}
void calculate_dist(vector<int> &sumdist, vector<vector<int>> &adj, int n, int node){
    for(int i=0;i<n;i++){
        vector<int> dist(n, -1);
        vector<int> vis(n, 0);
        queue<int> q;
        dist[i]=0;
        vis[i]=1;
        q.push(i);
        while(!q.empty()){
            int u=q.front();
            q.pop();
            for(int j:adj[u]){
                if (!vis[j]) {
                    vis[j] = 1;
                    dist[j] = dist[u] + 1;
                    q.push(j);
                }
            } 
        }
        int total=0;
        for(int d:dist){
            if(d>0) total+=d;
        }
        sumdist[i]=total;
    }
}
vector<vector<int>> detect(vector<vector<int>> &adj, int n){
    vector<int> vis(n,0);
    vector<vector<int>> communities;
    for(int i=0;i<n;i++){
        vector<int> groups;
        if(!vis[i]){
            dfs(adj, n, i, vis, groups);
            communities.push_back(groups);
        }
    }
    return communities;
}
void adde(int a, int b, vector<vector<int>> &adj){
    adj[a].push_back(b);
    adj[b].push_back(a);
}
int main() {
    int n;
    cout<<"Enter the number of nodes: ";
    cin>>n;
    int edges;
    cout<<"Number of edges : ";
    cin>>edges;
    vector<vector<int>> adj(n);
    for(int i=0;i<edges;i++){
        int a,b;
        cin >> a >> b;
        adde(a,b,adj);
    }
    vector<vector<int>> communities = detect(adj, n);
    //vector<pair<int,int>> max = closenesscentrality(adj,n,communities);
    vector<int> dist(n,0);
    calculate_dist(dist, adj, n, 0);
    vector<pair<int,int>> min;
    community_min(dist, communities, min);
    cout << "\n--- Community Closeness Centrality Analysis ---\n";
    for (int i = 0; i < communities.size(); i++) 
    {
        cout << "Community " << i + 1 << ": ";
        for (int node : communities[i]) cout << node << " ";
        cout << "\nMost Influential Node: " << min[i].first << "  (Total Distance: " << min[i].second << ")\n";
        cout << "Closeness Centrality: " << fixed << setprecision(2) << double(communities[i].size() - 1) / min[i].second << "\n\n";
    }
}
