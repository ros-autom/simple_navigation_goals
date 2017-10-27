//============================================================================
// Name        : box_picking.cpp
// Author      : ros-autom
// Version     :
// Copyright   : Your copyright notice
// Description :
//============================================================================
#include <fstream>
#include <iostream>
#include <math.h>
#include <pwd.h>
#include <ros/ros.h>
#include <sstream>
#include <string>
#include <sys/types.h>
#include <tf/transform_broadcaster.h>
#include <tf/transform_broadcaster.h>
#include <time.h>
#include <unistd.h>

class Box {
public:
  Box() {}

  void Init(int index) {

    delivered_ = false;
    // grab the home dir
    struct passwd *pw = getpwuid(getuid());
    const char *homedir = pw->pw_dir;
    std::string ss2 = "/warehouse_items.txt";
    std::string ss3 = homedir + ss2;

    std::ifstream input_stream(ss3, std::ios::in);
    if (!input_stream.is_open()) {
      ROS_ERROR("Failed to open the warehouse_items.txt");
      return;
    }

    int thresholdDown = (5 * index) + 1;
    int thresholdUp = thresholdDown + 4;

    int linecounter = 0;
    for (std::string line; getline(input_stream, line);) {
      linecounter++;
      if ((linecounter >= thresholdDown) && (linecounter <= thresholdUp)) {
        if (line.find("_") != std::string::npos) {
          id_ = stoi(line.substr(line.find("_") + 1));
          type_ = line.substr(0, line.find("_"));
        }
        if (line.find("x: ") != std::string::npos) {
          x_ = atof((line.substr(2).c_str()));
        }
        if (line.find("y: ") != std::string::npos) {
          y_ = atof((line.substr(2).c_str()));
        }
        if (line.find("z: ") != std::string::npos) {
          z_ = atof((line.substr(2).c_str()));
        }
      }
    }
    input_stream.close();
  }

  void Test(int index) {
    struct passwd *pw = getpwuid(getuid());
    const char *homedir = pw->pw_dir;
    std::string ss2 = "/test/test" + std::to_string(index) + ".txt";
    std::string ss3 = homedir + ss2;

    static std::ofstream poses_file;
    poses_file.open(ss3, std::ofstream::out | std::ofstream::app);

    if (poses_file.fail())
      ROS_ERROR("Failed to open the test files ");
    else {
      poses_file << "type:" << type_ << std::endl
                 << "id:" << id_ << std::endl
                 << "x:" << x_ << std::endl
                 << "y:" << y_ << std::endl
                 << "z:" << z_ << std::endl;

      poses_file.close();
    }
  }

  void ShowValues() {
    std::cout << "type:" << type_ << std::endl
              << "id:" << id_ << std::endl
              << "x:" << x_ << std::endl
              << "y:" << y_ << std::endl
              << "z:" << z_ << std::endl;
  }

  std::string GetType() { return type_; }

  int GetID() { return id_; }

  tf::Vector3 GetCoords() { return tf::Vector3(x_, y_, z_); }

  void SetDevStatus(bool St_) { delivered_ = St_; }

  bool GetDevStatus() { return delivered_; }


private:
  double x_;
  double y_;
  double z_;
  std::string type_;
  int id_;
  bool delivered_;

};
