
# cURL with https support and automatic CA renewal

When trying to access SSL/TLS protected server using cURL you'll eventually run into this problem:

```
root@PFC200V3-463483:~ curl https://www.google.com
curl: (60) SSL certificate problem: unable to get local issuer certificate
More details here: https://curl.haxx.se/docs/sslcerts.html

curl failed to verify the legitimacy of the server and therefore could not
establish a secure connection to it. To learn more about this situation and
how to fix it, please visit the web page mentioned above.
```

Basically, you need to update your `ca-certyficates` as said in the [manual](https://www.mmf.de/manual/wago_pfc200mane.pdf), page *246*, `14.3 Updating Root Certificates`.
This isn't quite enough to use cURL.

## TLDR;

Login as root and issue these commands:

```
wget https://curl.se/ca/cacert.pem -P /etc/ssl/certs
(crontab -l ; echo "0 0 1 * *  wget https://curl.se/ca/cacert.pem -P /etc/ssl/certs")| crontab -
echo 'export SSL_CERT_FILE=/etc/ssl/certs/cacert.pem' >> /etc/profile
reboot
```

Enjoy!

## Full explanation

### Step 1
`wget https://curl.se/ca/cacert.pem -P /etc/ssl/certs` downloads latest CA Bundle
and places it in `/etc/ssl/certs` folder.

### Step 2
`(crontab -l ; echo "0 0 1 * *  wget https://curl.se/ca/cacert.pem -P /etc/ssl/certs")| crontab -`
adds crontab entry, to download the file each month.

### Step 3
`echo 'export SSL_CERT_FILE=/etc/ssl/certs/cacert.pem' >> /etc/profile` exports profile wide variable
`SSL_CERT_FILE` that is used by cURL to locate CA bundle file.

This path `/etc/ssl/certs/cacert.pem` can be also used in your firmware.
