#include <games/Game.hpp>
#include <iostream>
using namespace std;

#ifndef CLASS_CFR
#define CLASS_CFR
/**
*   Class CFR
*   @EPS
*   @game:          juego a resolver
*   @R:             regrets inmediatos
*   @avg_s:         strategia promedio (acumulada)
*   @strategy:      estrategia actual
*   @player:        conjuntos de informacion por jugador
*/
template <
    typename State,
    typename Action,
    typename Properties,
    typename InformationSet,
    typename Hash
>
class CFR {
    double EPS;
    Game<State, Action, Properties, InformationSet, Hash> *game;
    vector <vector <double>> R;
    vector <vector <double>> avg_s;
    vector <vector <double>> strategy;
    vector <int> player;

    /**
    *   dfs
    *   calcula las utilidades de los nodos para un jugador
    *   mediante un dfs
    *
    *   parametros
    *   @i:     el jugador al que se le calculara las utilidades
    *   @p1:    la probabilidad del jugador 1 de llegar al estado actual
    *   @p2:    la probabilidad del jugador 2 de llegar al estado actual
    *
    *   @return utilidad del jugador i del estado actual
    */
    double dfs(int i, double p1, double p2);

    /**
    *   update_strategy
    *   actualiza las estrategias en un conjunto de informacion
    *   dado
    *
    *   parametros
    *   @I:     conjunto de informacion
    *   @N:     numero de acciones en ese conjunto de informacion
    *   @pi:    probabilidad del jugador de alcanzar ese conjunto
    *           de informacion
    */
    void update_strategy(int I, int N, double pi);

    /**
    *   initialize_game
    *   inicializa una corrida del juego
    */
    void initialize_game();

    /**
    *   normalize_strategy
    *   normaliza la estrategia dividiendo cada probabilidad
    *   entre la suma total, para que la suma de las probabilidades
    *   sea igual a 1
    */
    void normalize_strategy();

public:
    CFR(Game<State, Action, Properties, InformationSet, Hash> *g, double EPS = 1e-3);

    void dfs_initialization();

    /**
    *   train
    *   calcula una aproximacion de un equilibrio de Nash
    *   iterando multiples veces
    *   parametros
    *   @iterations:    numero de iteraciones
    */
    void train(int iterations, string file = "regret.txt");

    /**
    *   average_strategy
    *   estrategia
    *   @return:    la estrategia promedio
    */
    vector<vector<double>> average_strategy();

    /**
    *   regret
    *   @return:    el regret inmediato
    */
    vector <vector <double>> regret();

    /**
    *   print_strategy
    *   imprime la estrategia actual en un archivo
    */
    void print_strategy(string file = "strategy.txt");
};
#endif