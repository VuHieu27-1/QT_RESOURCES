#include <iostream>
#include <math.h>
#include <climits>
using namespace std;
int main()
{
    long long a[1000], dem[1000];
    int n, maxarr = INT_MIN;
    int res, max = INT_MIN;
    cin >> n;
    for (int i = 0; i < 1000; i++)
    {
        dem[i] = 0;
    }
    for (int i = 0; i < n; i++)
    {
        cin >> a[i];
        if (a[i] > maxarr)
        {
            maxarr = a[i];
        }
    }
    for (int i = 0; i < n; i++)
    {
        dem[a[i]]++;
    }
    for (int i = 0; i < maxarr; i++)
    {
        if (dem[i] > max)
        {
            max = dem[i];
            res = i;
        }
    }
    cout << res << " " << max << endl;
}