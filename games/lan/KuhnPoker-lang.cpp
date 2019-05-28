/**
    Implementacion de la clase KuhnPoker
*/
#include "KuhnPoker.hpp"

void KuhnPoker::deal_deck() {
    unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
    vector<Card> deck(properties.deck.begin(), properties.deck.end());
    shuffle(deck.begin(), deck.end(), default_random_engine(seed));
    state.cards.resize(2);
    for (int i = 0; i < state.cards.size(); i++)
        state.cards[i] = deck[i];
}

int KuhnPoker::player() {
    return state.player;
}

void KuhnPoker::change_player() {
    state.player = (state.player&1) + 1;
}

int KuhnPoker::information_set_id() {
    InformationSet inf_set = information_set();
    if(I.find(inf_set) == I.end())
        I[inf_set] = ++information_sets;
    return I[inf_set];
}

void KuhnPoker::initial_state() {
    deal_deck();
    state.player = 1;
    /******* COMPLETAR *******/
}

InformationSet KuhnPoker::information_set() {
    /******* COMPLETAR *******/
}

Action KuhnPoker::first_action() {
    /******* COMPLETAR *******/
}

Action KuhnPoker::next_action(const Action& action) {
    /******* COMPLETAR *******/
}

bool KuhnPoker::last_action(const Action& action) {
    /******* COMPLETAR *******/
}

void KuhnPoker::update_state(const Action& action) {
    /******* COMPLETAR *******/
}

void KuhnPoker::revert_state() {
    /******* COMPLETAR *******/
}

bool KuhnPoker::is_chance() {
    /******* COMPLETAR *******/
}

vector<double> KuhnPoker::distribution() {
    /******* COMPLETAR *******/
}

bool KuhnPoker::terminal_state() {
    /******* COMPLETAR *******/
}

int KuhnPoker::utility() {
    /******* COMPLETAR *******/
}
