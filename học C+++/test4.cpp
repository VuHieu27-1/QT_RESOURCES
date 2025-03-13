#include <iostream>
using namespace std;
long long a[1000000], dem[10000000];
int main()
{
    long long n;
    cin >> n;
    int maxx = INT_MIN;
    int res;
    for (int i = 0; i < n; i++)
    {
        cin >> a[i];
        if (a[i] > maxx)
        {
            maxx = a[i];
        }
    }
    for (int i = 0; i < 1000; i++)
    {
        dem[i] = 0;
    }
    for (int i = 0; i < n; i++)
    {
        dem[a[i]]++;
    }
    for (int i = 0; i < n; i++)
    {
        if (dem[a[i]] > 1)
        {
            cout << a[i] << " ";
            dem[a[i]] = 0;
        }
    }

    return 0;
}