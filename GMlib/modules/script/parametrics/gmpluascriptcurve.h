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



/*! \file gmpluascriptcurve.h
 *
 *  Interface for the gmPLuaScriptCurve class.
 */

#ifndef __gmPLUASCRIPTCURVE_H__
#define __gmPLUASCRIPTCURVE_H__

#include "../../parametrics/gmpcurve.h"
#include "../gmscript.h"

// stl
#include <string>

# ifdef GM_SCRIPT_LUA

namespace GMlib {

  namespace Script {

    namespace Lua {

      template <typename T>
      class PLuaScriptCurve : public PCurve<T,3>, public LuaScript {
        GM_SCENEOBJECT(PLuaScriptCurve)
      public:
        PLuaScriptCurve();
        PLuaScriptCurve( const PLuaScriptCurve<T>& copy );
        virtual ~PLuaScriptCurve();

        void            setClosed( bool closed );
        void            setParEnd( const T& end );
        void            setParStart( const T& start );

      protected:
        bool            _closed;
        T               _end;
        T               _start;

        void            constructResult();
        void            eval(T t, int d, bool l = true );
        T               getEndP();
        T               getStartP();
        void            init();
        bool            isClosed() const;

      }; // END class PLuaScriptCurve

    } // END namespace Lua

  } // END namespace Script

} // END namespace GMlib

// Include PLuaScriptCurve class function implementations
#include "gmpluascriptcurve.c"

# endif // GM_SCRIPT_LUA

#endif // __gmPLUASCRIPTCURVE_H__


