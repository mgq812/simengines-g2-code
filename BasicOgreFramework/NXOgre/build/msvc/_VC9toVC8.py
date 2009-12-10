import os, string

dest = open("NxOgre.VC8.sln", "w")
orig = open("NxOgre.VC9.sln", "r")

for line in orig.readlines():
		
	trimmed = line
	trimmed.strip()
	
	trimmed = string.replace(trimmed, "Format Version 10.00", "Format Version 9.00",1)
	trimmed = string.replace(trimmed, "2008", "2005",1)
	trimmed = string.replace(trimmed, "VC9", "VC8",1)	

	dest.write(trimmed)

orig.close()
dest.close()
#

for root, dirs, files in os.walk("./"):
	for name in files:
		if name.endswith(".VC9.vcproj"):
			in_name = os.path.join(root, name)
			out_name = string.replace(in_name, ".VC9.vcproj", ".VC8.vcproj",1)
			infile = open(in_name,"r")
			outfile = open(out_name, "w")
			for line in infile.readlines():
				line = string.replace(line, "Version=\"9.00\"","Version=\"8.00\"", 1)
				outfile.write(line)
			infile.close()
			outfile.close()