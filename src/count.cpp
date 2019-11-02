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

void dfs_kuhn() {
	using namespace kuhn_poker;
	KuhnPoker kuhn;
	int inf_sets = kuhn.discover_information_sets();
	int nodes = kuhn.total_nodes();
	cout << inf_sets << ' ' << nodes << endl;
}

void dfs_ocp() {
    using namespace ocp;
    int N = 6;
    vector<int>cards_num({3, 12, 50, 200, 1000, 4000});
    for (int i = 0; i < N; i++) {
        OCP ocp(cards_num[i]);
        int inf_sets = ocp.discover_information_sets();
        int nodes = ocp.total_nodes();
        cout << "Numero de cartas: " << cards_num[i] << endl;
        cout << inf_sets << ' ' << nodes << endl;
    }
}

void dfs_dudo() {
    using namespace dudo;
    int N = 4;
    // vector <int> K({4, 5, 6, 3, 3, 3, 4, 4, 4, 5, 5, 5});
    // vector <int>D1({1, 1, 1, 2, 1, 2, 2, 1, 2, 2, 1, 2});
    // vector <int>D2({1, 1, 1, 1, 2, 2, 1, 2, 2, 1, 2, 2});
    vector <int> K({3, 6, 6, 6});
    vector <int>D1({1, 2, 1, 2});
    vector <int>D2({1, 1, 2, 2});
    vector <vector <double>> d(6, vector<double> (6));
    for (int i = 0; i < N; i++) {
        Dudo dudo(K[i], D1[i], D2[i], d);
        int inf_sets = dudo.discover_information_sets();
        long long nodes = dudo.total_nodes();
        cout << "Parametros: " << K[i] << ' ' << D1[i] << ' ' << D2[i] << endl;
        cout << inf_sets << ' ' << nodes << endl;
    }
}

void dfs_domino() {
	using namespace domino;
	int N = 5;
	vector<int> max_points({1, 2, 3, 3, 3});
	vector <int> hand({1, 2, 2, 3, 4});
	for (int i = 0; i < N; i++) {
		int M = max_points[i], H = hand[i];
		Domino domino(M, H);
		int inf_sets = domino.discover_information_sets();
		int nodes = domino.total_nodes();
		cout << M << ' ' << H << endl;
		cout << inf_sets << ' ' << nodes << endl;
	}
}

int main(int argc, char** argv) {
    if(argc < 2) {
        cout << "Debes introducir el nombre del juego";
        cout << endl;
        return 0;
    }
    string game = argv[1];
    srand(time(NULL));
    if(game == "KuhnPoker"){
        dfs_kuhn();
    } else if(game == "OCP"){
        dfs_ocp();
    } else if(game == "Dudo") {
        dfs_dudo();
    } else if (game == "Domino") {
        dfs_domino();
    } else {
        cout << "Error" << endl;
        cout << "Los juegos validos son:" << endl;
        cout << "\tKuhnPoker" << endl;
        cout << "\tOCP" << endl;
        cout << "\tDudo" << endl;
        cout << "\tDomino" << endl;
    }
}
