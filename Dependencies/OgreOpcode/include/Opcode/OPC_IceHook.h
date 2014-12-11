
// Should be included by Opcode.h if needed

	#define ICE_DONT_CHECK_COMPILER_OPTIONS

	// From Windows...
	typedef int                 BOOL;
	#ifndef FALSE
	#define FALSE               0
	#endif

	#ifndef TRUE
	#define TRUE                1
	#endif

	#include <stdio.h>
	#include <stdlib.h>
	#include <assert.h>
	#include <string.h>
	#include <float.h>
	#include <math.h>

	#ifndef ASSERT
		#define	ASSERT(exp)	{}
	#endif
	#define ICE_COMPILE_TIME_ASSERT(exp)	extern char ICE_Dummy[ (exp) ? 1 : -1 ]

	extern void Opcode_Log (const char* msg, ...);
	extern bool Opcode_Err (const char* msg, ...);

	#define	OpcodeLog			Opcode_Log
	#define	SetIceError		Opcode_Err
//	#define	Log				{}
//	#define	SetIceError(a,b)	false

	#define	EC_OUTOFMEMORY	"Out of memory"

	#include "Opcode/Ice/IcePreprocessor.h"

	#include "Opcode/Ice/IceTypes.h"
	#include "Opcode/Ice/IceFPU.h"
	#include "Opcode/Ice/IceMemoryMacros.h"

	namespace IceCore
	{
		#include "Opcode/Ice/IceUtils.h"
		#include "Opcode/Ice/IceContainer.h"
		#include "Opcode/Ice/IcePairs.h"
		#include "Opcode/Ice/IceRevisitedRadix.h"
		#include "Opcode/Ice/IceRandom.h"
	}
	using namespace IceCore;

	namespace IceMaths
	{
		#include "Opcode/Ice/IceAxes.h"
		#include "Opcode/Ice/IcePoint.h"
		#include "Opcode/Ice/IceHPoint.h"
		#include "Opcode/Ice/IceMatrix3x3.h"
		#include "Opcode/Ice/IceMatrix4x4.h"
		#include "Opcode/Ice/IcePlane.h"
		#include "Opcode/Ice/IceRay.h"
		#include "Opcode/Ice/IceIndexedTriangle.h"
		#include "Opcode/Ice/IceTriangle.h"
		#include "Opcode/Ice/IceTriList.h"
		#include "Opcode/Ice/IceAABB.h"
		#include "Opcode/Ice/IceOBB.h"
		#include "Opcode/Ice/IceBoundingSphere.h"
		#include "Opcode/Ice/IceSegment.h"
		#include "Opcode/Ice/IceLSS.h"
	}
	//using namespace IceMaths;
