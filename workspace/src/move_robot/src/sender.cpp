
#include <iostream>
#include <fstream>
#include <sstream>
#include <stdio.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <fcntl.h>
#include <unistd.h>
///////////////////////////////////
#include "ros/ros.h"
#include <nav_msgs/Odometry.h>
#include <geometry_msgs/TransformStamped.h>
#include <geometry_msgs/PoseStamped.h>
#include <move_robot/NewGoal.h>

#define MYFIFO "Server/my_fifo" 
using namespace std;

move_robot::NewGoal msg;
int fd;

int main(int argc, char**argv) {
    
    ros::init(argc,argv,"send");
    ros::NodeHandle n;
    // topic: goal
    ros::Publisher pub = n.advertise<move_robot::NewGoal>("NewGoal",1000);
    //ros::Rate loop_rate(20);


    //APERTURA FIFO CON DENTRO COORDINATE
    int fd = open(MYFIFO, O_RDONLY);
    if (fd==-1) {
        cerr << "Errore apertura in lettura della fifo" << endl;
        return EXIT_FAILURE;
    }
    int i;
    int count=0;
    while(ros::ok()) {

        //LETTURA COORDINATE DALLA FIFO
        float* coordinate = (float*)malloc(sizeof(float)*3);
        for (i=0;i<3;i++) {
            if(read(fd,&coordinate[i], sizeof(float))==-1) {
                cerr << "Errore di lettura nella fifo" << endl;
                return EXIT_FAILURE;
            }
        }
        //SETTO LE COORDINATE NEL MESSAGGIO 
        
        msg.x =(float) coordinate[0];
        msg.y = (float) coordinate[1];
        msg.theta = (float) coordinate[2];
        if (count<2) {
            if (coordinate[0]>1 && coordinate[1]>1 ) {
                cout  << "COORDINATE: " << msg.x << " " << msg.y << " " << msg.theta << endl; 
                cout << "PUBBLICAZIONE GOAL..." << endl;
                for (i=0;i<2;i++) { //pubblico due volte visto che il primo non lo legge poichè il nodo move dorme 2sec
                    pub.publish(msg); 
                    sleep(5); //siamo sicuri che il nodo move è in ascolto
                }
                count++;
            }
        }
        else if (count==2) {
            sleep(3);
            msg.x = 51.52;
            msg.y = 12.04;
            msg.theta = 0.02;
            cout << "IL ROBOT È SCARICO..." << endl;
            cout  << "COORDINATE: " << msg.x << " " << msg.y << " " << msg.theta << endl; 
            cout << "PUBBLICAZIONE GOAL..." << endl;
            for (i=0;i<2;i++) { //pubblico due volte visto che il primo non lo legge poichè il nodo move dorme 2sec
                pub.publish(msg); 
                sleep(5); //siamo sicuri che il nodo move è in ascolto
            }
            count=0;
        }
        ros::spinOnce();
        sleep(5);

        
       // loop_rate.sleep();
    }
    
    close(fd);

    return EXIT_SUCCESS;
    
}