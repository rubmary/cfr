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
#include "Game.hpp"
using namespace std;

namespace domino
{
typedef pair<short int, short int> Piece;

struct Action {
    Piece taken;
    Piece placed;
    char side;
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

class Domino : public Game<State, Action, Properties, InformationSet, Hash>
{
    bool place_to_left(const Piece& piece);

    bool place_to_right(const Piece& piece);

    int opposite(int number, const Piece& piece);

    bool will_take();

    int count_points(const set<Piece> &hand);

    void deal_cards();

    void create_pack();

public:
    Domino(int max_point, int initial_hand);

    int player();

    void change_player();

    void initial_state();

    void first_state();

    bool next_state();

    virtual InformationSet information_set();

    vector<Action> actions();

    void update_state(const Action &action);

    void revert_state();

    bool terminal_state();

    double utility(int i);

    void print();
};
}