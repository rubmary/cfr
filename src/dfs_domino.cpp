#include <cstdlib>
#include <ctime>
#include "games/Domino/Domino.hpp"
#include "algorithms/DFS.cpp"
using namespace std;
using namespace domino;

int main() {
	srand(time(NULL));
    cout << "hola mundo" << endl;
    Domino domino(3, 2);
    DFS<State, Action, Properties, InformationSet, Hash> dfs({&domino});
    dfs.start_dfs();
    cout << "Termino dfs" << endl;
}