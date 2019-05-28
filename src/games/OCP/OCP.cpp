/**
    Implementacion de la clase OCP
*/
#include "OCP.hpp"
#include <cstdlib>
#include <ctime>
using namespace std;

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

int OCP::information_set_id() {
    InformationSet inf_set = information_set();
    if(I.find(inf_set) == I.end())
        I[inf_set] = information_sets++;
    return I[inf_set];
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

InformationSet OCP::information_set() {
    int card = state.cards[player() - 1];
    InformationSet inf_set({card, state.history});
    return inf_set;
}

Action OCP::first_action() {
    return pass;
}

Action OCP::next_action(const Action& action) {
    return bet;
}

int OCP::actions() {
    return 2;
}

bool OCP::last_action(const Action& action) {
    return action == bet;
}

void OCP::update_state(const Action& action) {
    change_player();
    state.history.push_back(action);
}

void OCP::revert_state() {
    change_player();
    state.history.pop_back();
}

bool OCP::is_chance() {
    return false;
}


vector<double> OCP::distribution() {
    return vector<double>();
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

int OCP::utility() {
    vector<int>bets(2, 1);
    for (int i = 0; i < state.history.size(); i++) {
        if(state.history[i] == bet)
            bets[i&1]++;
    }
    int N = state.history.size();
    Action last = state.history[N-1];
    Action pe   = state.history[N-2];
    if(pe == bet && last == pass)
        return (player() == 1 ? 1 : -1)*bets[player()&1];
    return state.cards[0] > state.cards[1] ? bets[1] : -bets[0];
}

void OCP::print() {
    cout << "Jugador: " << player() << endl;
    cout << "Cartas = " << "(" << state.cards[0] << ',' << state.cards[1] << ")\n";
    cout << "Historia = ( ";
    for (int i = 0; i < state.history.size(); i++)
        cout << (state.history[i] ? "apostar" : "pasar") << ' ';
    cout << ")" << endl;
    

    if(terminal_state()) {
        cout << "Estado terminal. Utilidad: ";
        cout << utility() << endl;
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
