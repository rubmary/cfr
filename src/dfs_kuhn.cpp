#include <cstdlib>
#include <ctime>
#include "games/KuhnPoker/KuhnPoker.hpp"
#include "algorithms/DFS.cpp"
using namespace std;
using namespace kuhn_poker;

int main() {
    srand(time(NULL));
    KuhnPoker kuhn_poker;
    DFS<State, Action, Properties, InformationSet, Hash> dfs({&kuhn_poker});
    dfs.start_dfs();
}