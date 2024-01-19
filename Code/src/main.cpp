
#include "Truck/Truck.h"
#define WATCHDOG_TIMEOUT_SECONDS 15
int main() {
    //std::cout << "Hello, World!" << std::endl;


    Truck truck1 = Truck(1);
    truck1.test_leader_exist_controller_behaviour = false;
    truck1.test_follower_exist_controller_behaviour = false;
    truck1.run();
    return 0;
}
