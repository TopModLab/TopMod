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

/**
 *
 *-----------------------------------------------------
 * File:          $RCSfile: CgData.hh,v $
 * Date modified: $Date: 2007/02/13 19:37:54 $
 * Version:       $Revision: 1.1 $
 *-----------------------------------------------------
 *
 * Copyright (C) 2007 Original Author(s)
 *
 * Original Author(s):
 *   Stuart T. Tett <stuart@tett.net>
 *  
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *  
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *  
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA.
 *  
 */

#ifndef _CG_DATA_H
#define _CG_DATA_H

#ifdef GPU_OK

#include <Cg/cg.h>
#include <Cg/cgGL.h>
#include <cstdio>

/*!
*	\file CgData.hh
*	\brief singleton class for Cg GPU Shading
* 
*/

namespace Cg {

class CgData {
public :
  /** Singleton Methods **/
  static CgData* instance( );
  ~CgData( ) { };

	CGcontext context;
	CGprogram vertProgram, fragProgram;
	CGprofile vertProfile, fragProfile;

	CGparameter camToWorld, worldToLight, camToWorldIT;

	// CGparameter texture, shadowMap;
	// CGparameter renderToTexSize;
	// CGparameter objectID;
	CGparameter attenDegrees;
	
	//from book
	CGparameter globalAmbient;
	
	//lighting
	CGparameter lightWarmColor;
	CGparameter lightCoolColor;
	CGparameter lightIntensity;
	CGparameter lightPosition;
	
	CGparameter eyePosition;
	CGparameter Ka, Kd, Ks, shininess, basecolor;

private:
	static CgData *mInstance;
	CgData();
  
};

  void checkCgError( CGcontext &context, int id );
	void checkForCgError(const char *situation);

} // end namespace Cg


//old implementation as a struct
// namespace Cg {
// 
//   struct CgData {
//     CGcontext context;
//     CGprogram vertProgram, fragProgram;
//     CGprofile vertProfile, fragProfile;
// 
// 	  CGparameter camToWorld, worldToLight, camToWorldIT;
// 
// 	  CGparameter texture, shadowMap;
// 	  CGparameter renderToTexSize;
// 	  CGparameter objectID;
// 	  CGparameter attenDegrees;
// 		//from book
// 		CGparameter globalAmbient;
// 		CGparameter lightColor;
// 		CGparameter lightPosition;
// 		CGparameter eyePosition;
// 		CGparameter Ke, Ka, Kd, Ks, shininess;
//   };
// 
//   void checkCgError( CGcontext &context, int id );
// 
// } // end namespace Cg

#endif // GPU_OK
#endif // _CG_DATA_H
