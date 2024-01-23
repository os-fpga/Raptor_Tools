#include "License_manager.hpp"
#include <iostream>


// Main function (for demonstration)
int main() {
    try {
        License_Manager manager(License_Manager::LicensedProductName::MPW1);
        // Additional logic for using the license manager
    } catch (const std::runtime_error &e) {
        std::cerr << "Exception caught: " << e.what() << std::endl;
    }

    return 0;
}
