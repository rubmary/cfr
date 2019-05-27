#include <cstdlib>
#include <ctime>
#include <games/KuhnPoker.hpp>
using namespace std;

template <
    typename State,
    typename Action,
    typename Properties,
    typename Informationset
>
class DFS{
public:
    Game<State, Action, Properties, InformationSet> *game;
    void do_something() {
        game -> print();
    }

    void dfs(){

        if(game -> terminal_state()){
            // do_something();
            return;
        }
        do_something();
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
int main() {
    srand(time(NULL));
    KuhnPoker kuhn_poker;
    DFS<State, Action, Properties, InformationSet> dfs({&kuhn_poker});
    for (int i = 0; i < 10; i++){
        dfs.start_dfs();
    }
}