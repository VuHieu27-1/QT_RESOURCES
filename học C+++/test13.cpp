#include <iostream>
#include <climits>
#include <map>
#include <algorithm>
using namespace std;
long long a[10000000], b[10000000];
int main()
{
    int n,  index = 0;
    cin >> n ;
    int min = INT_MAX;
    map<long long, long long> dem;
    for(int i = 0; i < n; i++)
    {
        cin >> a[i];
    }
    for(int i = 0; i < n; i++)
    {
        dem[a[i]] ++;
    }
    for(int i = 1; i <= n + 1 ; i++)
    {
        if(dem[i] == 0 )
        {
            cout << i;
            return 0;
        }
    }
    return 0;

}