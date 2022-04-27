#include <iostream>
#include <ros/ros.h>
#include <stdio.h>
#include <mosquitto.h>

#define mos_host "localhost"
#define mos_port 1883
void on_connect(struct mosquitto *mosq, void *obj, int rc);
void on_message(struct mosquitto *mosq, void *obj, const struct mosquitto_message *msg);
class mqtt_subscriber
{
    public:
        mqtt_subscriber();
        ~mqtt_subscriber();
    private:
        struct mosquitto *mosq;
        int error;
        int id;
        // void on_connect(struct mosquitto *mosq, void *obj, int rc);
        // void on_message(struct mosquitto *mosq, void *obj, const struct mosquitto_message *msg);
};

mqtt_subscriber::mqtt_subscriber()
{
    id=12;
    mosquitto_lib_init();
    mosq=mosquitto_new("subscribe-test",true,&id);
    mosquitto_connect_callback_set(mosq,on_connect);
    mosquitto_message_callback_set(mosq,on_message);
    error=mosquitto_connect(mosq,mos_host,mos_port,10);

    if(error!=0)
    {
        printf("Subscriber could not connect to broker! Error Code: %d\n",error);
        // mosquitto_destroy(mosq);
    }
    printf("We are now connected to the broker");
    // mosquitto_subscribe();
    mosquitto_loop_start(mosq);
    printf("Press Enter to quit...\n");
    getchar();
    mosquitto_loop_stop(mosq,true);
}
mqtt_subscriber::~mqtt_subscriber()
{
    mosquitto_disconnect(mosq);
    mosquitto_destroy(mosq);
    mosquitto_lib_cleanup();
}

void on_connect(struct mosquitto *mosq, void *obj, int rc)
{
    printf("ID: %d\n",*(int*)obj);
    if(rc)
    {
        printf("Error with result code: %d\n",rc);
        exit(-1);
    }
    mosquitto_subscribe(mosq,NULL,"test/t1",0);
}
void on_message(struct mosquitto *mosq, void *obj, const struct mosquitto_message *msg)
{
    printf("New message with topic %s: %s\n",msg->topic,(char*)msg->payload);
}