#include <ros/ros.h>
#include <sensor_msgs/PointCloud2.h>
#include <pcl_conversions/pcl_conversions.h>
#include <pcl/point_cloud.h>
#include <pcl/point_types.h>
#include "std_msgs/Float64MultiArray.h"
#include "geometry_msgs/Point.h"
#include <message_filters/subscriber.h>
#include <message_filters/synchronizer.h>
#include <message_filters/time_synchronizer.h>
#include <message_filters/sync_policies/exact_time.h>
#include <ros/time.h>
#include <pcl/console/parse.h>


using namespace sensor_msgs;
using namespace message_filters;
using namespace std_msgs;


geometry_msgs::Point humanDepth;

ros::Publisher pub;


int Coordinate[4];

void callback0(const std_msgs::Float64MultiArrayConstPtr& coordinate)
{

  Coordinate[0] = coordinate->data[0];
  Coordinate[1] = coordinate->data[1];
  Coordinate[2] = coordinate->data[2];
  Coordinate[3] = coordinate->data[3];

}


void callback1(const sensor_msgs::PointCloud2ConstPtr& input)
{

pcl::PointCloud<pcl::PointXYZRGB>::Ptr cloud (new pcl::PointCloud<pcl::PointXYZRGB>);
pcl::fromROSMsg(*input, *cloud);
//std::cout<<cloud->at(500,500)<<std::endl;
float sum=0;
int count=0;

for(int i=Coordinate[0]; i<Coordinate[2] ; i++){
	for(int j=Coordinate[1] ; j<Coordinate[3] ; j++){
		if(cloud->at(i,j).z>0){
		sum+= cloud->at(i,j).z;
		count++;	
		}
	}
	
}

humanDepth.x= 0;
humanDepth.y=0;
humanDepth.z=sum/count;
pub.publish(humanDepth);

}



int main (int argc, char** argv)
{
  	
  	ros::init (argc, argv, "human distance");
  	
  	ros::NodeHandle nh;
    ros::Subscriber sub1 = nh.subscribe ("/humanTracking/point", 1, callback0);
    ros::Subscriber sub2 = nh.subscribe ("/camera/depth/points", 1, callback1);

    pub = nh.advertise<geometry_msgs::Point>("/humanDepth", 1);

  	ros::Rate loop_rate(12); //adjust the loop rate !!! according to the network

    while(ros::ok())
    { 
      ros::spinOnce();
      loop_rate.sleep();
    }
  	return 0;
}
