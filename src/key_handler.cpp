
#include <stdio.h>
#include <stdlib.h>
#include <termios.h>
#include <unistd.h>
#include <fcntl.h>

#include <ros/ros.h>

#include <std_msgs/Int32.h>

int getch(void)
{
  static struct termios oldt, newt;
  tcgetattr( STDIN_FILENO, &oldt);           // save old settings
  newt = oldt;
  newt.c_lflag &= ~(ICANON);                 // disable buffering      
  tcsetattr( STDIN_FILENO, TCSANOW, &newt);  // apply new settings

  int ch = getchar();  // read character (non-blocking)

  tcsetattr( STDIN_FILENO, TCSANOW, &oldt);  // restore old settings

  return ch;
}


int main(int argc, char **argv) {
  ros::init(argc, argv, "key_handle");
  ros::NodeHandle handle;
  ros::Publisher pub = handle.advertise<std_msgs::Int32>("key_input", 10);
  ros::Rate r(20);
  std_msgs::Int32 msg;

  int key = -1;
  while(ros::ok()){
    key = getch();
    if(key != -1) {
      msg.data = key;
      pub.publish(msg);
    }
    r.sleep();
  }
}
