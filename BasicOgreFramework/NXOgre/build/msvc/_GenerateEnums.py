import os
import time
import datetime

gStringClasses = []
gEnumClasses = []

def getPhysXClasses():
	gPhysXFile = open("N:\libraries\PhysX\SDKs\Foundation\include\NxUserAllocator.h", "r")
	gInEnumSection = False
	
	for line in gPhysXFile.readlines():
		line = line.strip()
		
		if (line == "enum NxMemoryType"):
			gInEnumSection = True
			continue
		
		if (len(line) == 0):
			continue
		
		if (gInEnumSection == False):
			continue
				
		if (line[0:1] == "{"):
			continue
		
		if (line[0:1] == "/"):
			continue
			
		if (line[0:1] == "}"):
			break
		
		gStringClasses.append(line.rsplit(",", 1)[0])
		
	gPhysXFile.close()
	
def getCPPClasses():
	cppClasses = ["void", "bool", "char", "short", "int", "float", "double", "long", "unsigned_char", "unsigned_short", "unsigned_int", "unsigned_long"]
	for c in cppClasses:
		gStringClasses.append(c)
		gEnumClasses.append(c)

def getNxOgreClasses():
	gCommonFile = open("../source/NxOgreCommon.h", "r")
	gInClassesSection = False
	
	temp = []
	cppObjectTypes = ["class","struct","union"]
	inComment = False
	for line in gCommonFile.readlines():

		line = line.strip()

		if (len(line) == 0 or line[0:1] == "#"):
			continue
		
		if (inComment):
			if (line[0:2] == "*/"):
				inComment = False
				continue
			
		if (line[0:2] == "/*"):
			inComment = True
			continue
			
	
		if (line[0:2] == "//"):
			if (line[0:4] != "//::"):
				continue
				
	
		for t in cppObjectTypes:
			if (line.find(t) != -1):
				
				if (line[0:4] != "//::"):
					if (line.find("//") != -1):
						if (line.find("//") < line.find(t)):
							continue
				else:	
					if (line.rfind("//") > 4):
						line = line[0:line.rfind("//")]
					
				e = line[line.rfind(t) + len(t):line.find(";")].strip()
				temp.append(e)


	gCommonFile.close()
	
	clean = []
	for e in temp:
		if e not in clean:
			clean.append(e)
	clean.sort(key=str.lower)
	
	for c in clean:
		gStringClasses.append(c)
		gEnumClasses.append(c)




def writeCPP():
	FILE = open("../source/NxOgreClasses.cpp", "w+")
	FILE.write("""/** File: NxOgreClasses.cpp
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

#include "NxOgreStable.h"
#include "NxOgreClasses.h"

                                                                                       

namespace NxOgre_Namespace
{

                                                                                       

namespace Classes
{

                                                                                       

static const char*  sClasses""")
	FILE.write("[" + str(len(gStringClasses)) + "] = {\n")

	for c in gStringClasses:
		FILE.write("  \"" + c + "\",\n")

	FILE.write("""};\n
const char*  TypeToCStr(int i)
{
 if (i < RENDERSYSTEMS_CLASSES_BEGIN)
  return sClasses[i];
 return "User";
} // void TypeToCstr

                                                                                       

} // namespace Classes

                                                                                       

} // namespace NXOGRE_NAMESPACE

                                                                                       
	""")
	
	FILE.close()
	

def writeH():
	FILE = open("../source/NxOgreClasses.h", "w+")
	FILE.write("""/** File: NxOgreClasses.h
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
#ifndef NXOGRE_CLASSES_H
#define NXOGRE_CLASSES_H

namespace NxOgre_Namespace
{

namespace Classes
{

enum
{
""")

	for c in gEnumClasses:
		if (c == "NXOGRE_CLASSES_BEGIN"):
			FILE.write("  NXOGRE_CLASSES_BEGIN = " + str(gNxOgreEnumStart) + ",\n")
			continue
				
				
		if (c == "RENDERSYSTEMS_CLASSES_BEGIN"):
			FILE.write("  RENDERSYSTEMS_CLASSES_BEGIN,\n")
			continue
		
		if (c == "USER_CLASSES_BEGIN"):
			FILE.write("  USER_CLASS = 50000\n")
			continue
			
		FILE.write("  _" + c + ",\n")

	FILE.write("""};

                                                                                       

  /** \\brief Turns a NxOgre::Classes::Enum into a String and any PhysX MemoryType enum value as well.
  */
  const char*  TypeToCStr(int i);

                                                                                       

} // namespace Classes

                                                                                       

} // namespace NXOGRE_NAMESPACE

                                                                                       
#endif
""")
	
	FILE.close()
			
getPhysXClasses()
gNxOgreEnumStart = len(gStringClasses)
gStringClasses.append("USER_CLASSES_BEGIN")
gEnumClasses.append("NXOGRE_CLASSES_BEGIN")
getCPPClasses()
getNxOgreClasses()
gStringClasses.append("RENDERSYSTEMS_CLASSES_BEGIN")
gEnumClasses.append("RENDERSYSTEMS_CLASSES_BEGIN")
gStringClasses.append("USER_CLASSES_BEGIN")
gEnumClasses.append("USER_CLASSES_BEGIN")
writeCPP()
writeH()
