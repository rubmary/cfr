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

// Los ultimos tres bytes representan la primera cara
// los siguientes tres la segunda
byte piece_mask(Piece piece) {
    byte mask = byte{0};
    mask |= byte{(unsigned char) piece.first};
    mask |= byte{(unsigned char) (piece.second << 3)};
    return mask;
}

InformationSet Domino::information_set()
{
    int p = player() - 1;
    // history
    vector<short int> history(state.history.size(), 0);
    for(int i = 0; i < history.size(); i++) {
        Action action = state.history[i];
        if(action.side != 'n') {
            history[i] |= (1<<6); // El septimo bit indica si paso o no el jugador
            history[i] |= (int) piece_mask(action.placed)<<8;
            if(action.side == 'r') // El bit quince representa de que lado se jugo
                history[i] |= (1<<14);
        }
        if(action.taken != Piece({-1, -1})) {
            history[i] |= (1<<7); // El octavo bit indica si se tomo ficha o no
            history[i] |= (int) piece_mask(action.taken);
        }
    }
    for(int i = p; i < history.size(); i+=2) // borrar las fichas tomadas del oponente
        history[i] &= ~((1<<6)-1);

    // tanken_mask
    Piece taken_piece = state.pack[state.pack.size()-1];
    byte taken_mask = byte{0};
    if(will_take()) {
        taken_mask = piece_mask(taken_piece);
        taken_mask |= byte{1<<6};
    }
    // hand
    vector<byte>hand(state.hands[p].size(), byte{0});
    int i = 0;
    for (auto piece : state.hands[p]) {
        hand[i++] = piece_mask(piece);
    }
    return InformationSet({history, hand, taken_mask});
}

int Domino::place_to_left(const Piece& piece) {
    if(piece.first == state.left) {
        return piece.second;
    }
    if(piece.second == state.right) {
        return piece.first;
    }
    return -1;
}

int Domino::place_to_right(const Piece& piece) {
    if(piece.first == state.right){
        return piece.second;
    }
    if(piece.second == state.right){
        return piece.first;
    }
    return -1;
}

Action Domino::next_action(set<Piece>::iterator first) {
    set<Piece> &hand = state.hands[player() - 1];
    Action action({{-1, -1}, {-1, -1}, 'n', -1});
    for(auto pos = first; pos != hand.end(); pos++) {
        Piece piece = *pos;
        action.face = place_to_left(piece);
        if(action.face != -1) {
            action.placed = piece;
            action.side = 'l';
            return action;
        }
        action.face = place_to_right(piece);
        if(action.face != -1) {
            action.placed = piece;
            action.side = 'r';
            return action;
        }
    }
    action.taken = state.pack[state.pack.size() - 1];
    action.face = place_to_left(action.taken);
    if (action.face != -1) {
        action.placed = action.taken;
        action.side = 'l';
        return action;
    }
    action.face = place_to_right(action.taken);
    if (action.face != -1) {
        action.placed = action.taken;
        action.side = 'r';
        return action;
    }
    return action;
}

Action Domino::first_action()
{
    return next_action(state.hands[player() - 1].begin());
}

Action Domino::next_action(const Action& action) {
    Action next = action;
    if(action.side == 'l') {
        next.face = place_to_right(action.placed);
        if(next.face != -1 && state.left !=state.right) {
            next.side = 'r';
            return next;
        }
    }
    set<Piece>::iterator first = state.hands[player()-1].upper_bound(action.placed);
    return next_action(first);
}

bool Domino::last_action(const Action& action) {
    if(action.side == 'n') {
        return true;
    }
    Action next = next_action(action);
    if(next.side == 'n') {
        return true;
    }
    return false;
}

int Domino::actions() {
    int total = 0;
    set<Piece> &hand = state.hands[player() - 1];
    for (auto piece : state.hands[player() - 1]) {
        if(place_to_left(piece) != -1 ) total++;
        if (place_to_right(piece) != -1 && state.left != state.right)
            total++;
    }
    if (total == 0) {
        Piece taken_piece = state.pack[state.pack.size()-1];
        if(place_to_left(taken_piece) != -1) total++;
        if(place_to_right(taken_piece) != -1 && state.left != state.right)
            total++;
    }
    return max(total, 1);
}

void Domino::update_state(Action const& action) {
    set<Piece>&hand = state.hands[player() - 1];
    if(action.taken != Piece({-1, -1})) {
        state.pack.pop_back();
        hand.insert(action.taken);
    }
    if(action.side != 'n') {
        hand.erase(action.placed);
        int &prev_head = action.side == 'l' ? state.left : state.right;
        prev_head = action.face;
    }
    state.history.push_back(action);
    change_player();
}

void Domino::revert_state() {
    Action &action = state.history[state.history.size() - 1];
    set<Piece>&hand = state.hands[player() - 1];
    if(action.taken != Piece({-1, -1})) {
        state.pack.push_back(action.taken);
    }
    if(action.side != 'n') {
        Piece piece = action.placed;
        hand.insert(action.placed);
        int prev_head = action.face == piece.first
            ? piece.first : piece.second;
        if(action.side = 'l')
            state.left = prev_head;
        else
            state.right = prev_head;
    }
    state.history.pop_back();
    change_player();
}

bool Domino::is_chance() {
    return false;
}

vector<double> Domino::distribution() {
    return vector<double>(0);
}

bool Domino::terminal_state() {
    for(int i = 0; i < 2; i++) {
        if (state.hands[i].empty())
            return true;
    }
    int N = state.history.size();
    if( N > 2 && state.history[N-1].side == 'n' && state.history[N-2].side == 'n')
        return true;
    return false;
}

int Domino::count_points(const set<Piece> &hand) {
    int points = 0;
    for(auto piece : hand) {
        points += piece.first;
        points += piece.second;
    }
    return points;
}

double Domino::utility() {
    vector<int> points(2);
    for (int i = 0; i < 2; i++)
        points[i] = count_points(state.hands[i]);
    if(state.hands[0].empty()) {
        return points[1];
    }
    if(state.hands[1].empty()) {
        return -points[2];
    }
    if (points[0] == points[1]) {
        return 0;
    }
    return points[0] < points[1] ? points[1] : -points[0];
}

void Domino::print() {

}