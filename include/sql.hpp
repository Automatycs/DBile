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

    private:
        string keywords[6] = {"create", "show", "use", "describe", "insert", "select"};
        void (SQL::*keywords_function[6])() = {&SQL::case_create, &SQL::case_show, &SQL::case_use, &SQL::case_describe, &SQL::case_insert, &SQL::case_select};
        vector<string> current_args;

        void case_create();
        void case_show();
        void case_use();
        void case_describe();
        void case_insert();
        void case_select();

        string clean_sql(string str);
        void parse_sql(string str);
        bool is_good_values(string str);

};

#endif /* SQL_HPP_ */