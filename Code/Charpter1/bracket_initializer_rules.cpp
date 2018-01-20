#include <iostream>
#include <typeinfo>
#include <initializer_list>

int main() {
    
    // initializer_list
    {
        // auto x {123} type
        {
            int v1 = 1;
            if (auto v {1}; typeid(v).hash_code() == typeid(v1).hash_code()) {
                std::cout << "type v and v1 is same" << std::endl;
            } else {
                std::cout << "type v and v1 is not same" << std::endl;
            }
        }

        // auto x {1, 2} type
        {
            // auto x{1, 2} is error, please use auto x = {1, 2};
            std::initializer_list<int> v1 {1, 2};
            if (auto v = {1,3}; typeid(v).hash_code() == typeid(v1).hash_code()) {
                std::cout << "type v and v1 is same" << std::endl;
            } else {
                std::cout << "type v and v1 is not same" << std::endl;
            }
        }

        // use initializer_list style don't use type conversion
        {
            //int x {1.2}; this will be error
            int x (1.2); // this is ok
        }
    }
    return 0;
}
