#ifndef DATABASE_HPP
#define DATABASE_HPP
#include <string>
#include "Table.hpp"

using namespace std;

class Table;

class Database {

public:

    static void init();
    static vector<Database *> databases;
    static void show_databases();
    static Database *find_db(const string &req);

    const string name;
    
    Database(const string &p_name);

    vector<Table *> tables;    
    
    void show_tables();

    Table *find_table(const string &req) const;    

};

#endif // DATABASE_HPP