#include <iostream>
#include <iomanip>
#include "min_heap.h"

MinHeap::MinHeap(int capacity) {
    this->capacity = capacity;
    this->size = 0;
    this->IDs = std::vector<int>(capacity);
    this->keys = std::vector<int>(capacity);
    this->indices = std::vector<int>(capacity);
}

MinHeap::MinHeap(std::vector<Element> arr) {
    this->capacity = this->size = arr.size();
    this->IDs = std::vector<int>(capacity);
    this->keys = std::vector<int>(capacity);
    this->indices = std::vector<int>(capacity);
    for (int i = 0; i < this->capacity; i++) {
        Element el = arr[i];
        this->IDs[i] = el.ID;
        this->keys[i] = el.key;
        this->indices[el.ID] = i;
    }
    for (int i = arr.size() - 1; i >= 0; i--) {
        this->SiftDown(Element{ this->IDs[i], this->keys[i] }, i);
    }
}

MinHeap::~MinHeap() {}

int MinHeap::Size() {
    return this->size;
}


void MinHeap::Insert(Element el) {
    if (this->size == this->capacity) {
        // Full
        std::cout << "error: cannot insert; heap is full" << std::endl;
        return;
    }
    this->size++;
    this->BubbleUp(el, this->size - 1);
}

void MinHeap::IncreaseKey(int ID, int newKey) {
    // Don't do anything if ID was popped
    if (this->Index(ID) == -1)
        return;
    this->SiftDown(Element{ ID, newKey }, this->Index(ID));
}

void MinHeap::DecreaseKey(int ID, int newKey) {
    // Don't do anything if ID was popped
    if (this->Index(ID) == -1)
        return;
    this->BubbleUp(Element{ ID, newKey }, this->Index(ID));
}

int MinHeap::DeleteMin() {
    if (this->size != 0) {
        Element e = Element{ this->IDs[0], this->keys[0] };
        this->size--;
        this->SiftDown(Element{ this->IDs[this->size], this->keys[this->size] }, 0);
        // Set index to -1
        this->indices[e.ID] = -1;
        return e.ID;
    }
    // Empty
    std::cout << "error: cannot delete min; heap is empty" << std::endl;
    return -1;
}

void MinHeap::BubbleUp(Element e, int index) {
    int p = this->Parent(index);
    while (index > 0 && this->keys[p] > e.key) {
        int pid = this->IDs[p];
        this->IDs[index] = pid;
        this->keys[index] = this->keys[p];
        this->indices[pid] = index;
        index = p;
        p = this->Parent(index);
    }
    this->IDs[index] = e.ID;
    this->keys[index] = e.key;
    this->indices[e.ID] = index;
}

void MinHeap::SiftDown(Element e, int index) {
    int c = this->MinChild(index);
    while (c > 0 && this->keys[c] < e.key) {
        int cid = this->IDs[c];
        this->IDs[index] = cid;
        this->keys[index] = this->keys[c];
        this->indices[cid] = index;
        index = c;
        c = this->MinChild(index);
    }
    this->IDs[index] = e.ID;
    this->keys[index] = e.key;
    this->indices[e.ID] = index;
}

int MinHeap::MinChild(int index) {
    if (2 * index + 1 < this->size) {
        return this->MinIndex(this->Left(index), min(this->size - 1, this->Right(index)));
    }
    return 0;
}

int MinHeap::Index(int ID) {
    if (ID < this->capacity) {
        return this->indices[ID];
    }
    return -1;
}

int MinHeap::MinIndex(int a, int b) {
    return this->keys[a] < this->keys[b] ? a : b;
}

int MinHeap::Parent(int idx) {
    return (idx - 1) / 2;
}

int MinHeap::Left(int idx) {
    return 2 * idx + 1;
}

int MinHeap::Right(int idx) {
    return 2 * idx + 2;
}

std::ostream& operator<<(std::ostream& out, const MinHeap& h) {
    out.setf(std::ios::left);
    out << std::setw(10) << "i";
    for (int i = 0; i < h.capacity; i++) {
        out << std::setw(4) << i << " ";
    }
    out << "\n" << std::string(10 + h.capacity * 5, '-') << "\n";
    out << std::setw(10) << "IDs: ";
    for (int i = 0; i < h.size; i++) {
        out << std::setw(4) << h.IDs[i] << " ";
    }
    out << "\n";
    out << std::setw(10) << "Keys: ";
    for (int i = 0; i < h.size; i++) {
        out << std::setw(4) << h.keys[i] << " ";
    }
    out << "\n";
    out << std::setw(10) << "Indices: ";
    for (int i = 0; i < h.capacity; i++) {
        out << std::setw(4) << h.indices[i] << " ";
    }
    out << "\n" << std::string(10 + h.capacity * 5, '-') << "\n";
    out << std::setw(10) << "i";
    for (int i = 0; i < h.capacity; i++) {
        out << std::setw(4) << i << " ";
    }
    out << "\n";
    return out;
}

int min(int a, int b) {
    return a < b ? a : b;
}