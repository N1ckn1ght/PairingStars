#include <climits>
#include <iostream>
#include <fstream>
#include <set>
#include <vector>
#include <utility>

using namespace std;

int n, m;						// red and blue vertices amount
vector <vector <double>> graph;	// matrix of distances
vector <int> curr;				// contains chosen vertices of 1st fraction by index of 2nd

double dist(pair<int, int> a, pair<int, int> b) {
	double x1 = a.first;
	double y1 = a.second;
	double x2 = b.first;
	double y2 = b.second;
	return sqrt((x2 - x1) * (x2 - x1) + (y2 - y1) * (y2 - y1));
}

void read(string path) {
	ifstream in(path);
	in >> n;
	m = n;						// works only with N = M by now
	vector <pair <int, int>> red(n);
	vector <pair <int, int>> blue(m);
	graph.resize(n, vector<double>(m));
	for (int i = 0; i < n; i++) {
		in >> red[i].first >> red[i].second;
	}
	for (int i = 0; i < m; i++) {
		in >> blue[i].first >> blue[i].second;
	}
	for (int i = 0; i < n; i++) {
		for (int j = 0; j < m; j++) {
			graph[i][j] = dist(red[i], blue[j]);
		}
	}
	curr.assign(m, -1);
	in.close();
}

void write()
{
	for (int i = 0; i < m; i++)
	{
		if (curr[i] > -1)
		{
			cout << curr[i] + 1 << " " << i + 1 << endl;
		}
	}
}

bool dfs4(
	int vertice, 
	const vector <vector <int>>& graph,
	vector <char>& used,
	set <int>& z1,
	set <int>& z2) 
{
	if (used[vertice]) return false;
	used[vertice] = true;
	
	z1.insert(vertice);

	for (int i = 0; i < graph[vertice].size(); i++)
	{
		int destination = graph[vertice][i];	// vertice from 2nd fraction
		z2.insert(destination);
		if (curr[destination] == -1 || dfs4(curr[destination], graph, used, z1, z2)) {
			curr[destination] = vertice;
			return true;
		}
	}
	return false;
}

void diff4() {
	vector <double> u(n, INT_MAX);
	vector <double> v(n, INT_MAX);
	vector <vector <int>> graphKuhn;
	
	// contains data about whether vertice of 1st fraction has an assignment
	vector <char> used1(n, false);
	// for dfs
	vector <char> used;
	
	// initial potential calculations
	for (int i = 0; i < n; i++) {
		for (int j = 0; j < m; j++) {
			u[i] = min(u[i], graph[i][j]);
		}
	}
	for (int i = 0; i < m; i++) {
		for (int j = 0; j < n; j++) {
			v[i] = min(v[i], graph[j][i] - u[j]);
		}
	}

	int foundAssingments = 0;
	// cycle until all possible vertices of 1st fraction will have their pair
	while (foundAssingments < min(n, m)) {
		int currentFoundAssingments = foundAssingments;

		graphKuhn.assign(n, vector <int>());
		// assign edges of 0 weight
		for (int i = 0; i < n; i++) {
			for (int j = 0; j < m; j++) {
				if (graph[i][j] == u[i] + v[j]) {
					graphKuhn[i].push_back(j);
				}
			}
		}

		// store vertices that dfs() did reach
		set <int> z1;
		set <int> z2;

		for (int i = 0; i < n; i++) {
			if (!used1[i]) {
				used.assign(n, false);
				if (dfs4(i, graphKuhn, used, z1, z2)) {
					used1[i] = true;
					currentFoundAssingments++;
				}
			}
		}

		// if found new assingments, just cycle again, the magic comes in else
		if (currentFoundAssingments > foundAssingments) {
			foundAssingments = currentFoundAssingments;
			continue;
		}

		double delta = INT_MAX;
		for (set <int>::iterator it = z1.begin(); it != z1.end(); it++)
		{
			for (int j = 0; j < m; j++) {
				if (z2.find(j) == z2.end()) {
					delta = min(delta, graph[*it][j] - (u[*it] + v[j]));
				}
			}
			
		}

		for (set <int>::iterator it = z1.begin(); it != z1.end(); it++)
		{
			u[*it] += delta;
		}
		for (set <int>::iterator it = z2.begin(); it != z2.end(); it++)
		{
			v[*it] -= delta;
		}
	}
}

int main() {
	read("./input.txt");
	diff4();
	write();
	// diff3();
}
