 
/***************************************************************************
 *   Copyright (C) 2005 by Robot Group Leipzig                             *
 *    martius@informatik.uni-leipzig.de                                    *
 *    fhesse@informatik.uni-leipzig.de                                     *
 *    der@informatik.uni-leipzig.de                                        *
 *    frankguettler@gmx.de                                                 *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 *                                                                         *
 *   You should have received a copy of the GNU General Public License     *
 *   along with this program; if not, write to the                         *
 *   Free Software Foundation, Inc.,                                       *
 *   59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.             *
 *                                                                         *
 ***************************************************************************
 *                                                                         *
 *   This file provides basic primitives for ODE and openscenegraph        *
 *                                                                         *
 *                                                                         *
 *   $Log$
 *   Revision 1.2  2006-07-14 12:23:35  martius
 *   selforg becomes HEAD
 *
 *   Revision 1.1.2.18  2006/07/14 11:23:38  martius
 *   revert to older revision of robot3
 *
 *   Revision 1.1.2.16  2006/06/29 16:35:32  robot3
 *   -Mesh code optimized
 *   -includes cleared up, more using forward declarations
 *    (sometimes additionally #include "osgprimitive.h" is needed)
 *
 *   Revision 1.1.2.15  2006/06/27 14:14:29  robot3
 *   -optimized mesh and boundingshape code
 *   -other changes
 *
 *   Revision 1.1.2.14  2006/06/23 08:53:56  robot3
 *   made some changes on primitive Mesh
 *
 *   Revision 1.1.2.13  2006/05/29 22:03:26  martius
 *   cylinder
 *
 *   Revision 1.1.2.12  2006/05/29 21:27:02  robot3
 *   made some preparations for the boundingshape of the Mesh
 *
 *   Revision 1.1.2.11  2006/05/28 22:14:57  martius
 *   heightfield included
 *
 *   Revision 1.1.2.10  2006/05/24 12:23:10  robot3
 *   -passive_mesh works now (simple bound_version)
 *   -Primitive Mesh now exists (simple bound_version)
 *
 *   Revision 1.1.2.9  2006/04/04 14:13:24  fhesse
 *   documentation improved
 *
 *   Revision 1.1.2.8  2006/03/29 15:07:17  martius
 *   Dummy Primitive
 *
 *   Revision 1.1.2.7  2006/01/31 15:45:35  martius
 *   proper destruction
 *
 *   Revision 1.1.2.6  2006/01/12 14:21:00  martius
 *   drawmode, material
 *
 *   Revision 1.1.2.5  2005/12/29 12:58:42  martius
 *   *** empty log message ***
 *
 *   Revision 1.1.2.4  2005/12/15 17:03:43  martius
 *   cameramanupulator setPose is working
 *   joints have setter and getter parameters
 *   Primitives are not longer inherited from OSGPrimitive, moreover
 *   they aggregate them
 *
 *   Revision 1.1.2.3  2005/12/14 15:36:45  martius
 *   joints are visible now
 *
 *   Revision 1.1.2.2  2005/12/13 18:11:14  martius
 *   transform primitive added, some joints stuff done, forward declaration
 *
 *   Revision 1.1.2.1  2005/12/06 10:13:25  martius
 *   openscenegraph integration started
 *
 *                                                                 *
 *                                                                         *
 ***************************************************************************/
#ifndef __PRIMITIVE_H
#define __PRIMITIVE_H

#include <osg/Matrix>
#include <ode/common.h>

// another forward declaration "block"
#include "osgforwarddecl.h"

namespace lpzrobots {

   /***** begin of forward declaration block *****/
   class BoundingShape;
   class OSGPrimitive;
   class OSGPlane;
   class OSGBox;
   class OSGSphere;
   class OSGCapsule;
   class OSGCylinder;
   class OSGDummy;
   class OSGMesh;
   typedef struct GlobalData;
   class OdeHandle;
   class OsgHandle;
   /*****  end of forward declaration block  *****/


/// returns the osg (4x4) pose matrix of the ode geom
osg::Matrix osgPose( dGeomID geom );
/// returns the osg (4x4) pose matrix of the ode body
osg::Matrix osgPose( dBodyID body );
/// converts a position vector and a rotation matrix from ode to osg 4x4 matrix
osg::Matrix osgPose( const double * position , const double * rotation );
/// converts the rotation component of pose into an ode rotation matrix
void odeRotation( const osg::Matrix& pose , dMatrix3& odematrix);

/**
   Interface class for primitives represented in the physical and graphical world.
   This is intended to bring OSG and ODE together and hide most implementation details.
*/
class Primitive {
public:
  /** Body means that it is a dynamic object with a body.
      Geom means it has a geometrical represenation used for collision detection.
      Draw means the primitive is drawn
  */
  typedef enum Modes {Body=1, Geom=2, Draw=4};

  Primitive ();
  virtual ~Primitive ();
  /** registers primitive in ODE and OSG. 
      @param osgHandle scruct with ODE variables inside (to specify space, world...)
      @param mass Mass of the object in ODE (if withBody = true)
      @param osgHandle scruct with OSG variables inside (scene node, color ...)
      @param mode is a conjuction of Modes.
   */
  virtual void init(const OdeHandle& odeHandle, double mass,
		    const OsgHandle& osgHandle,
		    char mode = Body | Geom | Draw)  = 0 ;

  /** Updates the OSG nodes with ODE coordinates.
      This function must be overloaded (usually calls setMatrix of OsgPrimitives)
   */
  virtual void update() =0 ;

  /// returns the assoziated osg primitive if there or 0
  virtual OSGPrimitive* getOSGPrimitive() = 0;

  /// assigns a texture to the primitive
  virtual void setTexture(const std::string& filename);
  /// assigns a texture to the primitive, you can choose if the texture should be repeated
  virtual void setTexture(const std::string& filename, bool repeatOnX, bool repeatOnY);

  /// set the position of the primitive (orientation is preserved)
  void setPosition(const osg::Vec3& pos);
  /// set the pose of the primitive
  void setPose(const osg::Matrix& pose);
  /// returns the position
  osg::Vec3 getPosition() const;
  /// returns the pose
  osg::Matrix getPose() const;

  /// returns ODE geomID if there
  dGeomID getGeom() const;    
  /// returns ODE bodyID if there
  dBodyID getBody() const;

protected:
  dGeomID geom;
  dBodyID body;
  char mode;
};


/** Plane primitive */
class Plane : public Primitive {
public:
  Plane();
  virtual ~Plane();
  virtual void init(const OdeHandle& odeHandle, double mass, 
		    const OsgHandle& osgHandle,
		    char mode = Body | Geom | Draw) ;

  virtual void update();  
  virtual OSGPrimitive* getOSGPrimitive();

protected:
  OSGPlane* osgplane;
};


/** Box primitive */
class Box : public Primitive {
public:

  Box(float lengthX, float lengthY, float lengthZ);
  virtual ~Box();

  virtual void init(const OdeHandle& odeHandle, double mass,
		    const OsgHandle& osgHandle,
		    char mode = Body | Geom | Draw) ;

  virtual void update();
  virtual OSGPrimitive* getOSGPrimitive();

protected:
  OSGBox* osgbox;
};


/** Sphere primitive */
class Sphere : public Primitive {
public:
  Sphere(float radius);
  virtual ~Sphere();

  virtual void init(const OdeHandle& odeHandle, double mass, 
		    const OsgHandle& osgHandle,
		    char mode = Body | Geom | Draw) ;

  virtual void update();
  virtual OSGPrimitive* getOSGPrimitive();

protected:
  OSGSphere* osgsphere;
};

/** Capsule primitive */
class Capsule : public Primitive {
public:
  Capsule(float radius, float height);
  virtual ~Capsule();
  virtual void init(const OdeHandle& odeHandle, double mass,
		    const OsgHandle& osgHandle,
		    char mode = Body | Geom | Draw) ;

  virtual void update();
  virtual OSGPrimitive* getOSGPrimitive();

protected:
  OSGCapsule* osgcapsule;
};

/** Cylinder primitive */
class Cylinder : public Primitive {
public:
  Cylinder(float radius, float height);
  virtual ~Cylinder();
  virtual void init(const OdeHandle& odeHandle, double mass,
		    const OsgHandle& osgHandle,
		    char mode = Body | Geom | Draw) ;

  virtual void update();
  virtual OSGPrimitive* getOSGPrimitive();

protected:
  OSGCylinder* osgcylinder;
};


/** Mesh primitive */
class Mesh : public Primitive {
public:
  Mesh(const std::string& filename,float scale,GlobalData& global);
  virtual ~Mesh();
  virtual void init(const OdeHandle& odeHandle, double mass,
		    const OsgHandle& osgHandle,
		    char mode = Body | Geom | Draw) ;
  virtual void update();
  virtual OSGPrimitive* getOSGPrimitive();
  virtual float getRadius();

protected:
  OSGMesh* osgmesh;
  char drawBoundingMode;
  const std::string filename;
  float scale;
  BoundingShape* boundshape;
};


/**
   Primitive for transforming a geom (primitive without body) 
    in respect to a body (primitive with body). 
   Hides complexity of ODE TransformGeoms. 
*/
class Transform : public Primitive {
public:
  /** 
      @param parent primitive should have a body and should be initialised
      @param child  is transformed by pose in respect to parent. 
      This Primitive must NOT have a body
  */
  Transform(Primitive* parent, Primitive* child, const osg::Matrix& pose);
  /// mode is not ignored
  virtual void init(const OdeHandle& odeHandle, double mass, 
		    const OsgHandle& osgHandle,
		    char mode = Body | Geom | Draw);

  virtual void update();
  virtual OSGPrimitive* getOSGPrimitive();

protected:
  Primitive* parent;
  Primitive* child;
  osg::Matrix pose;
};

/**
   Dummy Primitive which returns 0 for geom and body. 
   Only useful for representing the static world in terms of primitives.
*/
class DummyPrimitive : public Primitive {
public:
  /** 
      @param parent primitive should have a body and should be initialised
      @param child  is transformed by pose in respect to parent. 
      This Primitive must NOT have a body
  */
  DummyPrimitive() {     
    body=0;
    geom=0;
  }
  virtual void init(const OdeHandle& odeHandle, double mass, 
		    const OsgHandle& osgHandle, char mode = Body | Geom | Draw) {
  }
  virtual void update() {}
  virtual OSGPrimitive* getOSGPrimitive() { return 0; }
};


}
#endif
