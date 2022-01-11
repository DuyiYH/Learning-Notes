#include<iostream>
#include<vector>
#include<algorithm>
using namespace std;

class solution
{
public:
    void quickSort(vector<int>& vec, int left, int right)
    {
        if(left >= right) return;
        int idx = partSort(vec, left, right);
        quickSort(vec, left, idx - 1);
        quickSort(vec, idx + 1, right);
    }

    int partSort(vector<int>& vec, int left, int right)
    {   
        //if(left >= right) return left;
        int key = vec[right];
        int i = left; int j = right - 1;
        while (i < j)
        {
            while(i < j && vec[i] <= key)
            {
                ++i;
            }
            while(i < j && vec[j] >= key)
            {
                --j;
            }
            swap(vec[i], vec[j]);
        }
        swap(vec[i], vec[right]);
        return i;
    }
};


int main()
{
    vector<int> vec{3, 4, 5, 9, 8, 7, 5, 8, 2, 9, 4, 1, 6, 6};
    solution s;
    s.quickSort(vec, 0, vec.size() - 1);
    for(int i : vec)
    {
        cout << i << " ";
    }

    system("pause");



    return 0;
}