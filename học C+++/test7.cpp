#include <iostream>
#include <map>
using namespace std;
long long a[1000000];
int main()
{
    map<long long, int> dem;
    long long n, maxarr;
    cin >> n;
    for (int i = 0; i < n; i++)
    {
        cin >> a[i];
    }
    for (int i = 0; i < n; i++)
    {
        dem[a[i]]++;
    }
    for (auto x : dem)
    {
        cout << x.first << " " << x.second << endl;
    }
    return 0;
}