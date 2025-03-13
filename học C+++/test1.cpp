#include <iostream>
#include <math.h>
#include <climits>
using namespace std;
int main()
{

    int n, a[100];
    cin >> n;
    int dem = 0;
    int dem_1 = 0;
    int dem_2 = 0;
    int max = INT_MIN;
    for (int i = 0; i < n; i++)
    {
        cin >> a[i];
    }
    for (int i = 0; i < n; i++)
    {
        if (a[i] == 1)
        {
            dem++;
        }
        else if (a[i] == 2)
        {
            dem_1++;
        }
        else if (a[i] == 3)
        {
            dem_2++;
        }
    }
    if (dem > max)
    {
        max = dem;
    }
    if (dem_1 > max)
    {
        max = dem_1;
    }
    if (dem_2 > max)
    {
        max = dem_2;
    }
    if (dem == max)
    {
        cout << 1 << " ";
        cout << max << endl;
        return 0;
    }
    if (dem_1 == max)
    {
        cout << 2 << " ";
        cout << max << endl;
        return 0;
    }
    if (dem_2 == max)
    {
        cout << 3 << " ";
        cout << max << endl;
        return 0;
    }
    return 0;
}