#include <games/Game.hpp>
#include <iostream>
using namespace std;

#ifndef CLASS_GEBR
#define CLASS_GEBR

/**
* Class GEBR
* @game:    juego
* @t:       utilidad esperadad de un conjunto de informacion y la accion
* @b:       probabilidades de un conjunto de informacion y la accion
* @depths:  vector de profundidades de los conjuntos de informacion
* @sigma:   estrategia analizada
* @N:       numero de conjuntos de informacion
*
*/
template <
    typename State,
    typename Action,
    typename Properties,
    typename InformationSet,
    typename Hash
>
class GEBR {
    Game<State, Action, Properties, InformationSet, Hash> *game;
    vector <vector<double>> *sigma;
    vector<vector<double>> t;
    vector<vector<double>> b;
    vector <vector<int>> depths;
    int N;

    /**
    * discover_tree
    * dfs para descubrir las profundidades
    * y los conjuntos de informacion
    * return: numero de conjuntos de informacion
    */
    void discover_tree(int d);

    /**
    * pass1: first pass
    * - set t[I][a] = b[I][a] = 0
    * - sort information sets by depth
    */
    void pass1();

    /**
    * pass2: second pass
    * Realizar multiples veces por profundidad
    */
    double pass2(int i, int d, int l, double pi);

    /**
    * best_response
    *   @i:     jugador
    *
    * Calcula la mejor respuesta sigma*_i de la estrategia
    * sigma_{-i} y retorna la ganancia esperada del perfil
    * estrategio sigma = (sigma*_i, sigma_{-i}) pra el
    * jugador i
    *
    * return
    *   u_i(sigma*_i, sigma_{-i})
    */
    double best_response(int i);

public:
    /**
    * Constructor con el juego
    */
    GEBR(Game<State, Action, Properties, InformationSet, Hash> *game);

    /**
    * explotability:
    * explotabilidad de la estrategia sigma
    *
    */
    double explotability(vector<vector<double>> *sigma);
};
#endif