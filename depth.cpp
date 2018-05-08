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
//#include "visualise.h"

using namespace sensor_msgs;
using namespace message_filters;
using namespace std_msgs;


geometry_msgs::Point humanDepth;

ros::Publisher pub;

void callback(const sensor_msgs::PointCloud2ConstPtr& input, const std_msgs::Float64MultiArrayConstPtr& Coordinate,ros::NodeHandle *nh, ros::Publisher *pub)
{

pcl::PointCloud<pcl::PointXYZRGB>::Ptr cloud (new pcl::PointCloud<pcl::PointXYZRGB>);
pcl::fromROSMsg(*input, *cloud);
//std::cout<<cloud->at(500,500)<<std::endl;
float sum=0;
int count=0;

for(int i=Coordinate->data[0]; i<Coordinate->data[2] ; i++){
	for(int j=Coordinate->data[1] ; j<Coordinate->data[3] ; j++){
		if(cloud->at(i,j).z>0){
		sum+= cloud->at(i,j).z;
		count++;	
		}
	}
	
}

*pub = nh->advertise<geometry_msgs::Point>("/humanDepth", 1);

humanDepth.x= 0;
humanDepth.y=0;
humanDepth.z=sum/count;
pub->publish(humanDepth);

}



int main (int argc, char** argv)
{
  	// Initialize ROS
  	ros::init (argc, argv, "human distance");
  	
  	ros::NodeHandle nh;

  	message_filters::Subscriber<PointCloud2> depth_sub(nh, "/camera/depth/points", 1);
  	message_filters::Subscriber<Float64MultiArray> point_sub(nh, "/humanTracking/point", 1);

  	// TimeSynchronizer<PointCloud2, Float64MultiArray> sync(depth_sub, point_sub, 10);
  	// sync.registerCallback(boost::bind(&callback, _1, _2));

  	typedef sync_policies::ExactTime<PointCloud2, Float64MultiArray> MySyncPolicy;


  	Synchronizer<MySyncPolicy> sync(MySyncPolicy(1), depth_sub, point_sub);
  	sync.registerCallback(boost::bind(callback, _1, _2, &nh, &pub));

  	// Create a ROS subscriber for the input point cloud
  	//ros::Subscriber sub1 = nh.subscribe ("/camera/stereo_camera_LR/points2", 1, cloud_cb);
  	//ros::Subscriber sub2 = nh.subscribe ("/camera/stereo_camera_LR/points2", 1, cloud_cb);


  	// Create a ROS publisher for the output model coefficients
  	//pub = nh.advertise<pcl_msgs::ModelCoefficients> ("output", 1);
  	
	//pub = nh.advertise<sensor_msgs::PointCloud2> ("output", 1);
  	// Spin
  	ros::spin ();
  	return 0;
}