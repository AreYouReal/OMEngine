
import os, sys, ntpath, re


def iterateThrougFiles(dir):
	for name in os.listdir(dir):
		path = os.path.join(dir, name);
		if os.path.isfile(path):
			cutInclude(path)
		else: 
			iterateThrougFiles(path)


def cutInclude(filename):
	fileContent = ""
	s = open(filename)
	for line in s.readlines():
		if line.startswith("#include"):
			betweenQuotes = re.findall(r'"([^"]*)"', line)
			if len(betweenQuotes) > 0:
				includeHeader = ntpath.basename(re.findall(r'"([^"]*)"', line)[0])
				if(includeHeader.startswith("bt") or includeHeader.startswith("b3")):
					fileContent += "#include "  +  "\"" + includeHeader + "\"\n"
				else:
					fileContent += line
			else:
				fileContent += line
		else:
			fileContent += line

	f = open(filename, 'w');
	f.write(fileContent)


dir = "testFolder"

iterateThrougFiles(dir)