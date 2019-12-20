#include <cmath>
#include <cstdlib>
#include <ctime>
#include <iostream>
#include <algorithm>
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
int Simulator<State, Action, Properties, InformationSet, Hash>::select_action(vector<double>&strategy) {
    double x = ((double) rand() / (RAND_MAX));
    int N = strategy.size();
    double cumulative = 0;
    for (int i = 0; i < N; i++) {
        if (x < cumulative + strategy[i]) {
            return i;
        }
        cumulative += strategy[i];
    }
    return N-1;
}

template <typename State, typename Action, typename Properties, typename InformationSet, typename Hash>
double Simulator<State, Action, Properties, InformationSet, Hash>::move_recursively() {
    if(game -> terminal_state()) {
        return game -> utility(1);
    }
    int information_set_id, action_id, N;
    vector<Action> actions = game -> actions();
    N = actions.size();
    if (N == 1) {
        action_id = 0;
    } else {
        information_set_id = game -> information_set_id();
        vector<double>&strategy = sigma[information_set_id];
        action_id = select_action(strategy);
    }
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
