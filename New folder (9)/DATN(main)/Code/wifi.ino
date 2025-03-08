#include <SoftwareSerial.h>
#include <ESP8266WiFi.h>
#include <FirebaseESP8266.h>
#define FIREBASE_HOST "sonhademo.firebaseio.com"
#define FIREBASE_AUTH "GqoZJn9BWCBBtmT6g3PGycDxNAR6yWDldnxCwnnF"
#define WIFI_SSID "autorace_2.4G"
#define WIFI_PASSWORD "autoraceonly"
SoftwareSerial ESP826612E(D3,D2);
FirebaseData firebaseData;
void setup()
{
	Serial.begin(9600);
	ESP826612E.begin(9600);
  Connectwifi();
  Firebase.begin(FIREBASE_HOST, FIREBASE_AUTH);
}
void Connectwifi()
{
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  Serial.print("connecting");
  while (WiFi.status() != WL_CONNECTED) 
  {
  Serial.print(".");
  delay(500);
  }
  Serial.println();
  Serial.print("connected: ");
  Serial.println(WiFi.localIP());
}
void receiveATmega328() 
 {
  while(ESP826612E.available()>0)
  {
  String dataPic = ESP826612E.readStringUntil('%');
  Serial.println(dataPic);
  if(dataPic.startsWith("*")){
    dataPic = dataPic.substring(1,dataPic.length());
    dataPic.concat("#");
    String value[12];
    for (int i=0; i<12; i++){
      value[i] = dataPic.substring(0,dataPic.indexOf("#",0));
      dataPic.remove(0,dataPic.indexOf("#",0)+1);
      }
      int device = value[0].toInt();
      Serial.println(device);
        switch(device)
        {
        case 1:
        Firebase.setString(firebaseData, "/thietbi/switch1",value[6]);
        Firebase.setString(firebaseData, "/thietbi/switch2",value[7]);
        Firebase.setString(firebaseData, "/thietbi/switch3",value[8]);
        Firebase.setString(firebaseData, "/thietbi/switch4",value[9]);
        Firebase.setString(firebaseData, "/thietbi/switch5",value[10]);
        Firebase.setString(firebaseData, "/thietbi/switch6",value[11]);
        value[2].remove(value[2].indexOf(".",0),3);
        value[3].remove(value[3].indexOf(".",0),3);
        value[4].remove(value[4].indexOf(".",0),3);
        value[5].remove(value[5].indexOf(".",0),3);
        Firebase.setString(firebaseData, "/nhakinh/doamdat",value[1]);
        Firebase.setString(firebaseData, "/nhakinh/khongkhi",value[2]);
        Firebase.setString(firebaseData, "/nhakinh/doam",value[3]);
        Firebase.setString(firebaseData, "/nhakinh/nhietdo",value[4]);
        Firebase.setString(firebaseData, "/nhakinh/anhsang",value[5]);
        break;
        case 0:
        value[2].remove(value[2].indexOf(".",0),3);
        value[3].remove(value[3].indexOf(".",0),3);
        value[4].remove(value[4].indexOf(".",0),3);
        value[5].remove(value[5].indexOf(".",0),3);
        Firebase.setString(firebaseData, "/nhakinh/doamdat",value[1]);
        Firebase.setString(firebaseData, "/nhakinh/khongkhi",value[2]);
        Firebase.setString(firebaseData, "/nhakinh/doam",value[3]);
        Firebase.setString(firebaseData, "/nhakinh/nhietdo",value[4]);
        Firebase.setString(firebaseData, "/nhakinh/anhsang",value[5]);
        break;
        default :
        Serial.print("Thiet bi chua duoc khai bao");
}
}
else
{
  Serial.println("Khong nhan duoc ky tu");
}
}
}
void sentATmega328()
{
  String valuedata;
  String switch1[12];
  Firebase.getString(firebaseData, "/thietbi/switch1");
  switch1[0]=firebaseData.stringData();
  Firebase.getString(firebaseData, "/thietbi/switch2");
  switch1[1]=firebaseData.stringData();
  Firebase.getString(firebaseData, "/thietbi/switch3");
  switch1[2]=firebaseData.stringData();
  Firebase.getString(firebaseData, "/thietbi/switch4");
  switch1[3]=firebaseData.stringData();
  Firebase.getString(firebaseData, "/thietbi/switch5");
  switch1[4]=firebaseData.stringData();
  Firebase.getString(firebaseData, "/thietbi/switch6");
  switch1[6]=firebaseData.stringData();
  Firebase.getString(firebaseData, "/thietbi/allow1");
  switch1[5]=firebaseData.stringData();
  Firebase.getString(firebaseData, "/thietbi/allow2");
  switch1[7]=firebaseData.stringData();
  Firebase.getString(firebaseData, "/thietbi/allow3");
  switch1[8]=firebaseData.stringData();
  Firebase.getString(firebaseData, "/thietbi/allow4");
  switch1[9]=firebaseData.stringData();
  Firebase.getString(firebaseData, "/thietbi/allow5");
  switch1[10]=firebaseData.stringData();
  Firebase.getString(firebaseData, "/thietbi/allow6");
  switch1[11]=firebaseData.stringData();
  if(switch1[5]=="1")
      {
  valuedata="*"+switch1[0]+"#"+switch1[1]+"#"+switch1[2]+"#"+switch1[3]+"#"+switch1[4]+"#"+switch1[6]+"%";
  ESP826612E.print(valuedata);  
  Firebase.setString(firebaseData, "/thietbi/allow1","0");
      }
  else if(switch1[7]=="1")
      {
  valuedata="*"+switch1[0]+"#"+switch1[1]+"#"+switch1[2]+"#"+switch1[3]+"#"+switch1[4]+"#"+switch1[6]+"%";
  ESP826612E.print(valuedata);
  Firebase.setString(firebaseData, "/thietbi/allow2","0");
      }
  else if(switch1[8]=="1")
      {
  valuedata="*"+switch1[0]+"#"+switch1[1]+"#"+switch1[2]+"#"+switch1[3]+"#"+switch1[4]+"#"+switch1[6]+"%";
  ESP826612E.print(valuedata);
  Firebase.setString(firebaseData, "/thietbi/allow3","0");
      }
  else if(switch1[9]=="1")
      {
  valuedata="*"+switch1[0]+"#"+switch1[1]+"#"+switch1[2]+"#"+switch1[3]+"#"+switch1[4]+"#"+switch1[6]+"%";
  ESP826612E.print(valuedata);
  Firebase.setString(firebaseData, "/thietbi/allow4","0");
      }
  else if(switch1[10]=="1")
      {
  valuedata="*"+switch1[0]+"#"+switch1[1]+"#"+switch1[2]+"#"+switch1[3]+"#"+switch1[4]+"#"+switch1[6]+"%";
  ESP826612E.print(valuedata);
  Firebase.setString(firebaseData, "/thietbi/allow5","0");
      }
  else if(switch1[11]=="1")
      {
  valuedata="*"+switch1[0]+"#"+switch1[1]+"#"+switch1[2]+"#"+switch1[3]+"#"+switch1[4]+"#"+switch1[6]+"%";
  ESP826612E.print(valuedata);
  Firebase.setString(firebaseData, "/thietbi/allow6","0");
      }
}
void loop()
{
  if(ESP826612E.available()){   
    receiveATmega328();
    sentATmega328();
}
}
 
