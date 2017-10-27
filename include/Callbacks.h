#include <actionlib/client/simple_action_client.h>
#include <move_base_msgs/MoveBaseAction.h>
#include <tf/transform_broadcaster.h>

typedef actionlib::SimpleActionClient<move_base_msgs::MoveBaseAction>
    MoveBaseClient;

void tf_publisher_callback(const ros::TimerEvent &event) {
  static tf::TransformBroadcaster tf_broadcaster;
  tf::Transform transform;
  for (int i = 0; i < BoxSize; i++) {

    if (!Boxes[i].GetDevStatus()) {

      transform.setOrigin(Boxes[i].GetCoords());

      tf::Quaternion q;
      q.setRPY(0, 0, 0);
      transform.setRotation(q);

      tf_broadcaster.sendTransform(tf::StampedTransform(
          transform, ros::Time::now(), "odom",
          Boxes[i].GetType() + std::to_string(int(Boxes[i].GetID()))));
    }
  }
}

void send_goal(double x, double y) {
  // tell the action client that we want to spin a thread by default
  MoveBaseClient ac("move_base", true);

  // wait for the action server to come up
  while (!ac.waitForServer(ros::Duration(5.0))) {
    ROS_INFO("Waiting for the move_base action server to come up");
  }

  move_base_msgs::MoveBaseGoal goal;

  // we'll send a goal to the robot to move 1 meter forward
  goal.target_pose.header.frame_id = "map";
  goal.target_pose.header.stamp = ros::Time::now();
  std::cout << "x " << x << " y " << y << std::endl;
  goal.target_pose.pose.position.x = x;
  goal.target_pose.pose.position.y = y;
  goal.target_pose.pose.orientation.w = 1.0;

  ROS_INFO("Sending goal");
  ac.sendGoal(goal);

  ac.waitForResult();

  if (ac.getState() == actionlib::SimpleClientGoalState::SUCCEEDED)
    ROS_INFO("Robot arrived to the goal position");
  else
    ROS_INFO("The base failed for some reason");
}
//get get the type of the product and return the stock in the warehouse
//if there are no more boxes,exit the programm
bool callService(simple_navigation_goals::SendGoalArray::Request &req,
                 simple_navigation_goals::SendGoalArray::Response &res) {

  int productId = -1;
  double xExit = 8.4282;
  double yExit = 0;
  double xParking = -15.94;
  double yParking = 8.97;
  bool isFound = false;
  if (req.productName == "CocaCola")
    productId = 0;
  else if (req.productName == "Fanta")
    productId = 1;
  else if (req.productName == "Lays")
    productId = 2;
  else if (req.productName == "Qr")
    productId = 3;
  else {
    ROS_ERROR("Incorrect product name ");
    return false;
  }
  switch (productId) {
  case 0:
    isFound = false;
    for (int i = 0; i < BoxSize; i++) {
      if ((Boxes[i].GetType() == "CocaCola") && !Boxes[i].GetDevStatus()) {

        ROS_INFO("Go to the position of the box");
        send_goal(Boxes[i].GetCoords().x(), Boxes[i].GetCoords().y());
        ROS_INFO("Receive the box and go to the exit");
        Boxes[i].SetDevStatus(true);
        send_goal(xExit, yExit);
        ROS_INFO("Go to the parking spot");
        send_goal(xParking, yParking);
        PrintProducts(Boxes, BoxSize);
        isFound = true;
        break;
      }
    }
    if (!isFound) {
      ROS_WARN("There are no more Coca Cola boxes in the warehouse");
      return false;
    }
    break;

  case 1:
    isFound = false;
    for (int i = 0; i < BoxSize; i++) {
      if ((Boxes[i].GetType() == "Fanta") && (!Boxes[i].GetDevStatus())) {
        ROS_INFO("Go to the position of the box");
        send_goal(Boxes[i].GetCoords().x(), Boxes[i].GetCoords().y());
        ROS_INFO("Receive the box and go to the exit");
        Boxes[i].SetDevStatus(true);
        send_goal(xExit, yExit);
        ROS_INFO("Go to the parking spot");
        send_goal(xParking, yParking);
        PrintProducts(Boxes, BoxSize);
        isFound = true;
        break;
      }
    }
    if (!isFound) {
      ROS_WARN("There are no more Fanta boxes in the warehouse");
      return false;
    }
    break;

  case 2:
    isFound = false;
    for (int i = 0; i < BoxSize; i++) {
      if ((Boxes[i].GetType() == "Lays") && (!Boxes[i].GetDevStatus())) {
        ROS_INFO("Go to the position of the box");
        send_goal(Boxes[i].GetCoords().x(), Boxes[i].GetCoords().y());
        ROS_INFO("Receive the box and go to the exit");
        Boxes[i].SetDevStatus(true);
        send_goal(xExit, yExit);
        ROS_INFO("Go to the parking spot");
        send_goal(xParking, yParking);
        PrintProducts(Boxes, BoxSize);
        isFound = true;
        break;
      }
    }
    if (!isFound) {
      ROS_WARN("There are no more Lays boxes in the warehouse");
      return false;
    }
    break;

  case 3:
    isFound = false;
    for (int i = 0; i < BoxSize; i++) {
      if ((Boxes[i].GetType() == "Qr") && (!Boxes[i].GetDevStatus())) {
        ROS_INFO("Go to the position of the box");
        send_goal(Boxes[i].GetCoords().x(), Boxes[i].GetCoords().y());
        ROS_INFO("Receive the box and go to the exit");
        Boxes[i].SetDevStatus(true);
        send_goal(xExit, yExit);
        ROS_INFO("Go to the parking spot");
        send_goal(xParking, yParking);
        PrintProducts(Boxes, BoxSize);
        isFound = true;
        break;
      }
    }
    if (!isFound) {
      ROS_WARN("There are no more Qr boxes in the warehouse");
      return false;
    }
    break;
  }

  for (int i = 0; i < BoxTypes.size(); i++) {
    res.BoxCounter[i] = CountProducts(Boxes, BoxSize, BoxTypes[i]);
    res.Boxes[i] = BoxTypes[i];
  }
  res.size = BoxTypes.size();

  return true;
}
