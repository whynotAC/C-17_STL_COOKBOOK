#include <iostream>
#include <cstdio>
#include <map>
#include <string>
#include <mutex>
#include <memory>

template <class T>
void print_charcount_cpp11(const T &map, char c) {
    const auto itr(map.find(c));
    if (itr != map.end()) {
        // *itr is valid. Do something with it
        std::cout << "There are " << itr->second << " " << c << " characters.\n";
    } else {
        // itr is invalid, must not be accessed, but is available
        std::cout << "There are " << 0 << " " << c << " characters.\n";
    }
    // itr is not necessarily safe to dereference, but still available
}

template <class T>
void print_charcount_cpp17(const T &map, char c) {
    if (const auto itr(map.find(c)); itr != map.end()) {
        // *itr is valid. Do something with it
        std::cout << "There are " << itr->second << " " << c << " characters.\n";
    } else {
        // itr is visible.
        std::cout << "There are " << 0 << " " << c << " characters.\n";
    }
    // itr is not visible here
}

int main() {
    {
        std::map<char, size_t> character_map {{'a', 10}, {'b', 15}, {'c', 9}};
        print_charcount_cpp11(character_map, 'a');
        print_charcount_cpp11(character_map, 'd');

        print_charcount_cpp17(character_map, 'a');
        print_charcount_cpp17(character_map, 'd');
    }

    {
        std::cout << "Please enter a character: ";
        switch (char c(getchar()); c) {
        case 'a' ... 'z':
            std::cout << "You entered a lower case letter: " << c << "\n";
            break;
        case 'A' ... 'Z':
            std::cout << "You entered a lower case letter: " << c << "\n";
            break;
        default:
            std::cout << "You entered something else: " << c << "\n";
        }
    }

    // if
    {
        // before C++17
        {
            auto var(1);
            if (var) {
                // branch A. var is accessible
            } else {
                // brance B. var is accessible
            }
            // var is still accessible
        }

        // since C++17
        {
            if (auto var(1); var) {
                // branch A. var is accessible
            } else {
                // brance B. var is accessible
            }
            // var is not accessible any longer 
        }
    }
    
    // switch
    {
        //before C++17
        {
            auto var (1);
            switch (var) {
            case 1: // do something
            case 2: // do something
            default:
              std::cout << var << std::endl;
            }
            // var is still accessible
        }

        // since C++17
        {
            switch (auto var(1); var) {
            case 1: // do something
            case 2: // do something
            default:
              std::cout << var << std::endl;
            }
            // var is not accessible any longer
        }
    }

    // there's more
    {
        // mutex
        {
          std::mutex my_mutex;
          if (std::lock_guard<std::mutex> lg {my_mutex}; true) {
            // Do something
          }
        }

        // weak_ptr
        {
            auto test_ptr = std::make_shared<int>(1);
            std::weak_ptr<int> weak_pointer = test_ptr; 
            std::cout << "per weak_ptr use_count: " << weak_pointer.use_count() << std::endl;
            if (auto shared_pointer = weak_pointer.lock(); shared_pointer != nullptr) {
                std::cout << "enter weak_ptr use_count: " << weak_pointer.use_count() << std::endl;
                std::cout << "number: " << *shared_pointer << std::endl;
                // Yes, the shared object does still exist
            } else {
                std::cout << "enter weak_ptr use_count: " << weak_pointer.use_count() << std::endl;
                std::cout << "shared_pointer is nullptr" << std::endl;
                // shared_pointer var is accessible, but a null pointer
            }
            std::cout << "over weak_ptr use_count: " << weak_pointer.use_count() << std::endl;
            // shared_pointer is not accessible any longer
        }

        // API
        // if (DWORD exit_code; GetExitCodeProcess(process_handle, &exit_code) {
        //      std::cout << "Exit code of process was: " << exit_code << "\n";
        // }
        // No useless exit_code variable outside the if-conditional
    }
    return 0;
}
