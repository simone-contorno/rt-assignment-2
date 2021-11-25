/* 
 * Robotics control in a simulated environment
 * Robotics Engineering - Simone Contorno
 * 
 * To see the map type:
 * rosrun stage_ros stageros $(rospack find second_assignment)/world/my_world.world
 */

#include "ros/ros.h"
#include "second_assignment_server/Robot.h"

// Robot velocity
float speed = 0.0;

// Manage commands from robot_controller and the request from robot
bool serverCallback(second_assignment_server::Robot::Request &req, second_assignment_server::Robot::Response &res){
    // Increase speed
    if (req.command == 1) { 
        res.speed = req.speed + 0.5;
    }
    // Decrease speed 
    else if (req.command == 2) { 
        res.speed = req.speed - 0.5;
    }
    // Stop the robot
    else if (req.command == 3) { 
        res.speed = req.speed;
    }
    // Return the actual robot velocity
    else if (req.command == 4) { 
        res.speed = speed;
    }
    
    // Update robot velocity
    speed = res.speed; 

    return true;
}

int main(int argc, char **argv) {
    // Initialize the node
    ros::init(argc, argv, "robot_server");
    ros::NodeHandle srv;

    // Service
    ros::ServiceServer service = srv.advertiseService("/robot_control", serverCallback);

    ros::spin();
    return 0;
}