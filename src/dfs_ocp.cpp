#include <cstdlib>
#include <ctime>
#include "games/OCP/OCP.hpp"
#include "algorithms/DFS.cpp"
using namespace std;

int main() {
    srand(time(NULL));
    OCP ocp(4);
    DFS<State, Action, Properties, InformationSet> dfs({&ocp});
    dfs.start_dfs();
}