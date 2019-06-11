/**
*   Interfaz del juego
*   KuhnPoker
*/
#include <vector>
#include <set>
#include <algorithm>
#include <random>
#include <ctime>
#include <unordered_map>
#include <chrono>
#include <boost/functional/hash.hpp>
#include <iostream>
#include "Game.hpp"
using namespace std;

#ifndef KUHN
#define KUHN

namespace kuhn_poker
{
enum Card:int { Q, K, A };

enum Action:int { pass, bet };

typedef vector <Action> History;

struct Properties {
    set<Card> deck{Q, K, A};
};

struct State {
    vector <Card> cards;
    History history;
    int player;
    bool operator == (const State& state) const {
        return  cards == state.cards &&
                history == state.history &&
                player == state.player;
    }
};

struct InformationSet {
    Card card;
    History history;
    bool operator == (const InformationSet& inf_set) const {
        return card == inf_set.card && history == inf_set.history;
    }
};

struct Hash
{
    size_t operator()(InformationSet const& inf_set) const noexcept
    {
        size_t hash = boost::hash_range(inf_set.history.begin(), inf_set.history.end());
        boost::hash_combine(hash, inf_set.card);
        return hash;
    }
};

class KuhnPoker : public Game<State, Action, Properties, InformationSet, Hash>
{
    void deal_deck();

    int player();

    void change_player();

    void initial_state();

    void first_state();

    bool next_state();

    InformationSet information_set();

    vector<Action> actions();

    void update_state(const Action &action);

    void revert_state();

    bool terminal_state();

    double utility(int i);

    void print();

    string print_information_set();
};
}

ostream& operator<<(ostream& os, const kuhn_poker::InformationSet& I);

istream& operator>>(istream& is, kuhn_poker::InformationSet &I);

#endif
