/*
*
	*-----------------------------------------------------
	* File:          $RCSfile: CgData.cc,v $
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

#include "CgData.hh"

#ifdef GPU_OK

namespace Cg {

	/*!
	* \brief private Constructor
	*/
	CgData::CgData(){
		
	}

	CgData* CgData::mInstance = 0; //!< initialize the singleton pointer to 0

	/*!
	* \brief ensures only 1 instance (singleton)
	*/
	CgData* CgData::instance( ) {
	  if ( !mInstance )
	    mInstance = new CgData;
	  return mInstance;
	}

	void checkCgError( CGcontext &context, int id ) {
		CGerror err = cgGetError();
		if (err != CG_NO_ERROR) {
			printf("CG error %d: %s\n", id, cgGetErrorString(err));
			if ( context ) {
				printf("%s\n", cgGetLastListing( context ));
			}
		}
	};
	
	void checkForCgError(const char *situation)	{
	  CGerror error;
	  const char *string = cgGetLastErrorString(&error);

	  if (error != CG_NO_ERROR) {
	    printf("%s: %s\n", situation, string);
	    if (error == CG_COMPILER_ERROR) {
	      printf("%s\n", cgGetLastListing(CgData::instance()->context));
	    }
	    // exit(1);
	  }
	}
	

} // end namespace Cg

#endif // GPU_OK
