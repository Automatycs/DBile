#ifndef TABLE_HPP
#define TABLE_HPP

#include <iostream>
#include <fstream>
#include <string>
#include <vector>

#include "Database.hpp"

using namespace std;

class Database;


class Table {
    
    string file_addr;

    void display_select(vector<vector<string>> data) const;
    vector<vector<string>> read_all() const;

public:

    Database &db;
    const string name;
    const vector<string> cols;

    void describe() const; 
    void insert(const vector<string> &values) const;
    void select(vector<string> cols) const;

    Table(string nm, Database &dbx, vector<string> columns);

    const string &get_file_addr() const{cout << file_addr<< endl; return file_addr; }
};

#endif // TABLE_HPP