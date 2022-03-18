# HowTo add a user or group to firmware

To add a user or a group, the PFC firmware includes the commands **useradd** and **groupadd**. But until firmare 19 the commands **adduser** and **addgroup** have to be used.

## FW20 and later
To add a user in a firmware with version 20 or later, use command ```useradd <user>```. This will add a new user as well as a new group with the same name. The user will not yet have a password now and will not be allowed to login. So add a password with ```passwd <user>```. Now the new user will be albe to login.

To add a group in a firmware with version 20 or later, use command ```groupadd <group>```. 

## FW19 and previous
To add a user in a firmware with version 19 or previous, use command ```adduser <user>```. This will add a new user as well as a new group with the same name. The command will automatically ask for a password. So no further ```passwd``` command is needed.

To add a group in a firmware with version 19 or previous, use command ```addgroup <group>```. 

## Reserved areas for IDs
The commands ```useradd/groupadd``` or ```adduser/addgroup``` will automatically choose group and user ids, that are valid. But in case you want to choose ids by yourselve, you should consider following ranges:

### By WAGO reserved areas
+ 500 to 1003
+ 3540
+ 5000 and up

### Free areas for user and group ids
+ < 500
+ 1003 to 3539
+ 3541 to 5000


# Compatibility list:
| PFC | Compatible |
|:-------------|:------------:|
| **PFC 100** | |
| 750-8100 | Y |
| 750-8101 | Y |
| 750-8102 | Y |
|  |  |
| **PFC 200** | |
| 750-8202 | Y |
| 750-8203 | Y |
| 750-8204 | Y |
| 750-8206 | Y |
| 750-8207 | Y |
| 750-8208 | Y |
|  |  |
| **PFC 200 G2** | |
| 750-8212 | Y |
| 750-8213 | Y |
| 750-8214 | Y |
| 750-8216 | Y |