#pragma once

#include <vector>
#include <functional>
#include <iostream>
#include <boost/graph/graph_traits.hpp>
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/property_iter_range.hpp>
#include <boost/numeric/ublas/vector.hpp>
#include <boost/numeric/ublas/matrix.hpp>
#include <boost/numeric/ublas/matrix_sparse.hpp>
#include <boost/numeric/ublas/triangular.hpp>
#include <boost/numeric/ublas/io.hpp>
#include <boost/shared_ptr.hpp>


namespace bnu = boost::numeric::ublas;


#include "./DDy_3Dy_2y.hpp"
#include "./DEStateSource.hpp"
#include "./EulerSolver.hpp"
#include "./ExpDE.hpp"
#include "./MatrixSupport.hpp"
#include "./ODE.hpp"
#include "./RK4Solver.hpp"
#include "./Intersection.hpp"
#include "./Ray.hpp"
#include "./Sphere.hpp"

const double Pi = 3.14159265;
