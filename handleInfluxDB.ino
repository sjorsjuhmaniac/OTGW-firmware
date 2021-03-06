/*
***************************************************************************  
**  Program  : handleInfluxDB 
**
**  Version  : v0.8.0
**
**  Copyright (c) 2021 Robert van den Breemen
**
**  Description: Handles sending data directly to an influxdb instance
**
**  TERMS OF USE: MIT License. See bottom of file.                                                            
***************************************************************************      
*/  

// InfluxDB  server url. Don't use localhost, always server name or ip address.
// For InfluxDB 2 e.g. http://192.168.1.48:9999 (Use: InfluxDB UI -> Load Data -> Client Libraries), 
// For InfluxDB 1 e.g. http://192.168.1.48:8086
#define INFLUXDB_URL "http://192.168.88.254:8086"
//#define INFLUXDB_URL "http://127.0.0.1:8086"
// InfluxDB 2 server or cloud API authentication token (Use: InfluxDB UI -> Load Data -> Tokens -> <select token>)
//#define INFLUXDB_TOKEN "toked-id"
// InfluxDB 2 organization id (Use: InfluxDB UI -> Settings -> Profile -> <name under tile> )
//#define INFLUXDB_ORG "org"
// InfluxDB 2 bucket name (Use: InfluxDB UI -> Load Data -> Buckets)
//#define INFLUXDB_BUCKET "bucket"
// InfluxDB v1 database name 
#define INFLUXDB_DB_NAME "otgw"
#include <InfluxDbClient.h>

// InfluxDB client instance
//InfluxDBClient client(INFLUXDB_URL, INFLUXDB_ORG, INFLUXDB_BUCKET, INFLUXDB_TOKEN);
// InfluxDB client instance for InfluxDB 1
InfluxDBClient client(INFLUXDB_URL, INFLUXDB_DB_NAME);

// Set timezone string according to https://www.gnu.org/software/libc/manual/html_node/TZ-Variable.html
// Examples:
//  Pacific Time:   "PST8PDT"
//  Eastern:        "EST5EDT"
//  Japanesse:      "JST-9"
//  Central Europe: "CET-1CEST,M3.5.0,M10.5.0/3"
// #define TZ_INFO "CET-1CEST,M3.5.0,M10.5.0/3"
#define WRITE_PRECISION WritePrecision::S
#define MAX_BATCH_SIZE 16
#define WRITE_BUFFER_SIZE 32



void startInfluxDB()
{
  if (!settingInfluxDBenable) return;
  if ( sizeof(settingInfluxDBhostname) < 8 )  return; 

 // Set InfluxDB 1 authentication params
  // Only needed when query's are done?
  //client.setConnectionParamsV1(INFLUXDB_URL, INFLUXDB_DB_NAME, INFLUXDB_USER, INFLUXDB_PASSWORD);   
  DebugTf("InfluxDB Connection Setup: [%s] - database: [%s]\r\n", cMsg , CSTR(settingInfluxDBdatabasename));
  snprintf(cMsg, sizeof(cMsg), "http://%s:%d", CSTR(settingInfluxDBhostname), settingInfluxDBport);
  client.setConnectionParamsV1(cMsg, CSTR(settingInfluxDBdatabasename));
  Debugln("InfluxDB client setConnectionsParamsV1 done!");
  // Connect to influxdb server connection

  if (client.validateConnection()) {
    DebugTf("InfluxDB connected to: %s\r\n", CSTR(client.getServerUrl()));
  } else {
    DebugTf("InfluxDB connection failed: %s\r\n", CSTR(client.getLastErrorMessage()));
  }
    
  //Enable messages batching and retry buffer
  //deprecated writeoptions: 
  client.setWriteOptions(WRITE_PRECISION, MAX_BATCH_SIZE, WRITE_BUFFER_SIZE);
  
  // client.setWriteOptions(WriteOptions().writePrecision(WRITE_PRECISION));
  // client.setWriteOptions(WriteOptions().batchSize(MAX_BATCH_SIZE));
  // client.setWriteOptions(WriteOptions().bufferSize(WRITE_BUFFER_SIZE));

  //setup the HTTPoptions to reuse HTTP
  //client.setHTTPOptions(HTTPOptions().connectionReuse(true));
}


void writeInfluxDataPoint(const char *key, const char *value, const char *unit, const time_t epoch){
  if (!settingInfluxDBenable) return;
  if (!client.validateConnection()) {
    DebugTln("InfluxDB connection problem!");
    return; // only write if there is a valid connection to InfluxDB
  }

  time_t thisEpoch = now()- (isDST() ? 7200 : 3600);
  DebugTf("InfluxDB write [%s]=[%s] [%s] (%d) | epoch (%d)", key, value, unit, epoch, thisEpoch );  

  Point pointItem(unit);
  //pointItem.setTime(thisEpoch);
  pointItem.addTag("instance", key);     
  pointItem.addField("value", value);
  pointItem.addField("epoch", epoch);
  
  // DebugTf("Writing to influxdb: %s", CSTR(pointItem.toLineProtocol()));          
  if (!client.writePoint(pointItem)) DebugTf("InfluxDB write failed: ", CSTR(client.getLastErrorMessage()));
}


void writeInfluxDataPoint(const char *key, const int value, const char *unit, const time_t epoch){
  if (!settingInfluxDBenable) return;
  if (!client.validateConnection()) {
    DebugTln("InfluxDB connection problem!");
    return; // only write if there is a valid connection to InfluxDB
  }

  time_t thisEpoch = now()- (isDST() ? 7200 : 3600);
  DebugTf("InfluxDB write [%s]=[%s] [%s] (%d) | epoch (%d)", key, value, unit, epoch, thisEpoch );  

  Point pointItem(unit);
  // pointItem.setTime(thisEpoch);
  pointItem.addTag("instance", key);     
  pointItem.addField("value", value);
  pointItem.addField("epoch", epoch);
  
  // DebugTf("Writing to influxdb: %s", CSTR(pointItem.toLineProtocol()));          
  if (!client.writePoint(pointItem)) DebugTf("InfluxDB write failed: ", CSTR(client.getLastErrorMessage()));
}

void writeInfluxDataPoint(const char *key, const bool value, const char *unit, const time_t epoch){
  if (!settingInfluxDBenable) return;
  if (!client.validateConnection()) {
    DebugTln("InfluxDB connection problem!");
    return; // only write if there is a valid connection to InfluxDB
  }

  time_t thisEpoch = now()- (isDST() ? 7200 : 3600);
  DebugTf("InfluxDB write [%s]=[%s] [%s] (%d) | epoch (%d)", key, value, unit, epoch, thisEpoch );  

  Point pointItem(unit);
  // pointItem.setTime(thisEpoch);
  pointItem.addTag("instance", key);     
  pointItem.addField("value", value);
  pointItem.addField("epoch", epoch);
  
  // DebugTf("Writing to influxdb: %s", CSTR(pointItem.toLineProtocol()));          
  if (!client.writePoint(pointItem)) DebugTf("InfluxDB write failed: ", CSTR(client.getLastErrorMessage()));
}

void writeInfluxDataPoint(const char *key, const float value, const char *unit, const time_t epoch){
  if (!settingInfluxDBenable) return;
  if (!client.validateConnection()) {
    DebugTln("InfluxDB connection problem!");
    return; // only write if there is a valid connection to InfluxDB
  }

  time_t thisEpoch = now()- (isDST() ? 7200 : 3600);
  DebugTf("InfluxDB write [%s]=[%s] [%s] (%d) | epoch (%d)", key, value, unit, epoch, thisEpoch );  

  Point pointItem(unit);
  //pointItem.setTime(thisEpoch);
  pointItem.addTag("instance", key);     
  pointItem.addField("value", value);
  pointItem.addField("epoch", epoch);
  
  // DebugTf("Writing to influxdb: %s", CSTR(pointItem.toLineProtocol()));          
  if (!client.writePoint(pointItem)) DebugTf("InfluxDB write failed: ", CSTR(client.getLastErrorMessage()));
}

/***************************************************************************
*
* Permission is hereby granted, free of charge, to any person obtaining a
* copy of this software and associated documentation files (the
* "Software"), to deal in the Software without restriction, including
* without limitation the rights to use, copy, modify, merge, publish,
* distribute, sublicense, and/or sell copies of the Software, and to permit
* persons to whom the Software is furnished to do so, subject to the
* following conditions:
*
* The above copyright notice and this permission notice shall be included
* in all copies or substantial portions of the Software.
*
* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS
* OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
* MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
* IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY
* CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT
* OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR
* THE USE OR OTHER DEALINGS IN THE SOFTWARE.
* 
***************************************************************************/
