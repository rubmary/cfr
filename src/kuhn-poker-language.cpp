#include <vector>
#include <set>
#include <algorithm>
#include <random>
#include <ctime>
#include <unordered_map>
#include <chrono>
#include <boost/functional/hash.hpp>
#include "Game.cpp"
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
        size_t hash = 0;
        for (int i = 0; i < inf_set.history.size(); i++) {
            boost::hash_combine(hash, inf_set.history[i]);
        }
        boost::hash_combine(hash, inf_set.card);
        return hash;
    }
};

class KuhnPoker : public Game<State, Action, Properties, InformationSet>
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
    unordered_map<InformationSet, int, Hash> I;
    
    void initial_state() {
        deal_deck();
        state.player = 1;
        /******* COMPLETAR *******/
    }

    int information_set_id() {
        InformationSet inf_set = information_set();
        if(I.find(inf_set) == I.end())
            I[inf_set] = ++information_sets;
        return I[inf_set];
    }

    InformationSet information_set() {
        /******* COMPLETAR *******/
    }

    void change_player() {
        state.player = (state.player&1) + 1;
    }

    int player() {
        return state.player;
    }

    Action first_action() {
        /******* COMPLETAR *******/
    }

    Action next_action(const Action& action) {
        /******* COMPLETAR *******/
    }

    bool last_action(const Action& action) {
        /******* COMPLETAR *******/
    }

    void update_state(const Action& action) {
        /******* COMPLETAR *******/
        state.history.push_back(action);
    }

    void revert_state() {
        /******* COMPLETAR *******/
        state.history.pop_back();
    }

    bool terminal_state() {
        /******* COMPLETAR *******/
    }

    int utility() {
        /******* COMPLETAR *******/
    }
};