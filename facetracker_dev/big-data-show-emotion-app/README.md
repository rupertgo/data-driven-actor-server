# BDS-App
 
## Installation on Raspberry Pi

### Install Nginx
- `sudo apt update`
- `sudo apt install nginx`
- `sudo /etc/init.d/nginx start`

### Download source
Download this project to the `/var/www/html` directory:
- `cd /var/www/html`
- `sudo git clone https://git.ecdf.ed.ac.uk/design-informatics/big-data-show-emotion-app.git`
- rename folder `sudo mv /var/www/html/big-data-show-emotion-app /var/www/html/face-tracker`

### Download Clmtrackr
- `cd /var/www/html/face-tracker/js/lib`
- `git clone https://github.com/auduno/clmtrackr.git`

### Create certificate for https

- If using Raspberry Pi with Node Red and https already set up as per [these instructions](https://git.ecdf.ed.ac.uk/design-informatics/vizblocks/vizblocks-software/blob/master/README.md) then you can use the existing certificates located in `~/.node-red/nodecerts`
- Edit the default nginx server block to look like this `sudo nano  /etc/nginx/sites-available/default`:
    ```
    server {
        listen 443 ssl;
        listen [::]:443 ssl;
        ssl_certificate /home/pi/.node-red/nodecerts/node-cert.pem;
        ssl_certificate_key /home/pi/.node-red/nodecerts/node-key.pem;

        root /var/www/html;

        index index.html index.htm index.nginx-debian.html;

        server_name 192.168.4.1;

        location / {
            # First attempt to serve request as file, then
            # as directory, then fall back to displaying a 404.
            try_files $uri $uri/ =404;
        }
    }


    server {
        listen 80;
        listen [::]:80;

        server_name 192.168.4.1;

        return 302 https://$server_name$request_uri;
    }
    ```
- Restart Nginx `sudo systemctl restart nginx`


 This tool is being used to transform Google Spreadsheets
 to plain HTML tables
 https://tableizer.journalistopia.com/tableizer.php
