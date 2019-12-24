#include <cstdlib>
#include <ctime>
#include <fstream>
#include <vector>
#include <iomanip>
#include "algorithms/CFR.cpp"
#include "games/KuhnPoker.hpp"
#include "games/OCP.hpp"
#include "games/Dudo.hpp"
#include "games/Domino.hpp"
using namespace std;
int total_seconds = 36;
#define EPS 1e-5

void cfr_kuhn(
    ostream& os_regret,
    ostream& os_strategy,
    ostream& os_inf_sets,
    ostream& os_iterations
    )
{
    using namespace kuhn_poker;
    KuhnPoker kuhn_poker;
    CFR<State, Action, Properties, InformationSet, Hash> cfr({&kuhn_poker}, EPS);
    cout << "training..." << endl;
    long long iterations = cfr.train(total_seconds, os_regret);
    os_iterations << "Total number of iterations:" << endl;
    os_iterations << iterations << endl;
    cout << "finished training" << endl;
    cfr.print_strategy(os_strategy);
    kuhn_poker.print_information_sets(os_inf_sets);
}

void cfr_ocp(
    ostream& os_regret,
    ostream& os_strategy,
    ostream& os_inf_sets,
    ostream& os_iterations,
    int N
) {
    using namespace ocp;
    OCP ocp(N);
    CFR<State, Action, Properties, InformationSet, Hash> cfr({&ocp}, EPS);
    long long iterations = cfr.train(total_seconds, os_regret);
    cfr.print_strategy(os_strategy);
    ocp.print_information_sets(os_inf_sets);
    os_iterations << "Total number of iterations:" << endl;
    os_iterations << iterations << endl;
}

void cfr_dudo(
    ostream& os_regret,
    ostream& os_strategy,
    ostream& os_inf_sets,
    ostream& os_iterations,
    int K,
    int D1,
    int D2
) {
    using namespace dudo;
    int N = max(D1, D2);
    vector<vector<double>> dudos(N+1, vector<double>(N+1, 0));
    for (int i = 0; i <= N; i++) {
        dudos[i][0] = 1;
        dudos[0][i] = -1; 
    }
    string tmp;
    for (int i = 1; i <= N; i++) {
        for (int j = 1; j <= N; j++) {
            if (i >= D1 && j >= D2)
                continue;
            string path = "results/Dudo/";
            path = path + to_string(K) + '_' + to_string(i) + '_' + to_string(j);
            path = path + "/strategy_evaluation.txt";
            ifstream is_expected_value(path.c_str());
            is_expected_value >> tmp >> tmp >> dudos[i][j];
            cout << i << ' ' << j << ' ' << dudos[i][j] << endl;
            is_expected_value.close();
        }
    }
    cout << "Inicializar dudo" << endl;
    Dudo dudo(K, D1, D2, dudos);
    cout << "Crear cfr.." << endl;
    CFR<State, Action, Properties, InformationSet, Hash> cfr({&dudo}, EPS);
    cout << "Entrenando..." << endl;
    long long iterations = cfr.train(total_seconds, os_regret);
    cout << "Cosas finales..." << endl;
    cfr.print_strategy(os_strategy);
    dudo.print_information_sets(os_inf_sets);
    os_iterations << "Total number of iterations:" << endl;
    os_iterations << iterations << endl;
}

void cfr_domino(
    ostream& os_regret,
    ostream& os_strategy,
    ostream& os_inf_sets,
    ostream& os_iterations,
    int max_point,
    int initial_hand
    )
{
    using namespace domino;
    cout << "max_point: " << max_point << endl;
    cout << "initial_hand: " << initial_hand << endl;
    Domino domino(max_point, initial_hand);
    CFR<State, Action, Properties, InformationSet, Hash> cfr({&domino}, EPS);
    cout << "training..." << endl;
    long long iterations = cfr.train(total_seconds, os_regret);
    cout << "printing strategy" << endl;
    cfr.print_strategy(os_strategy);
    cout << "printing information sets" << endl;
    domino.print_information_sets(os_inf_sets);
    os_iterations << "Total number of iterations:" << endl;
    os_iterations << iterations << endl;
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
    string path_iterations = "results/" + folder + "/iterations.txt";
    ofstream os_regret(path_regret.c_str());
    ofstream os_strategy(path_strategy.c_str());
    ofstream os_inf_sets(path_inf_sets.c_str());
    ofstream os_iterations(path_iterations.c_str());

    if(game == "KuhnPoker"){
        cfr_kuhn(os_regret, os_strategy, os_inf_sets, os_iterations);
    } else if(game == "OCP"){
        if (argc < 3) {
            cout << "Debes introducir el numero de cartas" << endl;
            return 0;
        }
        int N = atoi(argv[2]);
        cfr_ocp(os_regret, os_strategy, os_inf_sets, os_iterations, N);
    } else if(game == "Dudo") {
        if (argc < 5) {
            cout << "Debes introducir los parametros K, D1 y D2" << endl;
            return 0;
        }
        int K, D1, D2;
        K = atoi(argv[2]);
        D1 = atoi(argv[3]);
        D2 = atoi(argv[4]);
        cfr_dudo(os_regret, os_strategy, os_inf_sets, os_iterations, K, D1, D2);
    } else if (game == "Domino") {
        if (argc < 4) {
            cout << "Debes introducir el numero de puntos y la cantidad de fichas" << endl;
            return 0;
        }
        int max_point, initial_hand;
        max_point = atoi(argv[2]);
        initial_hand = atoi(argv[3]);
        cfr_domino(os_regret, os_strategy, os_inf_sets, os_iterations, max_point, initial_hand);
    } else {
        cout << "Error" << endl;
        cout << "Los juegos validos son:" << endl;
        cout << "\tKuhnPoker" << endl;
        cout << "\tOCP" << endl;
        cout << "\tDudo" << endl;
        cout << "\tDomino" << endl;
    }
    os_regret.close();
    os_strategy.close();
    os_inf_sets.close();
    os_iterations.close();
}
