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
