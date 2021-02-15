#if !defined(BINARY_HEAP_H)
#define BINARY_HEAP_H

#include <vector>

class BinaryHeap {
private:
    int* heap;
    int capacity;
    int size;

    int MinIndex(int a, int b);
    int Parent(int idx);
    int Left(int idx);
    int Right(int idx);

public:
    // Empty heap with specified capacity
    BinaryHeap(int capacity);
    // Intialize heap from existing array
    BinaryHeap(std::vector<int> arr);
    ~BinaryHeap();

    void Insert(int element);
    void DecreaseKey(int element);
    int DeleteMin();
    // Place element at position index and let the heap bubble up
    void BubbleUp(int element, int index);
    // Place element at position index and let the heap sift down
    void SiftDown(int element, int index);
    // Returns the index of the smaller child of heap[index]
    int MinChild(int index);
    // Returns the index of the element, -1 if not found
    int Index(int element);

    friend std::ostream& operator<<(std::ostream& out, const BinaryHeap& h);
};
int min(int a, int b);

#endif // BINARY_HEAP_H
