#include <vector>
#include <set>
#include <algorithm>
#include <random>
#include <ctime>
#include <unordered_map>
#include <chrono>
#include <boost/functional/hash.hpp>
#include "Game.cpp"
#include <iostream>
using namespace std;

enum Card { Q, K, A };

enum Action { pass, bet };

typedef vector <Action> History;

struct State {
    vector <Card> cards;
    History history;
    int player;
};

struct Properties {
    set<Card> deck{Q, K, A};
};

struct Hash
{
    size_t operator()(State const& state) const noexcept
    {
        /******** COMPLETAR ********/
    }
};

struct Equal
{
    bool operator() (const State& s1, const State& s2) const {
        /******** COMPLETAR ********/
    }
};

class KuhnPoker : public Game<State, Action, Properties>
{
private:
    void deal_deck() {
        unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
        vector<Card> deck(properties.deck.begin(), properties.deck.end());
        shuffle(deck.begin(), deck.end(), default_random_engine(seed));
        state.cards.resize(2);
        for (int i = 0; i < state.cards.size(); i++)
            state.cards[i] = deck[i];
    }
public:

    unordered_map<State, int, Hash, Equal> I;
    
    void initial_state() {
        deal_deck();
        state.player = 1;
    }

    int information_set() {
        if(I[state] == 0)
            I[state] = ++information_sets;
        return I[state];
    }

    void change_player() {
        state.player = (state.player&1) + 1;
    }

    int player() {
        return state.player;
    }

    Action first_action() {
        /******** COMPLETAR ********/
    }

    Action next_action(const Action& action) {
        /******** COMPLETAR ********/
    }

    bool last_action(const Action& action) {
        /******** COMPLETAR ********/
    }

    void update_state(const Action& action) {
        /******** COMPLETAR ********/
        state.history.push_back(action);
    }

    void revert_state() {
        /******** COMPLETAR ********/
        state.history.pop_back();
    }

    bool terminal_state() {
        /******** COMPLETAR ********/
    }

    int utility() {
        /******** COMPLETAR ********/
    }
};