#include "mqtt_subscriber.h"

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
	"MOSQ_ERR_ERROR",
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
	"MOSQ_ERR_OCSP"
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

const char* EI[]={
    "X",
    "Y",
    "H"
};
const char* GB[]={
    "BCellVolt",
    "BPackVolt",
    "BPackCurrent",
    "BChargeVolt",
    "BTemperature",
    "Battery",
    "BError",
    "BWarning",
    "ErrorCode"
};
const char* GQ[]={
    "X",
    "Y",
    "H",
    "State",
    "ActionID",
    "Traffic",
    "Mode",
    "Clamp",
    "Loaded",
    "Level",
    "CurrentNode",
    "NextNode",
    "TargetNode",
    "Battery",
    "Obstacle",
    "Lift",
    "Tilt",
    "SideShift",
    "ForkMover",
    "PalletFrontX",
    "PalletFrontY",
    "PalletFrontTheta",
    "PalletStackingX",
    "PalletStackingY",
    "PalletStackingTheta",
    "TagId",
    "ErrorCode"
};
const char* GD[]={
    "TMapVersion",
    "GMapVersion",
    "FWVersion",
    "ErrorCode"
};
const char* RES[]={
    "Mode"
    "PathList"
    "Node"
    "Work"
    "ErrorCode"  
};
mqtt_subscriber::mqtt_subscriber():topic_receive("ACS"+ID+">AMR"+ID),topic_send("AMR"+ID+">ACS"+ID),onetime(false)
{
    // ros::NodeHandle nh("~");
    // std::string temp="AMR001";

    // nh.param("Robot_id",Robot_id,temp);
    mqtt_connect(host,port,ID);
    // getchar();
    // while(ros::ok())
    // {
    //     //페어링 기능 완료시 다시 작동 x -> 연결 불가시 다시 페어링 모드로 복귀 -> 페어링 모드가 오래 지속될경우 베이스로 복귀
    //     //연결 불가능 감지 및 행동
    //     //센서 데이터 및 요청 데이터 publish
    //     //일반 명령 체크 (queue) 명령이 0개일때는 mutex 작동 1개 이상일시는 mutex 작동 안함 
    //     //긴급 명령 체크 예) 로봇 정지 및 종료
    //     //로봇 제어
    // }
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
    
    // int loop = mosquitto_loop_start(mosq);
    int loop = mosquitto_loop_forever(mosq,5,1);
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
    printf("%s\n", msg_con);

    Json::Value root;
    Json::Reader reader;
    reader.parse(msg_con,root);
    uint32_t* array;
    while(mos->sequence<18)
    {
        if(strcmp(root.get("Cmd","UTF-8").asCString(),enum_Order[mos->sequence])==0)
        {
            break;
        }
        mos->sequence++;
    }

    switch (mos->sequence)
    {
    case 0://AR
    //AR에 대한 기능
    //AR에 대한 array 제작
    mos->mqtt_send("AR",array);//array 값이 변경되어야함
        break;
    case 1://AC
    mos->mqtt_send("AC",array);
        break;
    case 2://AP
    mos->mqtt_send("AP",array);
        break;
    case 3://AJ
    mos->mqtt_send("AJ",array);
        break;
    case 4://SL
    mos->mqtt_send("SL",array);
        break;
    case 5://SP
    mos->mqtt_send("SP",array);
        break;
    case 6://AW
    mos->mqtt_send("AW",array);
        break;
    case 7://AG
    mos->mqtt_send("AG",array);
        break;
    case 8://AD
    mos->mqtt_send("AD",array);
        break;
    case 9://TS
    mos->mqtt_send("TS",array);
        break;
    case 10://TG
    mos->mqtt_send("TG",array);
        break;
    case 11://GS
    mos->mqtt_send("GS",array);
        break;
    case 12://GG
    mos->mqtt_send("GG",array);
        break;
    case 13://GQ
    mos->mqtt_send("GQ",array);
        break;
    case 14://EI
    mos->mqtt_send("EI",array);
        break;
    case 15://GB
    mos->mqtt_send("GB",array);
        break;
    case 16://GD
    mos->mqtt_send("GD",array);
        break;
    case 17://DC
    mos->mqtt_send("DC",array);
        break;
    }

    
    // if(mos->onetime==false)
    // {
    //     // int array[3]={10,10,10};
    //     // int array[4]={5,2,20,20,};
    //     int array[27]={1,1,1,1,1,1,4,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1};
    //     // mos->mqtt_send_EI(array);
        // mos->mqtt_send("GD",array);
    //     mos->onetime=true;
    // }
    
    // std::cout <<root.get("Cmd","UTF-8")<<std::endl;
    mosquitto_publish(mosq,NULL,(mos->topic_send).c_str(),msg->payloadlen,msg,0,0);

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
void mqtt_subscriber::mqtt_send(const char * order,const void* array)
{
    std::string msg=convert_array_to_json(order,array);
    Print_Error(mosquitto_publish(mosq,NULL,topic_send.c_str(),msg.size(),msg.c_str(),0,0));
}
std::string mqtt_subscriber::convert_array_to_json(const char* order,const void* array)
{   
    Json::Value root;
    Json::StyledWriter writer;
    int arr_size;
    int* temp_arr_int=(int*)array;
    if(strcmp(order,"EI")==0)
    {
        // arr_size=sizeof(array)/sizeof(temp_arr_int[0]); 왜 array 가 8바이트이지?
        arr_size=3;
        root["Cmd"]="EI";
        for(int i=0;i<arr_size;i++)
        {
            root[EI[i]]=(std::to_string(temp_arr_int[i]).c_str());
        }
    }        
    else if(strcmp(order,"GB")==0)
    {
        // arr_size =sizeof(temp_arr_double)/sizeof(temp_arr_double[0]);
        arr_size=9;
        root["Cmd"]="GB";
        root["Result"]="S";
        for(int i=0;i<arr_size;i++)
        {
            root[GB[i]]=(std::to_string(temp_arr_int[i]).c_str());
        }
    } 
    else if(strcmp(order,"GQ")==0)
    {
        // arr_size =sizeof(temp_arr_int)/sizeof(temp_arr_int[0]);
        arr_size=27;
        root["Cmd"]="GQ";
        root["Result"]="S";
        for(int i=0;i<arr_size;i++)
        {
            root[GQ[i]]=(std::to_string(temp_arr_int[i]).c_str());
        }
    }
    else if(strcmp(order,"GD")==0)
    {
        // arr_size =sizeof(temp_arr_double)/sizeof(temp_arr_int[0]);
        arr_size=4;
        root["Cmd"]="GD";
        root["Result"]="S";
        for(int i=0;i<arr_size;i++)
        {
            root[GD[i]]=(std::to_string(temp_arr_int[i]).c_str());
        }
    }
    else
    {
        arr_size=5;
        root["Cmd"]=order;
        root["Result"]="S";
        for(int i=0;i<arr_size;i++)
        {
            root[RES[i]]=(std::to_string(temp_arr_int[i]).c_str());
        }

    }
    std::string str=writer.write(root);
    return str;
}