#include <cstdlib>
#include <ctime>
#include <fstream>
#include <vector>
#include <iomanip>
#include "algorithms/CFR.cpp"
#include "games/KuhnPoker.hpp"
using namespace std;
using namespace kuhn_poker;

int iterations = 1000000;

void cfr_kuhn(string strategy, string output){
    KuhnPoker kuhn_poker;
    CFR<State, Action, Properties, InformationSet, Hash> cfr({&kuhn_poker}, 1e-5);
    cfr.train(iterations);
    cfr.print_strategy();
}
int main(int argc, char **argv) {
    srand(time(NULL));
    cfr_kuhn("", "");
}
