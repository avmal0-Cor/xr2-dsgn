[SCE CONFIDENTIAL DOCUMENT]
PLAYSTATION(R)3 Programmer Tool Runtime Library 190.002
                  Copyright (C) 2007 Sony Computer Entertainment Inc.
                                                 All Rights Reserved.
=====================================================================
This package includes files related to the NP Matching 2 Utility.

---------------------------------------------------------------------
Package Configuration
---------------------------------------------------------------------
cell/
|---Readme_*.txt
|---SDK_doc
|    +---jp/en
|         +---pdf
|              +---NP
|                   |---NP_Matching2_System-Overview_*.pdf
|                   |---NP_Matching2-Overview_*.pdf
|                   +---NP_Matching2-Reference_*.pdf
|---samples
|    +---sdk
|         +---network
|                +---np
|                     +---np_matching2
|                          +---np_gui_matching2
+---target
     +---ppu
          |---include
          |     +---np
          |          +---matching2.h
          +---lib
                +---libsysutil_np2_stub.a

----------------------------------------------------------------------
Required Environment
----------------------------------------------------------------------
- To use this package, decompress it on the SDK190 environment.

----------------------------------------------------------------------
Known Bugs
----------------------------------------------------------------------
- When a leave event occurs as many times as the value set to the total 
  slot number of a room, a notification for the following leave event 
  and later will not be sent to the game application. 

- When the presence option data (SceNpMatching2PresenceOptionData structure)
  is passed to the request parameter in the following processings, an event 
  may not be notified to other room member correctly.

  - Join in a room(sceNpMatching2JoinRoom())
  - Leave from a room (sceNpMatching2LeaveRoom())
  - Transfer the room owner(sceNpMatching2GrantRoomOwner())
  - Kick out a room member(sceNpMatching2KickoutRoomMember())

- When the room ownership is transferred to the user him/herself 
  by executing sceNpMatching2GrantRoomOwner(), the processing itself 
  will be performed successfully, but the room that the user is in 
  at the time of transfer will not be hit by the room search 
  after few seconds. 

- When 14 hours or more is elapsed after the initial execution of a request 
  function excluding sceNpMatching2GetServerInfo() without stopping or 
  discarding the context, the game application hangs up. 

---------------------------------------------------------------------
Permission and Restrictions on Use
---------------------------------------------------------------------
The permission, restrictions, etc. on using this software conform to 
the contract concluded between your company and our company (Sony 
Computer Entertainment Inc). 

---------------------------------------------------------------------
Note on Trademarks
---------------------------------------------------------------------
"PLAYSTATION" is a registered trademark of Sony Computer Entertainment Inc.

All other product and company names mentioned herein, with or without 
the registered trademark symbol (R) or trademark symbol (TM), are 
generally registered trademarks and/or trademarks of their respective 
owners. 

---------------------------------------------------------------------
Copyrights
---------------------------------------------------------------------
The copyright of this software belongs to Sony Computer Entertainment
Inc. 
