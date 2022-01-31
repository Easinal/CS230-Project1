#include <limits>
#include "box.h"

// Return whether the ray intersects this box.
bool Box::Intersection(const Ray& ray) const
{
     //float tx_min = (lo[0]-ray.endpoint[0]) / ray.direction[0];
     //float tx_max = (hi[0]-ray.endpoint[0]) / ray.direction[0];
     //float ty_min = (lo[1]-ray.endpoint[1]) / ray.direction[1];
     //float ty_max = (hi[1]-ray.endpoint[1]) / ray.direction[1];
     //float tz_min = (lo[2]-ray.endpoint[2]) / ray.direction[2];
     //float tz_max = (hi[2]-ray.endpoint[2]) / ray.direction[2];
     //if(tx_min > ty_max || ty_min > tx_max || tz_min > tz_max) {
       //return false;
     //} else {
       //return true;
     //}
    vec3 invdir = vec3(1.0/ray.direction[0], 1.0/ray.direction[1], 1.0/ray.direction[2]);
    vec3 in = (lo-ray.endpoint)*invdir;
    vec3 out = (hi-ray.endpoint)*invdir;

    vec3 tmax = in, tmin = in;
    for(int i = 0; i < 3; i++) {
        tmax[i] = std::max(tmax[i], out[i]);
        tmin[i] = std::min(tmin[i], out[i]);
    }
    float t1 = std::min(tmax[0], std::min(tmax[1], tmax[2]));
    float t0 = std::max(tmin[0], std::max(tmin[1], tmin[2]));
    return t1 >= t0;
}

// Compute the smallest box that contains both *this and bb.
Box Box::Union(const Box& bb) const
{
    Box box;

    for (int i = 0; i < 3; i++) {
		box.lo[i]=std::min(bb.lo[i],lo[i]);
		box.hi[i]=std::max(bb.hi[i],hi[i]);
	}
    return box;
}

// Enlarge this box (if necessary) so that pt also lies inside it.
void Box::Include_Point(const vec3& pt)
{
    for (int i = 0; i < 3; i++) {
        lo[i] = std::min(lo[i], pt[i]);
        hi[i] = std::max(hi[i], pt[i]);
	}
}

// Create a box to which points can be correctly added using Include_Point.
void Box::Make_Empty()
{
    lo.fill(std::numeric_limits<double>::infinity());
    hi=-lo;
}

// Create a box that contains everything.
void Box::Make_Full()
{
    hi.fill(std::numeric_limits<double>::infinity());
    lo=-hi;
}
