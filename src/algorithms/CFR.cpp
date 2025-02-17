#include <cmath>
#include <cstdlib>
#include <ctime>
#include <iostream>
#include <fstream>
#include <iomanip>
#include <chrono>
#include "CFR.hpp"
using namespace std;
using namespace std::chrono;

template <typename State, typename Action, typename Properties, typename InformationSet, typename Hash>
CFR<State, Action, Properties, InformationSet, Hash>::CFR(
    Game<State, Action, Properties, InformationSet, Hash> *game,
    double EPS)
    : EPS(EPS), game(game)
{
    int information_sets = game -> discover_information_sets();
    R = vector<vector<double>>(information_sets);
    strategy = vector<vector<double>>(information_sets);
    avg_s = vector<vector<double>>(information_sets);
    player = vector<int>(information_sets);
    game -> first_state();
    do{
        dfs_initialization();
    } while(game -> next_state());
}


template <typename State, typename Action, typename Properties, typename InformationSet, typename Hash>
void CFR<State, Action, Properties, InformationSet, Hash>::dfs_initialization() {
    if (game -> terminal_state())
        return;
    vector<Action> actions = game -> actions();
    int N = actions.size();
    if (N > 1) {
        int I = game -> information_set_id();
        R[I] = vector<double>(N, 0);
        strategy[I] = vector<double>(N, 0);
        avg_s[I] = vector<double>(N, 0);
        player[I] = game -> player();
    }
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
        if (abs(sum_R) <= EPS)
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

    vector <Action> actions = game -> actions();
    int N = actions.size();
    int I = N > 1 ? game -> information_set_id() : -1;
    double node_util = 0;
    vector <double> util(N, 0);
    int current_player = game -> player();
    for(int k = 0; k < N; k++) {
        double action_strategy = N > 1 ? strategy[I][k] : 1;
        game -> update_state(actions[k]);
        if (current_player == 1)
            util[k] = dfs(i, action_strategy*p1, p2);
        else
            util[k] = dfs(i, p1, action_strategy*p2);
        node_util += action_strategy*util[k];
        game -> revert_state();
    }
    if (current_player == i && N > 1) {
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
        for (int a = 0; a < N; a++) {
            if (abs(sum_strategy) <= EPS)
                avg_s[I][a] = 1.0/N;
            else
                avg_s[I][a] /= sum_strategy;
        }
    }
}

template <typename State, typename Action, typename Properties, typename InformationSet, typename Hash>
long long CFR<State, Action, Properties, InformationSet, Hash>::train(long long sec, ostream& os)
{
    vector <vector<int>> information_sets(2);
    auto total_seconds = (duration<long int, ratio<1, 1000000>>) (long long) sec*1000000;
    auto accumulated_time = (duration<long int, ratio<1, 1000000>>)0;
    long long i, next = 0, pot10 = 1;
    for (i = 0; ; i++) {
        auto start = high_resolution_clock::now();
        for (int k = 1; k <= 2; k++){
            initialize_game();
            dfs(k, 1, 1);
        }
        auto stop = high_resolution_clock::now();
        accumulated_time = accumulated_time + duration_cast<microseconds>(stop-start);

        if(i == next) {
            if (next < 1000){
                next++;
            } else {
                if (next == pot10*1000)
                    pot10*=100;
                next += pot10;
            }
            vector <double> r(2, 0);
            os << i << ' ';
            for (int k = 0; k < 2; k++) {
                for (auto I : game->player_information_sets(k+1)) {
                    double regret = 0;
                    for (int a = 0; a < (int) R[I].size(); a++)
                        regret = max(regret, R[I][a]);
                    r[k] += regret;
                }
                r[k] /= i+1;
                os << r[k] << ' ';
            }
            os << endl;
        }
        if (accumulated_time >= total_seconds)
            break;
    }
    normalize_strategy();
    return i;
}

template <typename State, typename Action, typename Properties, typename InformationSet, typename Hash>
vector <vector<double>> CFR<State, Action, Properties, InformationSet, Hash>::average_strategy()
{
    return avg_s;
}

template <typename State, typename Action, typename Properties, typename InformationSet, typename Hash>
void CFR<State, Action, Properties, InformationSet, Hash>::print_strategy(ostream& os)
{
    int M = avg_s.size();
    os << M << endl;
    for (int i = 0; i < M; i++) {
        int N = avg_s[i].size();
        os << N << ' ';
        for (int j = 0; j < N; j++)
            os << fixed << setprecision(5) << avg_s[i][j] << ' ';
        os << endl;
    }
}

template <typename State, typename Action, typename Properties, typename InformationSet, typename Hash>
vector <vector<double>> CFR<State, Action, Properties, InformationSet, Hash>::regret()
{
    return R;
}