/**********************************************************************************
**
** Copyright (C) 1994 Narvik University College
** Contact: GMlib Online Portal at http://episteme.hin.no
**
** This file is part of the Geometric Modeling Library, GMlib.
**
** GMlib is free software: you can redistribute it and/or modify
** it under the terms of the GNU Lesser General Public License as published by
** the Free Software Foundation, either version 3 of the License, or
** (at your option) any later version.
**
** GMlib is distributed in the hope that it will be useful,
** but WITHOUT ANY WARRANTY; without even the implied warranty of
** MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
** GNU Lesser General Public License for more details.
**
** You should have received a copy of the GNU Lesser General Public License
** along with GMlib.  If not, see <http://www.gnu.org/licenses/>.
**
**********************************************************************************/





#ifndef GM_PARAMETRICS_VISUALIZERS_PPCURVEDERIVATIVESVISUALIZER_H
#define GM_PARAMETRICS_VISUALIZERS_PPCURVEDERIVATIVESVISUALIZER_H

#include "gmpcurvevisualizer.h"

// gmlib
#include <core/types/gmpoint.h>
#include <core/containers/gmdvector.h>
#include <core/utils/gmcolor.h>


namespace GMlib {

  enum GM_PCURVE_DERIVATIVESVISUALIZER_SIZE {
    GM_PCURVE_DERIVATIVESVISUALIZER_NORMALIZED,
    GM_PCURVE_DERIVATIVESVISUALIZER_RELATIVE,
    GM_PCURVE_DERIVATIVESVISUALIZER_ABSOLUTE
  };


  template <typename T, int n>
  class PCurveDerivativesVisualizer : public PCurveVisualizer<T,n> {
    GM_VISUALIZER(PCurveDerivativesVisualizer)
  public:
    PCurveDerivativesVisualizer();
    PCurveDerivativesVisualizer(std::vector<DVector<Vector<T,3>>>& p);
    PCurveDerivativesVisualizer( const PCurveDerivativesVisualizer<T,n>& copy );

    virtual ~PCurveDerivativesVisualizer();

    const Color&    getColor() const;
    int             getDerivative() const;
    double          getSize() const;
    GM_PCURVE_DERIVATIVESVISUALIZER_SIZE getMode() const;

    void            setColor( const Color& color );
    void            setDerivative( int t = 1 );
    void            setSize( double size = 1.0 );
    void            setMode( GM_PCURVE_DERIVATIVESVISUALIZER_SIZE mode );

    void            render(const SceneObject* obj, const DefaultRenderer* renderer) const;

    void            replot( const std::vector< DVector< Vector<T, n> > >& p, int m, int d, bool closed = false ) override;
    void            update() override;

  protected:
    GL::Program               _prog;
    GL::VertexBufferObject    _vbo;
    int                       _no_elements;

    Color                                   _color;
    GM_PCURVE_DERIVATIVESVISUALIZER_SIZE    _mode;
    double                                  _size;
    int                                     _der;
  };



} // END namespace GMlib

// Include PCurveDerivativesVisualizer class function implementations
#include "gmpcurvederivativesvisualizer.c"



#endif // GM_PARAMETRICS_VISUALIZERS_PPCURVEDERIVATIVESVISUALIZER_H
