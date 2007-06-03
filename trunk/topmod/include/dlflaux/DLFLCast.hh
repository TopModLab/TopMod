#ifndef _DLFLCAST_H_
#define _DLFLCAST_H_

#include <DLFLObject.hh>

/**
 * Cast Objects to primitive shapes. Currently only sphere and plane
 **/
namespace DLFL {
  void spheralize( DLFLObjectPtr obj );
  void planarize ( DLFLObjectPtr obj );
} // end namespace

#endif // _DLFLCAST_H_
