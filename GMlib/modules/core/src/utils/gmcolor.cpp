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





#include "gmcolor.h"


namespace GMlib {

  namespace GMcolor {

    // Predefined colors
    const Color& aliceBlue()             { static const Color color = Color( 0xF0,  0xF8,  0xFF ); return color; }
    const Color& antiqueWhite()          { static const Color color = Color( 0xFA,  0xEB,  0xD7 ); return color; }
    const Color& aqua()                  { static const Color color = Color( 0x00,  0xFF,  0xFF ); return color; }
    const Color& aquaMarine()            { static const Color color = Color( 0x7F,  0xFF,  0xD4 ); return color; }
    const Color& azure()                 { static const Color color = Color( 0xF0,  0xFF,  0xFF ); return color; }
    const Color& beige()                 { static const Color color = Color( 0xF5,  0xF5,  0xDC ); return color; }
    const Color& bisque()                { static const Color color = Color( 0xFF,  0xE4,  0xC4 ); return color; }
    const Color& black()                 { static const Color color = Color( 0x00,  0x00,  0x00 ); return color; }
    const Color& blanchedAlamond()       { static const Color color = Color( 0xFF,  0xEB,  0xCD ); return color; }
    const Color& blue()                  { static const Color color = Color( 0x00,  0x00,  0xFF ); return color; }
    const Color& blueViolet()            { static const Color color = Color( 0x8A,  0x2B,  0xE2 ); return color; }
    const Color& brown()                 { static const Color color = Color( 0xA5,  0x2A,  0x2A ); return color; }
    const Color& burlyWood()             { static const Color color = Color( 0xDE,  0xB8,  0x87 ); return color; }
    const Color& cadetBlue()             { static const Color color = Color( 0x5F,  0x9E,  0xA0 ); return color; }
    const Color& chartreuse()            { static const Color color = Color( 0x7F,  0xFF,  0x00 ); return color; }
    const Color& chocolate()             { static const Color color = Color( 0xD2,  0x69,  0x1E ); return color; }
    const Color& coral()                 { static const Color color = Color( 0xFF,  0x7F,  0x50 ); return color; }
    const Color& cornflowerBlue()        { static const Color color = Color( 0x64,  0x95,  0xED ); return color; }
    const Color& cornsilk()              { static const Color color = Color( 0xFF,  0xF8,  0xDC ); return color; }
    const Color& crimson()               { static const Color color = Color( 0xDC,  0x14,  0x3C ); return color; }
    const Color& cyan()                  { static const Color color = Color( 0x00,  0xFF,  0xFF ); return color; }
    const Color& darkBlue()              { static const Color color = Color( 0x00,  0x00,  0x8B ); return color; }
    const Color& darkCyan()              { static const Color color = Color( 0x00,  0x8B,  0x8B ); return color; }
    const Color& darkGoldenRod()         { static const Color color = Color( 0xB8,  0x86,  0x0B ); return color; }
    const Color& darkGrey()              { static const Color color = Color( 0xA9,  0xA9,  0xA9 ); return color; }
    const Color& darkGreen()             { static const Color color = Color( 0x00,  0x64,  0x00 ); return color; }
    const Color& darkKhaki()             { static const Color color = Color( 0xBD,  0xB7,  0x6B ); return color; }
    const Color& darkMagenta()           { static const Color color = Color( 0x8B,  0x00,  0x8B ); return color; }
    const Color& darkOliveGreen()        { static const Color color = Color( 0x55,  0x6B,  0x2F ); return color; }
    const Color& darkOrange()            { static const Color color = Color( 0xFF,  0x8C,  0x00 ); return color; }
    const Color& darkOrchid()            { static const Color color = Color( 0x99,  0x32,  0xCC ); return color; }
    const Color& darkRed()               { static const Color color = Color( 0x8B,  0x00,  0x00 ); return color; }
    const Color& darkSalmon()            { static const Color color = Color( 0xE9,  0x96,  0x7A ); return color; }
    const Color& darkSeaGreen()          { static const Color color = Color( 0x8F,  0xBC,  0x8F ); return color; }
    const Color& darkSlateBlue()         { static const Color color = Color( 0x48,  0x3D,  0x8B ); return color; }
    const Color& darkSlateGrey()         { static const Color color = Color( 0x2F,  0x4F,  0x4F ); return color; }
    const Color& darkTurquoise()         { static const Color color = Color( 0x00,  0xCE,  0xD1 ); return color; }
    const Color& darkViolet()            { static const Color color = Color( 0x94,  0x00,  0xD3 ); return color; }
    const Color& deepPink()              { static const Color color = Color( 0xFF,  0x14,  0x93 ); return color; }
    const Color& deepSkyBlue()           { static const Color color = Color( 0x00,  0xBF,  0xFF ); return color; }
    const Color& dimGrey()               { static const Color color = Color( 0x69,  0x69,  0x69 ); return color; }
    const Color& dodgerBlue()            { static const Color color = Color( 0x1E,  0x90,  0xFF ); return color; }
    const Color& fireBrick()             { static const Color color = Color( 0xB2,  0x22,  0x22 ); return color; }
    const Color& floralWhite()           { static const Color color = Color( 0xFF,  0xFA,  0xF0 ); return color; }
    const Color& forestGreen()           { static const Color color = Color( 0x22,  0x8B,  0x22 ); return color; }
    const Color& fuchsia()               { static const Color color = Color( 0xFF,  0x00,  0xFF ); return color; }
    const Color& gainsboro()             { static const Color color = Color( 0xDC,  0xDC,  0xDC ); return color; }
    const Color& ghostWhite()            { static const Color color = Color( 0xF8,  0xF8,  0xFF ); return color; }
    const Color& gold()                  { static const Color color = Color( 0xFF,  0xD7,  0x00 ); return color; }
    const Color& goldenRod()             { static const Color color = Color( 0xDA,  0xA5,  0x20 ); return color; }
    const Color& grey()                  { static const Color color = Color( 0x80,  0x80,  0x80 ); return color; }
    const Color& green()                 { static const Color color = Color( 0x00,  0xFF,  0x00 ); return color; }
    const Color& greenYellow()           { static const Color color = Color( 0xAD,  0xFF,  0x2F ); return color; }
    const Color& honeyDew()              { static const Color color = Color( 0xF0,  0xFF,  0xF0 ); return color; }
    const Color& hotPink()               { static const Color color = Color( 0xFF,  0x69,  0xB4 ); return color; }
    const Color& indianRed()             { static const Color color = Color( 0xCD,  0x5C,  0x5C ); return color; }
    const Color& indigo()                { static const Color color = Color( 0x4B,  0x00,  0x82 ); return color; }
    const Color& ivory()                 { static const Color color = Color( 0xFF,  0xFF,  0xF0 ); return color; }
    const Color& khaki()                 { static const Color color = Color( 0xF0,  0xE6,  0x8C ); return color; }
    const Color& lavender()              { static const Color color = Color( 0xE6,  0xE6,  0xFA ); return color; }
    const Color& lavenderBlush()         { static const Color color = Color( 0xFF,  0xF0,  0xF5 ); return color; }
    const Color& lawnGreen()             { static const Color color = Color( 0x7C,  0xFC,  0x00 ); return color; }
    const Color& lemonChiffon()          { static const Color color = Color( 0xFF,  0xFA,  0xCD ); return color; }
    const Color& lightBlue()             { static const Color color = Color( 0xAD,  0xD8,  0xE6 ); return color; }
    const Color& lightCoral()            { static const Color color = Color( 0xF0,  0x80,  0x80 ); return color; }
    const Color& lightCyan()             { static const Color color = Color( 0xE0,  0xFF,  0xFF ); return color; }
    const Color& lightGoldenRodYellow()  { static const Color color = Color( 0xFA,  0xFA,  0xD2 ); return color; }
    const Color& lightGrey()             { static const Color color = Color( 0xD3,  0xD3,  0xD3 ); return color; }
    const Color& lightGreen()            { static const Color color = Color( 0x90,  0xEE,  0x90 ); return color; }
    const Color& lightPink()             { static const Color color = Color( 0xFF,  0xB6,  0xC1 ); return color; }
    const Color& lightSalmon()           { static const Color color = Color( 0xFF,  0xA0,  0x7A ); return color; }
    const Color& lightSeaGreen()         { static const Color color = Color( 0x20,  0xB2,  0xAA ); return color; }
    const Color& lightSkyBlue()          { static const Color color = Color( 0x87,  0xCE,  0xFA ); return color; }
    const Color& lightSlateGrey()        { static const Color color = Color( 0x77,  0x88,  0x99 ); return color; }
    const Color& lightSteelBlue()        { static const Color color = Color( 0xB0,  0xC4,  0xDE ); return color; }
    const Color& lightYellow()           { static const Color color = Color( 0xFF,  0xFF,  0xE0 ); return color; }
    const Color& lime()                  { static const Color color = Color( 0x00,  0xFF,  0x00 ); return color; }
    const Color& limeGreen()             { static const Color color = Color( 0x32,  0xCD,  0x32 ); return color; }
    const Color& linen()                 { static const Color color = Color( 0xFA,  0xF0,  0xE6 ); return color; }
    const Color& magenta()               { static const Color color = Color( 0xFF,  0x00,  0xFF ); return color; }
    const Color& maroon()                { static const Color color = Color( 0x80,  0x00,  0x00 ); return color; }
    const Color& mediumAquaMarine()      { static const Color color = Color( 0x66,  0xCD,  0xAA ); return color; }
    const Color& mediumBlue()            { static const Color color = Color( 0x00,  0x00,  0xCD ); return color; }
    const Color& mediumOrchid()          { static const Color color = Color( 0xBA,  0x55,  0xD3 ); return color; }
    const Color& mediumPurple()          { static const Color color = Color( 0x93,  0x70,  0xD8 ); return color; }
    const Color& mediumSeaGreen()        { static const Color color = Color( 0x3C,  0xB3,  0x71 ); return color; }
    const Color& mediumSlateBlue()       { static const Color color = Color( 0x7B,  0x68,  0xEE ); return color; }
    const Color& mediumSpringGreen()     { static const Color color = Color( 0x00,  0xFA,  0x9A ); return color; }
    const Color& mediumTurquoise()       { static const Color color = Color( 0x48,  0xD1,  0xCC ); return color; }
    const Color& mediumVioletRed()       { static const Color color = Color( 0xC7,  0x15,  0x85 ); return color; }
    const Color& midnightBlue()          { static const Color color = Color( 0x19,  0x19,  0x70 ); return color; }
    const Color& mintCream()             { static const Color color = Color( 0xF5,  0xFF,  0xFA ); return color; }
    const Color& mistyRose()             { static const Color color = Color( 0xFF,  0xE4,  0xE1 ); return color; }
    const Color& moccasin()              { static const Color color = Color( 0xFF,  0xE4,  0xB5 ); return color; }
    const Color& navajoWhite()           { static const Color color = Color( 0xFF,  0xDE,  0xAD ); return color; }
    const Color& navy()                  { static const Color color = Color( 0x00,  0x00,  0x80 ); return color; }
    const Color& oldLace()               { static const Color color = Color( 0xFD,  0xF5,  0xE6 ); return color; }
    const Color& olive()                 { static const Color color = Color( 0x80,  0x80,  0x00 ); return color; }
    const Color& oliveDrab()             { static const Color color = Color( 0x6B,  0x8E,  0x23 ); return color; }
    const Color& orange()                { static const Color color = Color( 0xFF,  0xA5,  0x00 ); return color; }
    const Color& orangeRed()             { static const Color color = Color( 0xFF,  0x45,  0x00 ); return color; }
    const Color& orchid()                { static const Color color = Color( 0xDA,  0x70,  0xD6 ); return color; }
    const Color& paleGoldenRod()         { static const Color color = Color( 0xEE,  0xE8,  0xAA ); return color; }
    const Color& paleGreen()             { static const Color color = Color( 0x98,  0xFB,  0x98 ); return color; }
    const Color& paleTurquoise()         { static const Color color = Color( 0xAF,  0xEE,  0xEE ); return color; }
    const Color& paleVioletRed()         { static const Color color = Color( 0xD8,  0x70,  0x93 ); return color; }
    const Color& papayaWhip()            { static const Color color = Color( 0xFF,  0xEF,  0xD5 ); return color; }
    const Color& peachPuff()             { static const Color color = Color( 0xFF,  0xDA,  0xB9 ); return color; }
    const Color& peru()                  { static const Color color = Color( 0xCD,  0x85,  0x3F ); return color; }
    const Color& pink()                  { static const Color color = Color( 0xFF,  0xC0,  0xCB ); return color; }
    const Color& plum()                  { static const Color color = Color( 0xDD,  0xA0,  0xDD ); return color; }
    const Color& powderBlue()            { static const Color color = Color( 0xB0,  0xE0,  0xE6 ); return color; }
    const Color& purple()                { static const Color color = Color( 0x80,  0x00,  0x80 ); return color; }
    const Color& red()                   { static const Color color = Color( 0xFF,  0x00,  0x00 ); return color; }
    const Color& rosyBrown()             { static const Color color = Color( 0xBC,  0x8F,  0x8F ); return color; }
    const Color& royalBlue()             { static const Color color = Color( 0x41,  0x69,  0xE1 ); return color; }
    const Color& saddleBrown()           { static const Color color = Color( 0x8B,  0x45,  0x13 ); return color; }
    const Color& salmon()                { static const Color color = Color( 0xFA,  0x80,  0x72 ); return color; }
    const Color& sandyBrown()            { static const Color color = Color( 0xF4,  0xA4,  0x60 ); return color; }
    const Color& seaGreen()              { static const Color color = Color( 0x2E,  0x8B,  0x57 ); return color; }
    const Color& seaShell()              { static const Color color = Color( 0xFF,  0xF5,  0xEE ); return color; }
    const Color& sienna()                { static const Color color = Color( 0xA0,  0x52,  0x2D ); return color; }
    const Color& silver()                { static const Color color = Color( 0xC0,  0xC0,  0xC0 ); return color; }
    const Color& skyBlue()               { static const Color color = Color( 0x87,  0xCE,  0xEB ); return color; }
    const Color& slateBlue()             { static const Color color = Color( 0x6A,  0x5A,  0xCD ); return color; }
    const Color& slateGrey()             { static const Color color = Color( 0x70,  0x80,  0x90 ); return color; }
    const Color& snow()                  { static const Color color = Color( 0xFF,  0xFA,  0xFA ); return color; }
    const Color& springGreen()           { static const Color color = Color( 0x00,  0xFF,  0x7F ); return color; }
    const Color& steelBlue()             { static const Color color = Color( 0x46,  0x82,  0xB4 ); return color; }
    const Color& tan()                   { static const Color color = Color( 0xD2,  0xB4,  0x8C ); return color; }
    const Color& teal()                  { static const Color color = Color( 0x00,  0x80,  0x80 ); return color; }
    const Color& thistle()               { static const Color color = Color( 0xD8,  0xBF,  0xD8 ); return color; }
    const Color& tomato()                { static const Color color = Color( 0xFF,  0x63,  0x47 ); return color; }
    const Color& turquoise()             { static const Color color = Color( 0x40,  0xE0,  0xD0 ); return color; }
    const Color& violet()                { static const Color color = Color( 0xEE,  0x82,  0xEE ); return color; }
    const Color& wheat()                 { static const Color color = Color( 0xF5,  0xDE,  0xB3 ); return color; }
    const Color& white()                 { static const Color color = Color( 0xFF,  0xFF,  0xFF ); return color; }
    const Color& whiteSmoke()            { static const Color color = Color( 0xF5,  0xF5,  0xF5 ); return color; }
    const Color& yellow()                { static const Color color = Color( 0xFF,  0xFF,  0x00 ); return color; }
    const Color& yellowGreen()           { static const Color color = Color( 0x9A,  0xCD,  0x32 ); return color; }
  }
}

