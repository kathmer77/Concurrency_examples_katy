#include<iostream>
#include<vector>
#include<ctime>
#include<algorithm>
#include <execution> 
#include <chrono>
#include<future>
#include <list> 


*/
std::list<int> parallel_quick_sort(std::list<int> data){
    if(data.size()<2){
        return data;
    }
    // 1. SELECT PIVOT VALUE
    std::list<int> result_list;
    result_list.splice(result_list.begin(), data, data.begin());
    int pivot = *result_list.begin();  // *result_list.begin() dereferences the iterator, giving us the value stored at the beginning of result_list.

    // 2. ARRANGE ARRAY
    std::list<int>::iterator divide_point = std::partition(data.begin(), data.end(), [&pivot](int &iterator){ 
        return iterator < pivot;
    });

    //3. RECURSIVE CALL
    std::list<int> lower_list;   // create new list
    lower_list.splice(lower_list.begin(), data, data.begin(), divide_point); // lowe_list will hold the left part of the pivot. the right part will remain in data.

    // I need to call recursive function to each part.
    auto new_lower = parallel_quick_sort(std::move(lower_list));
          // Run the second recursive call in parallel
    std::future<std::list<int>> upper_future;
    upper_future = std::async(std::launch::async, parallel_quick_sort, std::move(data));
    //auto new_upper = parallel_quick_sort(std::move(data));

    //4. ARRANGE RESULT
    result_list.splice(result_list.begin(), new_lower);
    result_list.splice(result_list.end(), upper_future.get());

    return result_list;

}


int main(int argc, char const *argv[])
{


    std::list<int> data = {1, 7, 2, 6, 4, 5, 3};
    auto result = parallel_quick_sort(data);

    std::cout << "Sorted list is: " << std::flush;
    for(auto &i: result){
          std::cout << i << " " << std::flush;  
    }
    std::cout << std::endl;

    return 0;
}