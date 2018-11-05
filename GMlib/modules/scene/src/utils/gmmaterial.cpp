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




#include "gmmaterial.h"


namespace GMlib {

  namespace GMmaterial {

    //  Predefined Materials
    /*
    const Material& () { static const name                            GL_AMBIENT                                   GL_DIFFUSE                                    GL_SPECULAR                                     GL_SHININESS
    */
    const Material& blackPlastic()    { static const Material material = Material (   Color( 0.0,      0.0,      0.0,      1.0),   Color( 0.01,      0.01,     0.01,     1.0),   Color( 0.50,      0.50,      0.50,      1.0),   32.0    ); return material; }
    const Material& blackRubber()     { static const Material material = Material (   Color( 0.02,     0.02,     0.02,     1.0),   Color( 0.01,      0.01,     0.01,     1.0),   Color( 0.4,       0.4,       0.4,       1.0),   10.0    ); return material; }
    const Material& brass()           { static const Material material = Material (   Color( 0.329412, 0.223529, 0.027451, 1.0),   Color( 0.780392,  0.568627, 0.113725, 1.0),   Color( 0.992157,  0.941176,  0.807843,  1.0),   27.8974 ); return material; }
    const Material& bronze()          { static const Material material = Material (   Color( 0.2125,   0.1275,   0.054,    1.0),   Color( 0.714,     0.4284,   0.18144,  1.0),   Color( 0.393548,  0.271906,  0.166721,  1.0),   25.6    ); return material; }
    const Material& chrome()          { static const Material material = Material (   Color( 0.25,     0.25,     0.25,     1.0),   Color( 0.4,       0.4,      0.4,      1.0),   Color( 0.774597,  0.774597,  0.774597,  1.0),   76.8    ); return material; }
    const Material& copper()          { static const Material material = Material (   Color( 0.19125,  0.0735,   0.0225,   1.0),   Color( 0.7038,    0.27048,  0.0828,   1.0),   Color( 0.256777,  0.137622,  0.086014,  1.0),   12.8    ); return material; }
    const Material& emerald()         { static const Material material = Material (   Color( 0.0215,   0.1745,   0.0215,   1.0),   Color( 0.07568,   0.61424,  0.07568,  1.0),   Color( 0.633,     0.727811,  0.633,     1.0),   76.8    ); return material; }
    const Material& gold()            { static const Material material = Material (   Color( 0.24725,  0.1995,   0.0745,   1.0),   Color( 0.75164,   0.60648,  0.22648,  1.0),   Color( 0.628281,  0.555802,  0.366065,  1.0),   51.2    ); return material; }
    const Material& jade()            { static const Material material = Material (   Color( 0.135,    0.2225,   0.1575,   1.0),   Color( 0.54,      0.89,     0.63,     1.0),   Color( 0.316228,  0.316228,  0.316228,  1.0),   12.8    ); return material; }
    const Material& obsidian()        { static const Material material = Material (   Color( 0.05375,  0.05,     0.06625,  1.0),   Color( 0.18275,   0.17,     0.22525,  1.0),   Color( 0.332741,  0.328634,  0.346435,  1.0),   38.4    ); return material; }
    const Material& pearl()           { static const Material material = Material (   Color( 0.25,     0.20725,  0.20725,  1.0),   Color( 1.0,       0.829,    0.829,    1.0),   Color( 0.296648,  0.296648,  0.296648,  1.0),   11.264  ); return material; }
    const Material& pewter()          { static const Material material = Material (   Color( 0.105882, 0.058824, 0.113725, 1.0),   Color( 0.427451,  0.470588, 0.541176, 1.0),   Color( 0.333333,  0.333333,  0.521569,  1.0),   9.84615 ); return material; }
    const Material& plastic()         { static const Material material = Material (   Color( 1.0,      1.0,      1.0,      1.0),   Color( 0.5,       0.5,      0.5,      1.0),   Color( 0.4,       0.4,       0.4,       1.0),   10.0    ); return material; }
    const Material& polishedBronze()  { static const Material material = Material (   Color( 0.25,     0.148,    0.06475,  1.0),   Color( 0.4,       0.2368,   0.1036,   1.0),   Color( 0.774597,  0.458561,  0.200621,  1.0),   66.8    ); return material; }
    const Material& polishedCopper()  { static const Material material = Material (   Color( 0.2295,   0.08825,  0.0275,   1.0),   Color( 0.5508,    0.2118,   0.066,    1.0),   Color( 0.580594,  0.223257,  0.0695701, 1.0),   51.2    ); return material; }
    const Material& polishedGold()    { static const Material material = Material (   Color( 0.24725,  0.2245,   0.0645,   1.0),   Color( 0.34615,   0.3143,   0.0903,   1.0),   Color( 0.797357,  0.723991,  0.208006,  1.0),   83.2    ); return material; }
    const Material& polishedGreen()   { static const Material material = Material (   Color( 0.0,      0.0,      0.0,      1.0),   Color( 0.1,       0.35,     0.1,      1.0),   Color( 0.45,      0.55,      0.45,      1.0),   50.0    ); return material; }
    const Material& polishedRed()     { static const Material material = Material (   Color( 0.0,      0.0,      0.0,      1.0),   Color( 0.5,       0.0,      0.0,      1.0),   Color( 0.7,       0.6,       0.6,       1.0),   50.0    ); return material; }
    const Material& polishedSilver()  { static const Material material = Material (   Color( 0.23125,  0.23125,  0.23125,  1.0),   Color( 0.2775,    0.2775,   0.2775,   1.0),   Color( 0.773911,  0.773911,  0.773911,  1.0),   89.6    ); return material; }
    const Material& ruby()            { static const Material material = Material (   Color( 0.1745,   0.01175,  0.01175,  1.0),   Color( 0.61424,   0.04136,  0.04136,  1.0),   Color( 0.727811,  0.626959,  0.626959,  1.0),   56.8    ); return material; }
    const Material& sapphire()        { static const Material material = Material (   Color( 0.01175,  0.01175,  0.1745,   1.0),   Color( 0.04136,   0.04136,  0.61424,  1.0),   Color( 0.626959,  0.626959,  0.727811,  1.0),   76.8    ); return material; }
    const Material& silver()          { static const Material material = Material (   Color( 0.19225,  0.19225,  0.19225,  1.0),   Color( 0.50754,   0.50754,  0.50754,  1.0),   Color( 0.508273,  0.508273,  0.508273,  1.0),   51.2    ); return material; }
    const Material& snow()            { static const Material material = Material (   Color( 1.0,      1.0,      1.0,      1.0),   Color( 0.5,       0.5,      0.5,      1.0),   Color( 0.4,       0.4,       0.4,       1.0),   10.0    ); return material; }
    const Material& turquoise()       { static const Material material = Material (   Color( 0.1,      0.18725,  0.1745,   1.0),   Color( 0.396,     0.74151,  0.69102,  1.0),   Color( 0.297254,  0.30829,   0.306678,  0.8),   12.8    ); return material; }
  }


  /*! void Material::set( const GLenum sfactor, const GLenum dfactor )
   *  \brief Pending Documentation
   *
   *  Pending Documentation
   */
  void Material::set( const GLenum sfactor, const GLenum dfactor ) {

    _source_blend_factor    = sfactor;
    _destination_blend_factor  = dfactor;
  }


  /*! void Material::set(float shininess)
   *  \brief Pending Documentation
   *
   *  Pending Documentation
   */
  void Material::set(float shininess) {

    _shininess = shininess;
  }

  /*! bool Material::operator < ( const Material& m ) const
   *
   *  Dummy operator for Array
   */
  bool Material::operator < ( const Material& /*m*/ ) const {

    return true;
  }

  /*! void  MaterialObjectList::initPreDef()
   *  \brief Pending Documentation
   *
   *  Pending Documentation
   */
  void  MaterialObjectList::initPreDef() {

    if( (getMaxSize() - getSize()) < 25 )
      setMaxSize( getMaxSize() + 25 );

    insert( MaterialObject( GMmaterial::blackPlastic(), "BlackPlastic" ) );
    insert( MaterialObject( GMmaterial::blackRubber(), "BlackRubber" ) );
    insert( MaterialObject( GMmaterial::brass(), "Brass" ) );
    insert( MaterialObject( GMmaterial::bronze(), "Bronze" ) );
    insert( MaterialObject( GMmaterial::chrome(), "Chrome" ) );
    insert( MaterialObject( GMmaterial::copper(), "Copper" ) );
    insert( MaterialObject( GMmaterial::emerald(), "Emerald" ) );
    insert( MaterialObject( GMmaterial::gold(), "Gold" ) );
    insert( MaterialObject( GMmaterial::jade(), "Jade" ) );
    insert( MaterialObject( GMmaterial::obsidian(), "Obsidian" ) );
    insert( MaterialObject( GMmaterial::pearl(), "Pearl" ) );
    insert( MaterialObject( GMmaterial::pewter(), "Pewter" ) );
    insert( MaterialObject( GMmaterial::plastic(), "Plastic" ) );
    insert( MaterialObject( GMmaterial::polishedBronze(), "PolishedBronze" ) );
    insert( MaterialObject( GMmaterial::polishedCopper(), "PolishedCopper" ) );
    insert( MaterialObject( GMmaterial::polishedGold(), "PolishedGold" ) );
    insert( MaterialObject( GMmaterial::polishedGreen(), "PolishedGreen" ) );
    insert( MaterialObject( GMmaterial::polishedRed(), "PolishedRed" ) );
    insert( MaterialObject( GMmaterial::polishedSilver(), "PolishedSilver" ) );
    insert( MaterialObject( GMmaterial::ruby(), "Ruby" ) );
    insert( MaterialObject( GMmaterial::sapphire(), "Sapphire" ) );
    insert( MaterialObject( GMmaterial::silver(), "Silver" ) );
    insert( MaterialObject( GMmaterial::snow(), "Snow" ) );
    insert( MaterialObject( GMmaterial::turquoise(), "Turquoise" ) );
  }


  /*! bool  MaterialObjectList::readFromFile(char* file_name)
   *  \brief Pending Documentation
   *
   *  Pending Documentation
   */
  bool  MaterialObjectList::readFromFile( char* /*file_name*/ ) {

    return true;
  }


  /*! bool  MaterialObjectList::storeToFile(char* file_name)
   *  \brief Pending Documentation
   *
   *  Pending Documentation
   */
  bool  MaterialObjectList::storeToFile( char* /*file_name*/ ) {

    return true;
  }
}
