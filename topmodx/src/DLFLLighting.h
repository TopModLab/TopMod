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

#ifndef _DLFL_LIGHTING_HH_
#define _DLFL_LIGHTING_HH_

// Subroutines for lighting computations in DLFL

#include "TMPatchObject.h"
#include <DLFLObject.h>

#include <QApplication>
#include <QProgressDialog>

#include "CgData.h"

#ifdef GPU_OK
using namespace Cg;
#endif // GPU_OK

void computeLighting(DLFLFacePtr fp, LightPtr lightptr, bool usegpu = false);
void computeLighting(DLFLObjectPtr obj, TMPatchObjectPtr po, LightPtr lightptr, bool usegpu = false);

#endif /* #ifndef _DLFL_LIGHTING_HH_ */
