#include <iostream>
#include <map>
using namespace std;
string s[10000000];
int main()
{

    map<string, long long> dem;
    long long maxarr = INT_MIN;
    string res;
    string n;
    getline(cin, n);
    n += " ";
    string t = "";
    for (int i = 0; i < n.size(); i++)
    {
        if (n[i] == ' ')
        // if (n[i] == ' ' || n[i] == NULL)
        {
            cout << t << endl;
            dem[t]++;
            t = "";
        }
        else
        {
            t += n[i];
        }
    }
    for (auto x : dem)
    {
        if (x.second > maxarr)
        {
            maxarr = x.second;
            res = x.first;
        }
    }
    cout << maxarr << " " << res << endl;

    return 0;
}