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


-- An evaluation function evaluating a parametric sphere

function eval ( u, v, d1, d2, lu, lv )



  -- Set up return value - matrix[u][v]

  local p = {}
  for i = 0, d1 do
    p[""..i] = {}
    for j = 0, d2 do
      p[""..i][""..j] = {}
      p[""..i][""..j].x = 0
      p[""..i][""..j].y = 0
      p[""..i][""..j].z = 0
    end
  end



  -- Compute help variables

  radius = 5.0

  cos_u = math.cos( u )
  cos_v = radius * math.cos( v )
  sin_u = math.sin( u )
  sin_v = radius * math.sin( v )

  cu_cv = cos_u * cos_v
  cu_sv = cos_u * sin_v
  su_cv = sin_u * cos_v
  su_sv = sin_u * sin_v



  -- Compute function values and derivatives

  -- S
  p['0']['0'].x = cos_u * cos_v
  p['0']['0'].y = sin_u * cos_v
  p['0']['0'].z = sin_v

  -- S_u
  if d1 > 0
  then
    p['1']['0'].x = -su_cv
    p['1']['0'].y =  cu_cv
    p['1']['0'].z =  0.0
  end

  -- S_v
  if d2 > 0
  then
    p['0']['1'].x = -cu_sv
    p['0']['1'].y = -su_sv
    p['0']['1'].z =  cos_v
  end

  -- S_uv
  if d1 > 0 and d2 > 0
  then
    p['1']['1'].x =  su_sv
    p['1']['1'].y = -cu_sv
    p['1']['1'].z =  0.0
  end

  -- S_uu
  if d1 > 1
  then
    p['2']['0'].x = -cu_cv
    p['2']['0'].y = -su_cv
    p['2']['0'].z =  0.0
  end

  -- S_vv
  if d2 > 1
  then
    p['0']['2'].x = -cu_cv
    p['0']['2'].y = -su_cv
    p['0']['2'].z = -sin_v
  end

  -- S_uuv
  if d1 > 1 and d2 > 0
  then
    p['2']['1'].x =  cu_sv
    p['2']['1'].y =  su_sv
    p['2']['1'].z =  0.0
  end

  -- S_uvv
  if d1 > 0 and d2 > 1
  then
    p['1']['2'].x =  su_cv
    p['1']['2'].y = -cu_cv
    p['1']['2'].z =  0.0
  end

  -- S_uuvv
  if d1 > 1 and d2 > 1
  then
    p['2']['2'].x =  cu_cv
    p['2']['2'].y =  su_cv
    p['2']['2'].z =  0.0
  end

  -- S_uuu
  if d1 > 2
  then
    p['3']['0'].x =  su_cv
    p['3']['0'].y = -cu_cv
    p['3']['0'].z =  0.0
  end

  -- S_vvv
  if d2 > 2
  then
    p['0']['3'].x =  cu_sv
    p['0']['3'].y =  su_sv
    p['0']['3'].z = -cos_v
  end

  -- S_uuuv
  if d1 > 2 and d2 > 0
  then
    p['3']['1'].x = -su_sv
    p['3']['1'].y =  cu_sv
    p['3']['1'].z =  0.0
  end

  -- S_uuuvv
  if d1 > 2 and d2 > 1
  then
    p['3']['2'].x = -su_cv
    p['3']['2'].y =  cu_cv
    p['3']['2'].z =  0.0
  end

  -- S_uvvv
  if d1 and d2 > 2
  then
    p['1']['3'].x = -su_sv
    p['1']['3'].y =  cu_sv
    p['1']['3'].z =  0.0
  end

  -- S_uuvvv
  if d1 > 1 and d2 > 2
  then
    p['2']['3'].x = -cu_sv
    p['2']['3'].y = -su_sv
    p['2']['3'].z =  0.0
  end

  -- S_uuuvvv
  if d1 > 2 and d2 > 2
  then
    p['3']['3'].x =  su_sv
    p['3']['3'].y = -cu_sv
    p['3']['3'].z =  0.0
  end



  -- Return result

  return p

end
