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
    * Inicializar el estado en una partida
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
    * Determina el numero de acciones posibles en un nodo
    * dado
    */
    virtual int actions() = 0;

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
    * Retorna el jugador del nodo actual
    */
    virtual int player() = 0;

    /**
    * Determina si un nodo es de azar o no
    */
    virtual bool is_chance() = 0;

    /**
    * Retorna la distribucion de probabilidad de un nodo de azar
    */
    virtual vector<double> distribution() = 0;

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
};
#endif
