#include <iostream>
#include <string>
#include <vector>
#include <utility>
using namespace std;

class Suffix
{
public:
    int index;
    int rank[2];
    Suffix()
    {
    }
    bool operator<(Suffix &b)
    {
        return (this->rank[0] == b.rank[0]) ? (this->rank[1] < b.rank[1] ? true : false) : (this->rank[0] < b.rank[0] ? true : false);
    }
};

int partition(vector<Suffix> &suffixes, int low, int high);
void sort(vector<Suffix> &suffixes, int low, int high);
vector<int> buildSuffixArray(string txt);

int main()
{
    string txt;
    getline(cin, txt);
    int origlen = txt.size();
    txt += txt;
    vector<int> suffixvec = buildSuffixArray(txt);

    for (int i = 0; i < suffixvec.size(); i++)
    {
        if (suffixvec[i] < origlen)
        {
            cout << txt.substr(suffixvec[i], origlen) << endl;
            break;
        }
    }
    return 0;
}

int partition(vector<Suffix> &suffixes, int low, int high)
{
    Suffix *pivot = &suffixes[high];
    int i = low - 1;
    for (int j = low; j < high; j++)
    {
        if (suffixes[j] < *pivot)
        {
            i++;
            swap(suffixes[i], suffixes[j]);
        }
    }
    swap(suffixes[i + 1], suffixes[high]);
    return i + 1;
}

void sort(vector<Suffix> &suffixes, int low, int high)
{
    if (low < high)
    {
        int pivot = partition(suffixes, low, high);
        sort(suffixes, low, pivot - 1);
        sort(suffixes, pivot + 1, high);
    }
}

vector<int> buildSuffixArray(string txt)
{
    int n = txt.size();
    vector<Suffix> suffixes(n, Suffix());

    for (int i = 0; i < n; i++)
    {
        suffixes[i].index = i;
        if (txt[i] >= 'A' && txt[i] <= 'Z')
        {
            suffixes[i].rank[0] = txt[i] - 'A';
            suffixes[i].rank[1] = ((i + 1) < n) ? (txt[i + 1] - 'A') : -1;
        }
        else if (txt[i] >= 'a' && txt[i] <= 'z')
        {
            suffixes[i].rank[0] = txt[i] - 'a';
            suffixes[i].rank[1] = ((i + 1) < n) ? (txt[i + 1] - 'a') : -1;
        }
        else
        {
            suffixes[i].rank[0] = txt[i] - '0';
            suffixes[i].rank[1] = ((i + 1) < n) ? (txt[i + 1] - '0') : -1;
        }
    }
    sort(suffixes, 0, n - 1);

    vector<int> ind(n);
    for (int j = 4; j < 2 * n; j = j * 2)
    {
        int rank = 0;
        int prev_rank = suffixes[0].rank[0];
        suffixes[0].rank[0] = rank;
        ind[suffixes[0].index] = 0;

        for (int i = 1; i < n; i++)
        {
            if (suffixes[i].rank[0] == prev_rank && suffixes[i].rank[1] == suffixes[i - 1].rank[1])
            {
                prev_rank = suffixes[i].rank[0];
                suffixes[i].rank[0] = rank;
            }
            else
            {
                prev_rank = suffixes[i].rank[0];
                suffixes[i].rank[0] = ++rank;
            }
            ind[suffixes[i].index] = i;
        }

        for (int i = 0; i < n; i++)
        {
            int nextindex = suffixes[i].index + j / 2;
            suffixes[i].rank[1] = (nextindex < n) ? suffixes[ind[nextindex]].rank[0] : -1;
        }
        sort(suffixes, 0, n - 1);
    }

    vector<int> suffixvec(n);
    for (int i = 0; i < n; i++)
        suffixvec[i] = suffixes[i].index;

    return suffixvec;
}