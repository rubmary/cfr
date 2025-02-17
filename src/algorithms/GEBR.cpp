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
    depths.resize(2);
}

template <typename State, typename Action, typename Properties, typename InformationSet, typename Hash>
void GEBR<State, Action, Properties, InformationSet, Hash>::discover_tree(int d) {
    if(game -> terminal_state()) {
        return;
    }
    vector<Action> actions = game -> actions();
    int total_actions = actions.size();
    if (total_actions > 1) {
        int I = game -> information_set_id();
        if(t[I].empty()){
            t[I].resize(total_actions, 0);
            b[I].resize(total_actions, 0);
        }
    }
    depths[game -> player()-1].insert(d);
    for (auto action : actions) {
        game -> update_state(action);
        discover_tree(d+1);
        game -> revert_state();
    }
}

template <typename State, typename Action, typename Properties, typename InformationSet, typename Hash>
void GEBR<State, Action, Properties, InformationSet, Hash>::pass1() {
    game -> first_state();
    do{
        discover_tree(0);
    } while(game -> next_state());
}

template <typename State, typename Action, typename Properties, typename InformationSet, typename Hash>
double GEBR<State, Action, Properties, InformationSet, Hash>::pass2(int i, int d, int l, double pi) {
    if(game -> terminal_state()) {
        return game -> utility(i);
    }
    vector <Action> actions = game -> actions();
    int N = actions.size();
    int I = N > 1 ? game -> information_set_id() : 1;
    double v = 0;
    if (game -> player() == i && l > d) {
        Action action = actions[0];
        if (N > 1) {
            double max_value = t[I][0]/b[I][0];
            for (int k = 0; k < N; k++) {
                if (t[I][k]/b[I][k] > max_value) {
                    action = actions[k];
                    max_value = t[I][k]/b[I][k];
                }
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
        double sigma_inf_set = N > 1 ? sigma[I][a] : 1;
        if (game -> player() != i){
            pi2 *= sigma_inf_set;
        }
        game -> update_state(action);
        double v2 = pass2(i, d, l+1, pi2); 
        game -> revert_state();
        if (game -> player() != i) {
            v += sigma_inf_set*v2;
        } else if(l == d && N > 1) {
            t[I][a] += v2*pi;
            b[I][a] += pi;
        }
        a++;
    }
    return v;
}

template <typename State, typename Action, typename Properties, typename InformationSet, typename Hash>
double GEBR<State, Action, Properties, InformationSet, Hash>::best_response(int i) {
    double v = 0, w;
    double k = 0;
    depths[i-1].insert(-1);
    for (auto it = depths[i-1].rbegin(); it != depths[i-1].rend(); ++it) {
        v = 0;
        k = 0;
        game -> first_state();
        do {
            w = game -> state_weight();
            v += pass2(i, *it, 0, w)*w;
            k += w;
        } while(game -> next_state());
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
double GEBR<State, Action, Properties, InformationSet, Hash>::explotability(istream& is, ostream& os) {
    read_sigma(is);
    double u = game -> expected_value(1, sigma);
    pass1();
    double u1 = best_response(1);
    double u2 = best_response(2);
    os << "Game value: " << u << endl;
    os << "Best response for player 1: " << u1 << endl;
    os << "Best response for player 2: " << u2 << endl;
    os << "Explotability: " << u1 + u2 << endl;
    return  u1 + u2;
}