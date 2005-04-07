This sample program demonstrates basic H.323 channel setup and teardown.

To see all available options to run the program, from the command prompt
   ./h323peer --help  



It should be run using two instances:
1. Instance 1 will be listening for incoming calls. It should be run as follows:
   ./h323peer [--user-ip ip] [--user-port port]

2. Instance 2 will initiate outgoing calls. It should be run as follows:
   ./h323peer [--user-ip ip] [--user-port port] -n <# of calls>
              -duration <call duration(sec)>
              -interval <interval between successive calls(sec)> destination



