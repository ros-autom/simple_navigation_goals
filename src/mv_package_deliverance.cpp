/*
 * 2d_nav_goals_2d_pose_wh.cpp
 *
 *  Created on: Feb 4, 2017
 *      Author: Moisiadis Vasileios
 */

#include <ros/ros.h>
#include <move_base_msgs/MoveBaseAction.h>
#include <geometry_msgs/PoseWithCovarianceStamped.h>
#include "std_msgs/String.h"
#include <actionlib/client/simple_action_client.h>
#include <tf/transform_broadcaster.h>
#include <sstream>
#include <iostream>
#include <time.h>

//declare subscriber
ros::Subscriber pose_sub;

// for publish point button
geometry_msgs::PointStamped pose_goal;




void pose_callback(const geometry_msgs::PointStamped& pose_goal);



//Declaring move base goals
//define_move_base_clients();
move_base_msgs::MoveBaseGoal goal; //the input goal

move_base_msgs::MoveBaseGoal goalD; //parking spot
move_base_msgs::MoveBaseGoal goalE; // exit spot


//Declaring a new SimpleActionClient with action of move_base_msgs::MoveBaseAction;
typedef
	actionlib::SimpleActionClient<move_base_msgs::MoveBaseAction>MoveBaseClient;


/*
	define the data of the D,E points


	E=The exit of the warehouse
	D=parking spot

*/






int main(int argc, char** argv)
{

	ros::init(argc, argv, "navigation_goals");
	ros::NodeHandle n;

	ROS_INFO("Wait for the goal to be published");

	//subscribes to the /clicked_point to get the goal pose for the robot
	pose_sub=n.subscribe("/clicked_point",100,pose_callback);
	
	ros::spin();

	return 0;
}




void pose_callback(const geometry_msgs::PointStamped& pose_goal)
{

	try
	{
		goal.target_pose.pose.position.x=pose_goal.point.x;
		goal.target_pose.pose.position.y=pose_goal.point.y;
		goal.target_pose.pose.position.z=pose_goal.point.z;

		goal.target_pose.pose.orientation.w=0.999;
		goal.target_pose.pose.orientation.z=0.1;


		goalD.target_pose.pose.position.x = 0.0154;
		goalD.target_pose.pose.position.y = 7.9827;
		goalD.target_pose.pose.position.z=0;

		goalD.target_pose.pose.orientation.x=0;
		goalD.target_pose.pose.orientation.y=0;
		goalD.target_pose.pose.orientation.z=-0.7143;
		goalD.target_pose.pose.orientation.w =0.69977;

		goalE.target_pose.pose.position.x =8;
		goalE.target_pose.pose.position.y = 0.009;
		goalE.target_pose.pose.position.z=0;

		goalE.target_pose.pose.orientation.x=0;
		goalE.target_pose.pose.orientation.y=0;
		goalE.target_pose.pose.orientation.z=-0.999;
		goalE.target_pose.pose.orientation.w =0.1;

	}
	catch (int e)
	{
		goal.target_pose.pose.position.x = 1.0;
		goal.target_pose.pose.position.y = 1.0;
		goal.target_pose.pose.orientation.w =1.0;
		ROS_INFO("Error while trying to send the goal");
			}

	std::cout<<" Goal position: "<<std::endl;
	std::cout<<goal.target_pose.pose.position.x<<" "<<goal.target_pose.pose.position.y<<std::endl;
	
	//Initiating move_base client
	MoveBaseClient ac("move_base", true);

	//Waiting for server to start
	while(!ac.waitForServer(ros::Duration(5.0)))
	{
 		ROS_INFO("Waiting for the move_base action server");
	}

	//Setting target frame id and time in the goal action
	goal.target_pose.header.frame_id = "map";
	goal.target_pose.header.stamp = ros::Time::now();

	ROS_INFO("Sending move base goal");

	//Sending goal
	ac.sendGoal(goal);
	ac.waitForResult();

	if(ac.getState() == actionlib::SimpleClientGoalState::SUCCEEDED)
		ROS_INFO("the robot arrived to the goal position");
	
	else
	{
		ROS_INFO("The base failed for some reason");
		exit(1);
	}

	//before going to the exit wait for five seconds
	ROS_INFO("wait five seconds and go at the exit");
	ros::Duration(5.0).sleep();

	ROS_INFO("Going to the EXIT");

	goalE.target_pose.header.frame_id = "map";
	goalE.target_pose.header.stamp = ros::Time::now();

	//sending the robot at the exit of the warehouse
	ROS_INFO("Sending move base goal");
	ac.sendGoal(goalE);
	ac.waitForResult();

	if(ac.getState() == actionlib::SimpleClientGoalState::SUCCEEDED)
		ROS_INFO("The robot arrived to the EXIT");
	
	else
		ROS_INFO("The base failed for some reason");

	//wait for five seconds
	ROS_INFO("wait five seconds and go at the parking spot");
	ros::Duration(5.0).sleep();

	ROS_INFO("Going to the parking spot");

	goalD.target_pose.header.frame_id = "map";
	goalD.target_pose.header.stamp = ros::Time::now();

	//sending the robot at the parking spot of the warehouse
	ROS_INFO("Sending move base goal");
	ac.sendGoal(goalD);
	
	ac.waitForResult();

	if(ac.getState() == actionlib::SimpleClientGoalState::SUCCEEDED)
		ROS_INFO("The robot arrived to the parking spot");
	
	else
		ROS_INFO("The base failed for some reason");
	
	ROS_INFO("Wait for the goal to be published");

}
