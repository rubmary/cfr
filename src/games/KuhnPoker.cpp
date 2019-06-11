/**
    Implementacion de la clase KuhnPoker
*/
#include "KuhnPoker.hpp"
using namespace std;
using namespace kuhn_poker;

void KuhnPoker::deal_deck() {
    vector<int> deck(properties.deck.begin(), properties.deck.end());
    random_shuffle(deck.begin(), deck.end());
    state.cards.resize(2);
    for (int i = 0; i < (int) state.cards.size(); i++)
        state.cards[i] = Card{deck[i]};
}

int KuhnPoker::player() {
    return state.player;
}

void KuhnPoker::change_player() {
    state.player = (state.player&1) + 1;
}

void KuhnPoker::initial_state() {
    deal_deck();
    state.player = 1;
}

void KuhnPoker::first_state() {
    state.cards.resize(2);
    state.cards[0] = Card{0};
    state.cards[1] = Card{1};
    state.history.clear();
    state.player = 1;
}

bool KuhnPoker::next_state() {
    int remaining_card = 3;
    for (auto card : state.cards){
        remaining_card -= card;
    }
    vector<int> perm{state.cards[0], state.cards[1], remaining_card};
    if (!next_permutation(perm.begin(), perm.end())) {
        return false;
    }
    for (int i = 0; i < 2; i++) {
        state.cards[i] = Card{perm[i]};
    }
    return true;
}

InformationSet KuhnPoker::information_set() {
    Card card = state.cards[player() - 1];
    InformationSet inf_set({card, state.history});
    return inf_set;
}

vector<Action> KuhnPoker::actions() {
    return vector<Action>{pass, bet};
}


void KuhnPoker::update_state(const Action& action) {
    change_player();
    state.history.push_back(action);
}

void KuhnPoker::revert_state() {
    change_player();
    state.history.pop_back();
}

bool KuhnPoker::terminal_state() {
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

double KuhnPoker::utility(int i) {
    vector<int>bets(2, 1);
    for (int i = 0; i < (int) state.history.size(); i++) {
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

void KuhnPoker::print() {
    cout << "Jugador: " << player() << endl;
    cout << "Cartas = " << "(" << state.cards[0] << ',' << state.cards[1] << ")\n";
    cout << "Historia = ";
    for (int i = 0; i < (int) state.history.size(); i++)
        cout << (state.history[i] ? "apostar" : "pasar") << ' ';
    cout << endl;
    if (terminal_state()) {
        cout << "Estado terminal..." << endl;
        cout << "Utilidades: " << utility(1) << ' ' << utility(2) << endl;
        cout << endl;
        return;
    }
    cout << "Conjunto de informacion: " << information_set_id() << endl;
    InformationSet inf_set = information_set();
    cout << "\tCarta = " << inf_set.card << endl;
    cout << "\tHistoria = ";
    for (int i = 0; i < (int) inf_set.history.size(); i++)
        cout << (inf_set.history[i] ? "apostar" : "pasar") << ' ';
    cout << endl << endl;
}

string KuhnPoker::print_information_set(){
    InformationSet inf_set = information_set();
    string card = to_string(inf_set.card);
    string history = "";
    for (auto action : inf_set.history) {
        history += (action ? "apostar " : "pasar");
    }
    return card + " " + history;
}

ostream& operator<<(ostream& os, const InformationSet& I) {
    os << I.history.size() << ' ';
    for (auto action : I.history)
        os << action << ' ';
    os << I.card;
    return os;
}
