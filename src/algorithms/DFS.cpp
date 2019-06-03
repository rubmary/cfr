#include <games/Game.hpp>

int k = 0;

#ifndef CLASS_DFS
#define CLASS_DFS
template <
    typename State,
    typename Action,
    typename Properties,
    typename InformationSet
>
class DFS{
public:
    Game<State, Action, Properties, InformationSet> *game;
    bool cont;
    void do_something() {
        game -> print();
    }

    void dfs(){
        k++;
        // if (k > 30)
        //     cont = false;
        if (!cont)
            return;
        do_something();
        if(game -> terminal_state()){
            // cont = false;
            return;
        }
        Action action = game -> first_action();
        while(true) {
            game -> update_state(action);
            dfs();
            game -> revert_state();
            if(game -> last_action(action))
                break;
            action = game -> next_action(action);
        }
    }
    void start_dfs() {
        game -> initial_state();
        dfs();
    }
};
#endif
