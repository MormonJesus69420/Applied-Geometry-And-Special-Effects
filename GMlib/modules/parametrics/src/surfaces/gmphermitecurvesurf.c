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





#include "../evaluators/gmevaluatorstatic.h"
#include "../curves/gmptriangcurve.h"

namespace GMlib {


template <typename T>
Hermite4Evaluator<T> PHermiteCurveSurf<T>::HCu;



template <typename T>
inline
PHermiteCurveSurf<T>::PHermiteCurveSurf( PCurve<T,3>* c1, PCurve<T,3>* c2, PCurve<T,3>* c3, PCurve<T,3>* c4, bool swap_par)
{
  init();

  _cu.setSize(4);
  _cu[0] = c1;
  _cu[1] = c2;
  _cu[2] = c3;
  _cu[3] = c4;
  _swap_par = swap_par;
}


template <typename T>
inline
PHermiteCurveSurf<T>::PHermiteCurveSurf( Array<PCurve<T,3>*> cu, bool swap_par)
{
    init();

  _cu = cu;
  _swap_par = swap_par;
}


template <typename T>
inline
PHermiteCurveSurf<T>::PHermiteCurveSurf( const PHermiteCurveSurf<T>& copy ) : PSurf<T,3>( copy )
{
    init();

  _cu = copy._cu;
  _swap_par = copy._swap_par;
}


template <typename T>
PHermiteCurveSurf<T>::~PHermiteCurveSurf() {}



//*********************************
//**   Public local functons     **
//*********************************


template <typename T>
void PHermiteCurveSurf<T>::makeSample(DMatrix<DMatrix<Vector<T,3> > >& m, int m1, int m2)
{
  PTriangCurve<T>* c1 = dynamic_cast<PTriangCurve<T>*>(_cu[2]);
  PTriangCurve<T>* c2 = dynamic_cast<PTriangCurve<T>*>(_cu[3]);
  if(_swap_par)
  { // In u-direction
    DVector<DVector<Vector<T,3> > > v1 = c1->getSample3(m1);
    DVector<DVector<Vector<T,3> > > v2 = c2->getSample3(m1);
    // In v-direction
    if(!_H->isMade(m2)) _H->preEval(m2);
    m.setDim(m2,m1);

    for(int i=0; i<m2; i++)
    {
      Matrix<T,4,4> h = _H->getPreEval(i);
      for(int j=0; j<m1; j++)
      {
        m[j][i].setDim(3,3);
        m[j][i][0][0] = h[0][0]*v1[j][0] + h[0][1]*v2[j][0] + h[0][2]*v1[j][1] + h[0][3]*v2[j][1];
        m[j][i][0][1] = h[1][0]*v1[j][0] + h[1][1]*v2[j][0] + h[1][2]*v1[j][1] + h[1][3]*v2[j][1];
        m[j][i][1][0] = h[0][0]*v1[j][2] + h[0][1]*v2[j][2] + h[0][2]*v1[j][3] + h[0][3]*v2[j][3];
        m[j][i][1][1] = h[1][0]*v1[j][2] + h[1][1]*v2[j][2] + h[1][2]*v1[j][3] + h[1][3]*v2[j][3];

        m[j][i][0][2] = h[2][0]*v1[j][0] + h[2][1]*v2[j][0] + h[2][2]*v1[j][1] + h[2][3]*v2[j][1];
        m[j][i][2][0] = h[0][0]*v1[j][4] + h[0][1]*v2[j][4] + h[0][2]*v1[j][5] + h[0][3]*v2[j][5];
      }
    }
  }
  else
  { // In v-direction
    DVector<DVector<Vector<T,3> > > v1 = c1->getSample3(m2);
    DVector<DVector<Vector<T,3> > > v2 = c2->getSample3(m2);
    // In u-direction
    if(!_H->isMade(m1)) _H->preEval(m1);
    m.setDim(m1,m2);

    for(int i=0; i<m1; i++)
    {
      Matrix<T,4,4> h = _H->getPreEval(i);
      for(int j=0; j<m2; j++)
      {
        m[i][j].setDim(3,3);
        m[i][j][0][0] = h[0][0]*v1[j][0] + h[0][1]*v2[j][0] + h[0][2]*v1[j][1] + h[0][3]*v2[j][1];
        m[i][j][1][0] = h[1][0]*v1[j][0] + h[1][1]*v2[j][0] + h[1][2]*v1[j][1] + h[1][3]*v2[j][1];
        m[i][j][0][1] = h[0][0]*v1[j][2] + h[0][1]*v2[j][2] + h[0][2]*v1[j][3] + h[0][3]*v2[j][3];
        m[i][j][1][1] = h[1][0]*v1[j][2] + h[1][1]*v2[j][2] + h[1][2]*v1[j][3] + h[1][3]*v2[j][3];

        m[i][j][2][0] = h[2][0]*v1[j][0] + h[2][1]*v2[j][0] + h[2][2]*v1[j][1] + h[2][3]*v2[j][1];
        m[i][j][0][2] = h[0][0]*v1[j][4] + h[0][1]*v2[j][4] + h[0][2]*v1[j][5] + h[0][3]*v2[j][5];
      }
    }
  }
}



//**************************************************
// Overrided (public) virtual functons from PSurf **
//**************************************************


template <typename T>
bool PHermiteCurveSurf<T>::isClosedU() const {
  return  false;
}


template <typename T>
bool PHermiteCurveSurf<T>::isClosedV() const {
  return false;
}




//*****************************************************
// Overrided (protected) virtual functons from PSurf **
//*****************************************************

template <typename T>
void PHermiteCurveSurf<T>::eval(T u, T v, int d1,int d2, bool /*lu*/, bool /*lv*/ ) const {
  if(_swap_par)
  {
    std::swap(u,v);
    std::swap(d1,d2);
  }

  DMatrix<Vector<T,3> > c(_cu.getSize(),d2+1);

  for(int i=0; i< _cu.getSize(); i++)
  {
    c[i] = _cu(i)->evaluateParent(v,d2);

//    cout << "c["<< i << "]=" << c[i] << endl;
  }

  DMatrix< T > hp; // Storing the Hermite Polynomials
  EvaluatorStatic<T>::evaluateH3d( hp, d1, u);

  DMatrix< Vector<T,3> >   p = hp * c;

  if(_swap_par) this->_p = p.transpose();
  else          this->_p = p;
}



template <typename T>
T PHermiteCurveSurf<T>::getStartPU() const {
  if(_swap_par)   return _cu(0)->getParStart();
  else            return T(0);
}


template <typename T>
T PHermiteCurveSurf<T>::getEndPU() const {
  if(_swap_par)   return _cu(0)->getParEnd();
  else            return T(1);
}


template <typename T>
T PHermiteCurveSurf<T>::getStartPV() const {
  if(_swap_par)   return T(0);
  else            return _cu(0)->getParStart();
}


template <typename T>
T PHermiteCurveSurf<T>::getEndPV() const {
  if(_swap_par)   return T(1);
  else            return _cu(0)->getParEnd();
}


//*****************************************
//     Local (protected) functons        **
//*****************************************

template <typename T>
void PHermiteCurveSurf<T>::init() {

  this->_dm = GM_DERIVATION_EXPLICIT;

  _H  = &HCu;
}


} // END namespace GMlib

