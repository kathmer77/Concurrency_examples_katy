#include<iostream>
#include<memory>
#include<condition_variable>
#include<thread>


class thread_safe_queue_linked_list{

    struct node{
        int data_queue;
        std::unique_ptr<node> ptr_next_element;

        node(int _data_queue): data_queue(_data_queue), ptr_next_element(nullptr) {}
    };

    std::unique_ptr<node> head_ptr;
    node* tail_ptr;

    std::mutex head_mutex;
    std::mutex tail_mutex;

    public:
    thread_safe_queue_linked_list(): head_ptr(std::make_unique<node>(0)), tail_ptr(head_ptr.get()) {}


    void push_function(int value){

        std::unique_ptr<node> new_node_created;
        new_node_created = std::make_unique<node>(0);
        node* const new_tail_ptr = new_node_created.get();

        { 
        std::lock_guard<std::mutex> lock_t(tail_mutex); 
        tail_ptr->data_queue = value;

        tail_ptr->ptr_next_element = std::move(new_node_created);
        tail_ptr = new_tail_ptr; 
        }
    }

    std::unique_ptr<int> pop_function(){


        std::lock_guard<std::mutex> lock_h(head_mutex); 
        if (head_ptr->ptr_next_element == nullptr){ 
            return std::make_unique<int>(0);    
        }
        else{
            std::unique_ptr<int> result;
            result = std::make_unique<int>(head_ptr->data_queue);

            std::unique_ptr<node> old_head;
            old_head = std::move(head_ptr);
            head_ptr = std::move(old_head->ptr_next_element);

            return result;
        }
    }
};

int main(int argc, char const *argv[])
{
    /* code */

    thread_safe_queue_linked_list  queue;
    /*
    // Push some elements into the queue
    queue.push_function(10);
    queue.push_function(20);
    queue.push_function(30);
    // Pop some elements into the queue
    std::unique_ptr<int> result_1;
    std::unique_ptr<int> result_2;
    std::unique_ptr<int> result_3;
    std::unique_ptr<int> result_4;

    result_1 = queue.pop_function();
    result_2 = queue.pop_function();
    result_3 = queue.pop_function();
    result_4 = queue.pop_function();

    std::cout << "If value 0, queue is empty." << std::endl;
    std::cout << "The values from queue are: " << *result_1 << std::endl;
    std::cout << "The values from queue are: " << *result_2 << std::endl;
    std::cout << "The values from queue are: " << *result_3 << std::endl;
    std::cout << "The values from queue are: " << *result_4 << std::endl;*/


    std::thread producer_thread([&queue](){
        for (int i=0; i<5; i++){
            queue.push_function(10*(i+1));
        }
    });

    std::thread consumer_thread([&queue](){
        for (int i=0; i<7; i++){
            std::this_thread::sleep_for(std::chrono::seconds(1));
            auto result = queue.pop_function();
            if (*result == 0){
                std::cout << "The queue is empty" << std::endl;
            }
            else{
                std::cout << "The queue vaues are: " << *result << std::endl;
            }
        }
    });    

    producer_thread.join();
    consumer_thread.join();

    return 0;
}
