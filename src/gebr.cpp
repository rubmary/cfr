#include <cstdlib>
#include <ctime>
#include <fstream>
#include <vector>
#include <iomanip>
#include "algorithms/GEBR.cpp"
#include "games/KuhnPoker.hpp"
#include "games/OCP.hpp"
#include "games/Dudo.hpp"
using namespace std;

void gebr_kuhn(ifstream &is_inf_sets, ifstream &is_strategy, ostream &os_eval)
{
	using namespace kuhn_poker;
	KuhnPoker kuhn;
	GEBR<State, Action, Properties, InformationSet, Hash> gebr(&kuhn, is_inf_sets);
	gebr.explotability(is_strategy, os_eval);
}

void gebr_ocp(ifstream &is_inf_sets, ifstream &is_strategy, ostream &os_eval, int N)
{
    using namespace ocp;
    OCP ocp(N);
    GEBR<State, Action, Properties, InformationSet, Hash> gebr(&ocp, is_inf_sets);
    gebr.explotability(is_strategy, os_eval);
}

void gebr_dudo(
    ifstream &is_inf_sets,
    ifstream &is_strategy,
    ostream &os_eval,
    int K,
    int D1,
    int D2
    )
{
    using namespace dudo;
    vector<vector<double>>dudos(3, vector<double>(3, 0));
    for (int i = 0; i < (int) dudos.size(); i++) {
        for (int j = 0; j < (int) dudos[i].size(); j++) {
            if(i == 0) {
                dudos[0][j] = -1;
            } else if (j == 0) {
                dudos[i][0] = 1;
            }
        }
    }
    Dudo dudo(K, D1, D2, dudos);
    GEBR<State, Action, Properties, InformationSet, Hash> gebr(&dudo, is_inf_sets);
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
    string folder = game;
    for (int i = 2; i < argc; i++) {
        string separator = i == 2 ? "/" : "_";
        folder = folder + separator + argv[i];
    }
    string path_inf_sets = "results/" + folder + "/information_sets.txt";
    string path_strategy = "results/" + folder + "/strategy.txt";
    string path_eval = "results/" + folder + "/strategy_evaluation.txt";
    ifstream is_inf_sets(path_inf_sets.c_str());
    ifstream is_strategy(path_strategy.c_str());
    ofstream os_eval(path_eval.c_str());

    if(game == "KuhnPoker"){
        gebr_kuhn(is_inf_sets, is_strategy, os_eval);
    } else if(game == "OCP"){
        if (argc < 3) {
            cout << "Debes introducir el numero de cartas" << endl;
            return 0;
        }
        int N = atoi(argv[2]);
        gebr_ocp(is_inf_sets, is_strategy, os_eval, N);
    } else if(game == "Dudo") {
        if (argc < 5) {
            cout << "Debes introducir los parametros: K, D1, D2" << endl;
            return 0;
        }
        int K, D1, D2;
        K = atoi(argv[2]);
        D1 = atoi(argv[3]);
        D2 = atoi(argv[4]);
        gebr_dudo(is_inf_sets, is_strategy, os_eval, K, D1, D2);
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