/**
    Interfaz para la representacion
    de los juegos no deterministas con informacion
    incompleta
*/
#include <unordered_map>
#include <vector>
#include <sstream>
#include <iostream>
using namespace std;

#ifndef CLASS_ABSTRACT_GAME
#define CLASS_ABSTRACT_GAME
template <
    typename State,
    typename Action,
    typename Properties,
    typename InformationSet,
    typename Hash
>
class Game {
protected:
    State state;
    Properties properties;
    unordered_map<InformationSet, int, Hash> I;
    vector<vector<int>> player_inf_sets = vector<vector<int>>(2);
    int information_sets = 0;

public:
    /**
    * Retorna el id del conjunto de informacion
    * del estado actual
    */
    virtual int information_set_id() {
        return I[information_set()];
    }

    /**
    * Retorna un vector con los conjuntos de informacion
    * descubiertos del jugador i
    */
    virtual vector<int> player_information_sets(int player) {
        return player_inf_sets[player - 1];
    }

    /**
    * Dfs para descubrir los conjuntos de informacion
    */
    virtual void inf_sets_dfs() {
        if(terminal_state()){
            return;
        }
        InformationSet inf_set = information_set();
        if(I.find(inf_set) == I.end()) {
            I[inf_set] = information_sets;
            player_inf_sets[player()-1].push_back(information_sets);
            information_sets++;
        }
        vector<Action> game_actions = actions();
        for(auto action : game_actions) {
            update_state(action);
            inf_sets_dfs();
            revert_state();
        }
    }

    /**
    * Descubre los conjuntos de informacion
    * retorna el numero de conjuntos de informacion
    */
    virtual int discover_information_sets() {
        first_state();
        do {
            inf_sets_dfs();
        } while(next_state());
        return information_sets;
    }

    /**
    * Descubre los conjuntos de informacion
    * a partir de un istream
    */
    virtual int discover_information_sets(istream& is) {
        is >> information_sets;
        InformationSet inf_set;
        int id;
        string _;
        for (int i = 0; i < information_sets; i++) {
            is >> id >> _ >> inf_set;
            I[inf_set] = id;
        }
        return information_sets;
    }

    /**
    * Primer estado inicial del juego
    */
    virtual void first_state() = 0;

    /**
    * Siguiente estado inicial
    */
    virtual bool next_state() = 0;

    /** 
    * Inicializar el estado en una partida
    * de forma aleatoria
    **/
    virtual void initial_state() = 0;

    /**
    * Retorna las acciones posibles en un estado
    */
    virtual vector<Action> actions() = 0;

    /**
    * Actualiza el estado actual aplicando la accion
    */
    virtual void update_state(const Action &action) = 0;

    /**
    * Revierte la ultima accion de la historia
    */
    virtual void revert_state() = 0;

    /**
    * Retorna la estructura que representa el conjunto de
    * informacion del estado actual
    */
    virtual InformationSet information_set() = 0;

    /**
    * Retorna el jugador del nodo actual
    */
    virtual int player() = 0;

    /**
    * Determina si un estado es terminal o no
    */
    virtual bool terminal_state() = 0;

    /**
    * Retorna la ganancia del primer jugador
    * dado un estado terminal
    */
    virtual double utility(int i) = 0;

    /*
    * Imprime cosas para testing
    */
    virtual void print() = 0;

    /**
    * print_information_sets
    * Imprime en un archivo los conjuntos de informacion
    * con su id
    **/
    virtual void print_information_sets(ostream& os = cout) {
        os << I.size() << endl;
        for (auto inf_set : I)
            os << inf_set.second << " : " << inf_set.first << endl;
    }
};
#endif
