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
*/

/*
 * @file    TdxDeviceWrappers.h
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

#ifndef __TdxDeviceWrappers_h__
#define __TdxDeviceWrappers_h__

#include "3DconnexionClient/ConnexionClientAPI.h"

// setenv MACOSX_DEPLOYMENT_TARGET 10.2

#ifdef __cplusplus
extern "C" {
#endif

// setenv(MACOSX_DEPLOYMENT_TARGET,10.2,1);

/* ========================================================================== */
#pragma mark * Weakly linked symbols *
    
/* Weak link 3Dx framework so this app can run without crashing even on 
 * systems that don't have the 3Dx driver installed. For more info on 
 * weak linking, see:
 * http://developer.apple.com/documentation/MacOSX/Conceptual/BPFrameworks/Concepts/WeakLinking.html 
 */
// extern OSErr InstallConnexionHandlers() __attribute__((weak_import));

/* ========================================================================== */
#pragma mark * 3Dx Types *

/* 3Dconnexion event class */
enum 
{
	kEventClassTdxDevice = 'tdxE'	
};

/* 3Dconnexion event kinds */
typedef enum 
{
	kTdxDeviceEventButton = 1,
	kTdxDeviceEventMotion,
	kTdxDeviceEventZero,
} TdxDeviceEventKind;

/* 3Dconnexion event parameters */
typedef enum {
	kEventParamTX             = 'Tpnx', /* typeSInt16 */
	kEventParamTY             = 'Tpny', /* typeSInt16 */
	kEventParamTZ             = 'Tpnz', /* typeSInt16 */
	kEventParamRX             = 'Trtx', /* typeSInt16 */
	kEventParamRY             = 'Trty', /* typeSInt16 */
	kEventParamRZ             = 'Trtz', /* typeSInt16 */
	kEventParamButtonID       = 'Tbtn', /* typeSInt16 */
	kEventParamButtonDown     = 'Tbts', /* typeSInt16 */
} TdxDeviceEventParam;

typedef SInt16 TdxDeviceAxes[6];

/* ========================================================================== */
// #pragma mark * Façade of 3DconnexionClient framework APIs *

/** Initialization routine for starting a data link to the 3Dconnexion driver.
 *	@param appID    signature of the parent application calling this function
 *  @param showOnlyMyClientEvents   this flag determines whether only events 
 *                                  meant for this registered client are 
 *                                  processed or if all events from the device 
 *                                  will be sent to this client
 *  @param mode     possible values are kConnexionClientModeTakeOver or 
 *                  kConnexionClientModePlugin
 *  @param mask     Possible values are kConnexionMask* as listed 
 *                  in ConnexionClient.h
 */
long TdxInitDevice(UInt32 appID, Boolean showOnlyMyClientEvents, UInt16 mode, UInt32 mask);

/** Cleanup rountine to close down the data link with the driver */
void TdxTerminateDevice();

/* ========================================================================== */
#pragma mark * Wrapper Callbacks *

/*******************************************************************************
 * Clients will need to define the callbacks below.
 * These callbacks are invoked by our wrappers.
 ******************************************************************************/

/** Callback for processing the axes data. 
 *  @param inArrayPtr           Array with translations and rotations data */
extern int TdxComputeAxes(const TdxDeviceAxes inArrayPtr);

/** Callback for processing a "release knob" event. */
extern int TdxComputeEventZero();

/** Callback for processing the button data. 
 *  @param currentButtonState   '1' if any button is held down, '0' otherwise. 
 *  @param lastBtnPressed       Buttons mask: 1 = btn1, 2 = btn2, 3 = btn1&btn2
 */
extern int TdxComputeButtons(UInt16 lastBtnPressed, SInt16 currentButtonState);

#ifdef __cplusplus
}
#endif

#endif

#endif