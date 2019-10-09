#include <cstdlib>
#include <ctime>
#include <fstream>
#include <vector>
#include <iomanip>
#include "algorithms/CFR.cpp"
#include "games/KuhnPoker.hpp"
#include "games/OCP.hpp"
#include "games/Dudo.hpp"
using namespace std;
int iterations = 10000000;
#define EPS 1e-5

void cfr_kuhn(ostream& os_regret, ostream& os_strategy, ostream& os_inf_sets) {
    using namespace kuhn_poker;
    KuhnPoker kuhn_poker;
    CFR<State, Action, Properties, InformationSet, Hash> cfr({&kuhn_poker}, EPS);
    cfr.train(iterations, os_regret);
    cfr.print_strategy(os_strategy);
    kuhn_poker.print_information_sets(os_inf_sets);
}

void cfr_ocp(ostream& os_regret, ostream& os_strategy, ostream& os_inf_sets, int N) {
    using namespace ocp;
    OCP ocp(N);
    CFR<State, Action, Properties, InformationSet, Hash> cfr({&ocp}, EPS);
    cfr.train(iterations, os_regret);
    cfr.print_strategy(os_strategy);
    ocp.print_information_sets(os_inf_sets);
}

void cfr_dudo(ostream& os_regret, ostream& os_strategy, ostream& os_inf_sets, int K, int D1, int D2) {
    using namespace dudo;
    vector<vector<double>> dudos(D1 + 1, vector<double>(D2 + 1, 0));
    ofstream tmp_os("output/dudo_regret.txt");
    dudos[0][1] = -1;
    dudos[1][0] = 1;
    for (int i = 1; i <= D1; i++) {
        for (int j = 1; j <= D2; j++) {
            if (i == 0) {
                dudos[0][j] = -1;
                continue;
            }
            if (j == 0) {
                dudos[i][0] = 1;
                continue;
            }
            if (i == D1 && j == D2)
                continue;
            Dudo dudo(K, i, j, dudos);
            CFR<State, Action, Properties, InformationSet, Hash> cfr({&dudo}, EPS);
            cfr.train(iterations, tmp_os);
            vector<vector<double>> strategy = cfr.average_strategy();
            dudos[i][j] = dudo.expected_value(1, strategy);
            cfr.print_strategy(tmp_os);
            cout << i << ' ' << j << ' ' << dudos[i][j] << endl;
        }
    }
    Dudo dudo(K, D1, D2, dudos);
    CFR<State, Action, Properties, InformationSet, Hash> cfr({&dudo}, EPS);
    cfr.train(iterations, os_regret);
    cfr.print_strategy(os_strategy);
    dudo.print_information_sets(os_inf_sets);
}

int main(int argc, char **argv) {
    srand(time(NULL));
    if(argc < 2) {
        cout << "Debes introducir el nombre del juego";
        cout << endl;
        return 0;
    }
    string game = argv[1];
    string folder = game;
    for (int i = 2; i < argc; i++) {
        string separator = i == 2 ? "/" : "_";
        folder = folder + separator + argv[i];
    }
    string path_regret = "regret/" + folder + "/regret.txt";
    string path_strategy = "results/" + folder + "/strategy.txt";
    string path_inf_sets = "results/" + folder + "/information_sets.txt";
    ofstream os_regret(path_regret.c_str());
    ofstream os_strategy(path_strategy.c_str());
    ofstream os_inf_sets(path_inf_sets.c_str());

    if(game == "KuhnPoker"){
        cfr_kuhn(os_regret, os_strategy, os_inf_sets);
    } else if(game == "OCP"){
        if (argc < 3) {
            cout << "Debes introducir el numero de cartas" << endl;
            return 0;
        }
        int N = atoi(argv[2]);
        cfr_ocp(os_regret, os_strategy, os_inf_sets, N);
    } else if(game == "Dudo") {
        if (argc < 5) {
            cout << "Debes introducir los parametros K, D1 y D2" << endl;
            return 0;
        }
        int K, D1, D2;
        K = atoi(argv[2]);
        D1 = atoi(argv[3]);
        D2 = atoi(argv[4]);
        cfr_dudo(os_regret, os_strategy, os_inf_sets, K, D1, D2);
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
