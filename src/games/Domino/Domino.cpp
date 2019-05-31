/**
* Implementacion de la clase Domino
*/

#include "Domino.hpp"
#include <cstdlib>
#include <ctime>
#include <algorithm>
using namespace domino;

Domino::Domino(int max_point, int initial_hand)
{
    properties = {max_point, initial_hand};
}

int Domino::player()
{
    return state.player;
}

void Domino::change_player()
{
    state.player = (state.player&1) + 1;
}

int Domino::information_set_id() {
    InformationSet inf_set = information_set();
    if(I.find(inf_set) == I.end())
        I[inf_set] = information_sets++;
    return I[inf_set];
}

void Domino::initial_state()
{
    int N = (properties.max_point+1)*(properties.max_point+2)/2;
    state.pack.resize(N);
    int k = 0;
    for(int i = 0; i <= properties.max_point; i++) {
        for (int j = i; j <= properties.max_point; j++) {
            state.pack[k++] = {i, j};
        }
    }
    unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
    shuffle(state.pack.begin(), state.pack.end(), default_random_engine(seed));
    for(int i = 0; i <2; i++) {
        for(int j = 0; j < properties.initial_hand; j++)
            state.hands[i].insert(state.pack[--k]);
    }
    state.pack.resize(k);
}

bool Domino::will_take() {
    set<Piece>&hand = state.hands[player()-1];
    for (auto piece : hand) {
        if(place_to_left(piece)){
            return true;
        }
        if (place_to_right(piece)) {
            return true;
        }
    }
    return false;
}

/** TERMINAR **/
InformationSet Domino::information_set()
{
    int p = player() - 1;
    vector<short int> history ;//= state.history_key;
    Piece taken_piece = state.pack[state.pack.size()-1];
    byte taken_mask = byte{0};
    if(will_take()) {
        taken_mask |= byte{1};
        taken_mask |= byte{taken_piece.first}<<1;
        taken_mask |= byte{taken_piece.second}<<4;
    }
    for(int i = p; i < history.size(); i+=2)
        history[i] &= ~((1<<6)-1);
    vector<byte>hand(state.hands[p].size(), byte{0});
    
    int i = 0;
    for (auto piece : state.hands[p]) {
        hand[i] |= byte{piece.first};
        hand[i] |= byte{piece.second << 3};
    }

    return InformationSet({history, hand, taken_mask});
}


bool Domino::place_to_left(const Piece& piece) {
    return piece.first == state.left || piece.second == state.right;
}

bool Domino::place_to_right(const Piece& piece) {
    return piece.first == state.right || piece.second == state.right;
}

Action Domino::first_action()
{
    Piece taken_piece = state.pack[state.pack.size()-1];
    Action action({{-1, -1}, {taken_piece}, 'l'});
    for(auto piece : state.hands[player() - 1]) {
        if(place_to_left(piece)){
            action.placed = piece;
            return action;
        }else if (place_to_right(piece)){
            action.placed = piece;
            action.side = 'r';
            return action;
        }
    }
    action.taken = taken_piece;
    if (place_to_left(taken_piece)) {
        return action;
    } else if (place_to_right(taken_piece)) {
        action.side = 'r';
        return action;
    }
    return {{-1, -1}, {-1, -1}, 'n'};
}

Action Domino::next_action(const Action& action) {
    Action next = action;
    if(action.side == 'l' && place_to_right(action.placed) && state.left != state.right) {
        next.side = 'r';
        return next;
    }
    set<Piece> &hand = state.hands[player() - 1];
    set<Piece>::iterator first = hand.upper_bound(action.placed);
    for(auto pos = first; pos != hand.end(); pos++) {
        Piece piece = *pos;
        if(place_to_left(piece)) {
            next.placed = piece;
            next.side = 'l';
            return next;
        } else if(place_to_right(piece)) {
            next.placed = piece;
            next.side = 'r';
            return next;
        }
    }
    next.placed = action.taken;
    if(place_to_left(action.taken)) {
        next.side = 'l';
    }else {
        next.side = 'r';
    }
    return next;
}

bool Domino::last_action(const Action& action) {
    if(action.side == 'n') {
        return true;
    }
    Action next = next_action(action);
    if (next == action) {
        return true;
    }
    if (next.side == 'l' && place_to_left(next.placed)) {
        return false;
    }
    if(next.side == 'r' && place_to_right(next.placed)) {
        return false;
    }
    return true;
}

int Domino::actions() {
    int total = 0;
    set<Piece> &hand = state.hands[player() - 1];
    for (auto piece : state.hands[player() - 1]) {
        if(place_to_left(piece)){
            total++;
        }
        if (place_to_right(piece) && state.left != state.right) {
            total++;
        }
    }
    if (total == 0) {
        Piece taken_piece = state.pack[state.pack.size()-1];
        if(place_to_left(taken_piece)){
            total++;
        }
        if(place_to_right(taken_piece) && state.left != state.right) {
            total++;
        }
    }
    return max(total, 1);
}

void Domino::update_state(Action const& action) {
    if(action.taken != Piece({-1, -1})) {
        state.pack.pop_back();
        state.hands[player()-1].erase(action.taken);   
    }
    if(action.side != 'n') {
        state.hands[player()-1].erase(action.placed);
        int &prev_head = action.side == 'l' ? state.left : state.right;
        int new_head = action.placed.first == prev_head
            ? action.placed.second
            : action.placed.first;
        prev_head = new_head;
    }
    state.history.push_back(action);
    change_player();
}

// Agregar el numero que coincidio en la accion para poder revertir
// los estados left y right
void Domino::revert_state() {
    Action &last_action = state.history[state.history.size() - 1];
    set<Piece>&hand = state.hands[player() - 1];
    if(last_action.taken != Action({-1, -1})) {
        deck.push_back(last_action);
    }
    if(last_action.placed != Action({-1, -1})) {
        hand.insert(last_action.placed);
    }

    state.history.pop_back();
    change_player();
}