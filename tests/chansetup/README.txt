This sample program demonstrates basic H.323 channel setup and teardown.
It should be run using two instances:

1. Instance 1 with no parameters will go into listen mode and wait for a
   connection request.

2. Instance 2 with "-makeCall [ip:port]" will initiate an outgoing call.
   If [ip:port] is not specified, call will be made to listener on
   the local machine.

Once the channel is setup and established, the receiver will delay for
a short period of time and then hang up the call.

