#include <iostream>
#include <climits>
#include <map>
#include <algorithm>
using namespace std;
long long a[10000000], b[1000000],res_1[10000000],c[10000000],res_2[10000000], tim[1000000];
int main()
{
    int n, m, x, index_1 = 0,index_2 = 0,so =0;
    cin >> n >> m >> x;
    map<long long, long long> dem;
    map<long long, long long> dem_1;
    for(int i = 0; i < n; i++)
    {
        cin >> a[i];
    }
    for(int i = 0; i < m; i++)
    {
        cin >> b[i];
    }
    for(int i = 0; i < x; i++)
    {
        cin >> c[i];
    }
    for(int i = 0; i < n; i++)
    {
        dem[a[i]]++;
    }
    for (int i = 0; i < m; i++)
    {
        if(dem[b[i]] > 0)
        {
            res_1[index_1] = b[i];
            index_1 ++;
        }
    }
    for (int i = 0; i < x; i++)
    {
        if(dem[c[i]] > 0)
        {
            res_2[index_2] = c[i];
            index_2 ++;
        }
    }
    for (int i = 0; i < index_1; i++)
    {
        dem_1[res_1[i]] ++;   
    }
    for(int i = 0; i < index_2; i++)
    {
        if(dem_1[res_2[i]] > 0)
        {
            tim[so] = res_2[i];
            so++;
        }
    }
    sort(tim,tim + so);
    for(int i =0; i< so ; i++ )
    {
        cout << tim[i] << " ";
    }
    return 0;
}