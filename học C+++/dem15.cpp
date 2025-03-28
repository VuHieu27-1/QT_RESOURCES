#include <iostream>
#include <climits>
#include <map>
using namespace std;
long long a[1000000];

int main()
{
    ios_base::sync_with_stdio(false);
    cin.tie(NULL); cout.tie(NULL);
    long long n;
    long long tong = 0; 
    map<long long , long long> dem;
    cin >> n ;
    for(int i = 0; i < n; i++)
    {
        cin >> a[i];
    }
    for(int i = 0; i < n; i++)
    {
        dem[a[i]] ++;
    }
    for(int i = 0 ; i < n; i++)
    {
        if(dem[a[i]] > 0 && dem[2*a[i]] > 0)
        {       
                if(a[i] == 2*a[i])
                {
                    tong += (dem[a[i]] - 1)*dem[a[i]]/2;
                    dem[a[i]] = 0;
                }else{
                tong += dem[a[i]]*dem[2*a[i]];
                dem[a[i]] = 0;
                dem[2*a[i]] = 0;
                }
        }
    }
    cout << tong << endl;
    return 0;
}