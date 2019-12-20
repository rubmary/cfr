// #include <cmath>
// #include <cstdlib>
// #include <ctime>
#include <iostream>
// #include <algorithm>
#include "Simulator.hpp"
using namespace std;

template <typename State, typename Action, typename Properties, typename InformationSet, typename Hash>
Simulator<State, Action, Properties, InformationSet, Hash>::Simulator(
    Game<State, Action, Properties, InformationSet, Hash> *game,
    istream& is_inf_sets,
    istream& is_strategy
) : game(game)
{
    game -> discover_information_sets(is_inf_sets);
    read_sigma(is_strategy);
}

template <typename State, typename Action, typename Properties, typename InformationSet, typename Hash>
void Simulator<State, Action, Properties, InformationSet, Hash>::read_sigma(istream& is) {
    int information_sets, actions;
    is >> information_sets;
    sigma.resize(information_sets);
    for (int i = 0; i < information_sets; i++) {
        is >> actions;
        sigma[i].resize(actions);
        for(int j = 0; j < actions; j++) {
            is >> sigma[i][j];
        }
    }
}

template <typename State, typename Action, typename Properties, typename InformationSet, typename Hash>
double Simulator<State, Action, Properties, InformationSet, Hash>::move_recursively() {
    if(game -> terminal_state()) {
        return game -> utility(1);
    }
    vector<Action> actions = game -> actions();
    int action_id = 0; //seleccionar accion
    auto action = actions[action_id];
    game -> update_state(action);
    return move_recursively();
}

template <typename State, typename Action, typename Properties, typename InformationSet, typename Hash>
double Simulator<State, Action, Properties, InformationSet, Hash>::play() {
    game -> initial_state();
    return move_recursively();
}


template <typename State, typename Action, typename Properties, typename InformationSet, typename Hash>
double Simulator<State, Action, Properties, InformationSet, Hash>::plays(int n) {
    double average_utility = 0;
    for (int i = 0; i < n; i++) {
        average_utility += play();
    }
    return average_utility/n;
}
