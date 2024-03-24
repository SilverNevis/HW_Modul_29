#include <iostream>
#include <mutex>

struct Node
{
    int value;
    Node* next;
    std::mutex* node_mutex;

    Node(int val) : value(val), next(nullptr), node_mutex(new std::mutex()) {}
    ~Node() {
        delete node_mutex;
    }
};

class FineGrainedQueue
{
private:
    Node* head;
    std::mutex* queue_mutex;

public:
    FineGrainedQueue() : head(nullptr), queue_mutex(new std::mutex()) {}
    ~FineGrainedQueue() {
        delete queue_mutex;
    }

    
    void insertIntoMiddle(int value, int pos) {
        std::lock_guard<std::mutex> queue_lock(*queue_mutex);

        if (head == nullptr) { 
            head = new Node(value);
            return;
        }

        Node* prev = head;

        
        for (int i = 0; i < pos - 1 && prev->next != nullptr; ++i) {
            prev = prev->next;
        }

        std::lock_guard<std::mutex> prev_lock(*prev->node_mutex);

        Node* newNode = new Node(value);

    
        newNode->next = prev->next;
        prev->next = newNode;
    }
};

int main() {
    FineGrainedQueue queue;

    
    queue.insertIntoMiddle(10, 1); 

    return 0;
}


