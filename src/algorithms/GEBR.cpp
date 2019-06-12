#include <cmath>
#include <cstdlib>
#include <ctime>
#include <iostream>
#include <algorithm>
#include "GEBR.hpp"
using namespace std;

template <typename State, typename Action, typename Properties, typename InformationSet, typename Hash>
GEBR<State, Action, Properties, InformationSet, Hash>::GEBR(Game<State, Action, Properties, InformationSet, Hash> *game, istream& is)
 : game(game)
{
    int information_sets = game -> discover_information_sets(is);
    t.resize(information_sets, vector<double>(0));
    b.resize(information_sets, vector<double>(0));
}

template <typename State, typename Action, typename Properties, typename InformationSet, typename Hash>
void GEBR<State, Action, Properties, InformationSet, Hash>::discover_tree(int d) {
    if(game -> terminal_state()) {
        return;
    }
    int I = game -> information_set_id();
    vector<Action> actions = game -> actions();
    int total_actions = actions.size();
    t[I].resize(total_actions, 0);
    b[I].resize(total_actions, 0);
    depths[game -> player() - 1].push_back(d);
    for (auto action : actions) {
        game -> update_state(action);
        discover_tree(d+1);
        game -> revert_state();
    }
}

template <typename State, typename Action, typename Properties, typename InformationSet, typename Hash>
void GEBR<State, Action, Properties, InformationSet, Hash>::pass1() {
    game -> initial_state();
    while(game -> valid_state()) {
        discover_tree(0);
        game -> next_state();
    }
    for (int i = 0; i < 2; i++){
        sort(depths[i].begin(), depths[i].end());
    }
}

template <typename State, typename Action, typename Properties, typename InformationSet, typename Hash>
double GEBR<State, Action, Properties, InformationSet, Hash>::pass2(int i, int d, int l, double pi) {
    if(game -> terminal_state()) {
        return game -> utility(i);
    }
    int I = game -> information_set_id();
    double v = 0;
    vector <Action> actions = game -> actions();
    if (game -> player() == i && l > d) {
        Action action = actions[0];
        double max_value = t[I][0]/b[I][0];
        for (int k = 0; k < (int) actions.size(); k++) {
            if (t[I][k]/b[I][k] > max_value) {
                action = actions[k];
                max_value = t[I][k]/b[I][k];
            }
        }
        game -> update_state(action);
        v = pass2(i, d, l+1, pi);
        game -> revert_state();
        return v;
    }
    int a = 0;
    for (auto action : actions) {
        double pi2 = pi;
        if (game -> player != i){
            pi2 *= sigma[I][a];
        }
        game -> update_state(action);
        double v2 = pass2(i, d, l+1, pi2); 
        game -> revert_state();
        if (game -> player() != i) {
            v += sigma[I][a]*v2;
        } else if(l == d) {
            t[I][a] += v2*pi;
            b[I][a] += pi;
        }
        a++;
    }
    return v;
}

template <typename State, typename Action, typename Properties, typename InformationSet, typename Hash>
double GEBR<State, Action, Properties, InformationSet, Hash>::best_response(int i) {
    double v;
    int k = 0;
    for (int j = depths[i].size() - 1; j >= 0; j--) {
        v = 0;
        k = 0;
        game -> first_state();
        while(game -> valid_state()) {
            v += pass2(i, depths[j], 0, 1);
            game -> next_state();
            k++;
        }
    }
    return v/k;
}

template <typename State, typename Action, typename Properties, typename InformationSet, typename Hash>
void GEBR<State, Action, Properties, InformationSet, Hash>::read_sigma(istream& is) {
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
double GEBR<State, Action, Properties, InformationSet, Hash>::explotability(istream& is) {
    read_sigma(is);
    return best_response(0) + best_response(1);
}