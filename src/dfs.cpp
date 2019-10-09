#include <cstdlib>
#include <ctime>
#include <fstream>
#include "games/KuhnPoker.hpp"
#include "games/Dudo.hpp"
#include "games/Domino.hpp"
#include "games/OCP.hpp"
#include "algorithms/DFS.cpp"
using namespace std;

void dfs_kuhn() {
    ofstream os("information_sets.txt");
    using namespace kuhn_poker;
    KuhnPoker kuhn_poker;
    DFS<State, Action, Properties, InformationSet, Hash> dfs({&kuhn_poker});
    dfs.start_dfs();
    kuhn_poker.print_information_sets(os);
    os.close();
    ifstream is("information_sets.txt");
    KuhnPoker kuhn_poker2;
    kuhn_poker2.discover_information_sets(is);
    ofstream os2("information_sets2.txt");
    kuhn_poker2.print_information_sets(os2);
}

void dfs_ocp() {
    ofstream os("information_sets.txt");
    using namespace ocp;
    OCP ocp(3);
    DFS<State, Action, Properties, InformationSet, Hash> dfs({&ocp});
    dfs.start_dfs();
    ocp.print_information_sets(os);
    os.close();
    ifstream is("information_sets.txt");
    OCP ocp2(3);
    ocp2.discover_information_sets(is);
    ofstream os2("information_sets2.txt");
    ocp2.print_information_sets(os2);

}

void dfs_dudo() {
    using namespace dudo;
    vector<vector<double>>dudos(3, vector<double>(3, 0));
    for(int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            dudos[i][j] = 3*i + j;
        }
    }
    Dudo dudo(2, 1, 1, dudos);
    DFS<State, Action, Properties, InformationSet, Hash> dfs({&dudo});
    dfs.start_dfs();
    ofstream os("information_sets.txt");
    dudo.print_information_sets(os);
    os.close();
    ifstream is("information_sets.txt");
    Dudo dudo2(2, 1, 1, dudos);
    dudo2.discover_information_sets(is);
    ofstream os2("information_sets2.txt");
    dudo2.print_information_sets(os2);
}

void dfs_domino() {
    using namespace domino;
    Domino domino(3, 2);
    DFS<State, Action, Properties, InformationSet, Hash> dfs({&domino});
    dfs.start_dfs();
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