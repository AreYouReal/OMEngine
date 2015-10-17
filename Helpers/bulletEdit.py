
import os, sys, ntpath



def iterateThrougFiles(dir):
	for name in os.listdir(dir):
		path = os.path.join(dir, name);
		if os.path.isfile(path):
			cutInclude(path)
		else: 
			iterateThrougFiles(path)


def cutInclude(filename):
	print("CutInclude for file " + filename)
	fileContent = ""
	s = open(filename)
	for line in s.readlines():
		if line.startswith("#include"):
			includeHeader = ntpath.basename(line[10:])
			if(includeHeader.startswith("bt") or includeHeader.startswith("b3")):
				fileContent += "#include "  +  "\"" + includeHeader
			else:
				fileContent += line
		else:
			fileContent += line

	f = open(filename, 'w');
	f.write(fileContent)


dir = "testFolder"

iterateThrougFiles(dir)