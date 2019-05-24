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

    virtual void initial_state() = 0;
    virtual Action next_action(const Action& action) = 0;
    virtual Action first_action() = 0;
    virtual bool last_action(const Action& action) = 0;
    virtual void update_state(const Action &action) = 0;
    virtual void revert_state() = 0;
    virtual InformationSet information_set() = 0;
    virtual int information_set_id() = 0;
    virtual int player() = 0;
    virtual bool terminal_state() = 0;
    virtual int utility() = 0;
    virtual void print() = 0;
};
