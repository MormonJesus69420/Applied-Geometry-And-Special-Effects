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



/*! \file gmpluascriptsurf.h
 *
 *  Interface for the PLuaScriptSurf class.
 */

#ifndef __gmPLUASCRIPTSURF_H__
#define __gmPLUASCRIPTSURF_H__

#include "../../parametrics/gmpsurf.h"
#include "../gmscript.h"

// stl
#include <string>


# ifdef GM_SCRIPT_LUA

namespace GMlib {

  namespace Script {

    namespace Lua {

      template <typename T>
      class PLuaScriptSurf : public PSurf<T,3>, public LuaScript {
        GM_SCENEOBJECT(PLuaScriptSurf)
      public:
        PLuaScriptSurf();
        PLuaScriptSurf( const PLuaScriptSurf<T>& copy );
        virtual ~PLuaScriptSurf();

        void            setClosedU( bool closed );
        void            setClosedV( bool closed );
        void            setParEndU( const T& end );
        void            setParEndV( const T& end );
        void            setParStartU( const T& start );
        void            setParStartV( const T& start );

      protected:
        void            constructResult();
        void            eval(T u, T v, int d1, int d2, bool lu = true, bool lv = true );
        T               getEndPV();
        T               getEndPU();
        T               getStartPU();
        T               getStartPV();
        void            init();
        bool            isClosedU() const;
        bool            isClosedV() const;

        bool            _closed_u;
        bool            _closed_v;
        T               _end_u;
        T               _end_v;
        T               _start_u;
        T               _start_v;

      }; // END class PLuaScriptSurf

    } // END namespace Lua

  } // END namespace Script

} // END namespace GMlib

// Include PLuaScriptSurf class function implementations
#include "gmpluascriptsurf.c"

# endif // GM_SCRIPT_LUA

#endif // __gmPLUASCRIPTSURF_H__

