#include <opencv2/dnn.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/core/utils/trace.hpp>
using namespace cv;
using namespace cv::dnn;
#include <fstream>
#include <iostream>
#include <cstdlib>
using namespace std;

VideoCapture cam(0);
Mat frame;

String classNames[] = {"background", "aeroplane", "bicycle", "bird", "boat","bottle", "bus", "car", "cat", "chair", "cow", "diningtable",
    "dog", "horse", "motorbike", "person", "pottedplant", "sheep","sofa", "train", "tvmonitor"};

static void getMaxClass(const Mat &probBlob, int *classId, double *classProb)
{
    Mat probMat = probBlob.reshape(1, 1); //reshape the blob to 1x1000 matrix
    Point classNumber;
    minMaxLoc(probMat, NULL, classProb, NULL, &classNumber);
    *classId = classNumber.x;
}
int main()
{   
    while(true)
    {
    cam.read(frame);
    Mat image = frame.clone();
    Mat img = image.clone();
    CV_TRACE_FUNCTION();
    String modelTxt = "MobileNetSSD_deploy.prototxt.txt";
    String modelBin = "MobileNetSSD_deploy.caffemodel";
    
    cout<<"Loading Model"<<endl;    
    Net net = dnn::readNetFromCaffe(modelTxt, modelBin);
    if (net.empty())
    {
        std::cerr << "Can't load network " << std::endl;
        exit(-1);
    }     
    Mat inputBlob = blobFromImage(img, 1, Size(224, 224),Scalar(104, 117, 123));   //Convert Mat to batch of images
    Mat prob;
    cv::TickMeter t;
    for (int i = 0; i < 10; i++)
    {
        CV_TRACE_REGION("forward");
        net.setInput(inputBlob, "data");        //set the network input
        t.start();
        prob = net.forward("prob");                          //compute output
        t.stop();
    }
    int classId;
    double classProb;
    getMaxClass(prob, &classId, &classProb);//find the best class
    
    cout << "Best class: #" << classId << " '" << classNames[classId] << "'" << std::endl;
    cout << "Probability: " << classProb * 100 << "%" << std::endl;
    cout << "Time: " << (double)t.getTimeMilli() / t.getCounter() << " ms (average from " << t.getCounter() << " iterations)" << std::endl;
    }
    return 0;
} 

// g++ humandnn.cpp -o test `pkg-config --cflags --libs opencv`