import os
import time
import datetime

FILE = open("../../SuperNxOgre.h", "w")
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

#ifndef SUPER_NXOGRE_H
#define SUPER_NXOGRE_H

""")
for root, dirs, files in os.walk("../source"):
	for name in files:
		if name != "NxOgre.h":
			if name.startswith("NxOgre"):
				if name.endswith(".h"):
					infile = open(os.path.join(root, name),"r")
					headerComment = False
					for line in infile.readlines():
						trimmed = line
						trimmed.strip()
						
						if (trimmed.startswith("#")):
							if (trimmed.startswith("#include \"NxOgre")):
								continue
									
						if (trimmed.startswith("/** File:")):
							headerComment = True
						
						if (trimmed.startswith("*/") and headerComment == True):
							headerComment = False
							continue
							
						if (headerComment == True):
							continue
							
						FILE.write(line)
 
FILE.write("\n#endif\n")
FILE.close()