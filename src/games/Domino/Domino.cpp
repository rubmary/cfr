/**
* Implementacion de la clase Domino
*/

#include "Domino.hpp"
#include <cstdlib>
#include <ctime>
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
            state.hands[i][j] = state.pack[--k];
        sort(state.hands[i].begin(), state.hands[i].end());
    }
    state.pack.resize(k);
}

InformationSet Domino::information_set()
{
    int p = player() - 1;
    vector<short int> history = state.history_key;
    byte taken = byte{0};
    if(state.will_take) {
        taken |= byte{1};
        taken |= byte{state.take.first<<1};
        taken |= byte{state.take.second<<4};
    }
    for(int i = p; i < history.size(); i+=2)
        history[i] &= ~((1<<6)-1);

    vector<byte>hand(state.hands[p].size(), byte{0});
    for(int i = 0; i < hand.size(); i++) {
        hand[i] |= byte{state.hands[p][i].first};
        hand[i] |= byte{state.hands[p][i].second << 3};
    }
    return InformationSet({history, hand, taken});
}