#include <cstdlib>
#include <ctime>
#include "games/KuhnPoker/KuhnPoker.hpp"
#include "games/Dudo/Dudo.hpp"
#include "games/Domino/Domino.hpp"
#include "games/OCP/OCP.hpp"
#include "algorithms/DFS.cpp"
using namespace std;
using namespace kuhn_poker;

int main() {
    srand(time(NULL));
    KuhnPoker kuhn_poker;
    DFS<State, Action, Properties, InformationSet, Hash> dfs({&kuhn_poker});
    dfs.start_dfs();
}