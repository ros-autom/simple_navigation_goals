#include <ros/ros.h>
#include <move_base_msgs/MoveBaseAction.h>
#include <geometry_msgs/PoseWithCovarianceStamped.h>
#include "std_msgs/String.h"
#include <actionlib/client/simple_action_client.h>
#include <tf/transform_broadcaster.h>
#include <sstream>
#include <iostream>
#include <time.h>
#include <sstream>

using namespace std; 


geometry_msgs::PoseWithCovarianceStamped pose_goal;

// declare the publisher

ros::Publisher initialpose_pub;


void move_pose(const geometry_msgs::PoseWithCovarianceStamped& pose_goal);

void reverse_matrix(void);


//Declaring move base goals
//define_move_base_clients();
move_base_msgs::MoveBaseGoal goal; //the input goal

//inner y points
double ypose[7]={-12.0602035522,-9.1, -6.16, -3.16, -0.16, 2.91,5.38};
int pose_counter=0;

//star-end x-y-z point
double save_pose[5][4]=
{
	{-6.77356386185,-12.0602035522, 0.708422913059,0.705788195037},
	{-2.8260531,5.74662399,-0.703920686813,0.710278583851},
	{1.28036, -12.0602035522,  0.708422913059,0.705788195037},
	{5.23330688477, 5.74662399, -0.708422913059,0.705788195037},
	{9.17163467407,-12.0602035522, 0.708422913059,0.705788195037}

};



bool indata=false;
 
//Declaring a new SimpleActionClient with action of move_base_msgs::MoveBaseAction;
typedef actionlib::SimpleActionClient<move_base_msgs::MoveBaseAction>MoveBaseClient;

int main(int argc, char** argv)
	{

	ros::init(argc, argv, "navigation_goals");
	ros::NodeHandle n;

	//initializing rotobo pose
	initialpose_pub=n.advertise< geometry_msgs::PoseWithCovarianceStamped>("/initialpose",2);
	pose_goal.header.seq=1;
	pose_goal.header.stamp.sec=0;
	pose_goal.header.stamp.nsec=0;
	pose_goal.header.frame_id="map";

	pose_goal.pose.pose.position.x=0;
	pose_goal.pose.pose.position.y=0;
	pose_goal.pose.pose.position.z=0;


	pose_goal.pose.pose.orientation.x=0;
	pose_goal.pose.pose.orientation.y=0;
	pose_goal.pose.pose.orientation.z=0;
	pose_goal.pose.pose.orientation.w=0;
	
	try
	{

		initialpose_pub.publish(pose_goal);

		move_pose(pose_goal);
		std::cout<<"initializing pose at 0 0 0 0 0 0 0"<<"+++++++++++++++++++++++++++++++++++++++"<<std::endl;


	}

	catch(int e)
	{
		ROS_INFO("Error while initializing the robot pose");
	}

	//Setting target frame id and time in the goal action
	
	// pose_goal.pose.covariance.elems=[0.25, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.25, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0];
	

	ros::spin();

	return 0;
}


void move_pose(const geometry_msgs::PoseWithCovarianceStamped& pose_goal)

{

	// moving point to point

	for(pose_counter=0;pose_counter<5;pose_counter++)
	{

		try
		{

			goal.target_pose.pose.position.x=save_pose[pose_counter][0];
			goal.target_pose.pose.position.y=save_pose[pose_counter][1];
			goal.target_pose.pose.orientation.z=save_pose[pose_counter][2];
			goal.target_pose.pose.orientation.w=save_pose[pose_counter][3];


		}

		catch(int e)
		{
			goal.target_pose.pose.position.x=0;
			goal.target_pose.pose.position.y=0;
			goal.target_pose.pose.orientation.z=0;
			ROS_INFO("Error while trying to send the goal");


		}
	
		std::cout<<"x "<<goal.target_pose.pose.position.x<<endl;
		std::cout<<"y "<<goal.target_pose.pose.position.y<<endl;

		std::cout<<"z "<<goal.target_pose.pose.orientation.z<<endl;
		std::cout<<"w "<<goal.target_pose.pose.orientation.w<<endl;

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
			ROS_INFO("the robot has arrived to the goal position");
		else
		{
			ROS_INFO("The base failed for some reason2");
			exit(1);
		}

		ros::Duration(1.5).sleep();
		ROS_INFO("Wait for 1s");

		for(int i=0;i<7;i++)
		{

			try
			{

				goal.target_pose.pose.position.x=save_pose[pose_counter][0];
				goal.target_pose.pose.position.y=ypose[i];

				goal.target_pose.pose.orientation.z=save_pose[pose_counter][2];
				goal.target_pose.pose.orientation.w=save_pose[pose_counter][3];
		
			}
	
			catch(int e)
			{
		
				goal.target_pose.pose.position.x=0;
				goal.target_pose.pose.position.y=0;
				goal.target_pose.pose.orientation.z=0;
				ROS_INFO("Error while trying to send the goal");
		
		
			}


				std::cout<<"x "<<goal.target_pose.pose.position.x<<endl;
				std::cout<<"y "<<goal.target_pose.pose.position.y<<endl;
				std::cout<<"z "<<goal.target_pose.pose.orientation.z<<endl;
				std::cout<<"w "<<goal.target_pose.pose.orientation.w<<endl;


				//Initialize move_base clinet


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
				ROS_INFO("the robot has arrived to the goal position");
			
			else
			{
			
				ROS_INFO("The base failed for some reason3");
				exit(1);
			}
			

			indata=true;

			ros::Duration(1.5).sleep();
			ROS_INFO("Wait for 1s qq");
		}
			
		reverse_matrix();
	}

	pose_counter=0;

}


void reverse_matrix(void)
{
	double temp;
	int j;
	for(int i=0, j=7-1;i<(7+1)/2;i++,j--)
		{
			temp=ypose[i];
			ypose[i]=ypose[j];
			ypose[j]=temp;
			std::cout<<ypose[i];
		}

}


