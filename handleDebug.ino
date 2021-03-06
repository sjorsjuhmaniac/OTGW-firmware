void handleDebug(){
    if (TelnetStream.available()>0){
        //read the next 
        char c;
        c = TelnetStream.read();
        switch (c){
            case 'm':
                DebugTln("Debug Command ==> Configure MQTT Discovery");
                DebugTf("Enable MQTT: %s\r\n", CBOOLEAN(settingMQTTenable));
                doAutoConfigure();
            break;
            case 'r':
                DebugTln("Debug Command ==> Initialize MQTT");
                startMQTT();
            break;
            default:
            break;
        }

    }
}
