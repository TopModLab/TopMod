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

/**
 * \file DLFLCommon.hh
 */

#include "DLFLCommon.hh"
#include "DLFLVertex.hh"
#include "DLFLFaceVertex.hh"
#include "DLFLEdge.hh"
#include "DLFLFace.hh"
#include "DLFLObject.hh"

namespace DLFL {

  void erase_dvp(DLFLVertexPtr vp) { delete vp; }

  void erase_dfvp(DLFLFaceVertexPtr fvp) { delete fvp; }

  void erase_dep(DLFLEdgePtr ep) { delete ep; }

  void erase_dfp(DLFLFacePtr fp) { delete fp; }

  void erase_dop(DLFLObjectPtr op) { delete op; }

  void erase_dmp(DLFLMaterialPtr mp) { delete mp; }

  void clear(DLFLVertexPtrArray& vparray) {
    for_each(vparray.begin(),vparray.end(),erase_dvp);
    vparray.clear();
  }

  void clear(DLFLFaceVertexPtrArray& fvparray) {
    for_each(fvparray.begin(),fvparray.end(),erase_dfvp);
    fvparray.clear();
  }

  void clear(DLFLEdgePtrArray& eparray) {
    for_each(eparray.begin(),eparray.end(),erase_dep);
    eparray.clear();
  }

  void clear(DLFLFacePtrArray& fparray) {
    for_each(fparray.begin(),fparray.end(),erase_dfp);
    fparray.clear();
  }

  void clear(DLFLObjectPtrArray& oparray) {
    for_each(oparray.begin(),oparray.end(),erase_dop);
    oparray.clear();
  }

  void clear(DLFLMaterialPtrArray& mparray) {
    for_each(mparray.begin(),mparray.end(),erase_dmp);
    mparray.clear();
  }


  void clear(DLFLVertexPtrList& vplist) {
    for_each(vplist.begin(),vplist.end(),erase_dvp);
    vplist.clear();
  }

  void clear(DLFLFaceVertexPtrList& fvplist) {
    for_each(fvplist.begin(),fvplist.end(),erase_dfvp);
    fvplist.clear();
  }

  void clear(DLFLEdgePtrList& eplist) {
    for_each(eplist.begin(),eplist.end(),erase_dep);
    eplist.clear();
  }

  void clear(DLFLFacePtrList& fplist) {
    for_each(fplist.begin(),fplist.end(),erase_dfp);
    fplist.clear();
  }

  void clear(DLFLObjectPtrList& oplist) {
    for_each(oplist.begin(),oplist.end(),erase_dop);
    oplist.clear();
  }

  void clear(DLFLMaterialPtrList& mplist) {
    for_each(mplist.begin(),mplist.end(),erase_dmp);
    mplist.clear();
  }

} // end namespace
