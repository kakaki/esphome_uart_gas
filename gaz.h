#include "esphome.h"

class Gaz : public PollingComponent, public UARTDevice {
 public:
 
  //sample data sensors
  Sensor *counter = new Sensor(); 
  Sensor *voltage = new Sensor(); 
  Sensor *level = new Sensor(); 
  
  Gaz(UARTComponent *parent) : PollingComponent(5000), UARTDevice(parent) {}
  
  String getValue(String data, char separator, int index) {
    int found = 0;
    int strIndex[] = { 0, -1 };
    int maxIndex = data.length() - 1;

    for (int i = 0; i <= maxIndex && found <= index; i++) {
        if (data.charAt(i) == separator || i == maxIndex) {
            found++;
            strIndex[0] = strIndex[1] + 1;
            strIndex[1] = (i == maxIndex) ? i+1 : i;
        }
    }
    return found > index ? data.substring(strIndex[0], strIndex[1]) : " blad ";
	}


  void setup() override {
  }

  void update() override {
  
    String line = "";
	  while (available()) {
	    line = readStringUntil('\n');
	    ESP_LOGD("custom", "readline %s", line.c_str());
	    }
    
    if (line!="") {

			String l1 = getValue(line, ';', 0);
			String l2 = getValue(line, ';', 1); 
			String l3 = getValue(line, ';', 2);     
			String l4 = getValue(line, ';', 3);     
			
			ESP_LOGD("custom", "l1 %s", l1.c_str());
			ESP_LOGD("custom", "l2 %s", l2.c_str());
			ESP_LOGD("custom", "l3 %s", l3.c_str());
			ESP_LOGD("custom", "l4 %s", l4.c_str());

			float licznik = l1.toFloat();
			float V = l2.toFloat();
			float a = l3.toFloat();			
			int sum = l4.toInt();			
			int checksum = 2*licznik+a+V+2020;
			
			if (sum==checksum) {
			    ESP_LOGD("custom", "sum ok %i vs %i", sum, checksum);
			    counter->publish_state(licznik/100);
			    voltage->publish_state(V);
			    level->publish_state(a);    
			}
			else {
			    ESP_LOGD("custom", "sum error %i vs %i", sum, checksum);
			}
		}
  }
};