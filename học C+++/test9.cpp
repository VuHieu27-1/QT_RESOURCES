#include <iostream>
#include <map>
using namespace std;
long long a[1000000];
int main()
{
    map<long long, long long> dem;
    long long n, tong = 0;
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
        tong += (x.second - 1) * x.second / 2;
    }
    cout << tong << endl;
}