#include <cstdlib>
#include <ctime>
#include <vector>
#include "games/Dudo/Dudo.hpp"
#include "algorithms/DFS.cpp"
using namespace std;
using namespace dudo;

int main() {
	srand(time(NULL));
	vector<vector<double>>dudos(3, vector<double>(3, 0));
	for(int i = 0; i < 3; i++) {
		for (int j = 0; j < 3; j++) {
			dudos[i][j] = 3*i + j;
		}
	}
	Dudo dudo(2, 2, 2, dudos);
	DFS<State, Action, Properties, InformationSet> dfs({&dudo});
	dfs.start_dfs();
}