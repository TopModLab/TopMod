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

/* $Id: SpotLight.hh,v 2.4 2000/03/02 10:39:57 vinod Exp $ */

#ifndef _SPOT_LIGHT_HH_

#define _SPOT_LIGHT_HH_

// class for a spot light. Derived from class Light
// A spot light has a radius which is the radius of the projected circle

#include "Light.hh"
#include <math.h>

class SpotLight;
typedef SpotLight * SpotLightPtr;

class SpotLight : public Light
{
  public :

     double radius;                                    // Spot light radius

  protected :
     
     double cutoff;                                    // Radius ratio for constant intensity
                                                       // Always between 0 and 1

  public :
     
        // Default constructor
     SpotLight()
       : Light(), radius(5.0), cutoff(0.5)
       {}

        // 1 argument constructor
     SpotLight(const Vector3d pos)
       : Light(pos), radius(5.0), cutoff(0.5)
       {}

        // Copy constructor
     SpotLight(const SpotLight& sl)
       : Light(sl), radius(sl.radius), cutoff(sl.cutoff)
       {}

        // Destructor
     ~SpotLight()
       {}

        // Assignment operator
     SpotLight& operator = (const SpotLight& sl)
       {
         Light :: operator = (sl); radius = sl.radius; cutoff = sl.cutoff;
         return (*this);
       }

        // Make a copy
     virtual BaseObjectPtr copy(void) const
       {
         SpotLightPtr newsl = new SpotLight(*this);
         return newsl;
       }
 
     double getCutoff(void) const
       {
         return cutoff;
       }

     void setCutoff(double c)
       {
         cutoff = c;
         if ( cutoff < 0.0 ) cutoff = 0.0;
         else if ( cutoff > 1.0 ) cutoff = 1.0;
       }
     
       // Type of light
     virtual LightType type(void) const
       {
         return Spot;
       }

        // Does this light illuminate given point?
        // If dist of point is within spot light radius on the ZX plane SpotLight illuminates
     virtual bool illuminates(const Vector3d& p) const
       {
         bool illum = false;
         Vector3d vec(position-p);
         double dist = sqrt(sqr(vec[0]) + sqr(vec[2]));

         if ( dist <= radius ) illum = true;
         return illum;
       }

        // Compute the cosine factor for given point/normal
     double cosfactor(const Vector3d& p, const Vector3d& n) const
       {
         double cf = 0.0;
         Vector3d vec(position-p); 
         double dist = sqrt(sqr(vec[0]) + sqr(vec[2]));
         Vector3d normal(n); normalize(normal);

         normalize(vec);
         cf = (1.0 + vec*normal)/2.0;
         if ( dist > radius ) cf = 0.0;
         else if ( dist > cutoff * radius )
            {
              cf *= (radius - dist)/(radius - cutoff*radius);
            }
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
					// e = e;
         if ( state == false ) return RGBColor(0);
            // For now do only diffuse lighting
         return illuminate(p,n);
       }
};

#endif /* #ifndef _SPOT_LIGHT_HH_ */


/*
  $Log: SpotLight.hh,v $
  Revision 2.4  2000/03/02 10:39:57  vinod
  Distance is now computed as sqrt

  Revision 2.3  2000/03/02 08:48:07  vinod
  Typo

  Revision 2.2  2000/03/02 07:43:47  vinod
  Added illuminates and cosfactor functions

  Revision 2.1  2000/02/29 06:27:01  vinod
  Uses intensity now

  Revision 2.0  2000/02/18 01:43:47  vinod
  Release 2.0

  Revision 1.2  2000/02/18 01:43:07  vinod
  Removed unused variable

  Revision 1.1  2000/02/18 01:19:16  vinod
  SpotLight class

*/
