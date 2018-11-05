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



/*! \file gmscript.h
 *  File Description
 */

#ifndef __gmSCRIPT_H__
#define __gmSCRIPT_H__


// system
#include <stdio.h>

// stl
#include <string>


namespace GMlib {

  namespace Script {

    class Script {
    public:
      Script();

      virtual std::string   getErrors();
      virtual void          exec();

    };
  } // END namespace Script
} // END namespace GMlib


#ifdef GM_SCRIPT_LUA

// gmlib
#include <core/containers/gmarray.h>
#include <core/containers/gmdvector.h>

namespace GMlib {

  namespace Script {

    namespace Lua {

      namespace luaC {

        #include <lua.hpp>
//        extern "C" {
//          #include <lua.h>
//          #include <lualib.h>
//          #include <lauxlib.h>
//        }
      } // END namespace luaC


      enum GM_LUA_VAR_TYPE {

        GM_LUA_VAR_TYPE_NIL,
        GM_LUA_VAR_TYPE_BOOL,
        GM_LUA_VAR_TYPE_NUM,
        GM_LUA_VAR_TYPE_STRING,
        GM_LUA_VAR_TYPE_CFUNC,
        GM_LUA_VAR_TYPE_CUSTOME
      };


      enum GM_LUA_LOAD_TYPE {

        GM_LUA_LOAD_TYPE_STRING,
        GM_LUA_LOAD_TYPE_FILE
      };


      class LuaVar {
      public:
        LuaVar();
        LuaVar( bool b );
        LuaVar( int i );
        LuaVar( float f );
        LuaVar( double d );
        LuaVar( char* str );
        LuaVar( const char* str );
        LuaVar( luaC::lua_CFunction func );
        LuaVar( const LuaVar& var );
        ~LuaVar();

        const std::string&    getName() const;
        virtual void          push( luaC::lua_State* L ) const;
        void                  setName( const std::string& name );
        bool                  toBoolean() const;
        double                toDouble() const;
        float                 toFloat() const;
        int                   toInt() const;
        std::string           toString() const;

        LuaVar&               operator = ( const LuaVar& var );

      protected:
        std::string           _name;
        GM_LUA_VAR_TYPE       _type;
        bool                  _var_bool;
        double                _var_num;
        std::string           _var_str;
        luaC::lua_CFunction   _var_func;

      }; // END class LuaVar






      class LuaScript : public Script {
      public:
        LuaScript();
        ~LuaScript();

        void                    addVar( LuaVar* var );
        void                    addVars( const ArrayT<LuaVar*>& vars );

        void                    clearVars();

        void                    exec();
        void                    exec( const std::string& fname );
        std::string             getErrors();
        int                     getErrfunc() const;
        const Array<LuaVar*>&   getResult() const;
        int                     getStatus() const;

        void                    loadProgram( const std::string& prog, GM_LUA_LOAD_TYPE type );
        void                    loadStdLibs();

                                inline operator luaC::lua_State* () { return _L; }   // Enables the possibility to use the object as a pointer of type lua_State

      protected:
        luaC::lua_State         *_L;

        GM_LUA_LOAD_TYPE        _load_type;
        std::string             _prog;

        int                     _s;
        int                     _errfunc;
        bool                    _valid;
        Array<LuaVar*>          _vars;
        Array<LuaVar*>          _result;

        virtual void            constructResult();
        void                    constructResult_table();
        void                    load();


      }; // END class LuaScript

    } // END namespace Lua

  } // END namespace Script

} // END namespace GMlib

#endif // GM_SCRIPT_LUA

#endif // __gmSCRIPT_H__







