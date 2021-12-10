# RT-Assignment-2
## Second Assignment of Research Track 1 - Robotics Engineering
### Author: Simone Contorno

<br>

Control of a robot in a simulated environment.

### Introduction
An overview of this program function.<br>
[Go to Introduction](#intro)

### How it works
A rapid description of how the program works (pseudo-code).<br>
[Go to How it works](#how)

### Installation and Execution
How install and run RT-Assignment-2 in Linux.<br>
[Go to Installation and Execution](#installation)

### Improvements
How this program could be improved.<br>
[Go to Improvements](#improve)

<a name="intro"></a>
### Introduction

This program manage a robot, endowed with laser scanners, which should move autonomously inside a circuit.<br>
You can use the robot controller to:
<ul>
    <li>Increase the linear velocity;</li>
    <li>Decrease the linear velocity;</li>
    <li>Stop the robot;</li>
    <li>Reset the robot to the initial position.</li>
</ul>

The circuit is this one:

![map](https://github.com/simone-contorno/RT-Assignment-2/blob/main/second_assignment_map.png)

<a name="how"></a>
### How it works

There are 3 packages:
<ul>
    <li>second_assignment: contains the map of the circuit.</li>
    <li>second_assignment_node: contains 2 source files .cpp
        <ul>
            <li>robot.cpp: controls the robot to follow the ciruit avoiding the borders; for this task it reads data from laser scanners,
            compare them (on the left and on the right) when it is too close to an obstacle (in front of its) and choose the direction 
            where there is more space.</li>
            <li>robot_controller.cpp: provides the user interface to digit commands.</li>
        </ul>
    </li>
    <li>second_assignment_server: contains the source file server.cpp that provides a service that takes in input the requested command from the robot_controller node, and           execute the corrisponding task; Robot.srv is the file used by this service.</li>
</ul>

Look the pseudocode files into the corresponding directories for more details.<br>

<a name="installation"></a>
### Installation and Execution

First of all download into your ROS workspace the current repository opening the terminal and typing:

<pre><code>git clone https://github.com/simone-contorno/RT-Assignment-2</code></pre>

Move the folders into the src directory of your ROS workspace.<br> 
After, go into the root folder of your ROS workspace and type: 

<pre><code>catkin_make</code></pre>

Afterwards type:

<pre><code>rospack profile</code></pre>

Now, open 5 terminals; in the first one run ROS core:

<pre><code>roscore</code></pre>

In the second one run the node to visualize the map:

<pre><code>rosrun stage_ros stageros $(rospack find second_assignment)/world/my_world.world</code></pre>

In the third one run the service node:

<pre><code>rosrun second_assignment_server server</code></pre>

In the fourth one run the robot controller node to manage the robot through the user interface:

<pre><code>rosrun second_assignment_node robot_controller</code></pre>

In the last one run the robot node:

<pre><code>rosrun second_assignment_node robot</code></pre>

<a name="improve"></a>
### Improvements

To improve the robot performance, it could be done an improvement to mantain better the robot in the center of the path.<br><br>

Thanks to have read this file, i hope it was clear and interesting.<br>
