/*
*
* ***** BEGIN GPL LICENSE BLOCK *****
*
* This program is free software; you can redistribute it and/or
* modify it under the terms of the GNU General Public License
* as published by the Free Software Foundation; either version 2
* of the License, or (at your option) any later version.
*
* This program is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
* GNU General Public License for more details.
*
* You should have received a copy of the GNU General Public License
* along with this program; if not, write to the Free Software  Foundation,
* Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.
*
* The Original Code is Copyright (C) 2005 by xxxxxxxxxxxxxx
* All rights reserved.
*
* The Original Code is: all of this file.
*
* Contributor(s): none yet.
*
* ***** END GPL LICENSE BLOCK *****
*
* Short description of this file
*
* name of .hh file containing function prototypes
*
*/

/*
 * @file    TdxDeviceEvents.c
 * @brief   Wrappers for 3Dx driver events and callbacks
 *
 * Copyright (c) 1998-2007 3Dconnexion. All rights reserved. 
 *
 * Permission to use, copy, modify, and distribute this software for all
 * purposes and without fees is hereby granted provided that this copyright
 * notice appears in all copies.  Permission to modify this software is granted
 * and 3Dconnexion will support such modifications only if said modifications
 * are approved by 3Dconnexion.
 *
 */

#ifdef WITH_SPACENAV

#include <unistd.h>
#include <pthread.h>
#include <Carbon/Carbon.h>

#include "TdxDeviceWrappers.hh"

/* ========================================================================== */
#pragma mark * Data structures *

typedef struct {
	UInt16 				clientID; /* ID assigned by the driver */
	Boolean				showClientEventsOnly;
	EventQueueRef		mainEventQueue;
} TdxDeviceWrapperInfo, *TdxDeviceWrapperInfoPtr;

/* ========================================================================== */
#pragma mark * Global variables Initialization *

TdxDeviceWrapperInfo gDevWrapperInfo;

/* ========================================================================== */
#pragma mark * Function prototypes *

static void tdx_drv_handler(io_connect_t connection, 
                            natural_t messageType, 
                            void *messageArgument);

/* ========================================================================== */
#pragma mark -
#pragma mark * Façade of 3DconnexionClient framework APIs *

/* -------------------------------------------------------------------------- */
long TdxInitDevice(UInt32 appID, Boolean showOnlyMyClientEvents, UInt16 mode, UInt32 mask){

	OSStatus err;
	gDevWrapperInfo.showClientEventsOnly = showOnlyMyClientEvents;

	/*  must save the main event queue while we're running within the main 
	thread. GetMainEventQueue() is not thread-safe and will cause big 
	problems if called from the spawned thread. */
	gDevWrapperInfo.mainEventQueue = GetMainEventQueue();

	/* make sure the framework is installed */
	if (InstallConnexionHandlers == NULL){
		printf("3Dconnexion framework not found!\n");
		return -1;
	}

	/* install 3dx message handler in order to receive driver events */
	err = InstallConnexionHandlers(tdx_drv_handler, 0L, 0L);
	assert(err == 0);
	if (err)	return err;

	/* register our app with the driver */
	// err = TdxInitDevice('3Dm1', TRUE, kConnexionClientModeTakeOver, (kConnexionMaskAxis | kConnexionMaskButtons));
	// gDevWrapperInfo.clientID = RegisterConnexionClient(appID, (UInt8*)"\pTopMod", mode, mask);
	gDevWrapperInfo.clientID = RegisterConnexionClient('TopM', (UInt8*)"\pTopMod", kConnexionClientModeTakeOver, kConnexionMaskAll);
	if (gDevWrapperInfo.clientID == 0)	return -2;

	printf("3Dconnexion device initialized. Client ID: %d\n", gDevWrapperInfo.clientID);
	
	return err;
}

/* -------------------------------------------------------------------------- */
void TdxTerminateDevice()
{
    UInt16 wasConnexionOpen = gDevWrapperInfo.clientID;
    
    /* make sure the framework is installed */
	if (InstallConnexionHandlers == NULL)
        return;
    
    /* close the connection with the 3dx driver */
    if (wasConnexionOpen)
        UnregisterConnexionClient(gDevWrapperInfo.clientID);
	CleanupConnexionHandlers();
    
    fprintf(stderr, "Terminated connection with 3Dconnexion device.\n");
}

#pragma mark -
#pragma mark * Static functions implementation *

/* ----------------------------------------------------------------------------
    Handler for driver events. This function is able to handle the events in
    different ways: (1) re-package the events as Carbon events, (2) compute
    them directly, (3) write the event info in a shared memory location for
    usage by reader threads.
*/
static void tdx_drv_handler(io_connect_t connection, natural_t messageType, void *messageArgument){
		
	ConnexionDeviceStatePtr msg = (ConnexionDeviceStatePtr)messageArgument;
	static UInt16 lastBtnPressed = 0;
    
    switch(messageType)
	{
		case kConnexionMsgDeviceState:
			/* Device state messages are broadcast to all clients.  It is up to
			 * the client to figure out if the message is meant for them. This
			 * is done by comparing the "client" id sent in the message to our
       * assigned id when the connection to the driver was established.
			 * 
			 * There is a special mode wherein all events are sent to this 
			 * client regardless if it was meant for it or not.  This mode is 
			 * determined by the showClientEventOnly flag.
			 */
			if (!gDevWrapperInfo.showClientEventsOnly || msg->client == gDevWrapperInfo.clientID)
			{
				switch (msg->command)
				{
                    case kConnexionCmdHandleAxis:
                    {
                        static SInt16 zerodata[] = {0, 0, 0, 0, 0, 0};
                        static Boolean isZero = FALSE, wasZero;
                        
                        wasZero = isZero;
                        if (isZero == (memcmp(msg->axis, zerodata, sizeof(zerodata)) == 0))
                        {
                            if (wasZero == FALSE)
                                TdxComputeEventZero();
                        }
                        else
                        {
                            TdxComputeAxes(msg->axis);
                        }
                        break;
                    }
                        
                    case kConnexionCmdHandleButtons:
                    {
                        SInt16 buttonState;

                        if (msg->value == 0)
                        {
                            buttonState = 0;
                        }
                        else
                        {
                            lastBtnPressed = msg->buttons;
                            buttonState = 1;
                        }
                        
                        TdxComputeButtons(lastBtnPressed, buttonState);
						break;
                    }
                        
                    default:
                        break;

				} /* switch */
            }

			break;

		default:
			/* other messageTypes can happen and should be ignored */
			break;
	}
    
    printf("connection: %X\n", connection);
    printf("messageType: %X\n", messageType);
    printf("version: %d\n", msg->version);
    printf("front app client: %d  ourID: %d\n", 
           msg->client, gDevWrapperInfo.clientID);
    printf("command: %u\n", msg->command);
    printf("value: %ld\n", msg->value);
    printf("param: %hd\n", msg->param);
    for (int i=0; i<8; i++)
        printf("report[%d]: %d\n", i, msg->report[i]);
    printf("buttons: %d\n", msg->buttons);
    printf("TX: %d\n", msg->axis[0]);
    printf("TY: %d\n", msg->axis[1]);
    printf("TZ: %d\n", msg->axis[2]);
    printf("RX: %d\n", msg->axis[3]);
    printf("RY: %d\n", msg->axis[4]);
    printf("RZ: %d\n", msg->axis[5]);
    printf("-----------------------------------------\n\n");
    //*/
#ifdef __MWERKS__
	#pragma unused(connection)
#endif	
}

#endif