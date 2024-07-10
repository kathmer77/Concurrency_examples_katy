

#include<iostream>
#include<thread>
#include<stack>
#include<mutex>
#include<memory>



class thread_safe_stack{
    std::stack<std::shared_ptr<int>> stk;
    std::mutex mutex;

    public:
    void push(int element_to_stk){
        std::lock_guard<std::mutex> lock(mutex);
        stk.push(std::make_shared<int>(element_to_stk));
    }

    std::shared_ptr<int> pop(){
        std::lock_guard<std::mutex> lock(mutex);
        if (!stk.empty()){
            std::shared_ptr<int> new_ptr(stk.top());
            stk.pop();          // REMOVE ELEMENT.
            return new_ptr;     // RETURN THE SHARED POINTER. Since I am returning the already constructed shared pointer, our return cannot throw an exception.    
            // returns a std::shared_ptr<int>.
        }
        else{
            throw std::runtime_error("Stack Empty");
        }

    /*
    void pop(T& value){
    mutex
    exception
    value = *(stk.top().get());
    stk.pop();}*/
    }

    bool empty(){
        std::lock_guard<std::mutex> lock(mutex);
        return stk.empty();
    }

    size_t size(){
        std::lock_guard<std::mutex> lock(mutex);
        return stk.size();
    }
    
};


int main(int argc, char const *argv[])
{
    /* code */

    thread_safe_stack class_object_stack;
    
    class_object_stack.push(10);
    class_object_stack.push(5);

    std::shared_ptr<int> value_t1;
    std::shared_ptr<int> value_t2;

    {
        std::thread thread_1([&class_object_stack, &value_t1](){
            value_t1 = class_object_stack.pop();
            std::cout << "Value from thread 1 is: " << *value_t1 << std::endl; 
        });
        std::thread thread_2([&class_object_stack, &value_t2](){
            value_t2 = class_object_stack.pop();
            std::cout << "Value from thread 2 is: " << *value_t2 << std::endl;
        });

        thread_1.join();
        thread_2.join();
    }


    return 0;
}
