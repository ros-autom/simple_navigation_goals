#!/usr/bin/env python
# Software License Agreement (BSD License)
#
# Copyright (c) 2008, Willow Garage, Inc.
# All rights reserved.
#
# Redistribution and use in source and binary forms, with or without
# modification, are permitted provided that the following conditions
# are met:
#
#  * Redistributions of source code must retain the above copyright
#    notice, this list of conditions and the following disclaimer.
#  * Redistributions in binary form must reproduce the abov
#    copyright notice, this list of conditions and the following
#    disclaimer in the documentation and/or other materials provided
#    with the distribution.
#  * Neither the name of Willow Garage, Inc. nor the names of its
#    contributors may be used to endorse or promote products derived
#    from this software without specific prior written permission.
#
# THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
# "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
# LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS
# FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE
# COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
# INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING,
# BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
# LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
# CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
# LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN
# ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
# POSSIBILITY OF SUCH DAMAGE.
#
# Revision $Id$

## Simple talker demo that published std_msgs/Strings messages
## to the 'chatter' topic
import rospy
from std_msgs.msg import String

import sys, tty, termios, time
from time import sleep
import pifacedigitalio as pfio
import getch

pfd = pfio.PiFaceDigital()
pfio.init()


def mlfr():
  pfio.digital_write(2,1)
  return;
def mrfr():
  pfio.digital_write(4,1)
  return;
def mlb():
  pfio.digital_write(3,1)
  return;
def mrb():
  pfio.digital_write(5,1)
  return;
def res():
  pfio.digital_write(2,0)
  pfio.digital_write(3,0)
  pfio.digital_write(4,0)
  pfio.digital_write(5,0)
  return;


def callback(data):
	char = data.data

	if(char == "w"):
		mlfr()
		mrfr()
		sleep(0.035)
		char = ""
		
	if(char == "s"):
		mlb()
		mrb()
		sleep(0.035)
		char = ""
	
	if(char == "a"):
		mrfr()
		sleep(0.035)
		char = ""
	
	if(char == "d"):
		mlfr()
		sleep(0.035)
		char = ""
	
	if(char==""):
		res()
		char = ""
	
	if(char == "x"):
		print("\nTelos")
		break
		pfio.init()
	
	char = ""
	
	#rospy.loginfo(rospy.get_caller_id() + 'I heard %s', data.data)

def listener():

    
    rospy.init_node('move_pi_node', anonymous=True)

    rospy.Subscriber('pi_tel', String, callback)

    # spin() simply keeps python from exiting until this node is stopped
    rospy.spin()

if __name__ == '__main__':
    listener()
