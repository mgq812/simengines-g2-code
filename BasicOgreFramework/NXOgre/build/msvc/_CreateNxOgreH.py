import os
import time
import datetime

FILE = open("./source/NxOgre.h", "w")
FILE.write("""/** File: NxOgre.h
    Created on: """)
FILE.write(datetime.date.today().strftime("%d-%b-%y"))
FILE.write("""
    Author: Robin Southern "betajaen"
    SVN: $Id$
    This is an automatically created file.

    (c) Copyright, 2008-2009 by Robin Southern, http://www.nxogre.org

    This file is part of NxOgre.

    NxOgre is free software: you can redistribute it and/or modify
    it under the terms of the GNU Lesser General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    NxOgre is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU Lesser General Public License for more details.

    You should have received a copy of the GNU Lesser General Public License
    along with NxOgre.  If not, see <http://www.gnu.org/licenses/>.
*/

#ifndef NXOGRE_H
#define NXOGRE_H

""")
for root, dirs, files in os.walk("../source"):
   for name in files:
   		if name != "NxOgre.h":
	   		if name.startswith("NxOgre"):
	   			if name.endswith(".h"):     
	   				FILE.write("#include \"" + name + "\"\n")
for root, dirs, files in os.walk("../msvc"):
   for name in files:
   		if name != "NxOgre.h":
	   		if name.startswith("NxOgre"):
	   			if name.endswith(".h"):     
	   				FILE.write("#include \"" + name + "\"\n")   				
FILE.write("\n#endif\n")
FILE.close()