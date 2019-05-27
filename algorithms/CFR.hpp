#include <games/KuhnPoker.hpp>

/**
    Class CFR
    @EPS
    @root:          raiz del arbol del juego
    @R:             inmediatos regrets
    @avg_s:         strategia promedio (acumulada)
    @strategy:      estrategia actual
    @player:        conjuntos de informacion por jugador
 */
template <
    typename State,
    typename Action,
    typename Properties,
    typename Informationset
>
class CFR {
    double EPS;
    Game<State, Action, Properties, InformationSet> *game;
    vector <vector <double>> R;
    vector <vector <double>> avg_s;
    vector <vector <double>> strategy;
    vector <int> player;

    /**
        dfs
        calcula las utilidades de los nodos para un jugador
        mediante un dfs

        parametros
        @i:     el jugador al que se le calculara las utilidades
        @p1:    la probabilidad del jugador 1 de llegar al estado actual
        @p2:    la probabilidad del jugador 2 de llegar al estado actual

        @return utilidad del jugador i del estado actual
     */
    double dfs(int i, double p1, double p2);

    /**
        update_strategy
        actualiza las estrategias en un conjunto de informacion
        dado

        parametros
        @I:     conjunto de informacion
        @N:     numero de acciones en ese conjunto de informacion
        @pi:    probabilidad del jugador de alcanzar ese conjunto
                de informacion
     */
    void update_strategy(int I, int N, double pi);

    /**
        initialize_game
        inicializa una corrida del juego
     */
    void initialize_game();

    /**
        normalize_strategy
        normaliza la estrategia dividiendo cada probabilidad
        entre la suma total, para que la suma de las probabilidades
        sea igual a 1
     */
    void normalize_strategy();
public:

    CFR(Game<State, Action, Properties, InformationSet> *g, eps = 1e-2) {
        game = g;
        R = vector <vector <double>>(0);
        avg_s = vector <vector <double>>(0);
        strategy = vector <vector <double>>(0);
        EPS = eps;
    }

    /**
        train
        calcula una aproximacion de un equilibrio de Nash
        iterando multiples veces

        parametros
        @iterations:    numero de iteraciones
     */
    vector <vector <double>> train(int iterations);

    /**
        average_strategy
        estrategia
        @return:    la estrategia promedio
     */
    vector <vector <double>> average_strategy();

    /**
        regret
        @return:    el regret inmediato
     */
    vector <vector <double>> regret();
};
