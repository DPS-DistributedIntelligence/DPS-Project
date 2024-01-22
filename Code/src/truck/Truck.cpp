#include "Truck.h"

void Truck::run(){
    /*
     * Create the threads that will be run in the system called: Truck
     */
    pthread_create(&t_communication, NULL, &CommsModule::run, &truck_communication);
    pthread_create(&t_controller, NULL, &controller::run, &truck_controller);
    pthread_create(&t_interface, NULL, &controller::key_board_run, &truck_controller);
    /*
     * Run the threads so the actual simulation can be run through the thread id called:
     * - t_controller
     * - t_interface
     * - t_communications
     */
    pthread_join(t_controller, NULL);
    pthread_join(t_interface, NULL);
    pthread_join(t_communication, NULL);
}

Truck::Truck(int truckId) : truckID(truckId)
{

}





