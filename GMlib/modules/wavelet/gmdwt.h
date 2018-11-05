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

#ifndef __GM_WAVELET_DWT_H__
#define __GM_WAVELET_DWT_H__


// gmlib
#include <core/containers/gmdvector.h>
#include <opencl/gmcommandqueue.h>
#include <opencl/gmprogram.h>
#include <opencl/gmkernel.h>
#include <opencl/memory/gmbuffer.h>

namespace GMlib {
namespace Wavelet {

  template <typename T> class Filter;

  template<typename T>
  class Dwt {

    enum BUFFER {
      BUFFER_01 = 0,
      BUFFER_02 = 1
    };

    enum FILTER {
      FILTER_LP = 0,
      FILTER_HP = 1
    };

    struct TmpFilterStruct {
      float s0;
      float s1;
    };

  public:
    Dwt();

    // Dwt Operations
    void deComposeLift2D( unsigned int res,
                          int lvls, int s_lvl = 0 );
    void deCompose( const Filter<T>* filter, unsigned int dim,
                    unsigned int res,
                    int lvls, int s_lvl = 0 );
//    void deCompose( const Filter<T>* filter, unsigned int dim,
//                    Vector<unsigned int,>& res,
//                    int lvls, int s_lvl = 0 );
    void reConstruct( const Filter<T>* filter, unsigned int dim,
                      unsigned int res,
                      int lvls, int s_lvl = 0 );
    void reConstructLift2D( unsigned int res,
                            int lvls, int s_lvl = 0 );
//    void reConstruct( const Filter<T>* filter, unsigned int dim,
//                      Vector<unsigned int,>& res,
//                      int lvls, int s_lvl = 0 );

    // write/read-back signal-data
    void writeToInBuffer( const T* signal, unsigned long int length );
    void writeToInBuffer( const DVector<T>& signal );
    void writeToInBuffer( const DMatrix<T>& signal );
    void writeToInBuffer( const CL::Buffer& signal, unsigned long int length );
    void readFromOutBuffer( T* signal, unsigned long int length ) const;
    void readFromOutBuffer( DVector<T>& signal ) const;
    void readFromOutBuffer( DMatrix<T>& signal ) const;
    void readFromOutBuffer( CL::Buffer& signal, unsigned long int length) const;

    void swapBuffers();

    bool setDwtSrc( const std::string& dwt_src, const std::string& idwt_src );

//    template <typename G>
    const CL::Buffer& getNativeInBuffer() const;

//    template <typename G>
    const CL::Buffer& getNativeOutBuffer() const;

  private:
    int                     _resolution;

    CL::CommandQueue        _queue;
    CL::Program             _program;
    CL::Program             _program_lift_2d;
    CL::Program             _program_ilift_2d;
    CL::Kernel              _dwt_k;
    CL::Kernel              _dwt_lift_2D;
    CL::Kernel              _idwt_k;
    CL::Kernel              _idwt_lift_2D;

    mutable cl::Event       _event;

    // Two alternating buffers (input/output)
    BUFFER                  _bA;
    BUFFER                  _bB;
    CL::Buffer              _buffers[2];

    void                    init();
    void                    initBuffers(int length );

  }; // END class Dwt

} // END namespace Wavelet
} // END namespace GMlib

#include "gmdwt.c"

#endif // __GM_WAVELET_DWT_H__
