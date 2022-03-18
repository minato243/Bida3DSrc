
SRC_PATH = "StaticWorld.h"
SRC_PATH2 = "gift_code_vang.txt"
def inplace_change(fileName, oldText, newText):
	with open(fileName) as f:
		s = f.read()
		s = s.replace(oldText, newText)
	with open(fileName, "w") as f:
		f.write(s)
		
def changeGiftCodeToJson(src, giftId):
	f = open(src, "r")
	new_content = ""
	#lines = f.readlines()
	#for line in lines:
	#while True:
	#	line = f.readline()
	#	if not line:
	#		break;
	#	new_content += "\""+line+"\":"+giftId+","
	tmp = f.read().split("\n")
	for line in tmp:
		if (line != ""):
			new_content += "\""+line+"\":"+giftId+","
	f.close()
	f2 = open("new"+src, "w")
	f2.write(new_content)
	
def removeSuffix(line, condition, removeAfter):
	if(condition in line):
		print line
		print len(line)
		index = line.find(removeAfter)
		print index
		
		newLine = line[:index-1]+";"
		return newLine
	return line
	
def removePrefix(line, condition, removeTo):
	if(condition in line):
		index = line.find(removeTo)
		newLine = line[index+len(removeTo):]
		return newLine
	return line
	
def main():
	f = open(SRC_PATH)
	new_content = ""
	tmp = f.read().split("\n")
	for line in tmp:
		newLine = removePrefix(line, "PhysicsWorld", "PhysicsWorld     - ")
		new_content+=newLine+"\n"	
	f.close()
	
	f2 = open(SRC_PATH+"_new","w")
	f2.write(new_content)
	
main()
#changeGiftCodeToJson(SRC_PATH2, "374")