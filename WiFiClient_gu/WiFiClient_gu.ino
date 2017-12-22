/*这篇把8266作为TCPcleint，加入电脑创建的tcpServer中
来控制一个继电器*/
 
#include <ESP8266WiFi.h>

 
#define relay1 2 //继电器连接在8266的GPIO2上
//#define relay2 0 //继电器连接在8266的GPIO2上
#define sensor 3 //继电器连接在8266的GPIO2上
//#define relay4 5 //继电器连接在8266的GPIO2上
const char *ssid     = "TP-LINK_gu_link";//"Enterprise Customer Department";//这里是我的wifi，你使用时修改为你要连接的wifi ssid
const char *password = "telecom2002";//"xzg18945471272";//你要连接的wifi密码
const char *host = "192.168.1.110";//219.147.235.204";//修改为你建立的Server服务端的IP地址
WiFiClient client;
const int tcpPort = 9999;//修改为你建立的Server服务端的端口号
String macaddress;
int sts=0;
String cmd;
void setup()
{
    Serial.begin(115200,SERIAL_8N1,SERIAL_TX_ONLY);    
    pinMode(relay1,OUTPUT);
    digitalWrite(relay1, HIGH);
    //pinMode(relay2,OUTPUT);
    //digitalWrite(relay2, HIGH);
    pinMode(sensor,INPUT_PULLUP);
    delay(10);
    Serial.println();
    Serial.println();
    Serial.print("Connecting to ");//写几句提示，哈哈
    Serial.println(ssid);
 
    WiFi.begin(ssid, password);
 
    while (WiFi.status() != WL_CONNECTED)//WiFi.status() ，这个函数是wifi连接状态，返回wifi链接状态
                                         //这里就不一一赘述它返回的数据了，有兴趣的到ESP8266WiFi.cpp中查看
    {
        delay(500);
        Serial.print(".");
    }//如果没有连通向串口发送.....
 
    Serial.println("");
    Serial.println("WiFi connected");
    Serial.println("IP address: ");
    Serial.println(WiFi.localIP());//WiFi.localIP()返回8266获得的ip地址
    macaddress=WiFi.macAddress();
    Serial.println(macaddress);//WiFi.localIP()返回8266获得的ip地址
    cmd="hrt"+macaddress;
}
 
 
void loop()
{
    while (!client.connected())//几个非连接的异常处理
    {
       
        if (!client.connect(host, tcpPort))
        {
            Serial.println("connection....");
            //client.stop();
            delay(500);
 
        }
    }
    //cmd="hrt"+macaddress;
    //Serial.print(cmd);
    
    while (client.available())//available()同ARDUINO，不解释了
    {
        //Serial.print("client available");
        String val = client.readStringUntil('\n');
        
        Serial.println(val);
        if(val.charAt(0)=='0')
        {
           if(val.charAt(1)=='0')
           {
              digitalWrite(relay1, LOW);
           }
           else
           {
              digitalWrite(relay1, HIGH);
           }
           cmd="hrt"+macaddress;
        }
        if(val.charAt(0)=='1')
        {
           if(val.charAt(1)=='0')
           {
              digitalWrite(relay1, LOW);
           }
           if(val.charAt(1)=='1')
           {
              digitalWrite(relay1, HIGH);
           }
           if(val.charAt(1)=='2')
           {
              digitalWrite(relay1, HIGH);
           }
           if(val.charAt(1)=='3')
           {
              digitalWrite(relay1, HIGH);
           }
           if(val.charAt(1)=='4')
           {
              digitalWrite(relay1, HIGH);
           }
           cmd="hrt"+macaddress;
        }
        if(val.charAt(0)=='2')
        {
           if(digitalRead(sensor)==HIGH)
           {
              cmd="sen"+macaddress;
               digitalWrite(relay1, HIGH);
           }else{
              cmd="non"+macaddress;
               digitalWrite(relay1, LOW);
           }
           //Serial.println(cmd); 
        }
      }
        if(sts==0){client.print("add"+macaddress);sts=1;} else client.print(cmd);
        /*
        if(val.indexOf("LIGHTOFF")>=0 )//|| digitalRead(relay3)==LOW)
        {//pc端发送0和1来控制
           digitalWrite(relay1, LOW);
           //digitalWrite(relay2, LOW);
           //digitalWrite(relay3, LOW);
           //digitalWrite(relay4, LOW);
           //Serial.println("realy low");
        }
        if(val.indexOf("LIGHTONE")>=0 )//&& digitalRead(relay3)==HIGH)
        {
            digitalWrite(relay1, HIGH);
            //digitalWrite(relay2, HIGH);
            //digitalWrite(relay3, LOW);
            //digitalWrite(relay4, LOW);
            //Serial.println("light one");
        }
        
        if(val.indexOf("LIGHTTWO")>=0)
        {
            digitalWrite(relay1, HIGH);
            digitalWrite(relay2, HIGH);
            //digitalWrite(relay3, LOW);
            //digitalWrite(relay4, LOW);
            Serial.println("two high");
        }
        
        if(val.indexOf("LIGHTTHREE")>=0)
        {
            digitalWrite(relay1, HIGH);
            digitalWrite(relay2, HIGH);
            digitalWrite(relay3, HIGH);
            digitalWrite(relay4, LOW);
           // Serial.println("three high");
        }
        if(val.indexOf("LIGHTFOUR")>=0)
        {
            digitalWrite(relay1, HIGH);
            digitalWrite(relay2, HIGH);
            digitalWrite(relay3, HIGH);
            digitalWrite(relay4, HIGH);
           // Serial.println("four high");
        }
        */
    
    //delay(500); 
 
}
