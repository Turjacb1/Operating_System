#include <iostream>
using namespace std;

int n, m;
int alloc[10][10], maxaR[10][10], need[10][10], avail[10];

bool isSafe()
{
    int work[10];
    bool finish[10] = {false};
    int safeSeq[10];
    int count = 0;

    for(int i=0;i<m;i++)
        work[i]=avail[i];

    while(count<n)
    {
        bool found=false;

        for(int i=0;i<n;i++)
        {
            if(!finish[i])
            {
                bool ok=true;

                for(int j=0;j<m;j++)
                {
                    if(need[i][j]>work[j])
                    {
                        ok=false;
                        break;
                    }
                }

                if(ok)
                {
                    for(int j=0;j<m;j++)
                        work[j]+=alloc[i][j];

                    safeSeq[count++]=i;
                    finish[i]=true;
                    found=true;
                }
            }
        }

        if(!found)
            break;
    }

    if(count<n)
    {
        cout<<"\nSystem is NOT in Safe State.\n";
        return false;
    }

    cout<<"\nSystem is in Safe State.\n";
    cout<<"Safe Sequence: ";

    for(int i=0;i<n;i++)
        cout<<"P"<<safeSeq[i]<<" ";

    cout<<endl;

    return true;
}

int main()
{
    cout<<"Enter number of processes: ";
    cin>>n;

    cout<<"Enter number of resources: ";
    cin>>m;

    cout<<"\nEnter Allocation Matrix:\n";

    for(int i=0;i<n;i++)
    {
        cout<<"P"<<i<<": ";
        for(int j=0;j<m;j++)
            cin>>alloc[i][j];
    }

    cout<<"\nEnter Maximum Matrix:\n";

    for(int i=0;i<n;i++)
    {
        cout<<"P"<<i<<": ";
        for(int j=0;j<m;j++)
            cin>>maxaR[i][j];
    }

    cout<<"\nEnter Available Resources:\n";

    for(int i=0;i<m;i++)
        cin>>avail[i];

    cout<<"\nNeed Matrix:\n";

    for(int i=0;i<n;i++)
    {
        cout<<"P"<<i<<": ";
        for(int j=0;j<m;j++)
        {
            need[i][j]=maxaR[i][j]-alloc[i][j];
            cout<<need[i][j]<<" ";
        }
        cout<<endl;
    }

    isSafe();

    return 0;
}