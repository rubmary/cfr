/**
    Interfaz para la representacion
    de los juegos no deterministas con informacion
    incompleta
*/

#include <vector>
using namespace std;

#ifndef CLASS_ABSTRACT_GAME
#define CLASS_ABSTRACT_GAME
template <
    typename State,
    typename Action,
    typename Properties,
    typename InformationSet
>
class Game {
public:
    State state;
    Properties properties;
    int information_sets = 0;

    /**
    * Primer estado inicial del juego
    */
    virtual void first_state() = 0;

    /**
    * Siguiente estado inicial
    */
    virtual void next_state() = 0;

    /**
    * Determina si es el ultimo estado
    * inicial
    */
    virtual bool valid_state() = 0;

    /** 
    * Inicializar el estado en una partida
    * de forma aleatoria
    **/
    virtual void initial_state() = 0;

    /**
    * Obtener la siguiente accion dado un estado y la
    * ultima accion de ese estado
    */
    virtual Action next_action(const Action& action) = 0;

    /** 
    * Obtneer la primera accion de un nodo
    */
    virtual Action first_action() = 0;

    /**
    * Determinar si una accion es la ultima accion de un
    * nodo o no
    */
    virtual bool last_action(const Action& action) = 0;

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
    * Retorna el id del conjunto de informacion
    * del estado actual
    */
    virtual int information_set_id() = 0;

    /**
    * Retorna un vector con los conjuntos de informacion
    * descubiertos del jugador i
    */
    virtual vector<int> information_sets_number(int player) = 0;

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
    virtual double utility() = 0;

    /*
    * Imprime cosas para testing
    */
    virtual void print() = 0;

    /**
    * Imprime el conjunto de informacion
    */
    virtual string print_information_set() {
        return "";
    }
};
#endif
