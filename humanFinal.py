import rospy
from sensor_msgs.msg import Image
from cv_bridge import CvBridge, CvBridgeError
import cv2
import numpy as np
import argparse
import imutils
import time
import geometry_msgs.msg
import std_msgs.msg

ap = argparse.ArgumentParser()
ap.add_argument("-p", "--prototxt", required=True,help="path to Caffe 'deploy' prototxt file")
ap.add_argument("-m", "--model", required=True, help="path to Caffe pre-trained model")
ap.add_argument("-c", "--confidence", type=float, default=0.2,help="minimum probability to filter weak detections")
args = vars(ap.parse_args())

CLASSES = ["background", "aeroplane", "bicycle", "bird", "boat","bottle", "bus", "car", "cat", "chair", "cow", "diningtable",
    "dog", "horse", "motorbike", "person", "pottedplant", "sheep","sofa", "train", "tvmonitor"]
COLORS = np.random.uniform(0, 0, size=(len(CLASSES), 3))

net = cv2.dnn.readNetFromCaffe(args["prototxt"], args["model"])


# Instantiate CvBridge
bridge = CvBridge()

centroid_human = rospy.Publisher('/humanTracking/centroid', std_msgs.msg.Float64MultiArray,queue_size=1)
cen = std_msgs.msg.Float64MultiArray()

def image_callback(msg):
    print("Received an image!")
    try:
        # Convert your ROS Image message to OpenCV2
        frame = bridge.imgmsg_to_cv2(msg, "bgr8")
    except CvBridgeError, e:
        print(e)
    else:
       
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

                   # x_centroid = (startX+endX)/2
                    #y_centroid = (startY+endY)/2
                    print("("+str(startX)+","+str(startY)+")  ("+str(endX)+" "+str(endY)+")"+"   centroid : ("+str((startX+endX)/2)+"),("+str((startY+endY)/2)+")" )
                                        
                    cen.data =[startX, startY, endX, endY]
                    centroid_human.publish(cen)
    
def main():
    rospy.init_node('image_listener')
    # Define your image topic
    image_topic = "/camera/rgb/image_raw"
    # Set up your subscriber and define its callback
    rospy.Subscriber(image_topic, Image, image_callback)
    # Spin until ctrl + c
    rospy.spin()

if __name__ == '__main__':
    main()
