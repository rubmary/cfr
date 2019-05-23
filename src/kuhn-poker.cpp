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

struct Hash
{
    size_t operator()(State const& state) const noexcept
    {
        size_t hash = boost::hash_range(state.history.begin(), state.history.end());
        boost::hash_combine(hash, state.cards[state.player - 1]);
        return hash;
    }
};

struct Equal
{
    bool operator() (const State& s1, const State& s2) const {
        return  s1.history == s2.history &&
                s1.cards[s1.player - 1] == s2.cards[s2.player - 1];
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