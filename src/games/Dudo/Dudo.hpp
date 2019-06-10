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

/**
*
* Properties
* @K  : numero de caras del dado
* @D1 : numero de dados del jugador 1
* @D2 : numero de dados del jugador 2
*/
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
        return hash;
    }
};

class Dudo : public Game<State, Action, Properties, InformationSet, Hash>
{
    bool next_sequence(vector <int> &P, int N, int S);

public:
    Dudo(int K, int D1, int D2, vector<vector<double>> &dudos);

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
};

}