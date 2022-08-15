# VizBlocks Server

Set up a remote VizBlocks server using Docker.

## Development

### Docker set up
1. [Install Docker and Docker Compose](https://docs.docker.com/compose/install/).
2. [Install Mosquitto](https://mosquitto.org/download/).
3. Duplicate the `dev_config_example` folder and rename it to `dev_config`.

4. In a terminal, navigate to the project root and use docker compose to start the Node Red and MQTT broker containers:
```
docker-compose up --force-recreate --build
```
5. Go to http://localhost:1880/ and log into Node Red with the credentials CivicDigits and DataIsFun!

### Reset/Create New MQTT Password
1. On lines 126 and 127 in [dev_config/node-red-data/settings.js](dev_config/node-red-data/settings.js), replace `<NODE_RED_DEV_USER>` and `<NODE_RED_DEV_PASSWORD>`  with the username and password that you'd like to use. *Note: passwords should be encrypted using bcrypt, see [here](https://nodered.org/docs/user-guide/runtime/securing-node-red)*.
2. In [dev_config/mosquitto/credentials.txt](dev_config/mosquitto/credentials.txt), replace `<MQTT_DEV_USER>` and `<MQTT_DEV_PASSWORD>`  with the plain text username and password that you want to use for MQTT authentication (e.g. `user:password`).
3. In a terminal navigate to the project root and run the following command to encrypt your MQTT password file:
```bash
mosquitto_passwd -U dev_config/mosquitto/credentials.txt
```


## Testing with a VizBlock

Before starting, check that the purple MQTT send node says `connected` beneath it. If not, double click the node to open its settings, then click the pencil icon next to the server input, then go to the security tab and make sure the username and password is entered for the MQTT broker.

### Setting up your VizBlock
In order for a the vizblock to work with this remote server, you will need configure the VizBlock (using Arduino code) so that it:
 a) connects to a WiFi network with internet access.
 b) connects to the remote MQTT server.
 
 Navigate to [vizblocks-firmware/ServerBasicBlock.ino](vizblocks-firmware/ServerBasicBlock.ino) to find the firmware. The code will need the wifi name and password and your IP address entered.
 
You can do this by editing the block configuration in the following lines:

```
VizBlock node(
  "null",       // Our ID
  "<wifi_net>",  //Wifi Access Point
  "<wifi_password>",  //WiFi Password
  "vizblocks-server.com",//IP address
   8883,         //Port for Node RED server
   "<mqtt_user>",  //MQTT username
   "<mqtt_password>",    //MQTT password
   "<F7:AD:10:85:97:1E:F8:68:08:17:A3:DC:68:F9:C3:42:DE:C9:A3:F8>" // SHA1 fingerprint (for mqtt server)
  );
```
Replace all of the settings in `<>` with your values (or those provided to you).


