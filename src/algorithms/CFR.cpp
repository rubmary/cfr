#include <cmath>
#include <cstdlib>
#include <ctime>
#include <iostream>
#include "CFR.hpp"
using namespace std;

template <typename State, typename Action, typename Properties, typename InformationSet, typename Hash>
CFR<State, Action, Properties, InformationSet, Hash>::CFR(
    Game<State, Action, Properties, InformationSet, Hash> *game,
    double EPS)
    : game(game), EPS(EPS)
{
    int information_sets = game -> discover_information_sets();
    R = vector<vector<double>>(information_sets);
    strategy = vector<vector<double>>(information_sets);
    avg_s = vector<vector<double>>(information_sets);
    player = vector<int>(information_sets);
    game -> first_state();
    do{
        dfs_initialization();
    }while(game -> next_state());
}


template <typename State, typename Action, typename Properties, typename InformationSet, typename Hash>
void CFR<State, Action, Properties, InformationSet, Hash>::dfs_initialization() {
    if (game -> terminal_state())
        return;
    vector<Action> actions = game -> actions();
    int N = actions.size();
    int I = game -> information_set_id();
    R[I] = vector<double>(N, 0);
    strategy[I] = vector<double>(N, 0);
    avg_s[I] = vector<double>(N, 0);
    player[I] = game -> player();
    for (auto action : actions) {
        game -> update_state(action);
        dfs_initialization();
        game -> revert_state();
    }
}

template <typename State, typename Action, typename Properties, typename InformationSet, typename Hash>
void CFR<State, Action, Properties, InformationSet, Hash>::update_strategy(int I, int N, double pi)
{
    double sum_R = 0;
    for (int a = 0; a < N; a++){
        strategy[I][a] = (R[I][a] > 0 ? R[I][a] : 0);
        sum_R += strategy[I][a];
    }
    for (int a = 0; a < N; a++) {
        if (sum_R == 0)
            strategy[I][a] = 1.00/N;
        else
            strategy[I][a] /= sum_R;
    }
    for (int a = 0; a < N; a++)
        avg_s[I][a] += pi*strategy[I][a];
}

template <typename State, typename Action, typename Properties, typename InformationSet, typename Hash>
void CFR<State, Action, Properties, InformationSet, Hash>::initialize_game()
{
    game -> initial_state();
}

template <typename State, typename Action, typename Properties, typename InformationSet, typename Hash>
double CFR<State, Action, Properties, InformationSet, Hash>::dfs(int i, double p1, double p2)
{
    if (game -> terminal_state()) {
        return game -> utility(i);
    }
    int I = game -> information_set_id();
    vector <Action> actions = game -> actions();
    int N = actions.size();
    double node_util = 0;
    vector <double> util(N, 0);
    int current_player = game -> player();
    for(int k = 0; k < N; k++) {
        Action action = actions[k];
        game -> update_state(action);        
        if (current_player == 1)
            util[k] = dfs(i, strategy[I][k]*p1, p2);
        else
            util[k] = dfs(i, p1, strategy[I][k]*p2);
        node_util += strategy[I][k]*util[k];
        game -> revert_state();
    }
    if (current_player == i) {
        double pi_op = (i == 1) ? p2 : p1;
        double pi    = (i == 1) ? p1 : p2;
        for (int k = 0; k < N; k++)
            R[I][k] += pi_op*(util[k] - node_util);
        update_strategy(I, N, pi);
    }
    return node_util;
}

template <typename State, typename Action, typename Properties, typename InformationSet, typename Hash>
void CFR<State, Action, Properties, InformationSet, Hash>::normalize_strategy()
{
    int M = avg_s.size();
    for (int I = 0; I < M; I++) {
        int N = avg_s[I].size();
        double sum_strategy = 0;
        for (int a = 0; a < N; a++)
            sum_strategy += avg_s[I][a];
        for (int a = 0; a < N; a++)
            avg_s[I][a] /= sum_strategy;
    }
}

template <typename State, typename Action, typename Properties, typename InformationSet, typename Hash>
vector<vector<double>> CFR<State, Action, Properties, InformationSet, Hash>::train(int iterations)
{
    vector<vector<double>> Ri;
    vector <vector<int>> information_sets(2);
    for (int k = 0; k < 2; k++)
        information_sets[k] = game -> player_information_sets(k+1);

    for (int i = 0; i < iterations; i++) {
        for (int k = 1; k <=2; k++){
            initialize_game();
            dfs(k, 1, 1);
        }
        vector <double> r(2, 0);
        for (int k = 0; k < 2; k++) {
            for (auto I : information_sets[k]) {
                double regret = 0;
                for (int a = 0; a < (int) R[I].size(); a++)
                    regret = max(regret, R[I][a]);
                r[k] += regret/(i+1);
            }
        }
        Ri.push_back(r);
        if (r[0] < EPS && r[1] < EPS && i > 100)
            break;
    }
    normalize_strategy();
    return Ri;
}

template <typename State, typename Action, typename Properties, typename InformationSet, typename Hash>
vector <vector<double>> CFR<State, Action, Properties, InformationSet, Hash>::average_strategy()
{
    return avg_s;
}

template <typename State, typename Action, typename Properties, typename InformationSet, typename Hash>
vector <vector<double>> CFR<State, Action, Properties, InformationSet, Hash>::regret()
{
    return R;
}