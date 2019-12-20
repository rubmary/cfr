#include <cstdlib>
#include <ctime>
#include <fstream>
#include <iostream>
#include "algorithms/Simulator.cpp"
#include "games/Domino.hpp"
using namespace std;
int total_seconds = 36000;
#define EPS 1e-5

void simulator_domino(
    ostream& os_strategy,
    ostream& os_inf_sets,
    int max_point,
    int initial_hand,
    int n
) {
    using namespace domino;
    cout << "(max_point, initial_hand) = (" << max_point << "," << initial_hand << ")";
    cout << endl;
    Domino domino(max_point, initial_hand);
    cout << "En construccion" << endl;
    // CFR<State, Action, Properties, InformationSet, Hash> cfr({&domino}, EPS);
}

int main(int argc, char **argv) {
    srand(time(NULL));
    if (argc < 2) {
        cout << "Debes introducir el numero de repeticiones";
        cout << endl;
        return 0;
    }

    int n = atoi(argv[1]);
    string game = "Domino";
    string folder = game;
    for (int i = 2; i < argc; i++) {
        string separator = i == 2 ? "/" : "_";
        folder = folder + separator + argv[i];
    }
    string path_strategy = "results/" + folder + "/strategy.txt";
    string path_inf_sets = "results/" + folder + "/information_sets.txt";
    ofstream os_strategy(path_strategy.c_str());
    ofstream os_inf_sets(path_inf_sets.c_str());

    if (argc < 4) {
        cout << "Debes introducir el numero de puntos y la cantidad de fichas" << endl;
        return 0;
    }
    int max_point, initial_hand;
    max_point = atoi(argv[2]);
    initial_hand = atoi(argv[3]);
    simulator_domino(os_strategy, os_inf_sets, max_point, initial_hand, n);
    os_strategy.close();
    os_inf_sets.close();
}