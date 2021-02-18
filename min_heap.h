#if !defined(MIN_HEAP_H)
#define MIN_HEAP_H

#include <vector>

struct Element {
    int ID;
    int key;
};

class MinHeap {
private:
    int capacity; // Max size
    int size; // Current size
    std::vector<int> IDs; // Contains the IDs of elements
    std::vector<int> keys; // Contains the keys of elements
    std::vector<int> indices; // Map data ID to index

    int MinIndex(int a, int b);
    int Parent(int idx);
    int Left(int idx);
    int Right(int idx);

    // Place element at position index and let the heap bubble up
    void BubbleUp(Element e, int index);
    // Place element at position index and let the heap sift down
    void SiftDown(Element e, int index);
    // Returns the index of the smaller child of heap[index]
    int MinChild(int index);

public:
    // Empty heap with specified capacity
    MinHeap(int capacity);
    // Intialize heap from existing array
    MinHeap(std::vector<Element> arr);
    ~MinHeap();

    int Size();

    void Insert(Element el);
    void IncreaseKey(int ID, int newKey);
    void DecreaseKey(int ID, int newKey);
    // Return -1 if no elements
    int DeleteMin();
    // Returns the index of the element, -1 if not found
    int Index(int ID);

    friend std::ostream& operator<<(std::ostream& out, const MinHeap& h);
};
int min(int a, int b);

#endif // MIN_HEAP_H
