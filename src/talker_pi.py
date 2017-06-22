import rospy
import getch
from std_msgs.msg import String

def talker():
    pub = rospy.Publisher('pi_tel', String, queue_size=10)
    rospy.init_node('talker_pi_node', anonymous=True)
    rate = rospy.Rate(10) # 10hz
    
    while not rospy.is_shutdown():
	char = getch.getch()

       
        rospy.loginfo(char)
        #pub.publish(caher)
        rate.sleep()

if __name__ == '__main__':
    try:
        talker()
    except rospy.ROSInterruptException:
        pass
