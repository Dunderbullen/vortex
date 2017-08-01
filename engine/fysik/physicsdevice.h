#pragma once
#include "core/refcounted.h"
#include "foundation/util/array.h"
#include <map>
#include "surfacecollider.h"
#include "physicsserver.h"

namespace Physics
{

struct SupportPoint
{
    // Minkowski difference
    Math::point pointDiff;

    //Individual point
    Math::point pointA;

    bool operator==(const SupportPoint& r) const
    {
        return pointDiff == r.pointDiff;
    }
};

struct Triangle
{
    SupportPoint points[3];
    Math::vec4 faceNormal;

    Triangle(const SupportPoint &a,const SupportPoint &b,const SupportPoint &c) {
        points[0] = a;
        points[1] = b;
        points[2] = c;
        //Remember to possibly having to normalize this before using it!
        faceNormal = Math::point::normalize(Math::point::cross3((b.pointDiff-a.pointDiff), (c.pointDiff-a.pointDiff)));
    }
};

struct Edge {
    SupportPoint points[2];

    Edge(const SupportPoint &a,const SupportPoint &b) {
        points[0] = a;
        points[1] = b;
    }
};

class PhysicsDevice
{
private:
    PhysicsDevice();

    struct PhysicsCollision
    {
        Math::point point;
        Math::point normal;
        float penetrationDepth;
    };


public:
    static PhysicsDevice* Instance()
    {
        static PhysicsDevice instance;
        return &instance;
    }

    // C++ 11
    // Delete the methods we don't want.
    PhysicsDevice(PhysicsDevice const&) = delete;
    void operator=(PhysicsDevice const&) = delete;

    void AddRigidBody(Ptr<RigidBody> rBody);
	void RemoveRigidBody(Ptr<RigidBody> rBody);

    void Solve();

    int GetCurrentAABBSortAxis() { return this->sortAxis; }

    bool hasCollision;

private:
    double time;
    double frameTime;

    Math::point Support(const Math::point& dir, const Ptr<Physics::SurfaceCollider>& entity);
	bool CheckForCollision(PhysicsEntity* E1, PhysicsEntity* E2, PhysicsCollision& collisionData);

	bool GJK(PhysicsEntity* E1, PhysicsEntity* E2, Util::Array<SupportPoint>& simplex);
	PhysicsCollision EPA(PhysicsEntity* E1, PhysicsEntity* E2, Util::Array<SupportPoint>& simplex);

    void BroadPhase();
    void NarrowPhase();

	void CollideEntities(PhysicsEntity* E1, PhysicsEntity* E2, const PhysicsCollision& collData);

    Util::Array<Ptr<RigidBody>> rigidBodies;

    // Potentially Colliding Entities
    Util::Array<std::pair<PhysicsEntity*, PhysicsEntity*>> PCEntities;


    int DoSimplex(Util::Array<SupportPoint> &points, Math::point &D);

    //Used for broad phase sorting
    int sortAxis;

	//This is used in both GJK and EPA so we save it temporarily as a member for quick access.
	Math::mat4 invRotE1;
	Math::mat4 invRotE2;
	Math::mat4 transformE1;
	Math::mat4 transformE2;
	Ptr<Physics::SurfaceCollider> E1Collider;
	Ptr<Physics::SurfaceCollider> E2Collider;
};
}