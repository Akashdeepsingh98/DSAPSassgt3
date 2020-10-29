#include <iostream>
#include <utility>
#include <climits>
using namespace std;

struct MinHeapNode
{
    long long int val;
    long long int index;
};

class MinHeap
{
private:
public:
    MinHeapNode *heaparr;
    long long int heap_size;

    MinHeap(MinHeapNode a[], long long int size)
    {
        heap_size = size;
        heaparr = a;
        long long int i = (heap_size - 1) / 2;
        while (i >= 0)
        {
            MinHeapify(i);
            i--;
        }
    }

    void MinHeapify(long long int i)
    {
        long long int l = left(i);
        long long int r = right(i);
        long long int smallest = i;
        if (l < heap_size && heaparr[l].val < heaparr[i].val)
        {
            smallest = l;
        }
        if (r < heap_size && heaparr[r].val < heaparr[smallest].val)
        {
            smallest = r;
        }

        if (smallest != i)
        {
            swap(heaparr[i], heaparr[smallest]);
            MinHeapify(smallest);
        }
    }

    long long int left(long long int i)
    {
        return (2 * i + 1);
    }

    long long int right(long long int i)
    {
        return (2 * i + 2);
    }

    MinHeapNode getMin()
    {
        return heaparr[0];
    }

    void replaceMin(MinHeapNode x)
    {
        heaparr[0] = x;
        MinHeapify(0);
    }
};

void externalSort(char *input_file, char *output_file, int part_size);
void mergeArray(long long int arr[], long long int low, long long int mid, long long int high);
void mergeSort(long long int arr[], long long int low, long long int high);
void mergeFiles(char *output_file, long long int part_size, long long int num_parts);
long long int makeParts(char *input_file, long long int part_size);

int main(int argc, char *argv[])
{
    int part_size = 100000;
    if (argc != 3)
    {
        printf("Give 3 arguments\n");
        exit(1);
    }
    externalSort(argv[1], argv[2], part_size);
    return 0;
}

void externalSort(char *input_file, char *output_file, int part_size)
{
    long long int num_parts = makeParts(input_file, part_size);
    mergeFiles(output_file, part_size, num_parts);
}

void mergeFiles(char *output_file, long long int part_size, long long int num_parts)
{
    FILE *in[num_parts];
    for (long long int i = 0; i < num_parts; i++)
    {
        string filename = to_string(i) + ".txt";
        in[i] = fopen(filename.c_str(), "r");
    }

    MinHeapNode *heaparr = new MinHeapNode[num_parts];
    long long int i;
    for (i = 0; i < num_parts; i++)
    {
        if (fscanf(in[i], "%lld ", &heaparr[i].val) != 1)
        {
            break;
        }
        heaparr[i].index = i;
    }
    MinHeap hp(heaparr, i);
    long long int count = 0;
    FILE *out = fopen(output_file, "w");
    while (count != i)
    {
        MinHeapNode root = hp.getMin();
        fprintf(out, "%lld ", root.val);

        if (fscanf(in[root.index], "%lld ", &root.val) != 1)
        {
            root.val = LONG_LONG_MAX;
            count++;
        }

        hp.replaceMin(root);
    }
    fclose(out);
    ;
    for (int i = 0; i < num_parts; i++)
    {
        fclose(in[i]);
    }
}

void mergeArray(long long int arr[], long long int low, long long int mid, long long int high)
{
    long long int lind, rind, arrind;
    long long int llen = mid - low + 1;
    long long int rlen = high - mid;

    long long int L[llen], R[rlen];
    for (lind = 0; lind < llen; lind++)
    {
        L[lind] = arr[low + lind];
    }
    for (rind = 0; rind < rlen; rind++)
    {
        R[rind] = arr[mid + 1 + rind];
    }
    lind = rind = 0;
    arrind = low;
    while (lind < llen && rind < rlen)
    {
        if (L[lind] <= R[rind])
        {
            arr[arrind++] = L[lind++];
        }
        else
        {
            arr[arrind++] = R[rind++];
        }
    }
    while (lind < llen)
    {
        arr[arrind++] = L[lind++];
    }
    while (rind < rlen)
    {
        arr[arrind++] = R[rind++];
    }
}

void mergeSort(long long int arr[], long long int low, long long int high)
{
    if (low < high)
    {
        long long int mid = low + (high - low) / 2;
        mergeSort(arr, low, mid);
        mergeSort(arr, mid + 1, high);
        mergeArray(arr, low, mid, high);
    }
}

long long int makeParts(char *input_file, long long int part_size)
{
    long long int num_parts, num_ele = 0;
    {
        FILE *in = fopen(input_file, "r");
        bool still_reading = true;
        while (still_reading)
        {
            long long int temp;
            if (fscanf(in, "%lld,", &temp) != 1)
            {
                still_reading = false;
                fclose(in);
            }
            else
            {
                num_ele++;
            }
        }
    }
    num_parts = num_ele / part_size;

    FILE *in = fopen(input_file, "r");
    FILE *out[num_parts];
    for (long long int i = 0; i < num_parts; i++)
    {
        string filename = to_string(i)+".txt";
        out[i] = fopen(filename.c_str(), "w");
    }

    long long int *arr = (long long int *)malloc(part_size * sizeof(long long int));

    long long int i;
    bool still_reading = true;
    int next_ofile = 0;
    while (still_reading)
    {
        for (i = 0; i < part_size; i++)
        {
            if (fscanf(in, "%lld,", &arr[i]) != 1)
            {
                still_reading = false;
                fclose(in);
                break;
            }
        }

        mergeSort(arr, 0, i - 1);
        for (int j = 0; j < i; j++)
        {
            fprintf(out[next_ofile], "%lld ", arr[j]);
        }
        next_ofile++;
    }

    for (int i = 0; i < num_parts; i++)
    {
        fclose(out[i]);
    }

    return num_parts;
}