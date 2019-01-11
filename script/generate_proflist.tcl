#************************************************************************************************
# 	RC Module 2014
# 	NeuroMatrix profiling library
# 	Autor: S.Mushkaev
#   Version 1.0
#
# ������ tcl-script ������������� ���������� ��������������� asm-���� (proflist_generated.asm) � ���������������� ���������.
# ������ ��������� �� ���� � �������� ��������� map-����: - tclsh $(PROFILER)/script/generate_proflist.tcl $(PROJECT).map
# � ������� ��� ���������� ����� �� �������, �������� � ������ .text*** .
# �������, �������� � ������ exclude_funcs, exclude_prefixes,
# � ����� � ������ �� ������ exclude_sections  ����� ��������� �� ��������������.
#************************************************************************************************


#======= ������ ������� � ������������� ����������� � �������� ����������  (����������� � PROFILE_NONSTD_FUNC )===========	
set nonstd_funcs   "list FAdd Mul32 FMul IDiv32 UDiv32 FDiv IMod32 UMod32 LShift32 RShift32 ARShift32 ConvI64toF ConvU64toF"
#======= ������� �� ���� ������ �� ����� ��������������� ===================
set exclude_sections ".text_nmvcore .text_profiler"
#======= �������, ������������ � ���� ���� �� ����� ��������������� ================
set exclude_prefixes "_realloc _nmprofiler __ _void._.8.8readNumber _void._C_Socket32 _C_Socket32"
#======= ������� �� ��������� ������� �� ����� ���������������   ===========
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

# ������� �����
set exclude_sections [string map { "\n" "," "\t" ""} $exclude_sections ]	
set exclude_section_list [split $exclude_sections]
# ������ ���� �����
set func_list [list]
# ������ ������ ���� ��� �������������� (� ������������ � .text)
set text_section_list [list]	



if { [file exists $mapfile] } {
	
	# ������� map-�����	
	set mapfileid [open $mapfile r];
	while {[gets $mapfileid line] >=0} {
		set line [string trim $line]
		#========= ���� ��� ������ ���� (.text****) �� "GLOBAL SYMBOLS:" � ���������� �� ������ =========================
		set idx [string last ".text" $line]
		regsub -all {\s\s+} $line { } line ; # remove multiple whitespaces
		

		if { $idx >= 0 } {
			#puts $line
			set addr 0
			set size 0
			set sect "__"
			# ��������� ������ � ������� �� ��� ����� � ������
			scan $line "%s[]%x[]%x" sect addr size;
			# ��������� �� ������ ����������
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
# ������� �������� �������
set nonstd_funcs  [string map { "\n" "," "\t" ""} $nonstd_funcs ]
set exclude_funcs [string map { "\n" "," "\t" ""} $exclude_funcs ]
set exclude_prefixes [string map { "\n" "," "\t" ""} $exclude_prefixes ]
# ������� ������ �� ������� c ������������� ������� 
set nonstd_func_list [split $nonstd_funcs {, }]
# ������� ������ �� ������� ���������� � �������������
set exclude_func_list [split $exclude_funcs {, }]
set exclude_prefix_list [split $exclude_prefixes {, }]
# ������ ������������ �������
set prof_list [list]


# ��������� ������� � ���������� �� � prof_list
foreach func_info $func_list {

	set func_name [lindex $func_info 0]
	set func_addr [lindex $func_info 1]
	#puts $func_name
	# ���������, ��� �� �� ������ �������������� ����
	if { [lsearch $exclude_func_list $func_name ]>=0} {
		continue
	}
	# ���������, ��� �� ����� ��������������� �������� 
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
	
	# ��������� ��� �������� � ������ ������������� ������
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

#========== ��������� ���������������� ����� ============================
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