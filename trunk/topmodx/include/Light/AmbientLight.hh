/* $Id: AmbientLight.hh,v 2.2 2000/03/02 07:43:07 vinod Exp $ */

#ifndef _AMBIENT_LIGHT_HH_

#define _AMBIENT_LIGHT_HH_

// class for ambient light. Derived from class Light
// No additional member data. Doesn't use position data

#include "Light.hh"

class AmbientLight;
typedef AmbientLight * AmbientLightPtr;

class AmbientLight : public Light
{
  public :

        // Default constructor
     AmbientLight()
       : Light()
       {}

        // 1 argument constructor - specifies color
     AmbientLight(const RGBColor& col)
       : Light(col)
       {}

        // Copy constructur
     AmbientLight(const AmbientLight& al)
       : Light(al)
       {}

        // Destructor
     ~AmbientLight()
       {}

        // Assignment operator
     AmbientLight& operator = (const AmbientLight& al)
       {
         Light :: operator = (al);
         return (*this);
       }

        // Make a copy
     virtual BaseObjectPtr copy(void) const
       {
         AmbientLightPtr newal = new AmbientLight(*this);
         return newal;
       }

        // Type of light
     virtual LightType type(void) const
       {
         return Ambient;
       }

        // Does this light illuminate given point? AmbientLight always does
     virtual bool illuminates(const Vector3d&) const
       {
         return true;
       }

        // Compute the cosine factor. This is independent point/normal
        // 0.5 since we want a equal mix of warm and cool colors
     virtual double cosfactor(const Vector3d&, const Vector3d&) const
       {
         return 0.5;
       }
     
        // Illuminate a given point with a given normal using this light and return color
     virtual RGBColor illuminate(const Vector3d&, const Vector3d&) const
       {
         if ( state == false ) return RGBColor(0);
         return RGBColor((warmcolor.color+coolcolor.color)*(0.5*intensity));
       }

        // Same as above but with specular lighting also
     virtual RGBColor illuminate(const Vector3d&, const Vector3d&, const Vector3d&) const
       {
         if ( state == false ) return RGBColor(0);
         return RGBColor((warmcolor.color+coolcolor.color)*(0.5*intensity));
       }
};

#endif /* #ifndef _AMBIENT_LIGHT_HH_ */


/*
  $Log: AmbientLight.hh,v $
  Revision 2.2  2000/03/02 07:43:07  vinod
  Added illuminates and cosfactor functions

  Revision 2.1  2000/02/29 06:27:10  vinod
  Uses intensity now

  Revision 2.0  2000/02/18 01:43:46  vinod
  Release 2.0

  Revision 1.4  2000/02/16 00:46:33  vinod
  Correction in color computation

  Revision 1.3  2000/02/14 21:09:13  vinod
  Changed color model to use warm and cool colors

  Revision 1.2  2000/02/08 10:24:51  vinod
  Changed type to return correct enum

  Revision 1.1.1.1  2000/02/08 02:55:22  vinod
  Light classes

*/
