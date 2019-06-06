/**
    Implementacion de la clase OCP
*/
#include "OCP.hpp"
#include <cstdlib>
#include <ctime>
using namespace std;
using namespace ocp;

OCP::OCP(int cards) {
    properties.N = cards;
    srand(time(NULL));
}

int OCP::player() {
    return state.player;
}

void OCP::change_player() {
    state.player = (state.player&1) + 1;
}

void OCP::initial_state() {
    state.cards.resize(2);
    state.cards[0] = rand() % properties.N;
    state.cards[1] = rand() % (properties.N-1);
    if (state.cards[0] <= state.cards[1]){
        state.cards[1]++;
    }
    state.player = 1;
}

void OCP::first_state() {
    state.cards.resize(2);
    for (int k = 0; k < 2; k++)
        state.cards[k] = k;
    state.player = 1;
    state.history.clear();
}

bool OCP::next_state() {
    state.cards[1]++;
    if(state.cards[0] == state.cards[1])
        state.cards[1]++;
    if(state.cards[1] >= properties.N) {
        state.cards[1] = 0;
        state.cards[0]++;
    }
    return state.cards[0] < properties.N;
}

InformationSet OCP::information_set() {
    int card = state.cards[player() - 1];
    InformationSet inf_set({card, state.history});
    return inf_set;
}

vector<Action> OCP::actions() {
    return vector<Action>{pass, bet};
}

void OCP::update_state(const Action& action) {
    change_player();
    state.history.push_back(action);
}

void OCP::revert_state() {
    change_player();
    state.history.pop_back();
}

bool OCP::terminal_state() {
    int N = state.history.size();
    if (N < 2)
        return false;
    Action last = state.history[N-1];
    Action pe   = state.history[N-2];
    if(pe == bet)
        return true;
    if(pe == pass && last == pass)
        return true;
    return false;
}

double OCP::utility(int i) {
    vector<int>bets(2, 1);
    for (int i = 0; i < state.history.size(); i++) {
        if(state.history[i] == bet)
            bets[i&1]++;
    }
    int N = state.history.size();
    Action last = state.history[N-1];
    Action pe   = state.history[N-2];
    int winner;
    if(pe == bet && last == pass){
        winner = player();
    }else{
        winner = state.cards[0] > state.cards[1] ? 1 : 2;
    }
    return (i == winner ? 1 : -1)*bets[winner&1];
}

void OCP::print() {
    cout << "Jugador: " << player() << endl;
    cout << "Cartas = " << "(" << state.cards[0] << ',' << state.cards[1] << ")\n";
    cout << "Historia = ( ";
    for (int i = 0; i < state.history.size(); i++)
        cout << (state.history[i] ? "apostar" : "pasar") << ' ';
    cout << ")" << endl;
    if(terminal_state()) {
        cout << "Estado terminal. Utilidades: ";
        cout << utility(1) << ' ' << utility(2) << endl;
    } else {
        cout << "Conjunto de informacion" << endl;
        InformationSet inf_set = information_set();
        cout << "\tCarta = " << inf_set.card << endl;
        cout << "\tHistoria = ( ";
        for (int i = 0; i < inf_set.history.size(); i++)
            cout << (inf_set.history[i] ? "apostar" : "pasar") << ' ';
        cout << ")" << endl;
    }
    cout << endl;
}
