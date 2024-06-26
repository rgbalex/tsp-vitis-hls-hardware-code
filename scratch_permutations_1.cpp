

// C++ implementation of the approach
#include <iostream>
using namespace std;
 
template <typename T>
class AllPermutation {
private:
    // The input array for permutation
    const T* Arr;
 
    // Length of the input array
    const int Length;
 
    // Index array to store indexes of input array
    int* Indexes;
 
    // The index of the first "increase"
    // in the Index array which is the smallest
    // i such that Indexes[i] < Indexes[i + 1]
    int Increase;
 
public:
    // Constructor
    AllPermutation(T* arr, int length)
        : Arr(arr), Length(length)
    {
        this->Indexes = nullptr;
        this->Increase = -1;
    }
 
    // Destructor
    ~AllPermutation()
    {
        if (this->Indexes != nullptr) {
            delete[] this->Indexes;
        }
    }
 
    // Initialize and output
    // the first permutation
    void GetFirst()
    {
 
        // Allocate memory for Indexes array
        this->Indexes = new int[this->Length];
 
        // Initialize the values in Index array
        // from 0 to n - 1
        for (int i = 0; i < this->Length; ++i) {
            this->Indexes[i] = i;
        }
 
        // Set the Increase to 0
        // since Indexes[0] = 0 < Indexes[1] = 1
        this->Increase = 0;
 
        // Output the first permutation
        this->Output();
    }
 
    // Function that returns true if it is
    // possible to generate the next permutation
    bool HasNext()
    {
 
        // When Increase is in the end of the array,
        // it is not possible to have next one
        return this->Increase != (this->Length - 1);
    }
 
    // Output the next permutation
    void GetNext()
    {
 
        // Increase is at the very beginning
        if (this->Increase == 0) {
 
            // Swap Index[0] and Index[1]
            this->Swap(this->Increase, this->Increase + 1);
 
            // Update Increase
            this->Increase += 1;
            while (this->Increase < this->Length - 1
                   && this->Indexes[this->Increase]
                          > this->Indexes[this->Increase + 1]) {
                ++this->Increase;
            }
        }
        else {
 
            // Value at Indexes[Increase + 1] is greater than Indexes[0]
            // no need for binary search,
            // just swap Indexes[Increase + 1] and Indexes[0]
            if (this->Indexes[this->Increase + 1] > this->Indexes[0]) {
                this->Swap(this->Increase + 1, 0);
            }
            else {
 
                // Binary search to find the greatest value
                // which is less than Indexes[Increase + 1]
                int start = 0;
                int end = this->Increase;
                int mid = (start + end) / 2;
                int tVal = this->Indexes[this->Increase + 1];
                while (!(this->Indexes[mid] < tVal
                         && this->Indexes[mid - 1] > tVal)) {
                    if (this->Indexes[mid] < tVal) {
                        end = mid - 1;
                    }
                    else {
                        start = mid + 1;
                    }
                    mid = (start + end) / 2;
                }
 
                // Swap
                this->Swap(this->Increase + 1, mid);
            }
 
            // Invert 0 to Increase
            for (int i = 0; i <= this->Increase / 2; ++i) {
                this->Swap(i, this->Increase - i);
            }
 
            // Reset Increase
            this->Increase = 0;
        }
        this->Output();
    }
 
private:
    // Function to output the input array
    void Output()
    {
        cout << "0 ";
        for (int i = 0; i < this->Length; ++i) {
 
            // Indexes of the input array
            // are at the Indexes array
            cout << (this->Arr[this->Indexes[i]])+1 << " ";
        }
        cout << "0" << endl;
    }
 
    // Swap two values in the Indexes array
    void Swap(int p, int q)
    {
        int tmp = this->Indexes[p];
        this->Indexes[p] = this->Indexes[q];
        this->Indexes[q] = tmp;
    }
};
 
// Driver code
int main()
{
    int arr[] = { 0, 1, 2 };
    AllPermutation<int> perm(arr, sizeof(arr) / sizeof(int));
    perm.GetFirst();
    while (perm.HasNext()) {
        perm.GetNext();
    }
 
    return 0;
}
