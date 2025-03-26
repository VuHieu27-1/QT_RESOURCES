#include <iostream>
#include <math.h>
#include <climits>
#include <map>
using namespace std;
long long a[10000000], b[10000000];
int main()
{
    int n, m;
    map<long long, long long> dem;
    cin >> n >> m;
    for(int i = 0; i < n; i++)
    {
        cin >> a[i];
    }
    for(int i = 0; i < m; i++)
    {
        cin >> b[i];
    }
    for(int i = 0; i < m; i++)
    {
        dem[b[i]]++;
    }
    for(int i = 0 ; i < n ; i++)
    {
        if(dem[a[i]] > 0)
        {
            cout << a[i] << " ";
        }
        
    }
    return 0;

}