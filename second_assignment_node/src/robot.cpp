/* 
 * Robotics control in a simulated environment
 * Robotics Engineering - Simone Contorno
 * 
 * To see the map type:
 * rosrun stage_ros stageros $(rospack find second_assignment)/world/my_world.world
 */

#include "ros/ros.h"
#include "sensor_msgs/LaserScan.h"
#include "geometry_msgs/Twist.h" 
#include "second_assignment_server/Robot.h"

// Publisher
ros::Publisher pub_vel;

// Service client
ros::ServiceClient client;

// Global variables
int flag = 0; // Just to manage the printed strings
float speed = 0.0; // Robot velocity

/*
 * 1. Make a request to the service for the actual robot velocity
 * 2. Take laser values and check for the obstacles
 * 3. Publish robot velocity
 */ 
void robotCallback(const sensor_msgs::LaserScan::ConstPtr& msg) {
    // Local variables
    float first_value = 0;
    float mid_value = 30;
    float last_value = 0;

    // 1. Make the request to the service 
    second_assignment_server::Robot com;
    com.request.command = 4.0;
    client.waitForExistence();
    client.call(com);

    if (speed != com.response.speed) {
        speed = com.response.speed;
        printf("Actual robot velocity: %f\n", speed);
    }

    // 2. Take the maximum value on the left
    int i = 0;
    while (i < 120) {
        if (msg->ranges[i] > first_value)
            first_value = msg->ranges[i];
        i++;
    }

    // Take the minimum value in the middle
    i = 300;
    while (i < 420) {
        if (msg->ranges[i] < mid_value)
            mid_value = msg->ranges[i];
        i++;
    }

    // Take the minimum value on the right
    i = 600;
    while (i <= 720) {
        if (msg->ranges[i] > last_value)
            last_value = msg->ranges[i];
        i++;
    }

    /* 
     * Compute the differenze between the minimum value
     * on the right and on the left to decide in which 
     * direction turn when an obstacle is too close to the robot
     */ 
    float diff = last_value - first_value;
    float abs_diff = diff;
    if (abs_diff < 0) 
        abs_diff *= -1;
    if (abs_diff > 1) {
        abs_diff = 1;
    }
    else if (abs_diff < -1) {
        abs_diff = -1;
    }

    // Set robot velocity
	geometry_msgs::Twist robot_vel;
    
    /* 
     * If the obstacle is too close use the diff value computed above
     * to choose in which direction turn the robot
     */ 
    if (mid_value < 1) {
        if (flag == 1) 
            printf("I'm too close to an obstacle!\n");
        
        // If the robot has more space on the right
        if (diff < 0) {
            printf("Turn Right...\n\n");
            if (diff < -1) {
                robot_vel.linear.x = 0;
                robot_vel.angular.z = diff; 
            }
            else {
                robot_vel.angular.z = -1;
                robot_vel.linear.x = abs_diff;
            }
        }

        // If the robot has more space on the left
        else if (diff > 0) {
            printf("Turn Left...\n\n");
            if (diff > 1) {
                robot_vel.linear.x = 0; 
                robot_vel.angular.z = diff; 
            }
            else {
                robot_vel.angular.z = 1;
                robot_vel.linear.x = abs_diff;
            }     
        }
        flag = 0;
    }

    // Go on 
    else {
        if (flag == 0) {
            printf("Go on...\n\n");
            flag = 1;
        }
        if (speed * (mid_value / 3) >= speed)
            robot_vel.linear.x = speed;
        else 
            robot_vel.linear.x = speed * (mid_value / 3);
        robot_vel.angular.z = 0.0;
    }
   
    // Publish robot velocity
	pub_vel.publish(robot_vel);
}

int main(int argc, char **argv) {
    // Initialize the node
    ros::init(argc, argv, "robot");
    ros::NodeHandle nh;
    
    // Publisher velocity
    pub_vel = nh.advertise<geometry_msgs::Twist>("/cmd_vel", 1000);

    // Client
    client = nh.serviceClient<second_assignment_server::Robot>("/robot_control");

    // Subscriber laser info
    ros::Subscriber sub_las = nh.subscribe("/base_scan", 1000, robotCallback);

    ros::spin();
    return 0;
}