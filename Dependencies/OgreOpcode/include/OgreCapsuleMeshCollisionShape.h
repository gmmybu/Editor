///////////////////////////////////////////////////////////////////////////////
///  @file OgreCapsule.h
///  @brief This class represents a CapsuleMeshCollisionShape, which is defined by 2 endpoints and a radius.
///			You can interpret it as a sphere that is sweept along a line.
///
///  @author The OgreOpcode Team
///  
///////////////////////////////////////////////////////////////////////////////
///  
///  This file is part of OgreOpcode.
///  
///  A lot of the code is based on the Nebula Opcode Collision module, see docs/Nebula_license.txt
///  
///  OgreOpcode is free software; you can redistribute it and/or
///  modify it under the terms of the GNU Lesser General Public
///  License as published by the Free Software Foundation; either
///  version 2.1 of the License, or (at your option) any later version.
///  
///  OgreOpcode is distributed in the hope that it will be useful,
///  but WITHOUT ANY WARRANTY; without even the implied warranty of
///  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
///  Lesser General Public License for more details.
///  
///  You should have received a copy of the GNU Lesser General Public
///  License along with OgreOpcode; if not, write to the Free Software
///  Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
///  
///////////////////////////////////////////////////////////////////////////////
#ifndef __OgreOpcodeCapsule_h__
#define __OgreOpcodeCapsule_h__

#include "OgreOpcodeLine.h"

#include "OgreOpcodeExports.h"
# include <Ogre.h>

#include "IOgreCollisionShape.h"
#include "OgreCollisionTypes.h"
#include "OgreOpcodeDebugObject.h"
#include "Opcode/Opcode.h"

namespace OgreOpcode
{
	/// Represents a CapsuleMeshCollisionShape defined by 2 endpoints and a radius
	/// TODO: add methods to this capsule.
	class _OgreOpcode_Export CapsuleMeshCollisionShape : public ICollisionShape
	{
	public:
		/// Constructs a SphereMeshCollisionShape
		CapsuleMeshCollisionShape(const Ogre::String& name);
		virtual ~CapsuleMeshCollisionShape();

		/** Default constructor: degenerated as an unitary sphere at origin
		 */
/*		CapsuleMeshCollisionShape():start(),end(),radius(1.0)
		{
		}
*/
		/// load collide geometry from mesh, and build a collision tree
		virtual bool load(const Ogre::String& name, Ogre::SceneNode* scnNode, const float radius, const float height, const int nRings = 16, const int nSegments = 16);

		/// Retrieve current vertex data from mesh and refit collision tree.
		/// This is an O(n) operation in the number of vertices in the mesh.
		virtual bool refit();

		/** Copy-constructor
		 */
/*		CapsuleMeshCollisionShape(const CapsuleMeshCollisionShape& c):start(c.start),end(c.end),radius(c.radius)
		{
		}
*/
		/** Complete constructor
		 */
/*		CapsuleMeshCollisionShape(const Ogre::Vector3& s, const Ogre::Vector3& e, Ogre::Real r ):start(s),end(e),radius(r)
		{
		}
*/
		/** Complete, headache constructor
		 */
/*		CapsuleMeshCollisionShape( Ogre::Real sx, Ogre::Real sy, Ogre::Real sz,
			     Ogre::Real ex, Ogre::Real ey, Ogre::Real ez, 
				 Ogre::Real r
			   ):start(sx,sy,sz),end(ex,ey,ez),radius(r)
		{
		}

		/// Gets the length of this line segment
		Ogre::Real length() const {  return (start - end).length(); }
		/// Gets the squared length of this line segment
		Ogre::Real length2() const {  return (start - end).squaredLength(); }

		/// Gets the surface area of this capsule
		Ogre::Real area() const
		{					
			return Ogre::Math::TWO_PI*radius*(2.0*radius + length() );
		}

		/// Gets the volume are this capsule
		Ogre::Real volume() const
		{
			return Ogre::Math::PI*radius*radius*( 1.333333333333333*length() );
		}
*/
// --------------------------------------------------------------------
// intersection tests

		/** Does this capsule contain the given point?
		 */
//		bool contains( const Ogre::Vector3& point ) const;

		/** Tests intersection between this capsule and the given Axis-Aligned
		 *  Bounding Box
         */
//		bool intersects( const Aabb& aabb ) const;

		/** Tests intersection between this capsule and the given sphere
		 */
//		bool intersects( const sphere& s ) const;

		/** Tests intersection between this capsule and the given Oriented Bounding Box
		 */
//		bool intersects( const OrientedBox& obb ) const;

		/** Tests intersection between this capsule and the given one
		 */
//		bool intersects( const CapsuleMeshCollisionShape& cap ) const;


		/** The start point of this capsule.
		 */
//		Ogre::Vector3 start;
		/** The end point of this capsule.
		 */
//		Ogre::Vector3 end;
		/** The radius of this capsule.
		 */
//		Ogre::Real radius;		
	protected:
		
		/// Reload the collision geometry from mesh, rebuild collision tree from scratch. 
		/// Potentially very slow. Only necessary if the mesh has drastically changed,
		/// like topology changing deformations, or a change in the number of tris.
		/// In most cases RefitToMesh() is sufficient, and much faster.
		/// Under usual circumstances there is no need to call this method.
		virtual bool rebuild();
		/// Refits the collision tree to the currently cached vertex data.
		/// This is an O(n) operation in the number of vertices in the mesh.
		/// This is an advanced method.  It assumes that the user is manually 
		/// updating both the SphereMeshCollisionShape's cached data and the actual mesh
		/// hardware buffers.  Mostly useful for implementing something like 
		/// deformable body physics.
		virtual bool _refitToCachedData();
		/// rebuild collision tree from scratch using currently cached vertex data
		/// This is potentially quite slow.  Only necessary if the mesh has drastically changed,
		/// like topology changing deformations, or a change in the number of tris.
		/// In most cases _RefitToCachedGeometry() is sufficient, and much faster.
		/// This is an advanced method.  It assumes that the user is manually 
		/// updating both the SphereMeshCollisionShape's cached data and the actual mesh
		/// hardware buffers.  Mostly useful for implementing something like
		/// deformable body physics.
		virtual bool _rebuildFromCachedData();

	private:
		Ogre::Entity* mEntity;

		void createCapsule(const std::string& strName, const float radius, const float height, const int nRings = 16, const int nSegments = 16);
		/// Count up the total number of vertices and indices in the Ogre mesh
		void countIndicesAndVertices(Ogre::Entity * entity, size_t & index_count, size_t & vertex_count);
		/// Convert ogre Mesh to simple float and int arrays
		void convertMeshData(Ogre::Entity * entity, float * vertexData, size_t vertex_count, size_t * faceData=0, size_t index_count=0);

		/// prevent default construction
		CapsuleMeshCollisionShape();
	};
}

#endif
