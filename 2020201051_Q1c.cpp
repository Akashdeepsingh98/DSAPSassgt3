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
string revString(string &s);
vector<int> kasai(string txt, vector<int> &suffixvec);
void printVec(vector<int> &v);
string getLongPal(string &txt, vector<int> &suffixvec, vector<int> &lcpvec, int origlen);

int main()
{
    string txt;
    getline(cin, txt);
    int origlen = txt.size();
    txt += "#" + revString(txt);
    vector<int> suffixvec = buildSuffixArray(txt);
    vector<int> lcpvec = kasai(txt, suffixvec);
    string longestpalin = getLongPal(txt, suffixvec, lcpvec, origlen);
    cout << longestpalin << endl;
    return 0;
}

string getLongPal(string &txt, vector<int> &suffixvec, vector<int> &lcpvec, int origlen)
{
    int longestlen = 0;
    int index = 0;
    for (int i = 1; i < lcpvec.size(); i++)
    {
        if (lcpvec[i] > longestlen)
        {
            if ((suffixvec[i - 1] < origlen && suffixvec[i] > origlen) ||
                (suffixvec[i] < origlen && suffixvec[i - 1] > origlen))
            {
                longestlen = lcpvec[i];
                index = suffixvec[i];
            }
        }
    }
    return txt.substr(index, longestlen);
}

void printVec(vector<int> &v)
{
    for (int i = 0; i < v.size(); i++)
    {
        cout << v[i] << " ";
    }
    cout << endl;
}

vector<int> kasai(string txt, vector<int> &suffixvec)
{
    int n = suffixvec.size();
    vector<int> lcpvec(n + 1, 0);
    vector<int> invSuff(n, 0);
    for (int i = 0; i < n; i++)
    {
        invSuff[suffixvec[i]] = i;
    }
    int k = 0;

    for (int i = 0; i < n; i++)
    {
        if (invSuff[i] == n - 1)
        {
            k = 0;
            continue;
        }

        int j = suffixvec[invSuff[i] + 1];
        while (i + k < n && j + k < n && txt[i + k] == txt[j + k])
        {
            k++;
        }

        lcpvec[invSuff[i] + 1] = k;
        if (k > 0)
        {
            k--;
        }
    }
    lcpvec[0] = lcpvec.back();
    lcpvec.pop_back();
    return lcpvec;
}

string revString(string &s)
{
    string result = "";
    for (int i = s.size() - 1; i >= 0; i--)
    {
        result += s[i];
    }
    return result;
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
        if (txt[i] == '#')
        {
            suffixes[i].rank[0] = 0;
            suffixes[i].rank[1] = 0;
        }
        else if (txt[i] >= 'A' && txt[i] <= 'Z')
        {
            suffixes[i].rank[0] = txt[i] - 'A' + 1;
            suffixes[i].rank[1] = ((i + 1) < n) ? (txt[i + 1] - 'A' + 1) : -1;
        }
        else if (txt[i] >= 'a' && txt[i] <= 'z')
        {
            suffixes[i].rank[0] = txt[i] - 'a' + 1;
            suffixes[i].rank[1] = ((i + 1) < n) ? (txt[i + 1] - 'a' + 1) : -1;
        }
        else
        {
            suffixes[i].rank[0] = txt[i] - '0' + 1;
            suffixes[i].rank[1] = ((i + 1) < n) ? (txt[i + 1] - '0' + 1) : -1;
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