#include "mqtt_subscriber.h"

// const char* enum_str[]={"AR","AC","AP","AD","DC"};
mqtt_subscriber::mqtt_subscriber():topic_receive("ACS"+ID+">"+"AMR"+ID),topic_send(" AMR001>ACS001")
{
    // ros::NodeHandle nh("~");
    // std::string temp="AMR001";

    // nh.param("Robot_id",Robot_id,temp);
    mqtt_connect(host,port,ID);
    getchar();
    while(ros::ok())
    {
        //페어링 기능 완료시 다시 작동 x -> 연결 불가시 다시 페어링 모드로 복귀 -> 페어링 모드가 오래 지속될경우 베이스로 복귀
        //연결 불가능 감지 및 행동
        //센서 데이터 및 요청 데이터 publish
        //일반 명령 체크 (queue) 명령이 0개일때는 mutex 작동 1개 이상일시는 mutex 작동 안함 
        //긴급 명령 체크 예) 로봇 정지 및 종료
        //로봇 제어
    }
}
mqtt_subscriber::~mqtt_subscriber()
{
    printf("Exit mqtt_subscriber\n");
    mosquitto_loop_stop(mosq,true);
    mosquitto_disconnect(mosq);
    mosquitto_destroy(mosq);
    mosquitto_lib_cleanup();
}
void mqtt_subscriber::mqtt_connect(const char * host,const int port, const std::string ID)
{
    Print_Error(mqtt_init(host,port,ID));
}
int mqtt_subscriber::mqtt_init(const char * host,const int port, const std::string ID)
{   
    mosquitto_lib_init();
    mosq=mosquitto_new("subscribe-test",true,this);
    if(mosq==NULL)
    {
        printf("can't initialize mqtt library\n");
        return 1;
    }
    error=mosquitto_connect(mosq,host,port,10);
    if(error!=0)
    {
        printf("Subscriber could not connect to broker! Error Code: %d\n",error);
        mosquitto_destroy(mosq);
        return error;
    }
    printf("We are now connected to the broker\n");
    mosquitto_connect_callback_set(mosq,connect);
    mosquitto_message_callback_set(mosq,message);
    // mosquitto_disconnect_callback_set(mosq,disconnect);
    // mosquitto_subscribe_callback_set(mosq,subscribe);
    
    int loop = mosquitto_loop_start(mosq);
    if(loop!=MOSQ_ERR_SUCCESS)
    {
        printf("mosquitto loop error\n");
        return loop;     
    }
    return MOSQ_ERR_SUCCESS;
}
void connect(struct mosquitto *mosq, void *obj, int rc)
{
    mqtt_subscriber* mos=(mqtt_subscriber*) obj;
    if(rc)
    {
        printf("Error with result code: %d\n",rc);
    }
    else
    {
        mosquitto_subscribe(mosq,NULL,(mos->topic_receive).c_str(),0);
    }   
}
void message(struct mosquitto *mosq, void *obj, const struct mosquitto_message *msg)
{
    mqtt_subscriber* mos=(mqtt_subscriber*) obj;
    // printf("New message with topic %s: %X\n",msg->topic,*(uint32_t*)msg->payload);
    char msg_con[msg->payloadlen];
    strcpy(msg_con,(const char*)msg->payload);
    printf("%s, %s\n",msg->topic, msg_con);

    //msg_payload 와 토픽에 따른 행동강령들
}
// void subscribe(struct mosquitto *mosq, void *obj,int mid,int qos_count,const int* granted_qos)
// {
//     mqtt_subscriber* mos=(mqtt_subscriber*) obj;
//     // printf("New subscribe with topic %d: %d\n",qos_count,*granted_qos);
//     //얜 뭔기능인지 잘 모르겠음   
// }
// void disconnect(struct mosquitto *mosq, void *obj, int rc)
// {
//     mqtt_subscriber* mos=(mqtt_subscriber*) obj;
//     printf("New disconnect %d, %d", *(int *)obj,rc);
//     //연결 불가시 명령어 flag 온
// }
void mqtt_subscriber::publish()
{
    mosquitto_publish(mosq,NULL,topic_send.c_str(),sizeof(Status_Feedback),Status_Feedback,0,0);
}
void mqtt_subscriber::Print_Error(int Error)
{
    Error+=4;
    if(Error!=4)
    {
        printf(ANSI_COLOR_RED "ERROR_OCCUR : %s" ANSI_COLOR_RESET "\n", enum_ERROR[Error]);
        this->~mqtt_subscriber();
    }
    else
    {
        printf(ANSI_COLOR_GREEN "OK" ANSI_COLOR_RESET "\n");
    }
}
//2개의 스레드가 필요
//1 명령을 감지하는 놈 -> mosquitto_message_callback_set
//2 명령을 처리하는 놈