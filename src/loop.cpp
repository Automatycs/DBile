#include "../include/loop.hpp"
#include "../include/sql.hpp"
#include "../include/connection.hpp"

void loop(int ac, char **av) {
    SQL DBile;
    Connection connect;

    bool quit = false;
    string action;

    connect.connect(ac, av);
    if (connect.get_status() == false)
        return;
    if (connect.get_sql() != "")
        DBile.choose_action(connect.get_sql());
    while (!quit) {
        cout << DBile.get_current() << '>';
        getline(cin, action);
        if (action == "quit" || action == "QUIT" || action == "q" || action == "Q")
            quit = true;
        else
            DBile.choose_action(action);
    }
    cout << "bye bye " << connect.get_user() << "!!" << endl;
}

int main(int ac, char **av) {
    loop(ac, av);
    return (0);
}