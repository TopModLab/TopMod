/* $Id: DLFLCommon.cc,v 4.0 2003/12/26 01:58:52 vinod Exp $ */

// Source for functions declared in DLFLCommon.hh

#include "DLFLCommon.hh"
#include "DLFLVertex.hh"
#include "DLFLFaceVertex.hh"
#include "DLFLEdge.hh"
#include "DLFLFace.hh"
#include "DLFLObject.hh"
#include "DLFLMaterial.hh"

void erase_dvp(DLFLVertexPtr vp)
{
  delete vp;
}

void erase_dfvp(DLFLFaceVertexPtr fvp)
{
  delete fvp;
}

void erase_dep(DLFLEdgePtr ep)
{
  delete ep;
}

void erase_dfp(DLFLFacePtr fp)
{
  delete fp;
}

void erase_dop(DLFLObjectPtr op)
{
  delete op;
}

void erase_dmp(DLFLMaterialPtr mp)
{
  delete mp;
}

void clear(DLFLVertexPtrArray& vparray)
{
  for_each(vparray.begin(),vparray.end(),erase_dvp);
  vparray.clear();
}

void clear(DLFLFaceVertexPtrArray& fvparray)
{
  for_each(fvparray.begin(),fvparray.end(),erase_dfvp);
  fvparray.clear();
}

void clear(DLFLEdgePtrArray& eparray)
{
  for_each(eparray.begin(),eparray.end(),erase_dep);
  eparray.clear();
}

void clear(DLFLFacePtrArray& fparray)
{
  for_each(fparray.begin(),fparray.end(),erase_dfp);
  fparray.clear();
}

void clear(DLFLObjectPtrArray& oparray)
{
  for_each(oparray.begin(),oparray.end(),erase_dop);
  oparray.clear();
}

void clear(DLFLMaterialPtrArray& mparray)
{
  for_each(mparray.begin(),mparray.end(),erase_dmp);
  mparray.clear();
}


void clear(DLFLVertexPtrList& vplist)
{
  for_each(vplist.begin(),vplist.end(),erase_dvp);
  vplist.clear();
}

void clear(DLFLFaceVertexPtrList& fvplist)
{
  for_each(fvplist.begin(),fvplist.end(),erase_dfvp);
  fvplist.clear();
}

void clear(DLFLEdgePtrList& eplist)
{
  for_each(eplist.begin(),eplist.end(),erase_dep);
  eplist.clear();
}

void clear(DLFLFacePtrList& fplist)
{
  for_each(fplist.begin(),fplist.end(),erase_dfp);
  fplist.clear();
}

void clear(DLFLObjectPtrList& oplist)
{
  for_each(oplist.begin(),oplist.end(),erase_dop);
  oplist.clear();
}

void clear(DLFLMaterialPtrList& mplist)
{
  for_each(mplist.begin(),mplist.end(),erase_dmp);
  mplist.clear();
}


/*
  $Log: DLFLCommon.cc,v $
  Revision 4.0  2003/12/26 01:58:52  vinod
  Major version sync.

  Revision 3.0  2003/09/19 16:06:47  vinod
  Major version update

  Revision 2.0  2001/07/25 05:12:42  vinod
  Major version sync

  Revision 1.1.1.1  2000/11/02 19:32:43  vinod
  New DLFL Sources

*/
