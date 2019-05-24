#include <vector>
#include <set>
#include <algorithm>
#include <random>
#include <ctime>
#include <unordered_map>
#include <chrono>
#include <boost/functional/hash.hpp>
#include "Game.cpp"
#include <iostream> // quitar luego
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
    }

    InformationSet information_set() {
        Card card = state.cards[player() - 1];
        InformationSet inf_set({card, state.history});
        return inf_set;
    }

    int information_set_id() {
        InformationSet inf_set = information_set();
        if(I.find(inf_set) == I.end())
            I[inf_set] = ++information_sets;
        return I[inf_set];
    }

    void change_player() {
        state.player = (state.player&1) + 1;
    }

    int player() {
        return state.player;
    }

    Action first_action() {
        return pass;
    }

    Action next_action(const Action& action) {
        return bet;
    }

    bool last_action(const Action& action) {
        return action == bet;
    }

    void update_state(const Action& action) {
        change_player();
        state.history.push_back(action);
    }

    void revert_state() {
        change_player();
        state.history.pop_back();
    }

    bool terminal_state() {
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

    int utility() {
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

    void print() {
        cout << "Repartir cartas:" << endl;
        cout << "\tJugador 1: " << state.cards[0] << endl;
        cout << "\tJugador 2: " << state.cards[1] << endl;

        cout << "Jugadas: ";
        for (int i = 0; i < state.history.size(); i++)
            cout << (state.history[i] ? "apostar" : "pasar") << ' ';
        cout << endl;
        
        cout << "Ganancia: " << utility() << endl << endl;
    }
};