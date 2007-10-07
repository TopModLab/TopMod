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

/* $Id: PointLight.hh,v 1.1 2006/09/04 19:22:53 stuart Exp $ */

#ifndef _POINT_LIGHT_HH_

#define _POINT_LIGHT_HH_

// class for a point light. Derived from class Light
// No additional member data

#include "Light.hh"

class PointLight;
typedef PointLight * PointLightPtr;

class PointLight : public Light
{
  public :

        // Default constructor
     PointLight()
       : Light()
       {}

        // 1 argument constructor
     PointLight(const Vector3d pos)
       : Light(pos)
       {}

        // Copy constructor
     PointLight(const PointLight& pl)
       : Light(pl)
       {}

        // Destructor
     ~PointLight()
       {}

        // Assignment operator
     PointLight& operator = (const PointLight& pl)
       {
         Light :: operator = (pl);
         return (*this);
       }

        // Make a copy
     virtual BaseObjectPtr copy(void) const
       {
         PointLightPtr newpl = new PointLight(*this);
         return newpl;
       }

        // Type of light
     virtual LightType type(void) const
       {
         return PtLight;
       }

        // Does this light illuminate given point? PointLight always does
     virtual bool illuminates(const Vector3d&) const
       {
         return true;
       }
     
        // Compute the cosine factor for given point/normal
     virtual double cosfactor(const Vector3d& p, const Vector3d& n) const
       {
         Vector3d vec(position-p); normalize(vec);
         Vector3d normal(n); normalize(normal);
         double cf = sqr((1.0 + vec*normal)/2.0);

         return cf;
       }

        // Illuminate a given point with a given normal using this light and return color
     virtual RGBColor illuminate(const Vector3d& p, const Vector3d& n) const
       {
         if ( state == false ) return RGBColor(0);

            // Since we don't have eye position do only diffuse computation
         double cf = cosfactor(p,n);
         RGBColor color;
         
         color = (warmcolor*cf + coolcolor*(1.0-cf))*intensity;
         return color;
       }

        // Same as above but with specular lighting also
     virtual RGBColor illuminate(const Vector3d& p, const Vector3d& n, const Vector3d& e) const
       {
		// float spec=0;
		// Vector3d V(e); // normalize(V);
		// Vector3d L(position-p); normalize(L);
		// Vector3d R(L - 2.0f * (L*N)*N);
		// float dot = V*R;
		// if (dot > 0) spec = powf( dot, 20 );
         if ( state == false ) return RGBColor(0);
            // For now do only diffuse lighting
         return illuminate(p,n);
       }
};

#endif /* #ifndef _POINT_LIGHT_HH_ */


/*
  $Log: PointLight.hh,v $
  Revision 1.1  2006/09/04 19:22:53  stuart
  Added includes and libs.

  Revision 2.2  2000/03/02 07:43:19  vinod
  Added illuminates and cosfactor functions

  Revision 2.1  2000/02/29 06:26:48  vinod
  Uses intensity now

  Revision 2.0  2000/02/18 01:43:47  vinod
  Release 2.0

  Revision 1.7  2000/02/18 01:42:56  vinod
  Missing normalize. cosfactor is now squared

  Revision 1.6  2000/02/18 01:19:06  vinod
  *** empty log message ***

  Revision 1.5  2000/02/14 21:09:13  vinod
  Changed color model to use warm and cool colors

  Revision 1.4  2000/02/11 08:54:26  vinod
  Changed n to normal in costheta calculation

  Revision 1.3  2000/02/08 10:27:02  vinod
  Minor bug fixes

  Revision 1.2  2000/02/08 10:24:50  vinod
  Changed type to return correct enum

  Revision 1.1.1.1  2000/02/08 02:55:22  vinod
  Light classes

*/
