#pragma once

#include <vector>
#include <functional>
#include <iostream>
#include <boost/graph/graph_traits.hpp>
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/property_iter_range.hpp>
#include <boost/numeric/ublas/vector.hpp>
#include <boost/numeric/ublas/matrix.hpp>
#include <boost/numeric/ublas/triangular.hpp>
#include <boost/numeric/ublas/io.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/enable_shared_from_this.hpp>

namespace bnu = boost::numeric::ublas;

#include <GL/glaux.h>
#include "./Camera.hpp"
#include "./ChangeApplyMode.hpp"
#include "./DirectionLight.hpp"
#include "./DiffSystemNode.hpp"
#include "./Material.hpp"
#include "./Mesh.hpp"
#include "./Node.hpp"
#include "./NodeVisitor.hpp"
#include "./PointLight.hpp"
#include "./PreRenderVisitor.hpp"
#include "./RenderVisitor.hpp"
#include "./RGBColor.hpp"
#include "./Rotate.hpp"
#include "./Scale.hpp"
#include "./TextureMap.hpp"
#include "./Translate.hpp"
#include "./TimeStepVisitor.hpp"
#include "./AmbientLight.hpp"
#include "./BoundNode.hpp"
#include "./BoundsCalculator.hpp"
