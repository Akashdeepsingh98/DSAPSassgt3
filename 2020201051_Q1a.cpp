#include <iostream>
#include <vector>
#include <string>
#include <utility>
using namespace std;

class SuffixArray
{
private:
    int partition(int low, int high)
    {
        string pivot = arr[high];
        int i = low - 1;
        for (int j = low; j < high; j++)
        {
            if (arr[j] < pivot)
            {
                i++;
                swap(arr[i], arr[j]);
            }
        }
        swap(arr[i + 1], arr[high]);
        return i + 1;
    }
    void sort(int low, int high)
    {
        if (low < high)
        {
            int pivot = partition(low, high);
            sort(low, pivot - 1);
            sort(pivot + 1, high);
        }
    }

public:
    vector<string> arr;
    int origlen;
    SuffixArray(string str)
    {
        this->origlen = str.size();
        str += str;

        for (int i = 1; i < 2 * this->origlen; i++)
        {
            this->arr.push_back(str);
            str = str.substr(1);
        }
        this->arr.push_back(str);
        sort(0, 2 * origlen - 1);
    }

    void printSA()
    {
        for (int i = 0; i < arr.size(); i++)
        {
            cout << arr[i] << endl;
        }
    }

    string getMinLexRot()
    {
        
    }
};

int main()
{
    SuffixArray sa("dcaba");
    
}