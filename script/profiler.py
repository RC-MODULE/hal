import re
import os.path
import sys
import string



program_name = sys.argv[0]
arguments = sys.argv[1:]
count = len(arguments)
mapfile =""
if count>0:
	#print(count,arguments)
	mapfile  = arguments[0]

section=""
max_funcname_length=16


#white list of section  RegExp patterns. Only matched sectinos will be passed
section_whitelist={'\.text\d*'}

#black list of section RegExp patterns . Matched sections will be blocked
section_blacklist={
'\.text_nmprofile',
'\.text_printf',
'\.text\.'
}

#black list of funcname RegExp patterns. Matched functions will be blocked
function_blacklist={
'^__'               ,
'_result_code'      ,
'loadStackAddr'     ,
'^start$'           ,
'^stop$'            ,
'^_sync$'           ,
'^_clock$'          ,
'^_exit$'           ,
'^_ncl'			    ,
'_clock_initialize' ,
'clock_initialize'  ,
'_\w*Interrupt'     ,
'_Libint'           ,
'_puts_'            ,
'_puts'             ,
'FPIEEE_Signal'     ,
'^_lstat'           ,
'^_utime'           ,
'^_fchmod'          ,
'^_fchown'          ,
'^_semi_'           ,
'^DResult$'			,
'^_stopwatch'		,
'^_Inter_Template$' ,
'^_DefaultIntHandler$',
'^_realloc',
'_sprintf',
'_vssprintf',
'_longjmp',
'_DstrDouble'
}

nonstd_names = {
'UDiv32'    ,
'IDiv32'    ,
'LShift32'  ,
'Mul32Ex'   ,
'ARShift32' ,
'DAdd'      ,
'DFrExp'    ,
'DResult'   ,
'DCmp'      ,
'DDiv'      ,
'DMul'      ,
'ConvDtoI32',
'ConvI32toD',
'UMod32'    ,
'IMod32'    ,
'Mul32'     ,
'RShift32'  ,
'ConvU32toD'
}

print('//****************************************************************************************')
print('// This is automaticly generated asm-file by profiler-gcc.py' );
print('//****************************************************************************************')
print('import from "nmprofiler.mlb";')
print('begin ".text_nmprofiler\"')
print("PROFILE_BEGIN(",max_funcname_length,",0);")
print('')

class csection:
	def __init__(self, name, addr, sizes):
		self.name=name	
		self.addr=addr
		self.size=sizes

address_list=[]

if os.path.isfile(mapfile):
	f = open(mapfile, 'r')
	 #с помощью данного списка исключаем повторы адресов 
	sections=[]
	for line in f:
		#section = csection()
		#try to detect section declaration with address and size
		match=re.search('^\s?([.\w]+)\s+0x([\dabcdef]*)\s+0x([\dabcdef]*)',line)
		if match:
			section = csection(match[1],match[2],match[3])
			#filter out by section whiteslist 
			matched=False
			for white_pattern in section_whitelist:
				match=re.search(white_pattern,section.name)
				if match:
					matched=True
					break
			if not matched:
				continue
			
			#filter out by section blackslist 
			matched=False
			for black_pattern in section_blacklist:
				match=re.search(black_pattern,section.name)
				if match:
					matched=True
					continue
			if matched:
				continue
			
			sections.append(section)
			#print(match[0])
			#print(section.name,section.addr,section.size)
		
		#try to detect lebel(func or data) declaration with address 
		match=re.search('^\s(\w+)\s+0x([\dabcdef0-9]+)$',line)
		if match:
			funcname=match[1]
			funcaddr=match[2]
			
			#print(funcname,funcaddr)
		else:
			continue
	
		
		#filter out by funcname blackslist 
		matched=False
		for black_pattern in function_blacklist:
			match=re.search(black_pattern,funcname)
			if match:
				matched=True
				continue
		if matched:
			continue
		

		#filter out by address 
		matched=False
		for section in sections:
			func_addr  = int(funcaddr,16) 
			sect_addr  = int(section.addr,16) 
			sect_size  = int(section.size,16) 
			sect_name  = section.name
			if (sect_addr<= func_addr and func_addr<sect_addr+sect_size):
				matched=True
				break
		if not matched:
			continue
		
		#print(funcname,funcaddr)	
		
		#match on non-std C-call function
		matched=False
		for name in nonstd_names:
			if name==funcname:
				matched=True
				break
		
		
		align = ('                          ')[:16-len(funcname)]
		outfuncname= (funcname+" "*max_funcname_length)[:max_funcname_length]
		if funcaddr in address_list:
			print("\t//FUNCTION(",funcname,",",align,"\""+outfuncname+"\");// ",funcaddr," [",sect_name,"]")
			continue

		address_list.append(funcaddr)
		
		if matched:
			print("\tNONCFUNC(",funcname,",",align,"\""+outfuncname+"\");// ",funcaddr," [",sect_name,"]")
		else:				
			print("\tFUNCTION(",funcname,",",align,"\""+outfuncname+"\");// ",funcaddr," [",sect_name,"]")
	

#print(sections)
	
print('') 
print('PROFILE_END();')
print('end ".text_nmprofiler";')


	
