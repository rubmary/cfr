/**
* Implementacion de la clase Dudo
*/
#include "Dudo.hpp"
#include <cstdlib>
#include <ctime>
using namespace dudo;

Dudo::Dudo(int K, int D1, int D2, vector<vector<double>> &dudos) {
    properties.K = K;
    properties.D1 = D1;
    properties.D2 = D2;
    properties.dudos = dudos;
}

int Dudo::player() {
    return state.player;
}

void Dudo::change_player() {
    state.player = (state.player&1) + 1;
}

int Dudo::information_set_id() {
    InformationSet inf_set = information_set();
    if(I.find(inf_set) == I.end())
        I[inf_set] = information_sets++;
    return I[inf_set];
}

void Dudo::initial_state() {
    state.player = 1;
    state.history.clear();
    state.bidding_sequence = "";
    state.calls_bluff = false;
    state.dice.resize(2);
    vector<int>lengths({properties.D1, properties.D2});
    for (int i = 0; i < 2; i++) {
        state.dice[i].resize(properties.K);
        for(int j = 0; j < lengths[i]; j++)
            state.dice[i][rand() % properties.K]++;
    }
}

InformationSet Dudo::information_set() {
    Dice dice = state.dice[player() - 1];
    unsigned long long mask = 0;
    for(int i = 0; i < (int) state.bidding_sequence.size(); i++) {
        if(state.bidding_sequence[i] == '1'){
            mask |= (1ULL << i);
        }
    }
    return InformationSet({dice, mask});
}

Action Dudo::first_action() {
    if(state.history.empty()){
        return Action({1, 1});
    }
    const Action &last = state.history[state.history.size() - 1];
    return next_action(last);
}

Action Dudo::next_action(const Action &action) {
    int total_dice = properties.D1 + properties.D2;
    int face = (action.face+1)%properties.K;
    int quantity = action.quantity + (face==1);
    if(quantity > total_dice) {
        quantity = 0;
    }
    return Action({quantity, face});
}

bool Dudo::last_action(const Action &action) {
    return action.quantity == 0;
}

int Dudo::actions() {
    int total = properties.K*(properties.D1 + properties.D2);
    int previous = state.bidding_sequence.size();
    return total - previous;
}

void Dudo::update_state(const Action &action) {
    state.history.push_back(action);
    change_player();
    if(action.quantity == 0){
        state.calls_bluff = true;
    }else{
        int action_number = (action.quantity-1)*properties.K;
        action_number += (action.face == 0 ? properties.K : action.face);
        state.bidding_sequence.resize(action_number, '0');
        state.bidding_sequence[action_number-1] = '1';
    }
}

void Dudo::revert_state() {
    if(state.calls_bluff) {
        state.calls_bluff = false;
    }else {
        int last = state.bidding_sequence.size() - 2;
        while(last > 0 && state.bidding_sequence[last] == '0')
            last--;
        state.bidding_sequence.resize(last+1);
    }
    change_player();
    state.history.pop_back();
}

bool Dudo::terminal_state() {
    return state.calls_bluff;
}

bool Dudo::is_chance() {
    return false;
}

vector<double> Dudo::distribution() {
    return vector<double>();
}

double Dudo::utility() {
    Action &bid = state.history[state.history.size() - 2];
    int total = 0, D1 = properties.D1, D2 = properties.D2;
    total = state.dice[0][bid.face] + state.dice[1][bid.face];
    if(bid.face != 0)
        total += state.dice[0][0] + state.dice[1][0];
    int winner, dice = bid.quantity - total;
    if (dice >= 0)
        winner = player();
    else
        winner = (player()&1) + 1;
    dice = max(abs(dice), 1);

    double sign = 1;
    if(winner == 1) {
        D2 = max(0, D2 - dice);
        swap(D1, D2);
        sign = -1;
    }
    else {
        D1 = max(0, D1 - dice);
    }

    return sign*properties.dudos[D1][D2];
}

void Dudo::print() {

}