#include "ros/ros.h"
#include <nav_msgs/Odometry.h>
#include <sensor_msgs/LaserScan.h>
#include <tf2_msgs/TFMessage.h>
#include <geometry_msgs/TransformStamped.h>
#include <tf2_ros/transform_listener.h>
#include <tf/tf.h>
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <geometry_msgs/PoseStamped.h>
#include <move_robot/NewGoal.h>


using namespace std;
using namespace ros;

vector<float> target_position(2,0);
vector<float> old_position(2,0);
vector<float> current_position(2,0);


geometry_msgs::PoseStamped new_goal_msg;
tf2_ros::Buffer tfBuffer;

size_t n = 10;
int message_published = 0;
int cruising = 0;
int T = 2;


void setGoal_CallBack( const move_robot::NewGoal& new_goal) {
    //SETTO I CAMPI DEL MESSAGGIO geometry_msgs::PoseStamped
    new_goal_msg.header.seq = n;
    n++;

    new_goal_msg.header.stamp = ros::Time::now();
    new_goal_msg.header.frame_id = "map";

    new_goal_msg.pose.position.x = new_goal.x;
    new_goal_msg.pose.position.y = new_goal.y;
    new_goal_msg.pose.position.z = 0;

    new_goal_msg.pose.orientation.x = 0;
    new_goal_msg.pose.orientation.y = 0;
    new_goal_msg.pose.orientation.z = 0;
    new_goal_msg.pose.orientation.w = new_goal.theta;

    message_published = 1;
    cruising = 1;

    //POSA GOAL
    target_position[0] = new_goal_msg.pose.position.x;
    target_position[1] = new_goal_msg.pose.position.y;

    ROS_INFO("SETTO IL GOAL");

}

void position_CallBack( const tf2_msgs::TFMessage& tf) {
    int trasform_ok;
    //CHECK PER VERIFICARE SE LA TRASFORMATA ESISTE
    trasform_ok = tfBuffer.canTransform("map", "base_link", ros::Time(0));

    if (trasform_ok!=0) {
        geometry_msgs::TransformStamped transformStamped;
        //MI PRENDO LA TRASFORMATA DA MAP A BASE_LINK CHE MI DA LA POSIZIONE DEL ROBOT
        transformStamped = tfBuffer.lookupTransform("map", "base_link", ros::Time(0));

        //POSA CORRENTE
        current_position[0] = transformStamped.transform.translation.x;
        current_position[1] = transformStamped.transform.translation.y;
    }
}

void check1_callBack(const ros::TimerEvent& event){
    if(cruising!=0){
        //CHECK SE SI STA MUOVENDO
        float distance;
        distance = sqrt(pow(current_position[0]-old_position[0],2)+pow(current_position[1]-old_position[1],2));
        if(distance < 0.5){
            //ROBOT BLOCCATO
            ROS_INFO("I'm stuck");
        }
        distance = sqrt(pow(current_position[0]-target_position[0],2)+pow(current_position[1]-target_position[1],2));
        if(distance < 1.5){
            //ROBOT ARRIVATO A DESTINAZIONE
            ROS_INFO("arrived to the goal");
            //cosa faccio ora? 
            //verifico se devo prendere il pacco o se lo ho portato -> agisco di consegueza
            cruising=0;
        }
    }
}

void check2_callBack(const ros::TimerEvent& event){
    //check per vedere se è passato troppo tempo
    float distance;
    if(cruising!=0){
        distance = sqrt(pow(current_position[0]-target_position[0],2)+pow(current_position[1]-target_position[1],2));
        if(distance > 0.5){
            ROS_INFO("destination could not be reached");
            //scegliere cosa fare in questo caso 
        }
    }  
}

int main(int argc, char**argv) {

    ros::init(argc,argv,"move");
    ros::NodeHandle n;
    // topic: goal
    ros::Publisher pub = n.advertise<geometry_msgs::PoseStamped>("/move_base_simple/goal",1000);
    
    tf2_ros::TransformListener tfListener(tfBuffer);

    ros::Rate loop_rate(T);

    ros::Subscriber sub = n.subscribe("NewGoal",1000,setGoal_CallBack);
    ros::Subscriber sub_tf = n.subscribe("tf",1000,position_CallBack);
   
    ros::Timer timer1 = n.createTimer(ros::Duration(0,5),check1_callBack);
    ros::Timer timer2 = n.createTimer(ros::Duration(50),check2_callBack);

    int count = 0;
    while(ros::ok){
        //trovare un modo per verificare se il messaggio va pubblicato 
        if(message_published){
            ROS_INFO("Publishing a new goal position");
            pub.publish(new_goal_msg); //PUÒ ESSERE FATTA DIRETTAMENTE NELLA CALLBACK?
            message_published=0;
        }
        ros::spinOnce();
        loop_rate.sleep();
        ++count;
    }
    return 0;
    
}