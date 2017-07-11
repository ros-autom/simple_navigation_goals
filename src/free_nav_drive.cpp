/*
 * free_nav_drive.cpp
 *
 *  Created on: Dec 3, 2016
 *      Author: Bill417
 */

#include <sstream>
#include "ros/ros.h"
#include "std_msgs/String.h"
#include "geometry_msgs/Twist.h"
#include "sensor_msgs/LaserScan.h"
#include "sensor_msgs/Range.h"
#include <iostream>
#include <fstream>
#include <cstdlib>
#include <cmath>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <time.h>
#include "image.h"

using namespace std;



//declare publisher
ros::Publisher cmd_vel_pub;

//declare subscriber
ros::Subscriber laser_scan_sub;

sensor_msgs::LaserScan laser_scan_msgs;

//function for free navigation drive
void free_nav_drive_function(void);

void free_nav_drive_callback(const sensor_msgs::LaserScan& laser_scan_msgs);



//this timer counts the time that the robot moves
void movement_timercallback(const ros::TimerEvent&);

//this timer counts the time that the program saves and process the pgm imaga
void map_timercallback(const ros::TimerEvent&);

//the functions that reads the image and the header of the image
int read_image_header(char[], int&, int&, int&, bool&);
int read_image(char[], Image&);


bool stop_flag=false;

//at the beginning of the program timercallback function is called two times with no reason ,so i use a flag to prevent this
bool error_flag =false;

//this flag is used once when the time runs out and the program stops
bool one_time_flag=true;


double white_rate[22]; //stores the % of the white color for each store

int rows, columns,gray_scale; // rows, cols, grayscale

int val;

bool type;


char file_name[100]="~/ros/husky_kinetic/src/husky/husky_navigation/maps/test_map.pgm";


int main(int argc ,char **argv)
	{



	//initialize the node
	ros::init(argc,argv,"free_nav_drive_node");
	ros::NodeHandle n;


	ROS_INFO("The robot has started");
	//initialize the general_time object for 60*30 sec~= 20 minutes
	ros::Timer movement_timer=n.createTimer(ros::Duration(120*60),movement_timercallback);
	ros::Timer map_timer=n.createTimer(ros::Duration(1*60),map_timercallback);



	//let the topic know that i want to subscribe /publish :
	//subscribe
	//call the callback function;
	laser_scan_sub=n.subscribe("/scan",1,free_nav_drive_callback);


	//publish for husky 	
	//cmd_vel_pub=n.advertise<geometry_msgs::Twist>("husky_velocity_controller/cmd_vel",10);
	cmd_vel_pub=n.advertise<geometry_msgs::Twist>("/cmd_vel",1);


	ros::spin();



	return 0;

}



void free_nav_drive_callback(const sensor_msgs::LaserScan& laser_scan_msgs)
{

	geometry_msgs::Twist cmd_msg_for_publish;
	
	float linear=0,angular=0;

	for(unsigned int i =0 ;i<laser_scan_msgs.ranges.size(); i++)
	{
		float real_distance=laser_scan_msgs.ranges[i];

		linear -=cos(laser_scan_msgs.angle_min + i*laser_scan_msgs.angle_increment)/(1.0+real_distance*real_distance);

		angular -=sin(laser_scan_msgs.angle_min+ i*laser_scan_msgs.angle_increment)/(1.0+real_distance*real_distance);
	}



	linear /= laser_scan_msgs.ranges.size();

	angular /= laser_scan_msgs.ranges.size();

	if(linear>0.3) //maybe 0.5
	{
		linear=0.3;
	}
	
	else if(linear<-0.3)
	{
		linear=-0.3;
	}
	
	

	if(stop_flag)
	{
		cmd_msg_for_publish.linear.x=0;
		cmd_msg_for_publish.angular.z=0;
		cmd_vel_pub.publish(cmd_msg_for_publish);  // publish x=0 z=0 and sleep for 0.5 sec then end the application
		ros::Duration(0, 500000000).sleep();
		exit(1);

	}
	else
	
	{

		cmd_msg_for_publish.linear.x=0.3+linear;  //maybe 0.5
		//cmd_msg_for_publish.angular.z=angular; //for mvrobot
		cmd_msg_for_publish.angular.z=10*angular; //*10 for husky robot
		cmd_vel_pub.publish(cmd_msg_for_publish);
		error_flag=true;
	}


}


void movement_timercallback(const ros::TimerEvent&)
{
	if(error_flag && one_time_flag)
	{
		stop_flag=true;
		ROS_INFO("Robot has stopped");
		ROS_INFO("The time is up");
		one_time_flag=false;
	}
}

void map_timercallback(const ros::TimerEvent&)
{
	if(error_flag)
	{
		static int map_count=0; //counts the times that map changes

		//save the current map
		std::system("rosrun map_server map_saver -f ~/ros/husky_kinetic/src/husky/husky_navigation/maps/test_map");
		map_count++;
		//open the current map
		// read image header-confirm that the image is pgm p5
		read_image_header(file_name, rows, columns, gray_scale, type);

		// allocate memory for the image array
		Image image(rows, columns, gray_scale);
		// read image
		read_image(file_name, image);

		// take the white rate and save it in a matrix  "white_rate"

		white_rate[map_count]=image.white_pixels();
		cout<<"The white rate is   "<<white_rate[map_count]<<"%"<<"  the map counter is  "<<map_count<<endl;

		 //if we have 5 maps check for white % ,if it is the same ,stop the programm
		if(map_count>=5)
			{
				if((white_rate[map_count]-white_rate[map_count-1])<0.05 && (white_rate[map_count]-white_rate[map_count-2])<=0.05 && (white_rate[map_count]-white_rate[map_count-3])<=0.05)
					{
						stop_flag=true;
						ROS_INFO("The map is ready ");
					}

			}

	}
}

int read_image(char fname[], Image& image)
{
	int i, j;
	int rows, columns, gray_scale;
	unsigned char *charImage;
	char header [100], *ptr;
	ifstream ifp;

	ifp.open(fname,  ios::binary);

	if (!ifp)
	{
		cout << "Can't read image: " << fname << endl;
		exit(1);
	}

	// read header

	ifp.getline(header,100,'\n');

	if ( (header[0]!=80) || (header[1]!=53) )
		{
			cout << "Image " << fname << " is not PGM" << endl;
			exit(1);
		}

	ifp.getline(header,100,'\n');

	while(header[0]=='#')
		ifp.getline(header,100,'\n');


	columns=strtol(header,&ptr,0);
	rows=atoi(ptr);

	ifp.getline(header,100,'\n');

	gray_scale=strtol(header,&ptr,0);

	charImage = (unsigned char *) new unsigned char [columns*rows];

	ifp.read( reinterpret_cast<char *>(charImage), (columns*rows)*sizeof(unsigned char));

	if (ifp.fail())
		{
			cout << "Image " << fname << " has wrong size" << endl;
			exit(1);
		}

	ifp.close();
	ROS_INFO("Load the new image");


	//
	// Convert the unsigned characters to integers
	//

	int val;

	for(i=0; i<rows; i++)
		for(j=0; j<columns; j++)
			{
				val = (int)charImage[i*columns+j];

				image.set_pixel_value(i,j,val);

			}

	delete [] charImage;


	return (1);

}

int read_image_header(char fname[], int& rows, int& columns, int& gray_scale, bool& type)
{
	int i, j;
	unsigned char *charImage;
	char header [100], *ptr;
	ifstream ifp;

	ifp.open(fname, ios::in | ios::binary);

	if (!ifp)
		{
		cout << "Can't read image: " << fname << endl;
		exit(1);
		}

	// read header

	type = false; // PGM
	ifp.getline(header,100,'\n');

	if ( (header[0] == 80) && (header[1]== 53) )
		{
			type = false;
		}
	
	else if ( (header[0] == 80) && (header[1] == 54) )
		{
			type = true;
		}
	
	else
	{
		cout << "Image " << fname << " is not PGM or PPM" << endl;
		exit(1);
	}
	
	ifp.getline(header,100,'\n');

	while(header[0]=='#')
		ifp.getline(header,100,'\n');


	columns=strtol(header,&ptr,0);
	rows=atoi(ptr);

	ifp.getline(header,100,'\n');

	gray_scale=strtol(header,&ptr,0);

	ifp.close();

	 return(1);
}




