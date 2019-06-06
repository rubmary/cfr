#include <games/Game.hpp>

#ifndef CLASS_DFS
#define CLASS_DFS
template <
typename State,
typename Action,
typename Properties,
typename InformationSet,
typename Hash
>
class DFS{
public:
    Game<State, Action, Properties, InformationSet, Hash> *game;
    void do_something() {
        game -> print();
    }

    void dfs(){
        do_something();
        if(game -> terminal_state()){
            return;
        }
        vector<Action> actions = game -> actions();
        for (auto action : actions) {
            game -> update_state(action);
            dfs();
            game -> revert_state();
        }
    }
    void start_dfs() {
        game -> first_state();
        do{
            dfs();
        } while(game -> next_state());
    }
};
#endif
