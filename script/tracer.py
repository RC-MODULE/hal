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

def get_dump_dict(file_name):
	dump_dict = {}
	try:
		dump_file = open(file_name, 'r')
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
		dump_file.close()
	except IOError as e:
		print("File " + file_name + " is not exist")
	return dump_dict

def get_trace_list(binary_file_name):
	result = []
	try:
		trace_file = open(binary_file_name, 'rb')
		while True:
			t = trace_file.read(8)
			if not t: break
			addr = '{:02x}{:02x}{:02x}{:02x}'.format(t[3], t[2], t[1], t[0])
			pswr = '{:02x}{:02x}{:02x}{:02x}'.format(t[7], t[6], t[5], t[4])
			result.append([addr,pswr])
		trace_file.close()
	except IOError as e:
		print("File " + binary_file_name + " is not exist")
	return result

def get_commands_list(addr_list, dump_dict):
	out_list = []
	for t in addr_list:
		result = re.findall('(0*(\w+)).*', t[0])
		key = result[0][1]
		label_key = result[0][0]
		if key != 'deadc0de' and key != 'c0dec0de':
			if dump_dict.get(key) != None:
				#метка
				if dump_dict.get(label_key)!= None and dump_dict.get(label_key).get('label') != None:
					out_list.append('\t' + dump_dict[label_key]['label'])
				#четная инструкция
				if key in dump_dict:
					out_list.append(key + '\t\t' + dump_dict[key]['instr'])
				#нечетная инструкция
				sec = re.findall('0x(.*)',hex(int(key,16) + 1))
				second_key = sec[0]
				if second_key in dump_dict:
					out_list.append(second_key + '\t\t' + dump_dict[second_key]['instr'])
	
	
				#проверка на команду перехода
				pattern2 = '.*(ireturn|return|call|goto).*'
				check1 = re.match(pattern2, dump_dict[key]['instr'])
				check2 = re.match(pattern2, dump_dict[second_key]['instr'])
				if check1 != None or check2 != None:
					for i in range (2, 4):
						add = re.findall('0x(.*)',hex(int(key,16) + i))
						add_key = add[0]
						if add_key in dump_dict:
							out_list.append(add_key + '\t\t' + dump_dict[add_key]['instr'])
			else:
				out_list.append('error\n')
		else:
			out_list.append('deadc0de' + '\n')
	return out_list

names = argv_input()
	
dump_dict = get_dump_dict(names['dump'])
trace_list = get_trace_list(names['trace'])
commands = get_commands_list(trace_list, dump_dict)
for c in commands:
	print(c)
