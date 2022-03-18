# HowTo split 'admin' user

In firmware 19 and below, the user **admin** for codesys and the web based management *(wbm)* are two different users, which can be configured with different passwords. With firmware 20 and later, this is not the case anymore. This HowTo describes how to seperate those users again.

To do so, three steps are necessary. 

1. Revoke the group **wmbadmin** from the user **admin**
```
gruppen=$(groups admin | sed 's/wbmadmin //' | sed 's/ /,/g'); usermod --groups "${gruppen}" admin
```

2. Forbid user **admin** to login with **ssh**.
```
usermod -s /sbin/nologin admin
```

3. Create a new user with the group **wbmadmin**. This user will have admin rights in wbm. **testadmin** in this example. Do not forget to create a password.
```
useradd --groups wbmadmin testadmin
passwd testadmin
```
Now you have two different users to access a PFC. User **admin** to upload an IEC-project and user **testadmin** to access the web based management with admin rights. Both users are linux users, but only testadmin can login with ssh. If both users need to login with ssh, ignore step 2.