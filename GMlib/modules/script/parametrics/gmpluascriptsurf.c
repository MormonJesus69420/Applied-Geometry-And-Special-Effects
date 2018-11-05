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



/*! \file gmpluascriptsurf.c
 *  \brief Inline PLuaScriptSurf class implementations
 *
 *  Implementation of the PLuaScriptSurf class.
 */

// stl
#include <sstream>

namespace GMlib {

  namespace Script {

    namespace Lua {


      template <typename T>
      PLuaScriptSurf<T>::PLuaScriptSurf() {

        _closed_u = false;
        _closed_v = false;
        _start_u = 0;
        _end_u = 1;
        _start_v = 0;
        _end_v = 1;
      }


      template <typename T>
      PLuaScriptSurf<T>::PLuaScriptSurf( const PLuaScriptSurf<T>& copy ) : PSurf<T>( copy ), LuaScript( copy ) {}


      template <typename T>
      PLuaScriptSurf<T>::~PLuaScriptSurf() {}


      template <typename T>
      void PLuaScriptSurf<T>::constructResult() {

        luaC::lua_getglobal( _L, "result" ); {

          for( int i = 0; i < this->_p.getDim1(); i++ ) {

            std::stringstream si;
            si << i;
            luaC::lua_getfield( _L, -1, si.str().c_str() ); {

              for( int j = 0; j < this->_p.getDim2(); j++ ) {

                std::stringstream sj;
                sj << j;
                luaC::lua_getfield( _L, -1, sj.str().c_str() ); {

                  luaC::lua_getfield( _L, -1, "x" );
                    this->_p[i][j][0] = luaC::lua_tonumber( _L, -1 );
                  luaC::lua_pop( _L, 1 );

                  luaC::lua_getfield( _L, -1, "y" );
                    this->_p[i][j][1] = luaC::lua_tonumber( _L, -1 );
                  luaC::lua_pop( _L, 1 );

                  luaC::lua_getfield( _L, -1, "z" );
                    this->_p[i][j][2] = luaC::lua_tonumber( _L, -1 );
                  luaC::lua_pop( _L, 1 );

                } luaC::lua_pop( _L, 1 );
              }
            } luaC::lua_pop( _L, 1 );
          }
        } luaC::lua_pop( _L, 1 );
      }


      template <typename T>
      void PLuaScriptSurf<T>::eval(T u, T v, int d1, int d2, bool lu, bool lv ) {

        // Set result dimension
        this->_p.setDim( d1, d2 );

        // Set up variables for lua
        clearVars();
        addVar( new LuaVar( u ) );
        addVar( new LuaVar( v ) );
        addVar( new LuaVar( d1 ) );
        addVar( new LuaVar( d2 ) );
        addVar( new LuaVar( lu ) );
        addVar( new LuaVar( lv ) );

        // Execute
        exec( "eval" );
      }


      template <typename T>
      inline
      T PLuaScriptSurf<T>::getEndPU() {

        return _end_u;
      }


      template <typename T>
      inline
      T PLuaScriptSurf<T>::getEndPV() {

        return _end_v;
      }


      template <typename T>
      inline
      T PLuaScriptSurf<T>::getStartPU() {

        return _start_u;
      }


      template <typename T>
      inline
      T PLuaScriptSurf<T>::getStartPV() {

        return _start_v;
      }


      template <typename T>
      inline
      void PLuaScriptSurf<T>::init() {

        _start_u = _start_v = T(0);
        _end_u = _end_v = T(1);
        _closed_u = _closed_v = false;
      }


      template <typename T>
      inline
      bool PLuaScriptSurf<T>::isClosedU() const {

        return _closed_u;
      }


      template <typename T>
      inline
      bool PLuaScriptSurf<T>::isClosedV() const {

        return _closed_v;
      }


      template <typename T>
      void PLuaScriptSurf<T>::setClosedU( bool closed ) {

        _closed_u = closed;
      }


      template <typename T>
      void PLuaScriptSurf<T>::setClosedV( bool closed ) {

        _closed_v = closed;
      }


      template <typename T>
      void PLuaScriptSurf<T>::setParEndU( const T& end ) {

        _end_u = end;
      }


      template <typename T>
      void PLuaScriptSurf<T>::setParEndV( const T& end ) {

        _end_v = end;
      }


      template <typename T>
      void PLuaScriptSurf<T>::setParStartU( const T& start ) {

        _start_u = start;
      }


      template <typename T>
      void PLuaScriptSurf<T>::setParStartV( const T& start ) {

        _start_v = start;
      }

    } // END namespace Lua

  } // END namespace Script

} // END namespace GMlib
