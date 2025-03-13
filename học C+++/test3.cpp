#include <iostream>
using namespace std;
int main()
{
    long long n, a[1000], dem[1000];
    cin >> n;
    int maxx = INT_MIN;
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
    for (int i = 0; i <= maxx; i++)
    {
        if (dem[i] != 0)
        {
            cout << i << " " << dem[i] << endl;
        }
    }

    return 0;
}