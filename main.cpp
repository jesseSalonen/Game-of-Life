#include <iostream>
#include <chrono>
#include <thread>

int main() {
    
    for (int x = 0; x < 3; x++) {
        for (int i = 1; i <= 10; i++) {
            std::cout << "Text" << i << std::endl;
            std::chrono::milliseconds duration(500);
            std::this_thread::sleep_for(duration);
        }
        std::cout << "\033[2J\033[1;1H";
    }

    return 0;
}