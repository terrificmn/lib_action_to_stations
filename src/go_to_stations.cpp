#include <ros/ros.h>
#include <actionlib/client/simple_action_client.h>

#include <move_base_msgs/MoveBaseAction.h>
#include <move_base_msgs/MoveBaseGoal.h>
#include <geometry_msgs/Pose.h>


class FollowWp2 {
private:
    std::string frame_id, odom_frame_id, base_frame_id;
    geometry_msgs::Pose pose_msg;

protected:
    ros::NodeHandle nh;
    actionlib::SimpleActionClient<move_base_msgs::MoveBaseAction> ac;    
    ros::AsyncSpinner spinner;
    ros::Subscriber pose_sub;

public:
    FollowWp2() : ac("move_base", true) , nh("~"), spinner(0) {
        ROS_INFO("Waypoint_Action_Client started!");
        // /// actionlib 만들기 (simpleactionclient)
        this->pose_sub = nh.subscribe<geometry_msgs::Pose>("/station_xy", 1, &FollowWp2::poseCb, this);
        int wait_count = 0;
        // while(!ac.waitForServer(ros::Duration(5.0))) { 
        //     ROS_INFO("Waiting for action server...");
        //     wait_count++;
        //     if(wait_count == 2) {
        //         ROS_ERROR("No response from action server... Exit");
        //         ros::shutdown();
        //         break;
        //     }
        // }
        spinner.start();
    }

    void poseCb(const geometry_msgs::Pose::ConstPtr &msg) {
        this->pose_msg.position.x = msg->position.x;
        this->pose_msg.position.y = msg->position.y;
        this->pose_msg.position.z = 0.0;
        this->pose_msg.orientation.x = 0.0;
        this->pose_msg.orientation.y = 0.0;
        this->pose_msg.orientation.z = -0.796423;
        this->pose_msg.orientation.w = 0.804740;
        ROS_INFO("now subscribed...");
    }


    // action client execute
    // void excuteSendGoal(const geometry_msgs::PoseStamped::ConstPtr &msg) {
    void excuteSendGoal() {
        ROS_WARN("BT SendGoal()");
        
        ros::Duration(2.0).sleep();
        move_base_msgs::MoveBaseGoal goal;
        goal.target_pose.header.frame_id = "map";
        goal.target_pose.pose.position = this->pose_msg.position;
        goal.target_pose.pose.orientation = this->pose_msg.orientation;

        int sec_for_waiting = 10;
        ROS_INFO("Connect to the action server ...");
        if (ac.waitForServer(ros::Duration(sec_for_waiting, 0))) {
        
        }
        //send goal
        ac.sendGoal(goal);

        
        ac.waitForResult();
        
        if(ac.getState() == actionlib::SimpleClientGoalState::SUCCEEDED) {
            ROS_WARN("We're here!");
        } else {
            ROS_INFO("We couldn't get there.");
        }
    } 
};

int main(int argc, char** argv) {
    ros::init(argc, argv, "waypoint_action_client");
    
    FollowWp2 fwObj;
    fwObj.excuteSendGoal();

    ros::waitForShutdown();

    return 0;
}

