# ESP32-Relay-Sleep-Control 

## What is it?
- This simple implementation aims to control a Wi-fi Mesh router to save energy and reduce exposure to electromagnetic radiation. Furthermore, using some RTOS functionalities, the system ensures that the ESP32 remains connected and that the relay remains in the state provided for the situation. The system also allows the ESP32 to go into deep sleep mode, waking up at a pre-set time.

## Tools use
- 1 ESP32
- 1 Relay
- Wifi


## How it works?


First, you must read the IP which the ESP32 connected to your local network in order to access the web page responsible for controlling the relay.

![image](https://user-images.githubusercontent.com/61205851/144754598-72e2e851-8bb6-4c31-acf3-f55dfdf5bb0c.png)

You should see the next:

<img src="https://user-images.githubusercontent.com/61205851/144754968-fcc137ad-0b5d-467b-9762-dc14adee19d7.png" width="300" height="300"/>

And the result should be:

<img src="https://user-images.githubusercontent.com/61205851/144756864-d0ecca36-4d3d-4f6f-a8df-cc4d05f00d59.png" width="300" height="300"/>

And if you click on "OFF" at the web page, it turns off:

<img src="https://user-images.githubusercontent.com/61205851/144756931-0cb4d9a0-b67c-4675-a5db-cca8c511a6de.png" width="300" height="300"/>

And the Web page will show:

<img src="https://user-images.githubusercontent.com/61205851/144754807-bc1cfffc-1773-4910-ab89-8a49e1ae1e79.png " width="300" height="300"/>

In [ext_manager.h](https://github.com/brenogsantos/ESP32-Relay-Sleep-Control/blob/master/lib/ext_manager/ext_manager.h) it's possible to configure the time that the relay will turn on on its own. The Relay Output Pin used here is GPIO15.

```
#define HORA_WAKEUP_FROM_OFF    7
#define RELAY_OUTPUT_PIN 15
```

In [ext_manager.cpp](https://github.com/brenogsantos/ESP32-Relay-Sleep-Control/blob/master/lib/ext_manager/ext_manager.cpp) its possible to configure the time zone:
```
configTime(-10800, 0, "pool.ntp.org");
```
To activate deep sleep mode, type in your browser (without the "):

```
"your-ESP32-IP"/sleepoff
```
The ESP32 will go into deep sleep and will wake up every 30min to check if its time to wake up for good, otherwise he will sleep again. You can change it in ext_manager.cpp
```
 esp_sleep_enable_timer_wakeup(1800000000);
 ```
