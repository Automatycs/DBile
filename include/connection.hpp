#ifndef CONNECTION_HPP_
#define CONNECTION_HPP_

#include <iostream>
#include <functional>
#include <string>
#include <fstream>

using namespace std;

class Connection {
    public:
        int connect(int size, char** args);
        string get_user();
        string get_sql();
        bool get_status();

    private:
        const string flags[8] = {"-n", "-h", "-v", "-l", "-u", "-p", "-s", "-f"};
        const string l_flags[8] = {"--new", "--help", "--version", "--login", "--user", "--password", "--sql", "--format"};
        void (Connection::*flags_fonction[8])(string str) = {&Connection::case_new, &Connection::case_help, &Connection::case_version, &Connection::case_login, &Connection::case_user, &Connection::case_password, &Connection::case_sql, &Connection::case_format};
        
        bool login = false;
        bool new_user = false;
        bool help = false;
        bool version = false;
        bool status = false;

        string user = "";
        string password = "";
        string sql_args = "";
        string format = "csv";
        string userfile = "users.csv";
        ifstream i_flux;
        ofstream o_flux;

        void case_new(string str);
        void case_help(string str);
        void case_version(string str);
        void case_login(string str);
        void case_user(string str);
        void case_password(string str);
        void case_sql(string str);
        void case_format(string str);

        bool is_valid(string str);
        void show();
        void show_version();
        void show_help();
        void create_user();
        void connect_user();
};

#endif /* CONNECTION_HPP_ */
