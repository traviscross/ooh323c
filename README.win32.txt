Copyright (C) 2004-2006 by Objective Systems, Inc.

Objective Open H.323 README

Introduction
============

Thank you for downloading the Objective Open H.323 software. This
package contains windows binaries of  H.323 stack developed in C. 
The ASN.1 messaging code was developed using the Objective Systems ASN1C
compiler to implement the core H.323 specifications (H.225, H.235,
and H.245).  Additional code was then developed which makes use of
the compiler generated code for presenting a comparatively high level
stack API.

The media part, which includes RTP and audio support, is implemented
as a sample library. The media library uses G.711 code made available
by Sun Microsystems to the open source community. Please refer to their
license for more details.


Features
========

Features supported in this package include the following:

* H.225/Q.931 - the following message types are supported (including
  support for FastStart and H.245 tunneling):
  - Setup
  - Connect
  - Call Proceeding
  - Alerting
  - Facility
  - ReleaseComplete

* H.225/RAS - the following message types are supported
  - GateKeeperRequest / Response
  - RegistrationRequest / Response
  - AdmissionRequest / Response
  - DisengageRequest / Response

* H.245 - the following message types are supported
  - MasterSlaveDetermination
  - MasterSlaveDeterminationAck
  - MasterSlaveDeterminationReject
  - MasterSlaveDeterminationRelease
  - TerminalCapabilitySet
  - TerminalCapabilitySetReject
  - TerminalCapabilitySetRelease
  - TerminalCapabilitySetAck
  - OpenLogicalChannel
  - OpenLogicalChannelAck
  - OpenLogicalChannelReject
  - CloseLogicalChannel
  - CloseLogicalChannelAck
  - RequestChannelClose
  - RequestChannelCloseAck
  - RequestChannelCloseReject
  - RequestChannelCloseRelease

* A sample media library is provided with the test programs that can
   send and receive RTP data. It can play the received audio onto
   speakers and can collect audio from a microphone for transmission
   as an RTP stream. This library also supports transmitting audio files
   over the RTP channel. The Windows version supports 16 bit, 8000 samples
   per second wave file whereas the Linux version supports raw 16 bit audio
   data files.


Getting Started
===============

Sample Programs

To run the sample programs, make sure the path to the media plug-in library
 (oomedia.dll) is in your PATH.  The DLL is located in the ooh323c-0.8.4\lib
directory.

The sample programs are locates in the 'tests' subdirectory.  The following
programs are available:

*  receiver - an audio receiver program that will listen for incoming
   requests and then play audio as it is received.

*  player - an audio player program that will play a sample audio file.

*  simple - an H.323 endpoint sample program.  This is a sample H.323
   VoIP phone application which opens g711ulaw64k channels and transmits
   and receives voice. It has been tested successfully against ohPhone,
   a widely used open source H.323 phone application and NetMeeting.

*  chansetup - This is a sample program developed for testing multiple calls.
   This program allows stack testing by placing multiple calls. The number of
   calls, duration of each call and interval between successive calls are
   configurable. 

The receiver and player program must be run in parallel as follows:

1. Run receiver first to listen for incoming connections:

        cd tests/receiver
        ooReceiver.exe [--use-ip <ip>] [--use-port <port>]

   where, [--use-ip <ip>] and [--use-port <port>] options are used to specify
   local ip address and port number. By deafault, receiver application tries
   to determine ip address on it's own and uses standard H.323 port 1720.

   A log file will be created in the tests/receiver directory
   (ooReceiver.log). Also, the log file for the media plugin will
   be created in the same directory (media.log).

2. Run the player:

        cd ../player
        ooPlayer.exe --audio-file states.wav [--use-ip <ip>]

   where, --audio-file is used to specify the audio file to be played.
   [--use-ip <ip>] is used to specify local ip address. By deafault, player
   application tries to determine ip address on it's own.

   A log file will be created in the tests/player directory
   (ooPlayer.log). Also, a log file for the media plug-in will be
   created in the same directory (media.log).


To run the H.323 endpoint sample application:

1. Set working directory to the simple test directory:

        cd tests/simple

2. To see the usage information including various options:
        simple.exe OR simple.exe --help

3. To make a call:

        simple.exe [options]  <remote>

   where,
 <remote>    -  is the dotted representation of the destinations IP address. In
                case of gatekeeper, aliases can also be used.
     
4. To receive a call:

        simple.exe [options] --listen

   You will find simple.log and media.log in the current directory.
     

To run the stack test application chansetup

1. Two instances of this program have to be run. Can be run on same machine or
   different machines.

2. First change to chansetup directory.

        cd tests/chansetup

2. For running listener instance,

        h323peer.exe [--use-ip ip] [--use-port port]

   where local ip address and port can be specified for listening to incoming
   calls. By default, application determines ip address and uses default H323
   port number 1720.

3. For running calling instance

        h323peer.exe [--use-ip ip] -n <number of calls> -duration <call duration>
                   -interval <inetrval between successive calls> destination
     
   where all times are in seconds. Interval of 0 means next call will be placed
   after current call finishes. "destination" is the dotted ip address of the
   endpoint running listener instance.


Reporting Problems:
==================

Report problems you encounter by sending E-mail to
ooh323c-devel@lists.sourceforge.net.

If you have any further questions or comments on what you would like to
see in the product or what is difficult to use or understand, please
communicate them to us. Your feedback is important to us. Please let us
know how it works out for you - either good or bad.

