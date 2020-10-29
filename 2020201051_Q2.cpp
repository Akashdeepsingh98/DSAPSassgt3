#include <iostream>
#include <vector>
using namespace std;

class Trienode
{
public:
    Trienode *left;
    Trienode *right;
    Trienode()
    {
        this->left = this->right = nullptr;
    }
};

void prepare_Trie(vector<unsigned long int> &v, Trienode *root);
void insert_In_Trie(Trienode *root, unsigned long int x);
unsigned long int findMaxXor(Trienode *root, unsigned long int x, int bitpos);

int main()
{
    int N, q;
    cin >> N >> q;
    vector<unsigned long int> v(N);
    for (int i = 0; i < N; i++)
    {
        cin >> v[i];
    }

    Trienode *root = new Trienode();
    prepare_Trie(v, root);

    vector<unsigned long int> queries(q);
    for (int i = 0; i < q; i++)
    {
        cin >> queries[i];
    }

    for (int i = 0; i < q; i++)
    {
        cout << findMaxXor(root, queries[i], (sizeof(unsigned long int) * 8) - 1) << endl;
    }
}

void prepare_Trie(vector<unsigned long int> &v, Trienode *root)
{
    for (int i = 0; i < v.size(); i++)
    {
        insert_In_Trie(root, v[i]);
    }
}

void insert_In_Trie(Trienode *root, unsigned long int x)
{
    cout << x << endl;
    int num_bits = sizeof(unsigned long int) * 8;
    cout << num_bits << endl;
    for (int i = num_bits - 1; i >= 0; i--)
    {
        unsigned int bitVal = (x & (1 << i));
        cout << bitVal;
        if (bitVal)
        {
            if (root->right == nullptr)
            {
                root->right = new Trienode();
            }
            root = root->right;
        }
        else
        {
            if (root->left == nullptr)
            {
                root->left = new Trienode();
            }
            root = root->left;
        }
    }
    cout << endl;
}

unsigned long int findMaxXor(Trienode *root, unsigned long int x, int bitpos)
{
    if (root == nullptr || bitpos < 0)
    {
        return 0;
    }
    if (bitpos == 0)
    {
        if (x & 1)
        {
            return (root->left) ? 1 : 0;
        }
        else
        {
            return (root->right) ? 1 : 0;
        }
    }

    unsigned int bitval = 0;
    unsigned long int number = 0;
    if ((x & (1 << bitpos)))
    {
        if (root->right)
        {
            number = findMaxXor(root->right, x, bitpos - 1);
            bitval = 1;
        }
        else
        {
            number = findMaxXor(root->left, x, bitpos - 1);
            bitval = 0;
        }
    }
    else
    {
        if (root->left)
        {
            number = findMaxXor(root->left, x, bitpos - 1);
            bitval = 1;
        }
        else
        {
            number = findMaxXor(root->right, x, bitpos - 1);
            bitval = 0;
        }
    }

    if (bitval)
    {
        number = number | (1 << bitpos);
    }
    else
    {
        number = number & ~(1 << bitpos);
    }
    return number;
}