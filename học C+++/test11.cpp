#include <iostream>
#include <climits>
#include <map>
#include <algorithm>
using namespace std;
long long a[10000000], b[1000000],res[10000000];
int main()
{
    int n, m, index = 0;
    cin >> n >> m;
    map<long long, long long> dem;
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
    for(int i = 0; i < n; i++)
    {
        if(dem[a[i]] > 1 )
        {
            dem[a[i]] = 0;
            res[index] = a[i];   
            index++;
        }
    }
    sort(res,res+index);
    for (int i = 0; i < index; i++)
    {
        cout<< res[i] << " ";
    }
    return 0;
}