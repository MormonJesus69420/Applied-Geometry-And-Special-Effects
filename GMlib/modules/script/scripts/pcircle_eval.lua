-----------------------------------------------------------------------------------
--
-- Copyright (C) 1994 Narvik University College
-- Contact: GMlib Online Portal at http://episteme.hin.no
--
-- This file is part of the Geometric Modeling Library, GMlib.
--
-- GMlib is free software: you can redistribute it and/or modify
-- it under the terms of the GNU Lesser General Public License as published by
-- the Free Software Foundation, either version 3 of the License, or
-- (at your option) any later version.
--
-- GMlib is distributed in the hope that it will be useful,
-- but WITHOUT ANY WARRANTY; without even the implied warranty of
-- MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
-- GNU Lesser General Public License for more details.
--
-- You should have received a copy of the GNU Lesser General Public License
-- along with GMlib.  If not, see <http://www.gnu.org/licenses/>.
--
-----------------------------------------------------------------------------------


-- An evaluation function evaluating a parametric circle

function eval ( t, d, l )



  -- Set up return value - vector[t]

  local p = {}
  for i = 0, d do
    p[""..i] = {}
    p[""..i].x = 0
    p[""..i].y = 0
    p[""..i].z = 0
  end



  -- Compute help variables

  radius = 5.0



  -- Compute function values and derivatives

  -- f
  p['0'].x = radius * math.cos( t )
  p['0'].y = radius * math.sin( t )
  p['0'].z = 0.0

  -- f'
  if d > 0 then
    p['1'].x = - radius * math.sin( t )
    p['1'].y =   radius * math.cos( t )
    p['1'].z = 0.0
  end

  -- f''
  if d > 1 then
    p['2'].x = - radius * math.cos( t )
    p['2'].y = - radius * math.sin( t )
    p['2'].z = 0.0
  end



  -- Return result

  return p

end
