#include "mqtt_subscriber.h"
using std::thread;
using namespace std;


void func1(void *mqtt_subscriber_)
{
    mqtt_subscriber* mqtt_temp=(mqtt_subscriber*)mqtt_subscriber_;
    mqtt_temp->mqtt_connect("192.168.200.220",1883,"001");
}
void func2(void *mqtt_subscriber_)
{
    sleep(1);//func1이 먼저 connect를 진행해줘야함
    mqtt_subscriber* mqtt_temp=(mqtt_subscriber*)mqtt_subscriber_;

    uint32_t a=0;
    while(true)
    {
        a++;
        GQ GQ_={a,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
        EI EI_={a,0,0};
        GB GB_={a,0,0,0,0,0,0,0,0};
        GD GD_={a,0,0,0};
        mqtt_temp->mqtt_status_update("GQ",&GQ_);
        mqtt_temp->mqtt_status_update("EI",&EI_);
        mqtt_temp->mqtt_status_update("GB",&GB_);
        mqtt_temp->mqtt_status_update("GD",&GD_);
        sleep(1);
    }
}
int main(int argc, char** argv)
{
    int result=0;
    mqtt_subscriber mqtt_subscriber_;
    thread t1(func1,&mqtt_subscriber_);
    thread t2(func2,&mqtt_subscriber_);
    t1.join();
    t2.join();
    return 0;
}

