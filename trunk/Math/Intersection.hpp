#include "./Ray.hpp"
#include "./Sphere.hpp"
#include "./LineSegment.hpp"
#include "./Triangle.hpp"


namespace Edge
{
	bool LineSegPlaneInt(const bnu::vector<double>& P0, 
		const bnu::vector<double>& P1, 
		const bnu::vector<double>& Normal, 
		double d,
		double& t);

	void FindIntersection(const Ray& R, const Sphere& S, std::vector < bnu::vector<double> >& Points);
	void FindIntersection(const Edge::LineSegment& LS, const Triangle& T, std::vector < bnu::vector<double> >& Points);
}
