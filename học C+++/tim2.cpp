#include <iostream>
#include <climits>
#include <vector>
using namespace std;
int main()
{
    int n, x;
    cin >> n >> x;
    vector<int> a(n);
    int index = 0;
    int max = INT_MIN;
    for (int i = 0; i < n; i++)
    {
        cin >> a[i];
    }
    int l = 0, r = n - 1, mid;
    int res;
    while(l <= r)
    {
        mid = (l + r) / 2;
        if(a[mid] == x)
        {
            res = mid;
            r = mid - 1;
            if(res > max)
            {
                max = res;
            }
        }else if (a[mid] > x)
        {
            r = mid - 1; 
        }else 
        {
            l = mid + 1;
        }
        cout << res << endl;
    }
    cout << max;
    return 0;
}