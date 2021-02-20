import re
import os
import sys

def argv_input():
	result = {'out': 'trace.dasm', 'trace': 'trace.bin', 'dump': 'main.dasm'}
	for i in range (1, len(sys.argv)):
		argv = sys.argv[i]
		param = re.findall('(dump|trace|out)=(.*)', argv)
		if param != []:
			if param[0][0] == 'dump':
				result['dump'] = param[0][1]
			elif param[0][0] == 'trace':
				result['trace'] = param[0][1]
			elif param[0][0] == 'out':
				result['out'] = param[0][1]		
		else:
			print('error')
	return result

def console_input():
	result = {}
	print("Enter trace file name:", end = ' ')
	result['trace'] = input()

	print("Enter dump file name:", end = ' ')	
	result['dump'] = input()	

	print("Enter out file name:", end = ' ')	
	result['out'] = input()

	return result

def get_dump_dict(dump_file):
	dump_dict = {}
	#						address		  			label(if exist)	 (instr)
	pattern = re.compile('([0-9a-fA-F]+):?(\s|\t)(\<[_a-zA-Z0-9]*\>)?(.*)')
	for d in dump_file:
		line = re.findall(pattern, d)
		if len(line) > 0:
			dump_dict.setdefault(line[0][0], {})
			key = line[0][0]
			if line[0][2] != '':
				dump_dict[key]['label'] = line[0][2]
			dump_dict[key]['instr'] = line[0][3]
	return dump_dict

def read_txt(trace_file, dump_dict, out_file):
	for t in trace_file:
		result = re.findall('(0*(\w+))\s+(\w+)', t)
		key = result[0][1]
		label_key = result[0][0]
		#print(key + ' ' + label_key)
		if key != 'deadc0de' and key != 'c0dec0de':
			if dump_dict.get(key) != None:
				#метка
				if dump_dict.get(label_key)!= None and dump_dict.get(label_key).get('label') != None:
					out_file.write('\t' + dump_dict[label_key]['label'] + '\n')
				#четная инструкция
				out_file.write(key + '\t\t' + dump_dict[key]['instr'] + '\n')
				#нечетная инструкция
				sec = re.findall('0x(.*)',hex(int(key,16) + 1))
				second_key = sec[0]
				out_file.write(second_key + '\t\t' + dump_dict[second_key]['instr'] + '\n')
	
	
				#проверка на команду перехода
				pattern2 = '.*(ireturn|return|call|goto).*'
				check1 = re.match(pattern2, dump_dict[key]['instr'])
				check2 = re.match(pattern2, dump_dict[second_key]['instr'])
				if check1 != None or check2 != None:
					for i in range (2, 4):
						add = re.findall('0x(.*)',hex(int(key,16) + i))
						add_key = add[0]
						out_file.write(add_key + '\t\t' + dump_dict[add_key]['instr'] + '\n')
			else:
				out_file.write('error\n')
		else:
			out_file.write('deadc0de' + '\n')

def read_bin(trace_file, dump_dict, out_file):
	while True:
		t = trace_file.read(8)
		if not t: break
		numbers = []
		for i in range(8):
			numbers.append(t[i])
		out_file.write('{:02x}{:02x}{:02x}{:02x}'.format(numbers[3], numbers[2], numbers[1], numbers[0]) + ' ')
		out_file.write('{:02x}{:02x}{:02x}{:02x}'.format(numbers[7], numbers[6], numbers[5], numbers[4]) + '\n')

names = argv_input()
#names = console_input()
try:
	dump_file = open(names['dump'], 'r')
	trace_file = open(names['trace'], 'rb')
	out_file = open(names['out'], 'w')	
	#if re.match('.*\.bin$',names['trace']) != None
	#	trace_file = open(names['trace'], 'rb')
	#else
	#	trace_file = open(names['trace'], 'r')
except IOError as e:
	print(e)
	sys.exit()	
	
dump_dict = get_dump_dict(dump_file)
dump_file.close()

tempfile_name = "tracer_temp.txt"
tempfile = open(tempfile_name, 'w')
read_bin(trace_file, dump_dict, tempfile)
tempfile.close()
tempfile = open(tempfile_name, 'r')
read_txt(tempfile, dump_dict, out_file)
tempfile.close()
os.remove(tempfile_name)

trace_file.close()
out_file.close()