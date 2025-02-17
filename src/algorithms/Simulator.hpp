#include <games/Game.hpp>
#include <iostream>
using namespace std;

#ifndef CLASS_Simulator
#define CLASS_Simulator
/**
* Class Simulator
* @game:    juego
* @sigma:   estrategia analizada
*
*/
template <
    typename State,
    typename Action,
    typename Properties,
    typename InformationSet,
    typename Hash
>
class Simulator {
    Game<State, Action, Properties, InformationSet, Hash> *game;
    vector <vector<double>> sigma;

    /**
    * read_sigma
    * lee la estrategia de un istream
    */
    void read_sigma(istream& is);

    /**
    * select_action
    *
    * seleccionar accion acorde a una distribucion
    * de probabilidad
    *
    * parametros
    *   @strategy:  distribucion de probabilidad
    *
    * return
    *   action elegida
    */
    int select_action(vector<double>&strategy);

    /**
    * movimientos recursivos para simular un juego
    */
    double move_recursively();

    /**
    * play:
    * simula 1 juego
    *
    * return
    *   ganancia del jugador 1
    */
    double play();

public:
    /**
    * Constructor del Simulador
    */
    Simulator(
        Game<State, Action, Properties, InformationSet, Hash> *game,
        istream& is_inf_sets,
        istream& is_strategy
    );
    /**
    * plays:
    * simula n juegos
    *
    * parametros
    *   @n:     numero de juegos a simular
    * 
    * return
    *   ganancia promedio del jugador 1
    */
    double plays(int n);
};

#endif
