#include <iostream>
#include <ros/ros.h>
#include <stdio.h>
#include <mosquitto.h>
#include <jsoncpp/json/json.h>
#include <queue>

#define ANSI_COLOR_RED "\x1b[31m" 
#define ANSI_COLOR_GREEN "\x1b[32m" 
#define ANSI_COLOR_YELLOW "\x1b[33m" 
#define ANSI_COLOR_BLUE "\x1b[34m" 
#define ANSI_COLOR_MAGENTA "\x1b[35m" 
#define ANSI_COLOR_CYAN "\x1b[36m" 
#define ANSI_COLOR_RESET "\x1b[0m"


/*SEND ORDER*/
//AMRID 차량번호
enum UpperType {none=0,Lift=1,CV_2=12,CV_3=13};
//X (mm)
//Y (mm)
//degree (0~360)
enum State {Wait,Run,Load,Unload,Run_com,Load_com,Unload_com,Traffic};
enum Charge_Status {unCharge,Charge};
enum Action_ID {Lift_on=1,Lift_off};
enum Mode {Manual,Auto};
//Loaded_1
//Loaded_2
//Loaded_3
//Current_Node
//Next_Node
//Target_Node
enum Obstacle {Idle,Warning};
//Speed
//Map_match_percent
//Error_code1
//Error_code2
//Error_code3
//Error_code4
//Error_code5
//Battery_Volt
//Battery_Temp
//Battery_Left
enum Manual_Complete {delete_order=1};
//Protocal_ver

/*RECEIVE ORDER*/

void connect(struct mosquitto *mosq, void *obj, int rc);
void message(struct mosquitto *mosq, void *obj, const struct mosquitto_message *msg);
void disconnect(struct mosquitto *mosq, void *obj, int rc);
void subscribe(struct mosquitto *mosq, void *obj,int mid,int qos_count,const int* granted_qos);
class mqtt_subscriber
{
    public:
        bool onetime;
        std::string topic_send;
        std::string topic_receive;
        void mqtt_connect(const char * host,const int port, const std::string ID);
        void mqtt_send(const char* order,const void* array);
        void mqtt_receive_request();
        void publish();
        mqtt_subscriber();
        ~mqtt_subscriber();
    private:
        std::string convert_array_to_json(const char* order,const void* array);
        void Print_Error(int Error);
        int mqtt_init(const char * host,const int port, const std::string ID);
        struct mosquitto *mosq;
        int error;
        uint8_t Status_Feedback[28];
};
