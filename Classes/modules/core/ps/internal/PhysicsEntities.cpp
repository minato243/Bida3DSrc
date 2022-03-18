#include "PhysicsEntities.hpp"
#include "CCPlatformMacros.h"

using namespace ps;

Pocket::Pocket() : id(0), position(vector::ZERO), radius(0.0) 
{
	warnPolygon = PocketAABB(this);
}

ps::Pocket::Pocket(const int & id, const vector & position, const double & radius)
	: id(id), position(position),radius(radius)
{
	warnPolygon = PocketAABB(this);
}

ps::Pocket::~Pocket()
{
}

Cushion::Cushion(int id, const segment & seg) : id(id), seg(seg) {}

void ps::Cushion::collapseStartPoint(double delta)
{
	segment &seg = this->seg;
	seg.s = vector::add(
		seg.e,
		vector::multiply(
			vector::distance(seg.s, seg.e) - delta,
			vector::unit(vector::sub(seg.s, seg.e))
		)
	);
}

void ps::Cushion::collapseEndPoint(double delta)
{
	seg.e = vector::add(
		seg.s,
		vector::multiply(
			vector::distance(seg.e, seg.s) - delta,
			vector::unit(vector::sub(seg.e, seg.s))
		)
	);
}

ps::CushionPoint::CushionPoint(vector & position, CushionSegment *cushion)
	:position(position), cushion(cushion)
{}

ps::CushionPoint::~CushionPoint()
{

}

ps::CushionPoint * ps::CushionPoint::create(vector & position, CushionSegment *cushion)
{
	CushionPoint *instance = new CushionPoint(position, cushion);
	return instance;
}


const double ps::PocketAABB::POCKET_AABB_PADDING = 0.1;

ps::PocketAABB::PocketAABB(const Pocket * pocket)
{
	vector position = pocket->position;
	double radius = pocket->radius;

	minX = position.x - radius - POCKET_AABB_PADDING;
	maxX = position.x + radius + POCKET_AABB_PADDING;
	minY = position.y - radius - POCKET_AABB_PADDING;
	maxY = position.y + radius + POCKET_AABB_PADDING;
}

ps::PocketAABB::PocketAABB()
	: minX(0), maxX(0), minY(0), maxY(0)
{

}

bool ps::PocketAABB::checkPointInside(vector point)
{
	return (
		minX <= point.x && point.x <= maxX &&
		minY <= point.y && point.y <= maxY
		);
}

ps::PocketAABB * ps::PocketAABB::create(Pocket *pocket)
{
	return new PocketAABB(pocket);
}

ps::CushionSegment::CushionSegment(int id, const segment & seg)
	:id(id), seg(seg)
{
}

void ps::CushionSegment::collapseStartPoint(double delta)
{
	segment &seg = this->seg;
	seg.s = vector::add(
		seg.e,
		vector::multiply(
			vector::distance(seg.s, seg.e) - delta,
			vector::unit(vector::sub(seg.s, seg.e))
		)
	);
}

void ps::CushionSegment::collapseEndPoint(double delta)
{
	seg.e = vector::add(
		seg.s,
		vector::multiply(
			vector::distance(seg.e, seg.s) - delta,
			vector::unit(vector::sub(seg.e, seg.s))
		)
	);
}
