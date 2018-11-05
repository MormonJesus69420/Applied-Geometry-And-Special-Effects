# ###############################################################################
# #
# # Copyright (C) 1994 Narvik University College
# # Contact: GMlib Online Portal at http://episteme.hin.no
# #
# # This file is part of the Geometric Modeling Library, GMlib.
# #
# # GMlib is free software: you can redistribute it and/or modify
# # it under the terms of the GNU Lesser General Public License as published by
# # the Free Software Foundation, either version 3 of the License, or
# # (at your option) any later version.
# #
# # GMlib is distributed in the hope that it will be useful,
# # but WITHOUT ANY WARRANTY; without even the implied warranty of
# # MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
# # GNU Lesser General Public License for more details.
# #
# # You should have received a copy of the GNU Lesser General Public License
# # along with GMlib. If not, see <http://www.gnu.org/licenses/>.
# #
# ###############################################################################


###
# <global>
list( APPEND HEADERS
  gmparametrics.h
  gmpcurve.h
  gmpsurf.h
  gmptriangle.h
)

list( APPEND HEADER_SOURCES
  gmparametrics.c
  gmpcurve.c
  gmpsurf.c
  gmptriangle.c
)



###
# Curves

list( APPEND HEADERS
  curves/gmparc.h
  curves/gmpbasiscurve.h
  curves/gmpbeziercurve.h
  curves/gmpbsplinecurve.h
  curves/gmpbsplinebasiscurve.h
  curves/gmpbutterfly.h
  curves/gmpchrysanthemumcurve.h
  curves/gmpcircle.h
  curves/gmperbscurve.h
  curves/gmpline.h
  curves/gmplogspiral.h
  curves/gmprosecurve.h
  curves/gmpsubcurve.h
  curves/gmpsurfcurve.h
  curves/gmptcurve.h
  curves/gmptriangcurve.h
)

list( APPEND HEADER_SOURCES
  curves/gmparc.c
  curves/gmpbasiscurve.c
  curves/gmpbeziercurve.c
  curves/gmpbsplinecurve.c
  curves/gmpbsplinebasiscurve.c
  curves/gmpbutterfly.c
  curves/gmpchrysanthemumcurve.c
  curves/gmpcircle.c
  curves/gmperbscurve.c
  curves/gmpline.c
  curves/gmplogspiral.c
  curves/gmprosecurve.c
  curves/gmpsubcurve.c
  curves/gmpsurfcurve.c
  curves/gmptcurve.c
  curves/gmptriangcurve.c
)



###
# Evaluators

list( APPEND HEADERS
  evaluators/gmbasisevaluator.h
  evaluators/gmbasistriangleerbs.h
  evaluators/gmbfbsevaluator.h
  evaluators/gmerbsevaluator.h
  evaluators/gmevaluatorstatic.h
  evaluators/gmhermite4evaluator.h
)

list( APPEND HEADER_SOURCES
  evaluators/gmbasisevaluator.c
  evaluators/gmbasistriangleerbs.c
  evaluators/gmbfbsevaluator.c
  evaluators/gmerbsevaluator.c
  evaluators/gmevaluatorstatic.c
  evaluators/gmhermite4evaluator.c
)



###
# Surfaces
list( APPEND HEADERS
  surfaces/gmpapple.h
  surfaces/gmpapple2.h
  surfaces/gmpasteroidalsphere.h
  surfaces/gmpbenthorns.h
  surfaces/gmpbeziercurvesurf.h
  surfaces/gmpbeziersurf.h
  surfaces/gmpbohemiandome.h
  surfaces/gmpbottle8.h
  surfaces/gmpboyssurface.h
  surfaces/gmpbsplinesurf.h
  surfaces/gmpcircularsurface.h
  surfaces/gmpcone.h
  surfaces/gmpcoonspatch.h
  surfaces/gmpcrosscap.h
  surfaces/gmpcylinder.h
  surfaces/gmpdinisurface.h
  surfaces/gmpeightsurface.h
  surfaces/gmpenneperssurface.h
  surfaces/gmperbssurf.h
  surfaces/gmpheart.h
  surfaces/gmphelicoid.h
  surfaces/gmphermitecurvesurf.h
  surfaces/gmphermitesurface.h
  surfaces/gmphermitesurf.h
  surfaces/gmpinsideouttorus.h
  surfaces/gmpkleinsbottle.h
  surfaces/gmpkuensurface.h
  surfaces/gmpmoebiusstrip.h
  surfaces/gmpplane.h
  surfaces/gmprotationalsurf.h
  surfaces/gmpseashell.h
  surfaces/gmpsinsurface.h
  surfaces/gmpslipperssurface.h
  surfaces/gmpsphere.h
  surfaces/gmpsteinersurf.h
  surfaces/gmpsubsurf.h
  surfaces/gmpswallowtail.h
  surfaces/gmpsweepsurf.h
  surfaces/gmptorus.h
  surfaces/gmptrianguloidtrefoil.h
  surfaces/gmpwhitneyumbrella.h
)

list( APPEND HEADER_SOURCES
  surfaces/gmpapple2.c
  surfaces/gmpapple.c
  surfaces/gmpasteroidalsphere.c
  surfaces/gmpbenthorns.c
  surfaces/gmpbeziercurvesurf.c
  surfaces/gmpbeziersurf.c
  surfaces/gmpbohemiandome.c
  surfaces/gmpbottle8.c
  surfaces/gmpboyssurface.c
  surfaces/gmpbsplinesurf.c
  surfaces/gmpcircularsurface.c
  surfaces/gmpcone.c
  surfaces/gmpcoonspatch.c
  surfaces/gmpcrosscap.c
  surfaces/gmpcylinder.c
  surfaces/gmpdinisurface.c
  surfaces/gmpeightsurface.c
  surfaces/gmpenneperssurface.c
  surfaces/gmperbssurf.c
  surfaces/gmpheart.c
  surfaces/gmphelicoid.c
  surfaces/gmphermitecurvesurf.c
  surfaces/gmphermitesurface.c
  surfaces/gmphermitesurf.c
  surfaces/gmpinsideouttorus.c
  surfaces/gmpkleinsbottle.c
  surfaces/gmpkuensurface.c
  surfaces/gmpmoebiusstrip.c
  surfaces/gmpplane.c
  surfaces/gmprotationalsurf.c
  surfaces/gmpseashell.c
  surfaces/gmpsinsurface.c
  surfaces/gmpslipperssurface.c
  surfaces/gmpsphere.c
  surfaces/gmpsteinersurf.c
  surfaces/gmpsubsurf.c
  surfaces/gmpswallowtail.c
  surfaces/gmpsweepsurf.c
  surfaces/gmptorus.c
  surfaces/gmptrianguloidtrefoil.c
  surfaces/gmpwhitneyumbrella.c
)



###
# Triangles
list( APPEND HEADERS
  triangles/gmpbeziertriangle.h
  triangles/gmperbstriangle.h
  triangles/gmpsubtriangle.h
)

list( APPEND HEADER_SOURCES
  triangles/gmpbeziertriangle.c
  triangles/gmperbstriangle.c
  triangles/gmpsubtriangle.c
)



###
# Utils
list( APPEND HEADERS
  utils/gmputils.h
)

list( APPEND HEADER_SOURCES
  utils/gmputils.c
)



###
# Visualizers
list( APPEND HEADERS
#  visualizers/gmpcurveContoursvisualizer.h
  visualizers/gmpcurvedefaultvisualizer.h
  visualizers/gmpcurvederivativesvisualizer.h
  visualizers/gmpcurvepointsvisualizer.h
  visualizers/gmpcurvevisualizer.h
#  visualizers/gmpsurfcontoursvisualizer.h
  visualizers/gmpsurfdefaultvisualizer.h
  visualizers/gmpsurfderivativesvisualizer.h
  visualizers/gmpsurfnormalsvisualizer.h
  visualizers/gmpsurfpointsvisualizer.h
  visualizers/gmpsurfparamlinesvisualizer.h
  visualizers/gmpsurftexvisualizer.h
  visualizers/gmpsurfvisualizer.h
#  visualizers/gmptrianglecolorpointvisualizer.h
#  visualizers/gmttrianglecolorvisualizer.h
  visualizers/gmptriangledefaultvisualizer.h
  visualizers/gmptrianglevisualizer.h
)

list( APPEND HEADER_SOURCES
#  visualizers/gmpcurvecontoursvisualizer.c
  visualizers/gmpcurvedefaultvisualizer.c
  visualizers/gmpcurvederivativesvisualizer.c
  visualizers/gmpcurvepointsvisualizer.c
  visualizers/gmpcurvevisualizer.c
#  visualizers/gmpsurfcontoursvisualizer.c
  visualizers/gmpsurfdefaultvisualizer.c
  visualizers/gmpsurfderivativesvisualizer.c
  visualizers/gmpsurfnormalsvisualizer.c
  visualizers/gmpsurfpointsvisualizer.c
  visualizers/gmpsurfparamlinesvisualizer.c
  visualizers/gmpsurftexvisualizer.c
  visualizers/gmpsurfvisualizer.c
#  visualizers/gmptrianglecolorpointvisualizer.c
#  visualizers/gmptrianglecolorvisualizer.c
  visualizers/gmptriangledefaultvisualizer.c
  visualizers/gmptrianglevisualizer.c
)


GM_ADD_HEADERS(${HEADERS})
GM_ADD_HEADER_SOURCES(${HEADER_SOURCES})

GM_ADD_TEMPLATE_TARGET(${HEADERS} ${HEADER_SOURCES})














#addHeaders(
#  gmParametrics
#  gmPCurve
#  gmPSurf
#  gmPTriangle
#)

#addTemplateSources(
#  gmparametrics.c
#  gmpcurve.c
#  gmpsurf.c
#  gmptriangle.c
#)

#addSubDirectory(curves)
#addSubDirectory(evaluators)
#addSubDirectory(surfaces)
#addSubDirectory(triangles)
#addSubDirectory(utils)
#addSubDirectory(visualizers)

#addTestDir(test)
