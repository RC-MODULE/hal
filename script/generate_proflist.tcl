#************************************************************************************************
# 	RC Module 2014
# 	NeuroMatrix profiling library
# 	Autor: S.Mushkaev
#   Version 1.0
#
# Данный tcl-script автоматически генерирует вспомогательный asm-файл (proflist_generated.asm) с профилировочными макросами.
# Скрипт принимает на вход в качестве аргумента map-файл: - tclsh $(PROFILER)/script/generate_proflist.tcl $(PROJECT).map
# и находит все глобальные метки на функции, попавшие в секции .text*** .
# Функции, попавшие в список exclude_funcs, exclude_prefixes,
# а также в секции из списка exclude_sections  будут исключены из профилирования.
#************************************************************************************************


#======= список функций с нестандартным соглашанием о передаче параметров  (заключаются в PROFILE_NONSTD_FUNC )===========	
set nonstd_funcs   "list FAdd Mul32 FMul IDiv32 UDiv32 FDiv IMod32 UMod32 LShift32 RShift32 ARShift32 ConvI64toF ConvU64toF"
#======= функции из этих секций не будут профилироваться ===================
set exclude_sections ".text_nmvcore .text_profiler"
#======= функции, начинающиеся с этих слов не будут профилироваться ================
set exclude_prefixes "_realloc _nmprofiler __ _void._.8.8readNumber _void._C_Socket32 _C_Socket32"
#======= функции со следущими именами не будут профилироваться   ===========
set exclude_funcs "
					start
					_void._.8.8swapBuffers.1.2 
					_int._.0.8.8currBuffer
					_int._.8.8inBuffer
					_void._.8.8dummy.1.2
					_clock
					clock_initialize
					_exit
					_abort
					_DestructFloat
					_ConstructFloat
					_DstrDouble
					_ConstructDouble
					_stderr
					_err_msg
					_mes
					loadStackAddr
					_longjmp
					_setjmp
					_int._.0.8.8beginMessage.1.2
					_boardOutServBuf
					_boardSend
					_sended
					_sendMostDistant
					_nm_io_debug
					_sendConfirmed
					stop
					_vsprintf
					_void._std.8.8terminate.1.2
					_printf
					_ncl_hostSync
					_void0._.0.8.8alloc_somewhere.1void._.0.9._unsigned._int.9._class.__heap_control_block._.0.2
					_void._.8.8completeMessage.1enum._NM_IO_ServiceID.9._int.2
					_halProfilerEnable
					profileList
				"
				
set profile [lindex $argv 0]				
set mapfile [lindex $argv 1]

#set mapfile "mc5103_solo_std.map"

# удаляем мусор
set exclude_sections [string map { "\n" "," "\t" ""} $exclude_sections ]	
set exclude_section_list [split $exclude_sections]
# список всех меток
set func_list [list]
# список секций кода для профилирования (и начинающихся с .text)
set text_section_list [list]	



if { [file exists $mapfile] } {
	
	# парсинг map-файла	
	set mapfileid [open $mapfile r];
	while {[gets $mapfileid line] >=0} {
		set line [string trim $line]
		#========= ищем все секции кода (.text****) до "GLOBAL SYMBOLS:" и запоминаем их адреса =========================
		set idx [string last ".text" $line]
		regsub -all {\s\s+} $line { } line ; # remove multiple whitespaces
		

		if { $idx >= 0 } {
			#puts $line
			set addr 0
			set size 0
			set sect "__"
			# разбираем строку с секцией на имя адрес и размер
			scan $line "%s[]%x[]%x" sect addr size;
			# проверяем на список исключений
			if { [lsearch $exclude_section_list $sect ]==-1} {
				set addr_end  [expr $addr + $size]
				set sect_info [list $sect $addr $addr_end]
				lappend text_section_list $sect_info
			}
		}
	
	
		if { [string last "0x" $line] ==-1 } {
			continue;
		}
		set lline [split $line]
		if { [llength $lline]!=2 } {
			continue;
		}
		set func "-"
		set addr 0
		# parse string 
		scan $line "%s[ ]%x" func addr
		
		# check label that it is function   
		foreach section $text_section_list {
			if {$addr < [lindex $section 1] } {
				continue;
			}
			if {$addr > [lindex $section 2] } {
				continue;
			}
			lappend func_list [list $func $addr]
		}
	}
	close $mapfileid
	puts "proflist_generated.asm is updated"
} else {

	puts "map file \"$mapfile\" does not exist"
	puts "Empty template proflist_generated.asm  has been generated"
	
}

puts "PROFILING SECTIONS: "
foreach section $text_section_list {
	puts $section
	#puts "[lindex $section 0] [lindex $section 1] [lindex $section 2]"
}

puts "FUNCS: "
foreach func $func_list {
	puts "found: $func "
	#puts [lindex $section 0]
}
# удаляем ненужные символы
set nonstd_funcs  [string map { "\n" "," "\t" ""} $nonstd_funcs ]
set exclude_funcs [string map { "\n" "," "\t" ""} $exclude_funcs ]
set exclude_prefixes [string map { "\n" "," "\t" ""} $exclude_prefixes ]
# создаем список из функций c нестандартным вызовом 
set nonstd_func_list [split $nonstd_funcs {, }]
# создаем список из функций исключений к проилированию
set exclude_func_list [split $exclude_funcs {, }]
set exclude_prefix_list [split $exclude_prefixes {, }]
# список прфилируемых функций
set prof_list [list]


# фильтруем функции и записываем их в prof_list
foreach func_info $func_list {

	set func_name [lindex $func_info 0]
	set func_addr [lindex $func_info 1]
	#puts $func_name
	# проверяем, что не из списка исключительных имен
	if { [lsearch $exclude_func_list $func_name ]>=0} {
		continue
	}
	# проверяем, что не имеет исключительного префикса 
	set has_excluded_prefix 0
	foreach prefix $exclude_prefix_list {
		if { [string first $prefix $func_name]>=0 } {
			set has_excluded_prefix 1
			break
		}
	}
	if {$has_excluded_prefix==1} {
		continue
	}
	
	# проверяем что попадает в список профилируемых секций
	foreach section $text_section_list {
		set addr_section_begin   [lindex $section 1]
		set addr_section_end     [lindex $section 2]
		if {[format "%u" [ expr (($addr_section_begin <=$func_addr) && ($func_addr<$addr_section_end)) ] ]} {
			lappend prof_list $func_name
			#puts "add: $func_name"
			break
		} else {
			#puts "ignored: $func_name $func_addr"
		}
	}
}

#========== Генерация профилировочного файла ============================
set asmfileid [open $profile w] 

puts $asmfileid {
//****************************************************************************************
// This is automaticly generated asm-file by generate_proflist.bat (generate_proflist.tcl )
// Use this script to parse map-file to extract functions from abs-file 
// and to generate initial asm-template for profiling.
// It is recommended to remove  all unnecessary functions to make profiling more precise.
//****************************************************************************************
import from nmprofiler.mlb;
begin ".text_nmprofiler"
PROFILE_BEGIN(16);

}
puts $asmfileid {
	// place here macros with function name for each function you wish to profile like:
	// PROFILE_FUNC(_myfunc,"myfunc");
}
	
foreach func_name $prof_list {
	if { [lsearch $nonstd_func_list $func_name ]>=0} {
		puts $asmfileid "	PROFILE_NONSTD_FUNC($func_name,\"$func_name\");"
	}	else {
		puts $asmfileid "	PROFILE_FUNC($func_name,\"$func_name\");"
	}
}

puts $asmfileid {
PROFILE_END();
end ".text_nmprofiler";
}
close $asmfileid