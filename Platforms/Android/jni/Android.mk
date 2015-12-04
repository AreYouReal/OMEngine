LOCAL_PATH			:= $(call my-dir)
SRC_PATH			:= $(LOCAL_PATH)/../../..

include $(CLEAR_VARS)

LOCAL_MODULE    := OME
LOCAL_CFLAGS    += -std=c++11

LOCAL_SRC_FILES := $(LOCAL_PATH)/esUtil_Android.cpp \
$(SRC_PATH)/CPPSources/Audio/Boombox.cpp\
$(SRC_PATH)/CPPSources/Audio/Sound.cpp\
$(SRC_PATH)/CPPSources/Camera/Camera.cpp\
$(SRC_PATH)/CPPSources/Component/MeshRendererComponent.cpp\
$(SRC_PATH)/CPPSources/DEBUG/WiredCube.cpp\
$(SRC_PATH)/CPPSources/Game.cpp\
$(SRC_PATH)/CPPSources/GameObject/GameObject.cpp\
$(SRC_PATH)/CPPSources/Light/Illuminator.cpp\
$(SRC_PATH)/CPPSources/Light/LightSource.cpp\
$(SRC_PATH)/CPPSources/Material/Materials.cpp\
$(SRC_PATH)/CPPSources/Material/ObjMaterial.cpp\
$(SRC_PATH)/CPPSources/Math/m4d.cpp\
$(SRC_PATH)/CPPSources/Math/q4d.cpp\
$(SRC_PATH)/CPPSources/Math/v3d.cpp\
$(SRC_PATH)/CPPSources/Obj/Obj.cpp\
$(SRC_PATH)/CPPSources/Obj/ObjMesh.cpp\
$(SRC_PATH)/CPPSources/Physics/PhysicalWorld.cpp\
$(SRC_PATH)/CPPSources/Scene/Scene.cpp\
$(SRC_PATH)/CPPSources/Shaders/Shader.cpp\
$(SRC_PATH)/CPPSources/Shaders/ShaderHelper.cpp\
$(SRC_PATH)/CPPSources/Shaders/ShaderProgram.cpp\
$(SRC_PATH)/CPPSources/Texture/Texture.cpp\
$(SRC_PATH)/CPPSources/Transform/Transform.cpp\
$(SRC_PATH)/CPPSources/Txt/Font.cpp\
$(SRC_PATH)/CPPSources/Utils/3dPart/BulletPhysics/BulletCollision/BroadphaseCollision/btAxisSweep3.cpp\
$(SRC_PATH)/CPPSources/Utils/3dPart/BulletPhysics/BulletCollision/BroadphaseCollision/btBroadphaseProxy.cpp\
$(SRC_PATH)/CPPSources/Utils/3dPart/BulletPhysics/BulletCollision/BroadphaseCollision/btCollisionAlgorithm.cpp\
$(SRC_PATH)/CPPSources/Utils/3dPart/BulletPhysics/BulletCollision/BroadphaseCollision/btDbvt.cpp\
$(SRC_PATH)/CPPSources/Utils/3dPart/BulletPhysics/BulletCollision/BroadphaseCollision/btDbvtBroadphase.cpp\
$(SRC_PATH)/CPPSources/Utils/3dPart/BulletPhysics/BulletCollision/BroadphaseCollision/btDispatcher.cpp\
$(SRC_PATH)/CPPSources/Utils/3dPart/BulletPhysics/BulletCollision/BroadphaseCollision/btMultiSapBroadphase.cpp\
$(SRC_PATH)/CPPSources/Utils/3dPart/BulletPhysics/BulletCollision/BroadphaseCollision/btOverlappingPairCache.cpp\
$(SRC_PATH)/CPPSources/Utils/3dPart/BulletPhysics/BulletCollision/BroadphaseCollision/btQuantizedBvh.cpp\
$(SRC_PATH)/CPPSources/Utils/3dPart/BulletPhysics/BulletCollision/BroadphaseCollision/btSimpleBroadphase.cpp\
$(SRC_PATH)/CPPSources/Utils/3dPart/BulletPhysics/BulletCollision/CollisionDispatch/btActivatingCollisionAlgorithm.cpp\
$(SRC_PATH)/CPPSources/Utils/3dPart/BulletPhysics/BulletCollision/CollisionDispatch/btBox2dBox2dCollisionAlgorithm.cpp\
$(SRC_PATH)/CPPSources/Utils/3dPart/BulletPhysics/BulletCollision/CollisionDispatch/btBoxBoxCollisionAlgorithm.cpp\
$(SRC_PATH)/CPPSources/Utils/3dPart/BulletPhysics/BulletCollision/CollisionDispatch/btBoxBoxDetector.cpp\
$(SRC_PATH)/CPPSources/Utils/3dPart/BulletPhysics/BulletCollision/CollisionDispatch/btCollisionDispatcher.cpp\
$(SRC_PATH)/CPPSources/Utils/3dPart/BulletPhysics/BulletCollision/CollisionDispatch/btCollisionObject.cpp\
$(SRC_PATH)/CPPSources/Utils/3dPart/BulletPhysics/BulletCollision/CollisionDispatch/btCollisionWorld.cpp\
$(SRC_PATH)/CPPSources/Utils/3dPart/BulletPhysics/BulletCollision/CollisionDispatch/btCollisionWorldImporter.cpp\
$(SRC_PATH)/CPPSources/Utils/3dPart/BulletPhysics/BulletCollision/CollisionDispatch/btCompoundCollisionAlgorithm.cpp\
$(SRC_PATH)/CPPSources/Utils/3dPart/BulletPhysics/BulletCollision/CollisionDispatch/btCompoundCompoundCollisionAlgorithm.cpp\
$(SRC_PATH)/CPPSources/Utils/3dPart/BulletPhysics/BulletCollision/CollisionDispatch/btConvex2dConvex2dAlgorithm.cpp\
$(SRC_PATH)/CPPSources/Utils/3dPart/BulletPhysics/BulletCollision/CollisionDispatch/btConvexConcaveCollisionAlgorithm.cpp\
$(SRC_PATH)/CPPSources/Utils/3dPart/BulletPhysics/BulletCollision/CollisionDispatch/btConvexConvexAlgorithm.cpp\
$(SRC_PATH)/CPPSources/Utils/3dPart/BulletPhysics/BulletCollision/CollisionDispatch/btConvexPlaneCollisionAlgorithm.cpp\
$(SRC_PATH)/CPPSources/Utils/3dPart/BulletPhysics/BulletCollision/CollisionDispatch/btDefaultCollisionConfiguration.cpp\
$(SRC_PATH)/CPPSources/Utils/3dPart/BulletPhysics/BulletCollision/CollisionDispatch/btEmptyCollisionAlgorithm.cpp\
$(SRC_PATH)/CPPSources/Utils/3dPart/BulletPhysics/BulletCollision/CollisionDispatch/btGhostObject.cpp\
$(SRC_PATH)/CPPSources/Utils/3dPart/BulletPhysics/BulletCollision/CollisionDispatch/btHashedSimplePairCache.cpp\
$(SRC_PATH)/CPPSources/Utils/3dPart/BulletPhysics/BulletCollision/CollisionDispatch/btInternalEdgeUtility.cpp\
$(SRC_PATH)/CPPSources/Utils/3dPart/BulletPhysics/BulletCollision/CollisionDispatch/btManifoldResult.cpp\
$(SRC_PATH)/CPPSources/Utils/3dPart/BulletPhysics/BulletCollision/CollisionDispatch/btSimulationIslandManager.cpp\
$(SRC_PATH)/CPPSources/Utils/3dPart/BulletPhysics/BulletCollision/CollisionDispatch/btSphereBoxCollisionAlgorithm.cpp\
$(SRC_PATH)/CPPSources/Utils/3dPart/BulletPhysics/BulletCollision/CollisionDispatch/btSphereSphereCollisionAlgorithm.cpp\
$(SRC_PATH)/CPPSources/Utils/3dPart/BulletPhysics/BulletCollision/CollisionDispatch/btSphereTriangleCollisionAlgorithm.cpp\
$(SRC_PATH)/CPPSources/Utils/3dPart/BulletPhysics/BulletCollision/CollisionDispatch/btUnionFind.cpp\
$(SRC_PATH)/CPPSources/Utils/3dPart/BulletPhysics/BulletCollision/CollisionDispatch/SphereTriangleDetector.cpp\
$(SRC_PATH)/CPPSources/Utils/3dPart/BulletPhysics/BulletCollision/CollisionShapes/btBox2dShape.cpp\
$(SRC_PATH)/CPPSources/Utils/3dPart/BulletPhysics/BulletCollision/CollisionShapes/btBoxShape.cpp\
$(SRC_PATH)/CPPSources/Utils/3dPart/BulletPhysics/BulletCollision/CollisionShapes/btBvhTriangleMeshShape.cpp\
$(SRC_PATH)/CPPSources/Utils/3dPart/BulletPhysics/BulletCollision/CollisionShapes/btCapsuleShape.cpp\
$(SRC_PATH)/CPPSources/Utils/3dPart/BulletPhysics/BulletCollision/CollisionShapes/btCollisionShape.cpp\
$(SRC_PATH)/CPPSources/Utils/3dPart/BulletPhysics/BulletCollision/CollisionShapes/btCompoundShape.cpp\
$(SRC_PATH)/CPPSources/Utils/3dPart/BulletPhysics/BulletCollision/CollisionShapes/btConcaveShape.cpp\
$(SRC_PATH)/CPPSources/Utils/3dPart/BulletPhysics/BulletCollision/CollisionShapes/btConeShape.cpp\
$(SRC_PATH)/CPPSources/Utils/3dPart/BulletPhysics/BulletCollision/CollisionShapes/btConvex2dShape.cpp\
$(SRC_PATH)/CPPSources/Utils/3dPart/BulletPhysics/BulletCollision/CollisionShapes/btConvexHullShape.cpp\
$(SRC_PATH)/CPPSources/Utils/3dPart/BulletPhysics/BulletCollision/CollisionShapes/btConvexInternalShape.cpp\
$(SRC_PATH)/CPPSources/Utils/3dPart/BulletPhysics/BulletCollision/CollisionShapes/btConvexPointCloudShape.cpp\
$(SRC_PATH)/CPPSources/Utils/3dPart/BulletPhysics/BulletCollision/CollisionShapes/btConvexPolyhedron.cpp\
$(SRC_PATH)/CPPSources/Utils/3dPart/BulletPhysics/BulletCollision/CollisionShapes/btConvexShape.cpp\
$(SRC_PATH)/CPPSources/Utils/3dPart/BulletPhysics/BulletCollision/CollisionShapes/btConvexTriangleMeshShape.cpp\
$(SRC_PATH)/CPPSources/Utils/3dPart/BulletPhysics/BulletCollision/CollisionShapes/btCylinderShape.cpp\
$(SRC_PATH)/CPPSources/Utils/3dPart/BulletPhysics/BulletCollision/CollisionShapes/btEmptyShape.cpp\
$(SRC_PATH)/CPPSources/Utils/3dPart/BulletPhysics/BulletCollision/CollisionShapes/btHeightfieldTerrainShape.cpp\
$(SRC_PATH)/CPPSources/Utils/3dPart/BulletPhysics/BulletCollision/CollisionShapes/btMinkowskiSumShape.cpp\
$(SRC_PATH)/CPPSources/Utils/3dPart/BulletPhysics/BulletCollision/CollisionShapes/btMultimaterialTriangleMeshShape.cpp\
$(SRC_PATH)/CPPSources/Utils/3dPart/BulletPhysics/BulletCollision/CollisionShapes/btMultiSphereShape.cpp\
$(SRC_PATH)/CPPSources/Utils/3dPart/BulletPhysics/BulletCollision/CollisionShapes/btOptimizedBvh.cpp\
$(SRC_PATH)/CPPSources/Utils/3dPart/BulletPhysics/BulletCollision/CollisionShapes/btPolyhedralConvexShape.cpp\
$(SRC_PATH)/CPPSources/Utils/3dPart/BulletPhysics/BulletCollision/CollisionShapes/btScaledBvhTriangleMeshShape.cpp\
$(SRC_PATH)/CPPSources/Utils/3dPart/BulletPhysics/BulletCollision/CollisionShapes/btShapeHull.cpp\
$(SRC_PATH)/CPPSources/Utils/3dPart/BulletPhysics/BulletCollision/CollisionShapes/btSphereShape.cpp\
$(SRC_PATH)/CPPSources/Utils/3dPart/BulletPhysics/BulletCollision/CollisionShapes/btStaticPlaneShape.cpp\
$(SRC_PATH)/CPPSources/Utils/3dPart/BulletPhysics/BulletCollision/CollisionShapes/btStridingMeshInterface.cpp\
$(SRC_PATH)/CPPSources/Utils/3dPart/BulletPhysics/BulletCollision/CollisionShapes/btTetrahedronShape.cpp\
$(SRC_PATH)/CPPSources/Utils/3dPart/BulletPhysics/BulletCollision/CollisionShapes/btTriangleBuffer.cpp\
$(SRC_PATH)/CPPSources/Utils/3dPart/BulletPhysics/BulletCollision/CollisionShapes/btTriangleCallback.cpp\
$(SRC_PATH)/CPPSources/Utils/3dPart/BulletPhysics/BulletCollision/CollisionShapes/btTriangleIndexVertexArray.cpp\
$(SRC_PATH)/CPPSources/Utils/3dPart/BulletPhysics/BulletCollision/CollisionShapes/btTriangleIndexVertexMaterialArray.cpp\
$(SRC_PATH)/CPPSources/Utils/3dPart/BulletPhysics/BulletCollision/CollisionShapes/btTriangleMesh.cpp\
$(SRC_PATH)/CPPSources/Utils/3dPart/BulletPhysics/BulletCollision/CollisionShapes/btTriangleMeshShape.cpp\
$(SRC_PATH)/CPPSources/Utils/3dPart/BulletPhysics/BulletCollision/CollisionShapes/btUniformScalingShape.cpp\
$(SRC_PATH)/CPPSources/Utils/3dPart/BulletPhysics/BulletCollision/Gimpact/btContactProcessing.cpp\
$(SRC_PATH)/CPPSources/Utils/3dPart/BulletPhysics/BulletCollision/Gimpact/btGenericPoolAllocator.cpp\
$(SRC_PATH)/CPPSources/Utils/3dPart/BulletPhysics/BulletCollision/Gimpact/btGImpactBvh.cpp\
$(SRC_PATH)/CPPSources/Utils/3dPart/BulletPhysics/BulletCollision/Gimpact/btGImpactCollisionAlgorithm.cpp\
$(SRC_PATH)/CPPSources/Utils/3dPart/BulletPhysics/BulletCollision/Gimpact/btGImpactQuantizedBvh.cpp\
$(SRC_PATH)/CPPSources/Utils/3dPart/BulletPhysics/BulletCollision/Gimpact/btGImpactShape.cpp\
$(SRC_PATH)/CPPSources/Utils/3dPart/BulletPhysics/BulletCollision/Gimpact/btTriangleShapeEx.cpp\
$(SRC_PATH)/CPPSources/Utils/3dPart/BulletPhysics/BulletCollision/Gimpact/gim_box_set.cpp\
$(SRC_PATH)/CPPSources/Utils/3dPart/BulletPhysics/BulletCollision/Gimpact/gim_contact.cpp\
$(SRC_PATH)/CPPSources/Utils/3dPart/BulletPhysics/BulletCollision/Gimpact/gim_memory.cpp\
$(SRC_PATH)/CPPSources/Utils/3dPart/BulletPhysics/BulletCollision/Gimpact/gim_tri_collision.cpp\
$(SRC_PATH)/CPPSources/Utils/3dPart/BulletPhysics/BulletCollision/NarrowPhaseCollision/btContinuousConvexCollision.cpp\
$(SRC_PATH)/CPPSources/Utils/3dPart/BulletPhysics/BulletCollision/NarrowPhaseCollision/btConvexCast.cpp\
$(SRC_PATH)/CPPSources/Utils/3dPart/BulletPhysics/BulletCollision/NarrowPhaseCollision/btGjkConvexCast.cpp\
$(SRC_PATH)/CPPSources/Utils/3dPart/BulletPhysics/BulletCollision/NarrowPhaseCollision/btGjkEpa2.cpp\
$(SRC_PATH)/CPPSources/Utils/3dPart/BulletPhysics/BulletCollision/NarrowPhaseCollision/btGjkEpaPenetrationDepthSolver.cpp\
$(SRC_PATH)/CPPSources/Utils/3dPart/BulletPhysics/BulletCollision/NarrowPhaseCollision/btGjkPairDetector.cpp\
$(SRC_PATH)/CPPSources/Utils/3dPart/BulletPhysics/BulletCollision/NarrowPhaseCollision/btMinkowskiPenetrationDepthSolver.cpp\
$(SRC_PATH)/CPPSources/Utils/3dPart/BulletPhysics/BulletCollision/NarrowPhaseCollision/btPersistentManifold.cpp\
$(SRC_PATH)/CPPSources/Utils/3dPart/BulletPhysics/BulletCollision/NarrowPhaseCollision/btPolyhedralContactClipping.cpp\
$(SRC_PATH)/CPPSources/Utils/3dPart/BulletPhysics/BulletCollision/NarrowPhaseCollision/btRaycastCallback.cpp\
$(SRC_PATH)/CPPSources/Utils/3dPart/BulletPhysics/BulletCollision/NarrowPhaseCollision/btSubSimplexConvexCast.cpp\
$(SRC_PATH)/CPPSources/Utils/3dPart/BulletPhysics/BulletCollision/NarrowPhaseCollision/btVoronoiSimplexSolver.cpp\
$(SRC_PATH)/CPPSources/Utils/3dPart/BulletPhysics/BulletDynamics/Character/btKinematicCharacterController.cpp\
$(SRC_PATH)/CPPSources/Utils/3dPart/BulletPhysics/BulletDynamics/ConstraintSolver/btConeTwistConstraint.cpp\
$(SRC_PATH)/CPPSources/Utils/3dPart/BulletPhysics/BulletDynamics/ConstraintSolver/btContactConstraint.cpp\
$(SRC_PATH)/CPPSources/Utils/3dPart/BulletPhysics/BulletDynamics/ConstraintSolver/btFixedConstraint.cpp\
$(SRC_PATH)/CPPSources/Utils/3dPart/BulletPhysics/BulletDynamics/ConstraintSolver/btGearConstraint.cpp\
$(SRC_PATH)/CPPSources/Utils/3dPart/BulletPhysics/BulletDynamics/ConstraintSolver/btGeneric6DofConstraint.cpp\
$(SRC_PATH)/CPPSources/Utils/3dPart/BulletPhysics/BulletDynamics/ConstraintSolver/btGeneric6DofSpring2Constraint.cpp\
$(SRC_PATH)/CPPSources/Utils/3dPart/BulletPhysics/BulletDynamics/ConstraintSolver/btGeneric6DofSpringConstraint.cpp\
$(SRC_PATH)/CPPSources/Utils/3dPart/BulletPhysics/BulletDynamics/ConstraintSolver/btHinge2Constraint.cpp\
$(SRC_PATH)/CPPSources/Utils/3dPart/BulletPhysics/BulletDynamics/ConstraintSolver/btHingeConstraint.cpp\
$(SRC_PATH)/CPPSources/Utils/3dPart/BulletPhysics/BulletDynamics/ConstraintSolver/btNNCGConstraintSolver.cpp\
$(SRC_PATH)/CPPSources/Utils/3dPart/BulletPhysics/BulletDynamics/ConstraintSolver/btPoint2PointConstraint.cpp\
$(SRC_PATH)/CPPSources/Utils/3dPart/BulletPhysics/BulletDynamics/ConstraintSolver/btSequentialImpulseConstraintSolver.cpp\
$(SRC_PATH)/CPPSources/Utils/3dPart/BulletPhysics/BulletDynamics/ConstraintSolver/btSliderConstraint.cpp\
$(SRC_PATH)/CPPSources/Utils/3dPart/BulletPhysics/BulletDynamics/ConstraintSolver/btSolve2LinearConstraint.cpp\
$(SRC_PATH)/CPPSources/Utils/3dPart/BulletPhysics/BulletDynamics/ConstraintSolver/btTypedConstraint.cpp\
$(SRC_PATH)/CPPSources/Utils/3dPart/BulletPhysics/BulletDynamics/ConstraintSolver/btUniversalConstraint.cpp\
$(SRC_PATH)/CPPSources/Utils/3dPart/BulletPhysics/BulletDynamics/Dynamics/btDiscreteDynamicsWorld.cpp\
$(SRC_PATH)/CPPSources/Utils/3dPart/BulletPhysics/BulletDynamics/Dynamics/btRigidBody.cpp\
$(SRC_PATH)/CPPSources/Utils/3dPart/BulletPhysics/BulletDynamics/Dynamics/btSimpleDynamicsWorld.cpp\
$(SRC_PATH)/CPPSources/Utils/3dPart/BulletPhysics/BulletDynamics/Featherstone/btMultiBody.cpp\
$(SRC_PATH)/CPPSources/Utils/3dPart/BulletPhysics/BulletDynamics/Featherstone/btMultiBodyConstraint.cpp\
$(SRC_PATH)/CPPSources/Utils/3dPart/BulletPhysics/BulletDynamics/Featherstone/btMultiBodyConstraintSolver.cpp\
$(SRC_PATH)/CPPSources/Utils/3dPart/BulletPhysics/BulletDynamics/Featherstone/btMultiBodyDynamicsWorld.cpp\
$(SRC_PATH)/CPPSources/Utils/3dPart/BulletPhysics/BulletDynamics/Featherstone/btMultiBodyJointLimitConstraint.cpp\
$(SRC_PATH)/CPPSources/Utils/3dPart/BulletPhysics/BulletDynamics/Featherstone/btMultiBodyJointMotor.cpp\
$(SRC_PATH)/CPPSources/Utils/3dPart/BulletPhysics/BulletDynamics/Featherstone/btMultiBodyPoint2Point.cpp\
$(SRC_PATH)/CPPSources/Utils/3dPart/BulletPhysics/BulletDynamics/MLCPSolvers/btDantzigLCP.cpp\
$(SRC_PATH)/CPPSources/Utils/3dPart/BulletPhysics/BulletDynamics/MLCPSolvers/btLemkeAlgorithm.cpp\
$(SRC_PATH)/CPPSources/Utils/3dPart/BulletPhysics/BulletDynamics/MLCPSolvers/btMLCPSolver.cpp\
$(SRC_PATH)/CPPSources/Utils/3dPart/BulletPhysics/BulletDynamics/Vehicle/btRaycastVehicle.cpp\
$(SRC_PATH)/CPPSources/Utils/3dPart/BulletPhysics/BulletDynamics/Vehicle/btWheelInfo.cpp\
$(SRC_PATH)/CPPSources/Utils/3dPart/BulletPhysics/BulletSoftBody/btDefaultSoftBodySolver.cpp\
$(SRC_PATH)/CPPSources/Utils/3dPart/BulletPhysics/BulletSoftBody/btSoftBody.cpp\
$(SRC_PATH)/CPPSources/Utils/3dPart/BulletPhysics/BulletSoftBody/btSoftBodyConcaveCollisionAlgorithm.cpp\
$(SRC_PATH)/CPPSources/Utils/3dPart/BulletPhysics/BulletSoftBody/btSoftBodyHelpers.cpp\
$(SRC_PATH)/CPPSources/Utils/3dPart/BulletPhysics/BulletSoftBody/btSoftBodyRigidBodyCollisionConfiguration.cpp\
$(SRC_PATH)/CPPSources/Utils/3dPart/BulletPhysics/BulletSoftBody/btSoftRigidCollisionAlgorithm.cpp\
$(SRC_PATH)/CPPSources/Utils/3dPart/BulletPhysics/BulletSoftBody/btSoftRigidDynamicsWorld.cpp\
$(SRC_PATH)/CPPSources/Utils/3dPart/BulletPhysics/BulletSoftBody/btSoftSoftCollisionAlgorithm.cpp\
$(SRC_PATH)/CPPSources/Utils/3dPart/BulletPhysics/Extras/Serialize/BulletFileLoader/bChunk.cpp\
$(SRC_PATH)/CPPSources/Utils/3dPart/BulletPhysics/Extras/Serialize/BulletFileLoader/bDNA.cpp\
$(SRC_PATH)/CPPSources/Utils/3dPart/BulletPhysics/Extras/Serialize/BulletFileLoader/bFile.cpp\
$(SRC_PATH)/CPPSources/Utils/3dPart/BulletPhysics/Extras/Serialize/BulletFileLoader/btBulletFile.cpp\
$(SRC_PATH)/CPPSources/Utils/3dPart/BulletPhysics/Extras/Serialize/BulletWorldImporter/btBulletWorldImporter.cpp\
$(SRC_PATH)/CPPSources/Utils/3dPart/BulletPhysics/Extras/Serialize/BulletWorldImporter/btWorldImporter.cpp\
$(SRC_PATH)/CPPSources/Utils/3dPart/BulletPhysics/LinearMath/btAlignedAllocator.cpp\
$(SRC_PATH)/CPPSources/Utils/3dPart/BulletPhysics/LinearMath/btConvexHull.cpp\
$(SRC_PATH)/CPPSources/Utils/3dPart/BulletPhysics/LinearMath/btConvexHullComputer.cpp\
$(SRC_PATH)/CPPSources/Utils/3dPart/BulletPhysics/LinearMath/btGeometryUtil.cpp\
$(SRC_PATH)/CPPSources/Utils/3dPart/BulletPhysics/LinearMath/btPolarDecomposition.cpp\
$(SRC_PATH)/CPPSources/Utils/3dPart/BulletPhysics/LinearMath/btQuickprof.cpp\
$(SRC_PATH)/CPPSources/Utils/3dPart/BulletPhysics/LinearMath/btSerializer.cpp\
$(SRC_PATH)/CPPSources/Utils/3dPart/BulletPhysics/LinearMath/btVector3.cpp\
$(SRC_PATH)/CPPSources/Utils/3dPart/loadpng/lodepng.cpp\
$(SRC_PATH)/CPPSources/Utils/3dPart/nvtristrip/NvTriStrip.cpp\
$(SRC_PATH)/CPPSources/Utils/3dPart/nvtristrip/NvTriStripObjects.cpp\
$(SRC_PATH)/CPPSources/Utils/3dPart/nvtristrip/VertexCache.cpp\
$(SRC_PATH)/CPPSources/Utils/3dPart/openal/alAuxEffectSlot.c\
$(SRC_PATH)/CPPSources/Utils/3dPart/openal/alBuffer.c\
$(SRC_PATH)/CPPSources/Utils/3dPart/openal/ALc.c\
$(SRC_PATH)/CPPSources/Utils/3dPart/openal/alcConfig.c\
$(SRC_PATH)/CPPSources/Utils/3dPart/openal/alcEcho.c\
$(SRC_PATH)/CPPSources/Utils/3dPart/openal/alcModulator.c\
$(SRC_PATH)/CPPSources/Utils/3dPart/openal/alcReverb.c\
$(SRC_PATH)/CPPSources/Utils/3dPart/openal/alcRing.c\
$(SRC_PATH)/CPPSources/Utils/3dPart/openal/alcThread.c\
$(SRC_PATH)/CPPSources/Utils/3dPart/openal/alDatabuffer.c\
$(SRC_PATH)/CPPSources/Utils/3dPart/openal/alEffect.c\
$(SRC_PATH)/CPPSources/Utils/3dPart/openal/alError.c\
$(SRC_PATH)/CPPSources/Utils/3dPart/openal/alExtension.c\
$(SRC_PATH)/CPPSources/Utils/3dPart/openal/alFilter.c\
$(SRC_PATH)/CPPSources/Utils/3dPart/openal/alListener.c\
$(SRC_PATH)/CPPSources/Utils/3dPart/openal/alSource.c\
$(SRC_PATH)/CPPSources/Utils/3dPart/openal/alState.c\
$(SRC_PATH)/CPPSources/Utils/3dPart/openal/alThunk.c\
$(SRC_PATH)/CPPSources/Utils/3dPart/openal/ALu.c\
$(SRC_PATH)/CPPSources/Utils/3dPart/openal/android.c\
$(SRC_PATH)/CPPSources/Utils/3dPart/openal/bs2b.c\
$(SRC_PATH)/CPPSources/Utils/3dPart/openal/null.c\
$(SRC_PATH)/CPPSources/Utils/3dPart/ttf/stb_truetype.cpp\
$(SRC_PATH)/CPPSources/Utils/3dPart/vorbis/analysis.c\
$(SRC_PATH)/CPPSources/Utils/3dPart/vorbis/bitrate.c\
$(SRC_PATH)/CPPSources/Utils/3dPart/vorbis/bitwise.c\
$(SRC_PATH)/CPPSources/Utils/3dPart/vorbis/block.c\
$(SRC_PATH)/CPPSources/Utils/3dPart/vorbis/codebook.c\
$(SRC_PATH)/CPPSources/Utils/3dPart/vorbis/envelope.c\
$(SRC_PATH)/CPPSources/Utils/3dPart/vorbis/floor0.c\
$(SRC_PATH)/CPPSources/Utils/3dPart/vorbis/floor1.c\
$(SRC_PATH)/CPPSources/Utils/3dPart/vorbis/framing.c\
$(SRC_PATH)/CPPSources/Utils/3dPart/vorbis/info.c\
$(SRC_PATH)/CPPSources/Utils/3dPart/vorbis/lookup.c\
$(SRC_PATH)/CPPSources/Utils/3dPart/vorbis/lpc.c\
$(SRC_PATH)/CPPSources/Utils/3dPart/vorbis/lsp.c\
$(SRC_PATH)/CPPSources/Utils/3dPart/vorbis/mapping0.c\
$(SRC_PATH)/CPPSources/Utils/3dPart/vorbis/mdct.c\
$(SRC_PATH)/CPPSources/Utils/3dPart/vorbis/psy.c\
$(SRC_PATH)/CPPSources/Utils/3dPart/vorbis/registry.c\
$(SRC_PATH)/CPPSources/Utils/3dPart/vorbis/res0.c\
$(SRC_PATH)/CPPSources/Utils/3dPart/vorbis/sharedbook.c\
$(SRC_PATH)/CPPSources/Utils/3dPart/vorbis/smallft.c\
$(SRC_PATH)/CPPSources/Utils/3dPart/vorbis/synthesis.c\
$(SRC_PATH)/CPPSources/Utils/3dPart/vorbis/vorbisfile.c\
$(SRC_PATH)/CPPSources/Utils/3dPart/vorbis/window.c\
$(SRC_PATH)/CPPSources/Utils/OMUtils.cpp\


LOCAL_C_INCLUDES	:= $(SRC_PATH) \
 $(COMMON_INC_PATH) \
$(SRC_PATH)/CPPSources/Audio\
$(SRC_PATH)/CPPSources/Camera\
$(SRC_PATH)/CPPSources/Component\
$(SRC_PATH)/CPPSources/DEBUG\
$(SRC_PATH)/CPPSources\
$(SRC_PATH)/CPPSources/GameObject\
$(SRC_PATH)/CPPSources/Light\
$(SRC_PATH)/CPPSources/Material\
$(SRC_PATH)/CPPSources/Math\
$(SRC_PATH)/CPPSources/Obj\
$(SRC_PATH)/CPPSources/Physics\
$(SRC_PATH)/CPPSources/Scene\
$(SRC_PATH)/CPPSources/Shaders\
$(SRC_PATH)/CPPSources/Texture\
$(SRC_PATH)/CPPSources/Transform\
$(SRC_PATH)/CPPSources/Txt\
$(SRC_PATH)/CPPSources/Utils/3dPart/BulletPhysics\
$(SRC_PATH)/CPPSources/Utils/3dPart/BulletPhysics/BulletCollision/BroadphaseCollision\
$(SRC_PATH)/CPPSources/Utils/3dPart/BulletPhysics/BulletCollision/CollisionDispatch\
$(SRC_PATH)/CPPSources/Utils/3dPart/BulletPhysics/BulletCollision/CollisionShapes\
$(SRC_PATH)/CPPSources/Utils/3dPart/BulletPhysics/BulletCollision/Gimpact\
$(SRC_PATH)/CPPSources/Utils/3dPart/BulletPhysics/BulletCollision/NarrowPhaseCollision\
$(SRC_PATH)/CPPSources/Utils/3dPart/BulletPhysics/BulletDynamics/Character\
$(SRC_PATH)/CPPSources/Utils/3dPart/BulletPhysics/BulletDynamics/ConstraintSolver\
$(SRC_PATH)/CPPSources/Utils/3dPart/BulletPhysics/BulletDynamics/Dynamics\
$(SRC_PATH)/CPPSources/Utils/3dPart/BulletPhysics/BulletDynamics/Featherstone\
$(SRC_PATH)/CPPSources/Utils/3dPart/BulletPhysics/BulletDynamics/MLCPSolvers\
$(SRC_PATH)/CPPSources/Utils/3dPart/BulletPhysics/BulletDynamics/Vehicle\
$(SRC_PATH)/CPPSources/Utils/3dPart/BulletPhysics/BulletSoftBody\
$(SRC_PATH)/CPPSources/Utils/3dPart/BulletPhysics/Extras/Serialize/BulletFileLoader/autogenerated\
$(SRC_PATH)/CPPSources/Utils/3dPart/BulletPhysics/Extras/Serialize/BulletFileLoader\
$(SRC_PATH)/CPPSources/Utils/3dPart/BulletPhysics/Extras/Serialize/BulletWorldImporter\
$(SRC_PATH)/CPPSources/Utils/3dPart/BulletPhysics/LinearMath\
$(SRC_PATH)/CPPSources/Utils/3dPart/loadpng\
$(SRC_PATH)/CPPSources/Utils/3dPart/nvtristrip\
$(SRC_PATH)/CPPSources/Utils/3dPart/openal\
$(SRC_PATH)/CPPSources/Utils/3dPart/ttf\
$(SRC_PATH)/CPPSources/Utils/3dPart/vorbis\
$(SRC_PATH)/CPPSources/Utils\


LOCAL_LDLIBS    := -llog -landroid -lEGL -lGLESv3

LOCAL_STATIC_LIBRARIES := android_native_app_glue

include $(BUILD_SHARED_LIBRARY)

$(call import-module,android/native_app_glue)