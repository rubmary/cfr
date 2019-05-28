#include <cmath>
#include <cstdlib>
#include <ctime>
#include <iostream>
#include "CFR.hpp"
using namespace std;

template <
    typename State,
    typename Action,
    typename Properties,
    typename InformationSet
>
void CFR<State, Action, Properties, InformationSet>::update_strategy(int I, int N, double pi)
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

template <
    typename State,
    typename Action,
    typename Properties,
    typename InformationSet
>
void CFR<State, Action, Properties, InformationSet>::initialize_game()
{
    game -> initial_state();
}

template <
    typename State,
    typename Action,
    typename Properties,
    typename InformationSet
>
double CFR<State, Action, Properties, InformationSet>::dfs(int i, double p1, double p2)
{
    
    if (game -> terminal_state()) {
        double sign = i == 1 ? 1 : -1;
        return sign*(game -> utility());
    }
    if (game->is_chance()){
        return 1;
    }
    int I = game -> information_set_id(), N = game -> actions();
    if(R.size() == I) {
        R.push_back(vector<double>(N, 0));
        strategy.push_back(vector<double>(N, 0));
        avg_s.push_back(vector<double>(N, 0));
        player.push_back(game -> player());
    }

    double node_util = 0;
    vector <double> util(N, 0);
    
    int current_player = game -> player();
    Action action = game -> first_action();
    for(int a = 0; a < N; a++) {
        if (a != 0)
            action = game -> next_action(action);
        game -> update_state(action);        
        if (current_player == 1)
            util[a] = dfs(i, strategy[I][a]*p1, p2);
        else
            util[a] = dfs(i, p1, strategy[I][a]*p2);
        node_util += strategy[I][a]*util[a];
        game -> revert_state();
    }
    if (current_player == i) {
        double pi_op = (i == 1) ? p2 : p1;
        double pi    = (i == 1) ? p1 : p2;
        for (int a = 0; a < N; a++)
            R[I][a] += pi_op*(util[a] - node_util);
        update_strategy(I, N, pi);
    }
    return node_util;
}

template <
    typename State,
    typename Action,
    typename Properties,
    typename InformationSet
>
void CFR<State, Action, Properties, InformationSet>::normalize_strategy()
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

template <
    typename State,
    typename Action,
    typename Properties,
    typename InformationSet
>
vector<vector<double>> CFR<State, Action, Properties, InformationSet>::train(int iterations)
{
    vector < vector <double> > Ri;
    for (int i = 0; i < iterations; i++) {
        for (int k = 1; k <=2; k++){
            initialize_game();
            dfs(k, 1, 1);

        }
        vector <double> r(2, 0);
        for (int I = 0; I < player.size(); I++) {
            double regret = 0;
            for (int a  = 0; a < (int) R[I].size(); a++)
                regret = max(regret, R[I][a]);
            r[player[I]-1] += regret/(i+1);
        }
        Ri.push_back(r);
        if (r[0] < EPS && r[1] < EPS && i > 100)
            break;
    }
    normalize_strategy();
    return Ri;
}

template <
    typename State,
    typename Action,
    typename Properties,
    typename InformationSet
>
vector <vector<double>> CFR<State, Action, Properties, InformationSet>::average_strategy()
{
    return avg_s;
}

template <
    typename State,
    typename Action,
    typename Properties,
    typename InformationSet
>
vector <vector<double>> CFR<State, Action, Properties, InformationSet>::regret()
{
    return R;
}