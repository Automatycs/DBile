#include "../include/Utils.hpp"
using namespace std;

namespace Utils
{
    string csvify(const vector<string> &v)
    {
        string ret;
        for (string s : v)
        {
            ret += s + ",";
        }
        return ret;
    }
}