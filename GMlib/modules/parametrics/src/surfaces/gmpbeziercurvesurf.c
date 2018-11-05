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

namespace GMlib {


//*****************************************
// Constructors and destructor           **
//*****************************************

  template <typename T>
  inline
  PBezierCurveSurf<T>::PBezierCurveSurf( Array<PCurve<T,3>*> cu, bool swap_par)
  {
      init();
      _cu       = cu;
      _swap_par = swap_par;
  }


   template <typename T>
   inline
   PBezierCurveSurf<T>::PBezierCurveSurf(PCurve<T,3>* c1, PCurve<T,3>* c2, bool swap_par) {

       init();
       _cu.setSize(2);
       _cu[0] = c1;
       _cu[1] = c2;
       _swap_par = swap_par;
   }


  template <typename T>
  inline
  PBezierCurveSurf<T>::PBezierCurveSurf( const PBezierCurveSurf<T>& copy ) : PSurf<T,3>( copy ) {
      init();
      _cu       = copy._cu;
      _swap_par = copy._swap_par;
  }


  template <typename T>
  PBezierCurveSurf<T>::~PBezierCurveSurf() {}


  //*********************************
  //**   Public localfunctons      **
  //*********************************

  template <typename T>
  inline
  void PBezierCurveSurf<T>::showCurves() {

      int m;
      if(_swap_par)
          m = this->_no_sam_u;
      else
          m = this->_no_sam_v;

      for(int i=0; i<_cu.getSize(); i++) {
          _cu[i]->toggleDefaultVisualizer();
          _cu[i]->replot(m);
          this->insert(_cu[i]);
      }
  }



  template <typename T>
  inline
  void PBezierCurveSurf<T>::hideCurves(  ) {

      for(int i=0; i<_cu.getSize(); i++){
          _cu[i]->toggleDefaultVisualizer();
          this->remove(_cu[i]);
      }
  }



  //**************************************************
  // Overrided (public) virtual functons from PSurf **
  //**************************************************

  template <typename T>
  bool PBezierCurveSurf<T>::isClosedU() const {
      return  false;
  }


  template <typename T>
  bool PBezierCurveSurf<T>::isClosedV() const {
      return false;
  }

  //*****************************************************
  // Overrided (protected) virtual functons from PSurf **
  //*****************************************************

  template <typename T>
  void PBezierCurveSurf<T>::eval(T u, T v, int d1,int d2, bool /*lu*/, bool /*lv*/ ) const {

      if(_swap_par) {
          std::swap(u,v);
          std::swap(d1,d2);
      }

      DMatrix<Vector<T,3> > c(_cu.getSize(), d2+1);

      for(int i=0; i < _cu.getSize(); i++)
          c[i] = _cu(i)->evaluateParent(v, d2);

      // Compute the Bernstein-Hermite Polynomials
      DMatrix< T > bhp;
      EvaluatorStatic<T>::evaluateBhp( bhp, _cu.getSize()-1, u, 1);

      DMatrix< Vector<T,3> >    p = bhp * c;
      p.resetDim(d1+1,d2+1);

      if(_swap_par) this->_p = p.transpose();
      else          this->_p = p;

      this->_p.resetDim(d1+1,d2+1);
  }



  template <typename T>
  T PBezierCurveSurf<T>::getStartPU() const {
      if(_swap_par) return _cu(0)->getParStart();
      else          return T(0);
  }


  template <typename T>
  T PBezierCurveSurf<T>::getEndPU() const {
      if(_swap_par) return _cu(0)->getParEnd();
      else          return T(1);
  }


  template <typename T>
  T PBezierCurveSurf<T>::getStartPV() const {
      if(_swap_par) return T(0);
      else          return _cu(0)->getParStart();
  }


  template <typename T>
  T PBezierCurveSurf<T>::getEndPV() const {
      if(_swap_par) return T(1);
      else          return _cu(0)->getParEnd();
  }



  //*****************************************
  //     Local (protected) functons        **
  //*****************************************

  template <typename T>
  void PBezierCurveSurf<T>::init() {

    this->_dm = GM_DERIVATION_EXPLICIT;
  }


} // END namespace GMlib

