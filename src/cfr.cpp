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

void cfr_kuhn(ostream& os_regret, ostream& os_strategy, ostream& os_inf_sets){
    KuhnPoker kuhn_poker;
    CFR<State, Action, Properties, InformationSet, Hash> cfr({&kuhn_poker}, 1e-5);
    cfr.train(iterations, os_regret);
    cfr.print_strategy(os_strategy);
    kuhn_poker.print_information_sets(os_inf_sets);
}
int main(int argc, char **argv) {
    srand(time(NULL));
    if(argc < 2) {
        cout << "Debes introducir el nombre del juego";
        cout << endl;
        return 0;
    }

    string game = argv[1];
    string path_regret = "regret/" + game + "/regret.txt";
    string path_strategy = "results/" + game + "/strategy.txt";
    string path_inf_sets = "results/" + game + "/information_sets.txt";
    ofstream os_regret(path_regret.c_str());
    ofstream os_strategy(path_strategy.c_str());
    ofstream os_inf_sets(path_inf_sets.c_str());

    if(game == "KuhnPoker"){
        cfr_kuhn(os_regret, os_strategy, os_inf_sets);
    } else if(game == "OCP"){
        cout << "En construccion" << endl;
    } else if(game == "Dudo") {
        cout << "En construccion" << endl;
    } else if (game == "Domino") {
        cout << "En construccion" << endl;
    } else {
        cout << "Error" << endl;
        cout << "Los juegos validos son:" << endl;
        cout << "\tKuhnPoker" << endl;
        cout << "\tOCP" << endl;
        cout << "\tDudo" << endl;
        cout << "\tDomino" << endl;
    }
}
