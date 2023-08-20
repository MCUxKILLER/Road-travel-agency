// Prints a path
void printPath(const vector<int>& path, const vector<vector<int>>& matrix,int &count) {
    cout<<"["<<count<<"] ";
    count++;
    for (int i = 0; i < path.size() - 1; i++) {
        cout << path[i] << " -> ";
    }
    cout << path.back() << endl;

    // Calculate the cost of the path
    int cost = 0;
    for (int i = 0; i < path.size() - 1; i++) {
        cost += matrix[path[i]][path[i+1]];
    }
    cout << "Cost: " << cost << endl;
}

// Finds all paths from the source to the destination using DFS
void findAllPaths(const vector<vector<int>>& matrix, int n, int m, int src, int dst, vector<bool>& visited, vector<int>& path,vector<vector<int>> &store) {
    visited[src] = true;
    path.push_back(src);
    if (src == dst) {
        store.push_back(path);
        printPath(path, matrix,count);
    } else {
        for (int neighbor = 0; neighbor < m; neighbor++) {
            if (matrix[src][neighbor] > 0 && !visited[neighbor]) {
                findAllPaths(matrix, n, m, neighbor, dst, visited, path,store);
            }
        }
    }
    path.pop_back();
    visited[src] = false;
}