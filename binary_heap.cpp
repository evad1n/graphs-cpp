#include <iostream>
#include "binary_heap.h"

BinaryHeap::BinaryHeap(int capacity) {
    this->heap = new int[capacity];
    this->capacity = capacity;
    this->size = 0;
}

BinaryHeap::BinaryHeap(std::vector<int> arr) {
    this->heap = new int[arr.size()];
    this->capacity = arr.size();
    this->size = arr.size();
    for (auto el : arr) {
        this->heap[this->size + 1] = el;
    }
    for (int i = arr.size(); i > 0; i--) {
        this->SiftDown(this->heap[i], i);
    }
}

BinaryHeap::~BinaryHeap() {
    delete this->heap;
}

void BinaryHeap::Insert(int element) {
    if (this->size == this->capacity) {
        // Full
        std::cout << "error: cannot insert; heap is full" << std::endl;
        return;
    }
    this->size++;
    this->BubbleUp(element, this->size - 1);
}

void BinaryHeap::DecreaseKey(int element) {
    this->BubbleUp(element, this->Index(element));
}

int BinaryHeap::DeleteMin() {
    if (this->size != 0) {
        int x = this->heap[0];
        this->size--;
        this->SiftDown(this->heap[this->size], 0);
        return x;
    }
    return __INT32_MAX__;
}

void BinaryHeap::BubbleUp(int element, int index) {
    int p = this->Parent(index);
    while (index > 0 && this->heap[p] > element) {
        this->heap[index] = this->heap[p];
        index = p;
        p = this->Parent(index);
    }
    this->heap[index] = element;
}

void BinaryHeap::SiftDown(int element, int index) {
    int c = this->MinChild(index);
    while (c > 0 && this->heap[c] < element) {
        this->heap[index] = this->heap[c];
        index = c;
        c = this->MinChild(index);
    }
    this->heap[index] = element;
}

int BinaryHeap::MinChild(int index) {
    if (2 * index < this->size) {
        return MinIndex(this->Left(index), min(this->size - 1, this->Right(index)));
    }
    return 0;
}

int BinaryHeap::Index(int element) {
    for (int i = 0; i < this->size; i++) {
        if (this->heap[i] == element) {
            return i;
        }
    }
    return -1;
}


int BinaryHeap::MinIndex(int a, int b) {
    return this->heap[a] < this->heap[b] ? a : b;
}

int BinaryHeap::Parent(int idx) {
    return (idx - 1) / 2;
}

int BinaryHeap::Left(int idx) {
    return 2 * idx + 1;
}

int BinaryHeap::Right(int idx) {
    return 2 * idx + 2;
}

std::ostream& operator<<(std::ostream& out, const BinaryHeap& h) {
    for (int i = 0; i < h.size; i++) {
        out << h.heap[i] << " ";
    }
    return out;
}

int min(int a, int b) {
    return a < b ? a : b;
}