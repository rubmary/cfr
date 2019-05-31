/**
*   Interfaz del juego
*   Dudo
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
#include <utility>
#include "games/Game.hpp"
using namespace std;

namespace domino
{
typedef pair<short int, short int> Piece;

struct Action {
    Piece taken;
    Piece placed;
    char side;
    char face; // Actualizar
    bool operator == (Action const& action) const {
        return  taken == action.taken &&
                placed == action.placed &&
                side == action.side;
    }
};

typedef vector<Action>History;

struct Properties {
    int max_point;
    int initial_hand;
};

struct State {
    vector<Piece> pack;
    vector<set<Piece>> hands;
    int left, right;
    History history;
    int player;
};

struct InformationSet {
    vector<short int> history;
    vector<byte> hand;
    byte card_taken;
    bool operator == (InformationSet const& inf_set) const {
        return  history == inf_set.history &&
                hand == inf_set.hand &&
                card_taken == inf_set.card_taken;
    }
};

struct Hash {
    size_t operator()(InformationSet const& inf_set) const noexcept {
        size_t hash = 0;
        boost::hash_range(hash, inf_set.history.begin(), inf_set.history.end());
        boost::hash_range(hash, inf_set.hand.begin(), inf_set.hand.end());
        return hash;
    }
};

class Domino : public Game<State, Action, Properties, InformationSet>
{
    unordered_map<InformationSet, int, Hash> I;

    bool place_to_left(const Piece& piece);

    bool place_to_right(const Piece& piece);

    bool will_take();
public:
    Domino(int max_point, int initial_hand);

    int player();

    void change_player();

    void initial_state();

    virtual InformationSet information_set();

    Action first_action();

    Action next_action(const Action& action);

    bool last_action(const Action& action);

    int actions();

    void update_state(const Action &action);

    void revert_state();

    int information_set_id();

    bool terminal_state();

    bool is_chance();

    vector <double> distribution();

    double utility();

    void print();
};

}