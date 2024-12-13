//#include <algorithm>
//#include <chrono>
//#include <iostream>
//#include <limits>
//#include <queue>
//#include <vector>
//#include "string"
//
//
//using namespace std;
//using namespace chrono;
//
//struct Edge {
//    int to, weight;
//};
//
//vector<int> dijkstra(int start, int target, const vector<vector<Edge>>& graph) {
//    auto s = high_resolution_clock::now();
//    vector<int> dist(graph.size(), numeric_limits<int>::max());
//    vector<int> prev(graph.size(), -1);
//    dist[start] = 0;
//
//    priority_queue<pair<int, int>, vector<pair<int, int>>, greater<>> pq;
//    pq.push({0, start});
//
//    while (!pq.empty()) {
//        int v = pq.top().second;
//        int current_dist = pq.top().first;
//        pq.pop();
//
//        if (current_dist > dist[v]) continue;
//
//        for (const Edge& edge : graph[v]) {
//            if (dist[v] + edge.weight < dist[edge.to]) {
//                dist[edge.to] = dist[v] + edge.weight;
//                prev[edge.to] = v;
//                pq.push({dist[edge.to], edge.to});
//            }
//        }
//    }
//
//    vector<int> path;
//    for (int at = target; at != -1; at = prev[at]) {
//        path.push_back(at);
//    }
//    reverse(path.begin(), path.end());
//
//    if (path.size() == 1 && path[0] != start) {
//        path.clear();
//    }
//
//    auto e = high_resolution_clock::now();
//    auto duration = duration_cast<milliseconds>(e - s);
//    cout << "================" << endl;
//    cout << "Total time: " << duration.count() << " ms" << endl;
//    cout << "================" << endl;
//
//    return path;
//}
//
//int main(int argc, char *argv[]) {
//    if (argc != 4) {
//        cerr << "Using: " << argv[0] << " <number of vertices> <target vertex> <repetitions>\n";
//        return 1;
//    }
//
//    int vertex_count = stoi(argv[1]);
//    int target_vertex = stoi(argv[2]);
//    int repetitions = stoi(argv[3]);
//
//
//    if (target_vertex >= vertex_count) {
//        cerr << "Error: Target vertex exceeds the number of vertices in the graph.\n";
//        return 1;
//    }
//
//    vector<vector<Edge>> graph(vertex_count);
//    for (int i = 0; i < vertex_count; ++i) {
//        for (int j = 0; j < 3; ++j) {
//            int to = rand() % vertex_count;
//            int weight = rand() % 100 + 1;
//            graph[i].push_back({to, weight});
//        }
//    }
//
//    vector<int> shortest_path;
//    for (int i = 0; i < repetitions; i++) {
//        shortest_path = dijkstra(0, target_vertex, graph);
//    }
//
//    return 0;
//}
#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include <chrono>

using namespace std;
using namespace chrono;

vector<int> primeFactors(long long n) {
    vector<int> factors;
auto s = high_resolution_clock::now();
    while (n % 2 == 0) {
        factors.push_back(2);
        n /= 2;
    }

    for (int i = 3; i * i <= n; i += 2) {
        while (n % i == 0) {
            factors.push_back(i);
            n /= i;
        }
    }

    if (n > 2) {
        factors.push_back(n);
    }
    auto e = high_resolution_clock::now();
    auto duration = duration_cast<milliseconds>(e - s);
    cout << "================" << endl;
    cout << "Total time: " << duration.count() << " ms" << endl;
    cout << "================" << endl;
    return factors;
}

int main(int argc, char* argv[]) {
    if (argc != 2) {
        cerr << "Usage: " << argv[0] << " <number>\n";
        return 1;
    }

    long long number = stoll(argv[1]);

    if (number <= 1) {
        cout << "Number must be greater than 1." << endl;
        return 1;
    }
    string filename = argv[1];
    vector<int> factors = primeFactors(number);

    cout << "Prime factors of " << number << ": ";
    for (int factor : factors) {
        cout << factor << " ";
    }
    cout << endl;

    return 0;
}
