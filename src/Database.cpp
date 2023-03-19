#include "../include/Database.hpp"
#include <array>
#include <memory>
#include <stdexcept>
#include <cstdio>
#include <sstream>

vector<Database *> Database::databases;

inline std::string exec(const char *cmd)
{
    std::array<char, 128> buffer;
    std::string result;
    std::unique_ptr<FILE, decltype(&pclose)> pipe(popen(cmd, "r"), pclose);
    if (!pipe)
    {
        throw std::runtime_error("popen() failed!");
    }
    while (fgets(buffer.data(), buffer.size(), pipe.get()) != nullptr)
    {
        result += buffer.data();
    }
    return result;
}

Database::Database(const string &p_name) : name(p_name)
{
    string path = "db/" + name + "/";
    // create if they don't exist
    system(string("mkdir -p " + path).c_str());
    databases.push_back(this);
}

void Database::show_databases()
{
    cout << "SHOWING DATABASES" << endl;
    for (Database *datb : databases)
    {
        cout << datb->name << endl;
    }
}

void Database::show_tables()
{
    cout << "SHOWING TABLES FROM " + name << endl;
    for (Table *table : tables)
    {
        cout << table->name << endl;
    }
}

Table *Database::find_table(const string &req) const
{
    for (Table *t : tables)
    {
        if (t->name == req)
            return t;
    }
    return nullptr;
}

Database *Database::find_db(const string &req)
{
    for (Database *d : databases)
    {
        if (d->name == req)
            return d;
    }
    return nullptr;
}

void Database::init()
{

    stringstream dbs = stringstream(exec("ls -l db | grep -E '^d' | awk '{print $NF}'")), tabs, cols ;
    string db, tab, req, temp, path, first_line, temp_col;
    vector<string> tab_columns;
    ifstream file;

    while (getline(dbs, db))
    { // for each database
        Database *db_p = new Database(db);
        req = "ls -l db/" + db_p->name + " | grep csv | awk '{print $NF}' | cut -f 1 -d '.'";
        tabs = stringstream(exec(req.c_str())); 
        while (getline(tabs, tab)) // for each table
        {
            tab_columns = vector<string>();
            first_line.clear();
            cols.clear();
        

            temp = tab;
            path = "db/" + db_p->name + "/" + temp + ".csv";
            cout << "new table " << temp << ", columns : " << tab_columns.size() << endl;
            file.open(path);
            getline(file, first_line);
            cout << " first_line: " << first_line <<  endl;
            cols = stringstream(first_line);

            while (getline(cols, temp_col, ','))
            { // for each column
                string test =  temp_col;
                cout << "new column " << test << endl;
                tab_columns.push_back(test);
            }
            file.close();

            Table *tp = new Table(temp, *db_p, tab_columns);
        }
    }
}