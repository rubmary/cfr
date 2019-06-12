#include <cstdlib>
#include <ctime>
#include <fstream>
#include <vector>
#include <iomanip>
#include "algorithms/GEBR.cpp"
#include "games/KuhnPoker.hpp"
using namespace std;

void gebr_kuhn(ifstream &is_inf_sets, ifstream &is_strategy) {
	using namespace kuhn_poker;
	KuhnPoker kuhn;
	cout << "Creando gebr..." << endl;
	GEBR<State, Action, Properties, InformationSet, Hash> gebr(&kuhn, is_inf_sets);
	cout << "Calculando explotabilidad..." << endl;
	double e = gebr.explotability(is_strategy);
	cout << e << endl;
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
    ifstream is_inf_sets(path_inf_sets.c_str());
    ifstream is_strategy(path_strategy.c_str());

    if(game == "KuhnPoker"){
        gebr_kuhn(is_inf_sets, is_strategy);
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