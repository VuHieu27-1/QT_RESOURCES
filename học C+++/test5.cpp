#include <iostream>
using namespace std;
long long a[1000000], dem[10000000], b[1000000];
int main()
{
    long long n, m;
    cin >> n >> m;
    int maxx = INT_MIN;
    int dem_1 = 0;
    for (int i = 0; i < n; i++)
    {
        cin >> a[i];
    }
    for (int i = 0; i < m; i++)
    {
        cin >> b[i];
    }
    for (int i = 0; i < m; i++)
    {
        dem[b[i]]++;
    }
    for (int i = 0; i < n; i++)
    {
        if (dem[a[i]] >= 1)
        {
            cout << a[i] << " ";
            dem_1++;
        }
    }
    cout << endl;
    cout << dem_1 << endl;
    return 0;
}