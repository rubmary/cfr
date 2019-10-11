/**
* Implementacion de la clase Dudo
*/
#include "Dudo.hpp"
#include <cstdlib>
#include <ctime>
using namespace dudo;

Dudo::Dudo(int K, int D1, int D2, vector<vector<double>> &dudos) {
    properties = Properties({K, D1, D2, dudos});
}

int Dudo::player() {
    return state.player;
}

void Dudo::change_player() {
    state.player = (state.player&1) + 1;
}

void Dudo::initial_state() {
    state.player = 1;
    state.history.clear();
    state.bidding_sequence = "";
    state.calls_bluff = false;
    state.dice.resize(2);
    vector<int>dice_number({properties.D1, properties.D2});
    for (int i = 0; i < 2; i++) {
        state.dice[i].resize(properties.K);
        fill(state.dice[i].begin(), state.dice[i].end(), 0);
        for(int j = 0; j < dice_number[i]; j++)
            state.dice[i][rand() % properties.K]++;
    }
}

void Dudo::first_state() {
    state.player = 1;
    state.history.clear();
    state.bidding_sequence = "";
    state.calls_bluff = false;
    state.dice.resize(2);
    vector<int>dice_number({properties.D1, properties.D2});
    for (int i = 0; i < 2; i++) {
        state.dice[i] = vector<int>(properties.K, 0);
        state.dice[i][properties.K-1] = dice_number[i];
    }
}

bool Dudo::next_sequence(vector <int> &P, int N) {
    for (int i = N-1; i > 0; i--) {
        if (P[i] > 0) {
            P[i-1]++;
            P[N-1] = P[i] - 1;
            if (i < N-1) {
                P[i] = 0;
            }
            return true;
        }
    }
    return false;
}

bool Dudo::next_state() {
    int K = properties.K, D2 = properties.D2;
    if(next_sequence(state.dice[1], K)) {
        return true;
    }
    state.dice[1][0] = 0;
    state.dice[1][K-1] = D2;
    if(next_sequence(state.dice[0], K)) {
        return true;
    }
    return false;
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

vector<Action> Dudo::actions() {
    int quantity = 0, face = 0, first_turn = true;
    vector<Action> A;
    if(!state.history.empty()) {
        first_turn = false;
        Action &last = state.history[state.history.size()-1];
        quantity = last.quantity;
        face = last.face;
    }
    int total_dice = properties.D1 + properties.D2;
    while(true) {
        if (face == 0) {
            quantity++;
        }
        face = (face+1)%properties.K;
        if (quantity > total_dice)
            break;
        A.push_back(Action({quantity, face}));
    }
    if (!first_turn){
        A.push_back(Action({0, 0}));
    }
    return A;
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
        while(last >= 0 && state.bidding_sequence[last] == '0')
            last--;
        state.bidding_sequence.resize(last+1);
    }
    change_player();
    state.history.pop_back();
}

bool Dudo::terminal_state() {
    return state.calls_bluff;
}

double Dudo::utility(int i) {
    Action &bid = state.history[state.history.size() - 2];
    int total_dice, D1 = properties.D1, D2 = properties.D2;
    total_dice = state.dice[0][bid.face] + state.dice[1][bid.face];
    int winner = (total_dice - bid.quantity < 0) ? (player()&1) + 1 : player();
    if(winner == 1) {
        --D2;
        swap(D1, D2);
    } else {
        --D1;
    }
    // cout << "Utility: " << D1 << ' ' << D2 << ' ' << properties.dudos[D1][D2] << endl;
    // cout << (winner == i ? -1 : 1)*properties.dudos[D1][D2] << endl;
    return (winner == i ? -1 : 1)*properties.dudos[D1][D2];
}

double Dudo::factorial(int n) {
    double ans = 1;
    for (int i = 1; i <= n; i++)
        ans *= i;
    return ans;
}

double Dudo::state_weight() {
    double w1 = factorial(properties.D1);
    double w2 = factorial(properties.D2);
    for (int i = 0; i < properties.K; i++) {
        w1 /= factorial(state.dice[0][i]);
        w2 /= factorial(state.dice[1][i]);
    }
    return w1*w2;
}

void Dudo::print() {
    int K = properties.K;

    for (int i = 0; i < 2; i++) {
        cout << "Dados jugador " << i+1 << ": ";
        for (int j = 0; j < properties.K; j++)
            cout << state.dice[i][j] << ' ';
        cout << endl;
    }
    cout << "Secuencia:" << endl;
    cout << "\tString:    " << state.bidding_sequence << endl;
    cout << "\tLong long: " << information_set().bidding_sequence << endl;
    cout << "\tHistoria:  ";
    for (int i = 0; i < (int) state.history.size() - 1; i++) {
        int face = state.history[i].face;
        cout << "(" << state.history[i].quantity << ',' << (face == 0 ? K : face) << ")" << ' ';
    }

    if (terminal_state()) {
        cout << "(dudo)" << endl;
        double u = utility(1);
        cout << "player = " << player() << ", winner = " << (u > 0 ? 1 : 2) << endl;
        int v = (int) (abs(u) + 0.1);
        int i = v/3, j = v%3;
        cout << "Nuevos dados: " << i << ' ' << j << endl;
    } else {
        int i = (int) state.history.size() - 1;
        if (i >= 0){
            int face = state.history[i].face;
            cout << "(" << state.history[i].quantity << ',' << (face == 0 ? K : face) << ")";
        }
        cout << endl;
    }
    cout << endl;
}

ostream& operator<<(ostream& os, const InformationSet& I) {
    os << I.dice.size() << ' ';
    for(auto die : I.dice) {
        os << die << ' ';
    }
    os << I.bidding_sequence;
    return os;
}

istream& operator>>(istream& is, InformationSet& I) {
    int size;
    is >> size;
    I.dice.resize(size);
    for (int i = 0; i < size; i++) {
        is >> I.dice[i];
    }
    is >> I.bidding_sequence;
    return is;
}
