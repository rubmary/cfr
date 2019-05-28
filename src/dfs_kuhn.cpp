#include <cstdlib>
#include <ctime>
#include "games/KuhnPoker/KuhnPoker.hpp"
#include "algorithms/DFS.cpp"
using namespace std;

int main() {
    srand(time(NULL));
    KuhnPoker kuhn_poker;
    DFS<State, Action, Properties, InformationSet> dfs({&kuhn_poker});
    //for (int i = 0; i < 10; i++){
        dfs.start_dfs();
    //}
}