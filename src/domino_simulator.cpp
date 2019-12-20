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
    ifstream &is_inf_sets,
    ifstream &is_strategy,
    int max_point,
    int initial_hand,
    int n
) {
    using namespace domino;
    cout << "(max_point, initial_hand) = (" << max_point << "," << initial_hand << ")";
    cout << endl;
    Domino domino(max_point, initial_hand);
    cout << "En construccion" << endl;
    Simulator<State, Action, Properties, InformationSet, Hash>
        simulator(&domino, is_inf_sets, is_strategy);
}

int main(int argc, char **argv) {
    srand(time(NULL));
    if (argc < 2) {
        cout << "Debes introducir el numero de repeticiones";
        cout << endl;
        return 0;
    }

    if (argc < 4) {
        cout << "Debes introducir el numero de puntos y la cantidad de fichas" << endl;
        return 0;
    }

    int n = atoi(argv[1]);
    string game = "Domino";
    string folder = game;
    for (int i = 2; i < argc; i++) {
        string separator = i == 2 ? "/" : "_";
        folder = folder + separator + argv[i];
    }
    string path_inf_sets = "results/" + folder + "/information_sets.txt";
    string path_strategy = "results/" + folder + "/strategy.txt";
    ifstream is_inf_sets(path_inf_sets.c_str());
    ifstream is_strategy(path_strategy.c_str());

    int max_point, initial_hand;
    max_point = atoi(argv[2]);
    initial_hand = atoi(argv[3]);
    simulator_domino(is_inf_sets, is_strategy, max_point, initial_hand, n);
    is_inf_sets.close();
    is_strategy.close();
}