

exclude_section_prefix ={ '%.text%.', ".text_nmprofiler" }
include_section = { ".text" }
nonstd_name = { "dummy" }

exclude_name = { 
"_result_code"                 ,
"loadStackAddr"                ,
"start"                        ,
"_sync"                        ,
"_exit"                        ,
"_ncl_getProcessorNo"          ,
"_ncl_sendInterrupt"           ,
"_ncl_hostSync"                ,
"_ncl_hostSyncArray"           ,
"stop"                         ,
"_clock_initialize"            ,
"clock_initialize"             ,
"_clock"                       ,
"_Inter_Template"              ,
"_DefaultIntHandler"           ,
"_LibintIntManager"            ,
"_EnableInterrupts_PSWR"       ,
"_DisableInterrupts_PSWR"      ,
"_EnableInterrupts_IMR_Low"    ,
"_DisableInterrupts_IMR_Low"   ,
"_EnableInterrupts_IMR_High"   ,
"_DisableInterrupts_IMR_High"  ,
"_FlushInterrupts"             ,
"_GetInterruptMask_IMR_Low"    ,
"_GetInterruptMask_IMR_High"   ,
"_LibintGetCPUID"              ,
"_SetInterrupt"                ,
"_GetInterrupt"                ,
"_ClearInterrupt"              ,
"_LibintSetFPCCR"              ,
"_stopwatch_head"              ,
"_nmprofiler_printf"           ,
"_nmprofiler_sprintf"          ,
"_nmprofiler_print2tbl"        ,
"_nmprofiler_print2xml"        ,
"_printf"                      ,
"_puts_"                       ,
"_puts"                        ,
"FPIEEE_Signal"                ,
"_lstat"                       ,
"_utime"                       ,
"_fchmod"                      ,
"_fchown"                      ,
"_semi_fopen"                  ,
"_semi_fclose"                 ,
"_semi_fwrite"                 ,
"_semi_fread"                  ,
"_semi_fseek"                  
}




-----------------------------------------------------------------------------------

 
 list = {}
 func = {}
 section = "null"
 -- count = 0;
 
 
 
 local function contains(table, val)
	if #table==0 then 
		return true
	end
	for i=1,#table do
		--print(table[i],val);
		if table[i] == val then 
			return true
		end
	end
	return false
end
 
 local function contains_prefix(table, val)
   for i=1,#table do
	 --print(table[i],val);
	  indx,last=string.find(val,table[i]);
	  if (indx<>nul) then
		if indx==1 then
		     return true
        end
	  end
   end
   return false
end
 
 
 	
print('//****************************************************************************************')
print('// This is automaticly generated asm-file by profiler-gcc.lua' );
print('//****************************************************************************************')
print('import from "d:/git/hal/include/nmprofiler.mlb";')
print('begin ".text_nmprofiler\"')
print('PROFILE_BEGIN(16);')
print('')
	
-- print(arg[1])	
if #arg==1 then
	for line in io.lines(arg[1]) do 
		idx=string.find(line,"^%.text[%.%a_%d]*%s?");
		if idx<>nil then
			section=string.sub(line,string.find(line,"%.text[%.%a_%d]*"));
		end

		idx=string.find(line,"^%s+0x%x+%s+[_%a]%a[_%a%d]+$");
		if idx<>nil then
			
			
			idx0,idx1=string.find(line,"0x%x+"); -- find address
			func.addr=string.sub(line,idx0,idx1); -- extract address
			func.name=string.sub(line, string.find(line,"[_%a%d]+",idx1+1)); -- extract name
			func.sect=section;
			-- count= count+1;
			if not contains_prefix(exclude_section_prefix, func.sect) then
				if contains(include_section, func.sect) then
					if not contains(exclude_name, func.name) then
						if contains(nonstd_name, func.name) then
							print("PROFILE_NONSTD_FUNC("..func.name..",\""..func.name.."\");");
							--print(count,func.addr,func.name,func.sect);
							--print("\""..func.name.."\",");
						else
							print("PROFILE_FUNC("..func.name..",\""..func.name.."\");");
							--print("\""..func.name.."\",");
							--print(count,func.addr,func.name,func.sect);
						end
					end
				end
			end
		end
	end
 end 
 
print('') 
print('PROFILE_END();')
print('end ".text_nmprofiler";')