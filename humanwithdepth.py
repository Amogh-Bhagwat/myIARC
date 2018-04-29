import rospy
import pcl
import sys
import cv2
import numpy as np
import argparse
import imutils
import time
import geometry_msgs.msg
from cv_bridge import CvBridge, CvBridgeError
from sensor_msgs.msg import Image
from sensor_msgs.msg import PointCloud2
import sensor_msgs.point_cloud2 as pc2

ap = argparse.ArgumentParser()
ap.add_argument("-p", "--prototxt", required=True,help="path to Caffe 'deploy' prototxt file")
ap.add_argument("-m", "--model", required=True, help="path to Caffe pre-trained model")
ap.add_argument("-c", "--confidence", type=float, default=0.2,help="minimum probability to filter weak detections")
args = vars(ap.parse_args())

CLASSES = ["background", "aeroplane", "bicycle", "bird", "boat","bottle", "bus", "car", "cat", "chair", "cow", "diningtable",
    "dog", "horse", "motorbike", "person", "pottedplant", "sheep","sofa", "train", "tvmonitor"]
COLORS = np.random.uniform(0, 0, size=(len(CLASSES), 3))

net = cv2.dnn.readNetFromCaffe(args["prototxt"], args["model"])
bridge = CvBridge()
centroid_human = rospy.Publisher('/humanTracking/centroid', geometry_msgs.msg.Point,queue_size=1)
depth_human = rospy.Publisher('/humanTracking/depth', geometry_msgs.msg.Point,queue_size=1)
def image_callback(msg):
    print("Received an image for detection!")
    try:  
        frame = bridge.imgmsg_to_cv2(msg, "bgr8")
    except CvBridgeError, e:
        print(e)
    else:
        global startX,startY,endX,endY
        global x_centroid,y_centroid    
        frame = imutils.resize(frame, width=400)
        (h, w) = frame.shape[:2]
        blob = cv2.dnn.blobFromImage(cv2.resize(frame, (300, 300)),0.007843, (300, 300), 127.5)
        net.setInput(blob)
        detections = net.forward()
        for i in np.arange(0, detections.shape[2]):
            confidence = detections[0, 0, i, 2]
            if confidence > args["confidence"]:
                idx = int(detections[0, 0, i, 1])

                if idx==15:
                    box = detections[0, 0, i, 3:7] * np.array([w, h, w, h])
                    (startX, startY, endX, endY) = box.astype("int")
                    x_centroid = (startX+endX)/2
                    y_centroid = (startY+endY)/2
                    print("("+str(startX)+","+str(startY)+")  ("+str(endX)+" "+str(endY)+")"+"   centroid : ("+str(x_centroid)+"),("+str(x_centroid)+")" )
                    cen = geometry_msgs.msg.Point()
                    cen.x = x_centroid
                    cen.y = y_centroid  
                    cen.z = 0
                    centroid_human.publish(cen)                   

def image_callback2(msg2):
    print("Received an image for depth!")
   

    points_list = []
    
    for data in pc2.read_points(msg2, skip_nans=True):
        points_list.append(np.array([data[0], data[1], data[2], data[3]]))
        #print(str(data[0])+" "+str(data[1])+" "+str(data[2])+" "+str(data[3]))
       
    p = pcl.PointCloud_PointXYZRGB()
    p.from_list(points_list)
    count = 0
    depthSum = 0
    # #print("("+str(startX)+","+str(startY)+")  ("+str(endX)+" "+str(endY)+")"+"   centroid : ("+str(x_centroid)+"),("+str(x_centroid)+")" )  

    for x in range(startX, endX):        
        for y in range(startY, endY):
            #print(str(x)+" "+str(y))
            for data in p:
                if ((data[0]==x) && (data[1]==y) && (data[3]!=0))
                    depthSum = depthSum + data[3]          
                    count= count+1            
            
    depth = depthSum/count

    dep = geometry_msgs.msg.Point()
    dep.x = 0
    dep.y = 0  
    dep.z = depth
    depth_human.publish(dep)    

def main():
    rospy.init_node('image_listener')  
    image_topic = "/camera/rgb/image_raw"
    image_topic2 = "/camera/depth/points"
    rospy.Subscriber(image_topic, Image, image_callback)
    rospy.Subscriber(image_topic2, PointCloud2, image_callback2)
    rospy.spin()
if __name__ == '__main__':
    main()
