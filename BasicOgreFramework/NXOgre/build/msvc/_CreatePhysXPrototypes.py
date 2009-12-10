import os
import time
import datetime
import re

def niceComment(first, maxLength):
	t = first
	for i in range(maxLength - len(first)):
		t += ' '
	
	t += "//< \\internal Prototyped class. DO NOT USE.";
	return t

physXClasses = ["class NxUtilLib;", "class NxJoint;", "class NxSpringDesc;", "class NxJointLimitDesc;", "class NxClothDesc;", "class NxMeshData;", "class NxSoftBodyDesc;"]
sdkVersMaj = "?"
sdkVersMin = "?"
sdkVersBug = "?"
for root, dirs, files in os.walk("N:\libraries\PhysX\SDKs"):
	for name in files:
		if name.endswith(".h"):
			f = open(root + "\\" + name)
			for line in f:
				
				if (len(line) == 0):
					continue
					
				# Don't prototype templates! To much hassle, never used.
				if (line.find("<") > 0):
					continue
					
				x = re.findall("class\s+(Nx\w+)\s*[:;]", line)
				if (len(x)):
					physXClasses.append("class "+ x[0] + ";")
					
				x = re.findall("struct\s+(Nx\w+)\s*[:;]", line)
				if (len(x)):
					physXClasses.append("struct "+ x[0] + ";")
				
				x = re.findall("#define NX_SDK_VERSION_MAJOR\s+(\w*)\s+", line)
				if (len(x)):
					sdkVersMaj = x[0];	
					
				x = re.findall("#define NX_SDK_VERSION_MINOR\s+(\w*)\s+", line)
				if (len(x)):
					sdkVersMin = x[0];	

				x = re.findall("#define NX_SDK_VERSION_BUGFIX\s+(\w*)\s+", line)
				if (len(x)):
					sdkVersBug = x[0];	
										
			f.close();
			
			
FILE = open("../source/NxOgrePhysXPrototypes.h", "w")
FILE.write("""/** File: NxOgrePhysXPrototypes.h
    Created on: """)
FILE.write(datetime.date.today().strftime("%d-%b-%y"))
FILE.write("""
    Author: Robin Southern "betajaen"
    SVN: $Id$
    This is an automatically created file.

    (c) Copyright, 2008-2009 by Robin Southern, http://www.nxogre.org

    This file is part of NxOgre.

    This file is released under the public domain.
*/

#ifndef NXOGRE_PHYSX_PROTOTYPES_H
#define NXOGRE_PHYSX_PROTOTYPES_H

/*
    
          This isn't PhysX. This is just a copy of the PhysX classes and structs, so they can be used in NxOgre, without
          the user's application requiring the PhysX headers. You can't actually do anything with them unless you import
          the headers and libraries from the PhysX SDK into your application; otherwise it's just an unknown class that
          your application cannot use.
*/


                                                                                       

""")

FILE.write("//    Prototype classes generated with the PhysX SDK " + sdkVersMaj + "." + sdkVersMin + "." + sdkVersBug + ".\n")

FILE.write("""
                                                                                       

""");

clean = []
maxLength = 0

for e in physXClasses:
	if e not in clean:
		if len(e) > maxLength:
			maxLength = len(e)
		clean.append(e)
clean.sort(key=str.lower)

for c in clean:
	FILE.write(niceComment(c, maxLength + 2) + "\n")

FILE.write("""

                                                                                       

""");
FILE.write("\n#endif\n")
FILE.close()