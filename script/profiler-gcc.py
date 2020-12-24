import re
import os.path
import sys

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
section_whitelist={'\.text_*'}

#black list of section RegExp patterns . Matched sections will be blocked
section_blacklist={
'\.text_nmprofiler',
'\.text_printf',
'\.text\.',
'\.text '
}

#black list of funcname RegExp patterns. Matched functions will be blocked
function_blacklist={
'ConvI32toF'	,
'ConvU32toF'	,
'ConvFtoI32'	,
'ConvFtoU32'	,
'ConvFtoI64'	,
'ConvFtoU64'	,
'ConvI32toD'	,
'ConvU32toD'	,
'FCmp'	,
'ConvDtoI32'	,
'ConvDtoU32'	,
'ConvDtoF'	,
'ConvDtoI64'	,
'ConvDtoU64'	,
'ConvI64toD'	,
'ConvU64toD'	,
'ConvFtoD'	,
'LShift64'	,
'RShift64'	,
'ARShift64'	,
'DCmp'	,
'DAdd'	,
'Mul64'	,
'DMul'	,
'IDiv64'	,
'UDiv64'	,
'UMod64'	,
'IMod64'	,
'DDiv'	,
'Mul32HighU'	,
'Mul32HighS'	,
'^vec_',
'__[a-zA-W]',
'_print_32x',
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
'^_DefaultIntHandler$'
}

nonstd_names = {
'UDiv32'    ,
'IDiv32'    ,
'LShift32'  ,
'ARShift32' ,
'UMod32'    ,
'IMod32'    ,
'Mul32'     ,
'RShift32'  ,
'FAdd'      ,
'FMul'		,    
'FDiv'		,
'ConvI64toF',
'ConvU64toF',
'Mul32Ex'   ,
'FFrExp'    ,
'DFrExp'    
#generateCall42
#'DAdd',
#'Mul64',
#'DMul',
#'IDiv64',
#'UDiv64',
#'UMod64',
#'IMod64',
#'DDiv'

}

print('//****************************************************************************************')
print('// This is automaticly generated asm-file by profiler-gcc.py' );
print('//****************************************************************************************')
print('import from "nmprofiler.mlb";')
print('begin ".text_nmprofiler\"')
print("PROFILE_BEGIN(",max_funcname_length,",0);")
print('')

address_list=[]
if os.path.isfile(mapfile):
	f = open(mapfile, 'r')
	
	for line in f:
		#try to detect pure section declaration without address
		match=re.search('^([.\w]+)\n',line)
		if match:
			section=match[1]
			continue
		else:
			#try to detect section declaration with address and size
			match=re.search('^\s?([.\w]+)\s+0x([\dabcdef]*)\s+0x([\dabcdef]*)',line)
			if match:
				section=match[1]
				continue
		
		
		#try to detect lebel(func or data) declaration with address 
		match=re.search('^\s+0x([\dabcdef]*)\s+([_a-zA-Z][_a-zA-Z0-9]+)',line)
		if match:
			funcname=match[2]
			funcaddr=match[1]
			#print('[',section,']', func)
		else:
			continue
		
		#filter out by section whiteslist 
		matched=False
		for white_pattern in section_whitelist:
			match=re.search(white_pattern,section)
			if match:
				matched=True
				break
		if not matched:
			continue
		
		#filter out by section blackslist 
		matched=False
		for black_pattern in section_blacklist:
			match=re.search(black_pattern,section)
			if match:
				matched=True
				continue
		if matched:
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
		

		#match on non-std C-call function
		matched=False
		for name in nonstd_names:
			if name==funcname:
				matched=True
				break

		#print(outfuncname)
		match=re.search("(_)?(_Z\d*)?(.*)",funcname)
		if match:
			outfuncname=match[3]
		
		match=re.search("(.+)PK.*",outfuncname)
		if match:
			outfuncname=match[1]
		
		match=re.search("(.+)RK.*",outfuncname)
		if match:
			#print("**********")
			outfuncname=match[1]
		
		
		align = ('                          ')[:16-len(outfuncname)]
		outfuncname= (outfuncname+" "*max_funcname_length)[:max_funcname_length]

		if funcaddr in address_list:
			print("\t//FUNCTION(",funcname,",",align,"\""+outfuncname+"\");// ",funcaddr," [",section,"]")
			continue
		address_list.append(funcaddr)

		if matched:
			print("\tNONCFUNC(",funcname,",",align,"\""+outfuncname+"\");// ",funcaddr," [",section,"]")
		else:				
			print("\tFUNCTION(",funcname,",",align,"\""+outfuncname+"\");// ",funcaddr," [",section,"]")
	


	
print('') 
print('PROFILE_END();')
print('end ".text_nmprofiler";')
	
