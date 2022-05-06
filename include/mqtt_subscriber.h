

#include <iostream>
#include <ros/ros.h>
#include <stdio.h>
#include <mosquitto.h>

#define ANSI_COLOR_RED "\x1b[31m" 
#define ANSI_COLOR_GREEN "\x1b[32m" 
#define ANSI_COLOR_YELLOW "\x1b[33m" 
#define ANSI_COLOR_BLUE "\x1b[34m" 
#define ANSI_COLOR_MAGENTA "\x1b[35m" 
#define ANSI_COLOR_CYAN "\x1b[36m" 
#define ANSI_COLOR_RESET "\x1b[0m"

const char *host="192.168.50.220";
const int port = 1883;
const std::string ID = "001";        

const char* enum_ERROR[]={
    "MOSQ_ERR_AUTH_CONTINUE",
	"MOSQ_ERR_NO_SUBSCRIBERS",
	"MOSQ_ERR_SUB_EXISTS",
	"MOSQ_ERR_CONN_PENDING",
	"MOSQ_ERR_SUCCESS",
	"MOSQ_ERR_NOMEM",
	"MOSQ_ERR_PROTOCOL",
	"MOSQ_ERR_INVAL",
	"MOSQ_ERR_NO_CONN",
	"MOSQ_ERR_CONN_REFUSED",
	"MOSQ_ERR_NOT_FOUND",
	"MOSQ_ERR_CONN_LOST",
	"MOSQ_ERR_TLS",
	"MOSQ_ERR_PAYLOAD_SIZE",
	"MOSQ_ERR_NOT_SUPPORTED",
	"MOSQ_ERR_AUTH",
	"MOSQ_ERR_ACL_DENIED",
	"MOSQ_ERR_UNKNOWN",
	"MOSQ_ERR_ERRNO",
	"MOSQ_ERR_EAI",
	"MOSQ_ERR_PROXY",
	"MOSQ_ERR_PLUGIN_DEFER",
	"MOSQ_ERR_MALFORMED_UTF8",
	"MOSQ_ERR_KEEPALIVE",
	"MOSQ_ERR_LOOKUP",
	"MOSQ_ERR_MALFORMED_PACKET",
	"MOSQ_ERR_DUPLICATE_PROPERTY",
	"MOSQ_ERR_TLS_HANDSHAKE",
	"MOSQ_ERR_QOS_NOT_SUPPORTED",
	"MOSQ_ERR_OVERSIZE_PACKET",
	"MOSQ_ERR_OCSP",
};

const char* enum_Order[]={
    "AR",//자동운형
    "AC",//운행 및 작업취소
    "AP",//일시정지/해제
    "AJ",//수동조작
    "SL",//근접센서 설정
    "SP",//네트워크 설정
    "AW",//Action Work
    "AG",//충전/충전해제
    "AD",//지게차 모드
    "TS",//T 맵 Set
    "TG",//T 맵 Get
    "GS",//G 맵 Set
    "GG",//G 맵 받기
    "GQ",//상태
    "EI",//초기상태 (?)
    "GB",//배터리 상태
    "GD",//버전 상태
    "DC" //수동 모드에서 작업삭제요청응답
};
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
        std::string topic_send;
        std::string topic_receive;
        void mqtt_connect(const char * host,const int port, const std::string ID);
        void publish();
        mqtt_subscriber();
        ~mqtt_subscriber();
    private:

        void Print_Error(int Error);
        int mqtt_init(const char * host,const int port, const std::string ID);
        struct mosquitto *mosq;
        int error;
        uint8_t Status_Feedback[28];
};
