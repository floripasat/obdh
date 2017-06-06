file_name = "uG-FloripaSat-RawData.txt"

def text_treatment(start, end):

	block = []
	current_byte = 0
	count = 0
	count2 = 0
	auxiliary = []
	blocks = []

	file = open(file_name, 'r')
	text = file.readlines()

	for i, line in enumerate(text):
		if (i < end and i >= start):
			input_line = line[:(len(line)-1)]
			part = input_line.replace('\\', " ").split(" ")
			for x in range(0,len(part)):
				block.append(part[x])
				
		elif (i == end):
			break

	for byte in block:

		if (byte=="7B" and block[current_byte-1]=="7B" and block[current_byte+1]=="7B"):
			count = count + 1
	
		if(count != 0):
			auxiliary.append(byte)
		

		if (byte=="0A" and block[current_byte-1]=="7D" and block[current_byte+1]=="0D"):
			blocks.append(auxiliary)
			if (blocks[count2] != []) :
				del (blocks[count2])[0]	
				del (blocks[count2])[0]	
				del (blocks[count2])[len(blocks[count2])-1]			
				del (blocks[count2])[len(blocks[count2])-1]	
				count2 = count2 + 1
			else:
				del blocks[count2]	
			auxiliary = []
			count = 0
			

		current_byte = current_byte + 1

	return blocks

"""
teste = text_treatment(start = 0, end = 200)

for r in range(0,len(teste)):
	print("---------------------------------------------------------------------------------------")
	print((teste[r]))
print(len(teste))
"""


