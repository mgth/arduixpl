/*
  Arbouyno - xPL for arduino
  Copyright (c) 2012 Mathieu GRENET.  All right reserved.

  This file is part of Arbouyno.

    Arbouyno is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    Arbouyno is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with Arbouyno.  If not, see <http://www.gnu.org/licenses/>.

	  Modified Oct 25 2012 by Mathieu GRENET
*/

#include "xPL_Adapter.h"

void xPL_Adapter::reg()
{
	xPL_Schema::reg();
	xPL.setAdapter(this);
}
