/* 
 * Robotics control in a simulated environment
 * Robotics Engineering - Simone Contorno
 * 
 * To see the map type:
 * rosrun stage_ros stageros $(rospack find second_assignment)/world/my_world.world
 */

#include "ros/ros.h"
#include "std_srvs/Empty.h"
#include "second_assignment_server/Robot.h"

// Publisher
ros::Publisher pub_vel;

// Service clients
ros::ServiceClient client1;
ros::ServiceClient client2;

// Global variable
float speed = 0.0; // Robot velocity

int main(int argc, char **argv) {
    // Initialize the node
    ros::init(argc, argv, "robot_controller");
    ros::NodeHandle rc;
    
    // Clients
    client1 = rc.serviceClient<second_assignment_server::Robot>("/robot_control");
    client2 = rc.serviceClient<std_srvs::Empty>("/reset_positions");
    
    int in = -1; // User's input

    // Show list of commands 
    printf("Type a command:\n\n"
        "0 - Reset robot position\n"
        "1 - Increase robot velocity\n"
        "2 - Decrease robot velocity\n"
        "3 - Stop the robot\n"
        "4 - Exit\n\n"
        "-------------------------------\n\n"
    );

    while (in != 4) {
        in = -1;

        // Take input value
        printf("Command: ");
        std::cin >> in;

        // Reset
        if (in == 0) {
            std_srvs::Empty res;
            client2.waitForExistence();
            client2.call(res);
            printf("Robot position reset.\n");
        }

        // Manage robot's velocity
        else if (in == 1 || in == 2 || in == 3) {
            // Command
            second_assignment_server::Robot com;

            // Increase velocity
            if (in == 1) {
                printf("Increasing velocity...");
                com.request.command = 1.0;
                com.request.speed = speed;
            }
            // Decrease velocity
            else if (in == 2) {
                printf("Decreasing velocity...");
                com.request.command = 2.0;
                com.request.speed = speed;
            }
            // Stop the robot
            else {
                printf("Stopping the robot... ");
                com.request.command = 3.0;
                com.request.speed = 0;
            }

            // Send the request to the server and wait for the response
            client1.waitForExistence();
            client1.call(com);
            speed = com.response.speed;
            printf("New speed: %f\n", speed);
        }
        // Manage wrong inputs
        else {
            printf("ERROR! Command not valid: type '0', '1', '2', '3' or '4'.\n");
        }
    }
    return 0;
}