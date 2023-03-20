#ifndef SQL_HPP_
#define SQL_HPP_

#include <iostream>
#include <functional>
#include <string>
#include <fstream>
#include <algorithm>
#include <vector>

using namespace std;

class SQL {
    public:
        void choose_action(string str);
        string get_current();

    private:
        string keywords[6] = {"create", "show", "use", "describe", "insert", "select"};
        void (SQL::*keywords_function[6])() = {&SQL::case_create, &SQL::case_show, &SQL::case_use, &SQL::case_describe, &SQL::case_insert, &SQL::case_select};
        vector<string> current_args;
        string current_db = "";

        void case_create();
        void case_show();
        void case_use();
        void case_describe();
        void case_insert();
        void case_select();

        string clean_sql(string str);
        void parse_sql(string str);
        vector<string> parse_parentheses(const string &str);
        bool is_good_values(string str);
        bool check_current_db();

};

#endif /* SQL_HPP_ */