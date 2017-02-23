#include "polygon.h"
#include "3dmath.h"
#include "../utils.h"

Polyg::Polyg()
{
    m_drawoutva = NULL;
}

Polyg::~Polyg()
{
    freeva();
}

Polyg::Polyg(const Polyg& original)
{
    *this = original;
}

Polyg& Polyg::operator=(const Polyg& original)
{
    m_drawoutva = NULL;
    m_edv = original.m_edv;
    makeva();

    return *this;
}

void Polyg::makeva()
{
    freeva();
    m_drawoutva = new Vec3f[m_edv.size()];
    int i=0;
	for(std::list<Vec3f>::iterator j=m_edv.begin(); j!=m_edv.end(); i++, j++)
        m_drawoutva[i] = *j;
}

void Polyg::freeva()
{
    if(m_drawoutva)
    {
        delete [] m_drawoutva;
        m_drawoutva = NULL;
    }
}

bool InsidePoly(Vec3f vIntersection, Vec3f Poly[], long verticeCount)
{
    const FixFrac MATCH_FACTOR = 0.99999;		// Used to cover up the error in FixFracing point
    FixFrac Angle = 0.0;						// Initialize the angle
    Vec3f vA, vB;						// Create temp vectors

    for (int i = 0; i < verticeCount; i++)		// Go in a circle to each vertex and get the angle between
    {
        vA = Vector(Poly[i], vIntersection);	// Subtract the intersect point from the current vertex
        // Subtract the point from the next vertex
        vB = Vector(Poly[(i + 1) % verticeCount], vIntersection);

        Angle += AngleBetweenVectors(vA, vB);	// Find the angle between the 2 vectors and add them all up as we go along
    }

    if(Angle >= (MATCH_FACTOR * (2.0 * M_PI)) )
        return true;							// The point is inside of the polygon

    return false;								// If you get here, it obviously wasn't inside the polygon, so Return FALSE
}

bool InterPoly(Vec3f vPoly[], Vec3f vLine[], int verticeCount, Vec3f* vIntersection)
{
    Vec3f vNormal;// = {0};
    FixFrac originDistance = 0;

    // First we check to see if our line intersected the plane.  If this isn't true
    // there is no need to go on, so return false immediately.
    // We pass in address of vNormal and originDistance so we only calculate it once

    // Reference   // Reference
    if(!InterPlane(vPoly, vLine,   vNormal,   originDistance))
        return false;

    // Now that we have our normal and distance passed back from InterPlane(),
    // we can use it to calculate the intersect point.  The intersect point
    // is the point that actually is ON the plane.  It is between the line.  We need
    // this point test next, if we are inside the polygon.  To get the I-Point, we
    // give our function the normal of the plan, the points of the line, and the originDistance.

    Vec3f vTemp = IntersectionPoint(vNormal, vLine, originDistance);

    // Now that we have the intersect point, we need to test if it's inside the polygon.
    // To do this, we pass in :
    // (our intersect point, the polygon, and the number of vertices our polygon has)

    if(InsidePoly(vTemp, vPoly, verticeCount))
    {
        if(vIntersection != NULL)
            (*vIntersection) = vTemp;

        return true;
    }

    return false;
}
