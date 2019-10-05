#include <cstdlib>
#include <ctime>
#include <fstream>
#include <vector>
#include <iomanip>
#include "algorithms/GEBR.cpp"
#include "games/KuhnPoker.hpp"
#include "games/OCP.hpp"
using namespace std;

void gebr_kuhn(ifstream &is_inf_sets, ifstream &is_strategy, ostream &os_eval) {
	using namespace kuhn_poker;
	KuhnPoker kuhn;
	GEBR<State, Action, Properties, InformationSet, Hash> gebr(&kuhn, is_inf_sets);
	gebr.explotability(is_strategy, os_eval);
}

void gebr_ocp(ifstream &is_inf_sets, ifstream &is_strategy, ostream &os_eval) {
    using namespace ocp;
    OCP ocp(2);
    GEBR<State, Action, Properties, InformationSet, Hash> gebr(&ocp, is_inf_sets);
    gebr.explotability(is_strategy, os_eval);
}

int main(int argc, char **argv) {
    if(argc < 2) {
        cout << "Debes introducir el nombre del juego";
        cout << endl;
        return 0;
    }
    srand(time(NULL));
    string game = argv[1];
    string path_inf_sets = "results/" + game + "/information_sets.txt";
    string path_strategy = "results/" + game + "/strategy.txt";
    string path_eval = "results/" + game + "/strategy_evaluation.txt";
    ifstream is_inf_sets(path_inf_sets.c_str());
    ifstream is_strategy(path_strategy.c_str());
    ofstream os_eval(path_eval.c_str());

    if(game == "KuhnPoker"){
        gebr_kuhn(is_inf_sets, is_strategy, os_eval);
    } else if(game == "OCP"){
        gebr_ocp(is_inf_sets, is_strategy, os_eval);
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