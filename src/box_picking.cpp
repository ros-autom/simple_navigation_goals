//============================================================================
// Name        : box_picking.cpp
// Author      : ros-autom
// Version     :
// Copyright   : Your copyright notice
// Description :
//============================================================================
#include <ros/ros.h>
#include <iostream>
#include <fstream>
#include <sstream>
#include <pwd.h>
#include <time.h>
#include <tf/transform_broadcaster.h>
#include <string>
#include "simple_navigation_goals/SendGoalArray.h"
#include <math.h>
#include <sys/types.h>
#include "Box.h"
#include <vector>

std::string vecInit[]={"CocaCola","Fanta","Qr","Lays"};
std::vector<std::string> BoxTypes(vecInit,vecInit+sizeof(vecInit)/sizeof(std::string));
Box * Boxes;
int BoxSize;
#include "Actions.h"
#include "Callbacks.h"


int main(int argc, char* argv[]) {
	ros::init(argc, argv, "warehouse_goals");
	ros::NodeHandle n;


	BoxSize = BoxCount();
	Boxes = new Box[BoxSize];
	for (int i = 0 ; i < BoxSize ; i++)
		Boxes[i].Init(i);

	PrintProducts(Boxes,BoxSize);
	ros::Timer timer=n.createTimer(ros::Duration(1),tf_publisher_callback);
    ros::ServiceServer service=n.advertiseService("SendGoalArray",callService);


	ros::AsyncSpinner spinner(4);
	spinner.start();
	ros::waitForShutdown();

}
