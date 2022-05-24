#include <iostream>
#include <ros/ros.h>
#include <stdio.h>
#include <mosquitto.h>
#include <jsoncpp/json/json.h>
#include <queue>
#include <mutex>
#include <thread>

#define ANSI_COLOR_RED "\x1b[31m" 
#define ANSI_COLOR_GREEN "\x1b[32m" 
#define ANSI_COLOR_YELLOW "\x1b[33m" 
#define ANSI_COLOR_BLUE "\x1b[34m" 
#define ANSI_COLOR_MAGENTA "\x1b[35m" 
#define ANSI_COLOR_CYAN "\x1b[36m" 
#define ANSI_COLOR_RESET "\x1b[0m"

struct GQ
{
    uint32_t X;
    uint32_t Y;
    uint32_t H;
    uint32_t State;
    uint32_t ActionID;
    uint32_t Traffic;
    uint32_t Mode;
    uint32_t Clamp;
    uint32_t Loaded;
    uint32_t Level;
    uint32_t CurrentNode;
    uint32_t NextNode;
    uint32_t TargetNode;
    uint32_t Battery;
    uint32_t Obstacle;
    uint32_t Lift;
    uint32_t Tilt;
    uint32_t SideShift;
    uint32_t ForkMover;
    uint32_t PalletFrontX;
    uint32_t PalletFrontY;
    uint32_t PalletFrontTheta;
    uint32_t PalletStackingX;
    uint32_t PalletStackingY;
    uint32_t PalletStackingTheta;
    uint32_t TagId;
    uint32_t ErrorCode;
};
struct EI
{
    uint32_t X;
    uint32_t Y;
    uint32_t Z;
};

struct GB
{
    uint32_t BCellVolt;
    uint32_t BPackVolt;
    uint32_t BPackCurrent;
    uint32_t BChargeVolt;
    uint32_t BTemperature;
    uint32_t Battery;
    uint32_t BError;
    uint32_t BWarning;
    uint32_t ErrorCode;
};
struct GD
{
    uint32_t TMapVersion;
    uint32_t GMapVersion;
    uint32_t FWVersion;
    uint32_t ErrorCode;
};
/*RECEIVE ORDER*/

void connect(struct mosquitto *mosq, void *obj, int rc);
void message(struct mosquitto *mosq, void *obj, const struct mosquitto_message *msg);
void disconnect(struct mosquitto *mosq, void *obj, int rc);
void subscribe(struct mosquitto *mosq, void *obj,int mid,int qos_count,const int* granted_qos);
class mqtt_subscriber
{
    public:
        std::vector<char*> order_vec;
        std::mutex m;
        struct GQ* GQ_;
        struct EI* EI_;
        struct GB* GB_;
        struct GD* GD_;
        int sequence;
        bool onetime;
        std::string topic_send;
        std::string topic_receive;
        void mqtt_connect(const char * host,const int port, const std::string ID);
        void mqtt_send(const char* order,const void* array);
        void mqtt_receive_request();
        void publish();
        void mqtt_status_update(const char* order, void* array);
        
        mqtt_subscriber();
        ~mqtt_subscriber();
    private:

        std::string convert_array_to_json(const char* order,const void* array);
        void Print_Error(int Error);
        int mqtt_init(const char * host,const int port, const std::string ID);
        struct mosquitto *mosq;
        int error;
        
};
