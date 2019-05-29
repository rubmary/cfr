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
#include "games/Game.hpp"
using namespace std;

namespace dudo
{

struct Action {
    int quantity;
    int face;
};

typedef vector<Action>History;
typedef vector<int>Dice;

struct Properties {
    int K;
    int D1;
    int D2;
    vector<vector<double>>dudos; // Valores esperados de los juegos mas pequeños con k caras
};

struct State {
    int player;
    History history;
    string bidding_sequence;
    bool calls_bluff;
    vector<Dice> dice;
};

struct InformationSet {
    Dice dice;
    unsigned long long bidding_sequence;
    bool operator == (InformationSet const& inf_set) const {
        return  dice == inf_set.dice &&
                bidding_sequence == inf_set.bidding_sequence;
    }
};

struct Hash {
    size_t operator()(InformationSet const& inf_set) const noexcept {
        size_t hash = boost::hash_range(inf_set.dice.begin(), inf_set.dice.end());
        boost::hash_combine(hash, inf_set.bidding_sequence);
    }
};

class Dudo : public Game<State, Action, Properties, InformationSet>
{
public:

    Dudo(int K, int D1, int D2, vector<vector<double>> &dudos);

    unordered_map<InformationSet, int, Hash> I;

    int player();

    void change_player();

    void initial_state();

    InformationSet information_set();

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