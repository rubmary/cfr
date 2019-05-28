/**
*   Interfaz del juego
*   One Card Poker (OCP)
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
#include <games/Game.hpp>
using namespace std;

enum Action { pass, bet };

typedef vector <Action> History;

struct Properties {
    int N;
};

struct State {
    vector <int> cards;
    History history;
    int player;
    bool operator == (const State& state) const {
        return  cards == state.cards &&
                history == state.history &&
                player == state.player;
    }
};

struct InformationSet {
    int card;
    History history;
    bool operator == (const InformationSet& inf_set) const {
        return card == inf_set.card && history == inf_set.history;
    }
};

struct Hash
{
    size_t operator()(InformationSet const& inf_set) const noexcept
    {
        size_t hash = 0;
        for (int i = 0; i < inf_set.history.size(); i++) {
            boost::hash_combine(hash, inf_set.history[i]);
        }
        boost::hash_combine(hash, inf_set.card);
        return hash;
    }
};

class OCP : public Game<State, Action, Properties, InformationSet>
{
    unordered_map<InformationSet, int, Hash> I;

public:

    OCP(int cards);

    int player();

    void change_player();

    InformationSet information_set();

    void initial_state();

    Action next_action(const Action& action);

    Action first_action();

    bool last_action(const Action& action);

    int actions();

    void update_state(const Action &action);

    void revert_state();

    int information_set_id();

    bool terminal_state();

    bool is_chance();

    vector <double> distribution();

    int utility();

    void print();
};