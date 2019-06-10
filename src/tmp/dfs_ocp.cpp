#include <cstdlib>
#include <ctime>
#include "games/OCP/OCP.hpp"
#include "algorithms/DFS.cpp"
using namespace std;
using namespace ocp;

int main() {
    srand(time(NULL));
    OCP ocp(4);
    DFS<State, Action, Properties, InformationSet, Hash> dfs({&ocp});
    dfs.start_dfs();
}