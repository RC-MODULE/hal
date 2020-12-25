//***************************************************************************/
//*                     RC Module Inc., Moscow, Russia                      */
//*                     NeuroMatrix(r) Software                             */
//*                                                                         */
//*   nmprofiler library                                                      */
//*                                                                         */
//*                                                                         */
//*   Software design:  S.Mushkaev                                          */
//*                                                                         */
//*   Version           1.0                                                 */
//*   Start date:       15.04.2015                                          */
//*                                                                         */
//*                                                                         */
//***************************************************************************/



//=======================================================================================================================

// �����������. ������ ������������� �������������� . ������ ���� ������. 
// MAXFUNCLEN - ���-�� �������� �� �������� ����� ��������� ��� ������� ��� ������ � ������� ��������������

//begin ".init"
//	extern clock_initialize	:label;
//	extern _nmprofiler_init	:label;
//	call   _nmprofiler_init;	// code replacement
//end ".init";


begin ".text_nmprofiler"
	//global _cache_enable: label;
	//<_cache_enable>
	//	CACHE_ENABLE();
	//return;
	
	global _nmprofiler_namesize: 	word= 0;
	global _nmprofiler_head_addr:	word= 0;
	global _nmprofiler_tail_addr: 	word= 0;
	global _nmprofiler_delay: 		word= -3;	
	global _nmprofiler_check: 		word= 0600DBEEFh; 
	global _nmprofiler_shared:		word= 0600DBEEFh; 
	global _nmprofiler_infosize:  	word= 0;
	global _nmprofiler_enabled:		word= -1;
	global _nmprofiler_depth:		word= -64;
	global _nmprofiler_size_current:word=0;
	global _nmprofiler_gr7			:word;
	
	global _nmprofiler_log_data_ptr	:word;
	global _nmprofiler_log_cnt1_ptr :word;
	global _nmprofiler_log_head_ptr :word;
	global _nmprofiler_log_tail_ptr	:word;

	
global _nmprofiler_set_depth:label;
<_nmprofiler_set_depth>
	ar5 = sp-2;
	gr7 = [--ar5];
	gr7 = -gr7;
	gr7 --;
	delayed return;
		[_nmprofiler_depth]=gr7;


	return;
global _nmprofiler_enable:label;
<_nmprofiler_enable>
	delayed return;
		with gr7=true; 			// gr7=-1
		[_nmprofiler_enabled]=gr7;

global _nmprofiler_disable:label;		
<_nmprofiler_disable>
	delayed return;
		with gr7=false; 		// gr7=0
		[_nmprofiler_enabled]=gr7;


goto_cop:label;
<goto_cop>
	goto 0;

skip_cop:label;
<skip_cop>
	skip 0;

call_cop:label;
<call_cop>
	call 0;

ret_cop:label;
<ret_cop>
	return;

iret_cop:label;
<iret_cop>
	ireturn;
	
nop_cop:label;
<nop_cop>
	nul;
	nul;
	
		
// ������� ��������� ��� ������� ���������� � gr0 �� �������������� � call goto skip return ireturn 
// ���������� gr7=0  ���� �����������
// ���������� gr7=-1 ���� �� �����������
//global nmprofiler_check_jump_cop:label;
<nmprofiler_check_jump_cop>
	push ar0,gr0;
	push ar1,gr1;
	push ar2,gr2;
	
	
	
	gr1 = 0111_0000_0011_0000_0000_0000_0000_0000b;// cop mask
	// ��������� �� call
	//gr2 = 0000_0000_0010_0000_0000_0000_0000_0000b;// call cop
	gr2 = [call_cop];
	gr7 = gr0 and gr1;
	gr7 = gr7 - gr2;
	if =0 delayed goto jump_detected  ; 
	
	// ��������� �� skip
	//gr2 = 0100_0000_0010_0000_0000_0000_0000_0000b;// skip cop
	gr2 = [skip_cop];
	gr7 = gr0 and gr1;
	gr7 = gr7 - gr2;
	if =0 delayed goto jump_detected  ; 
	
	gr1 = 0111_1111_1111_0000_0000_0000_0000_0000b;// cop mask
	// ��������� �� return
	//gr2 = 0000_1001_1111_0000_0000_0000_0000_0000b;// return cop
	gr2 = [ret_cop];
	gr7 = gr0 and gr1;
	gr7 = gr7 - gr2;
	if =0 delayed goto jump_detected  ; 
	
	// ��������� �� ireturn
	//gr2 = 0000_0011_1111_0000_0000_0000_0000_0000b;
	gr2 = [iret_cop];
	gr7 = gr0 and gr1;// ireturn cop
	gr7 = gr7 - gr2;
	if =0 goto jump_detected  ; 
	gr7 = true;
	
	 <jump_detected>
	 pop ar2,gr2;
	 pop ar1,gr1;
	 pop ar0,gr0;
	
return;


// ������� ��������� 4 ���� ������� ���������� �� ������ ar0 �� �������������� � call goto skip return ireturn 
// ���������� gr7=0  ���� �����������
// ���������� gr7=-1 ���� �� �����������

global nmprofiler_check_jump_cop4:label;
<nmprofiler_check_jump_cop4>
	push ar0,gr0;
	
	gr0=[ar0++];
	call nmprofiler_check_jump_cop;
	if =0 goto detected;
	
	gr0=[ar0++];
	call nmprofiler_check_jump_cop;
	if =0 goto detected;
	
	gr0=[ar0++];
	call nmprofiler_check_jump_cop;
	if =0 goto detected;
	
	gr0=[ar0++];
	call nmprofiler_check_jump_cop;
	if =0 goto detected;
	
		
	<detected>
	pop ar0,gr0;
return ;
	
//-----------------------------------------------	
// S_ProfilerData* nmprofiler_head();
// ������� ���������� ��������� �� ������ ��������� �������������� � ������� ������ 
global _nmprofiler_head:label;
<_nmprofiler_head>
	delayed return;
		nul;
		ar5 = [_nmprofiler_head_addr];

//------------------------------------------------		
// S_ProfilerData* nmprofiler_next(S_ProfilerData* p);
// ������� ���������� ��������� �� ��������� ��������� �������������� ����� ���������
global _nmprofiler_next:label;
<_nmprofiler_next>
	ar5 = ar7-2;
	push ar0,gr0;
	gr7 = [--ar5]; 	// head
	gr0 = [gr7];	// [head->next]
	gr7+= gr0;		// next head
		pop ar0,gr0 with gr0;
	if =0 delayed return;
		ar5=0;
	ar5 = gr7;
	return;
		
		
//-----------------------------------------------	 
global _nmprofiler_reset:label;
<_nmprofiler_reset>
	push ar0,gr0;
	push ar1,gr1;
	push ar2,gr2;
	push ar3,gr3;

	ar0 = [_nmprofiler_head_addr]  with gr0 = false;
	<next_profile>
		gr7 = [ar0]; 			// next=0
		//[ar0+1] = gr0; 		// funcaddr
		[ar0+2] = gr0; 			// calls=0
		[ar0+3] = gr0 with gr7;	// summary=0
	if <>0 delayed goto next_profile;
		ar0 = gr7;
		nul;

	pop ar3,gr3;
	pop ar2,gr2;
	pop ar1,gr1;
	pop ar0,gr0;
return;
	
//---------------------------------------------------	
// ������� ���-�� �������������  ������� 
global _nmprofiler_count:label;
<_nmprofiler_count>
	push ar0,gr0;
	gr0 = [_nmprofiler_head_addr] with gr7 = false;
	ar0 = gr0 with gr0;
	if =0 goto nmprofiler_count_finish; 
	<next_profile_count>
		gr0 = [ar0] with gr7++; 	// next=0
		ar0+= gr0 with gr0;
	if <>0 delayed goto next_profile_count;
		nul;
		nul;
	<nmprofiler_count_finish>
	pop ar0,gr0;
return;
	
	
//-------------------------------------------------	

global _nmprofiler_copy:label;
<_nmprofiler_copy>
	ar5 = ar7-2;
	push ar0,gr0;
	push ar4,gr4;
	push ar5,gr5;
	push ar6,gr6;
	

	ar0 = [_nmprofiler_head_addr];	// src head
	gr0 = [ar0];
	ar6 = [--ar5]		with gr0;	// dst
	if =0 delayed goto nmprofiler_copy_finish;
		gr5 = [_nmprofiler_infosize] ;	 
	<next_copy_profile>
		ar4     = ar6	with gr7=gr5;	//infosize
		[ar6++] = gr5	with gr7--;
		ar5 = ar0+1 	with gr7--;
		<copy_next_word>
		if <>0 delayed goto copy_next_word with gr7--;		
			gr6 = [ar5++];			// read
			[ar6++] = gr6;			// write
		gr0 = [ar0]; 				// next disp
		with gr0;
	if <>0 delayed goto next_copy_profile;
		ar0+= gr0;
		nul;
	[ar4] = gr0;
	<nmprofiler_copy_finish>
	pop ar6,gr6;
	pop ar5,gr5;
	pop ar4,gr4;
	pop ar0,gr0;
return;
	

		
// ������� ��������� �������� ��� � ������������� ������� �� goto �� ������� �������� . 
// ���������� ��� ����������� � �������-��������
// ��������� �������:
// ar0 = funcname;  		��� ������������� �������
// gr0 = timer_replaced		����� ����� ������� �������� 4 ���������� ������������� �������
// gr1 = master_enter;		����� ��������� ������� (����)
// �� ��������� �������� ar0,gr0,ar1,gr1,ar2,gr2
global inject_goto:label;
<inject_goto>
	// �������� 4 ������� �� ��������� ������������� ������� � ������-�������	
	//timer_replaced:label;
	call nmprofiler_check_jump_cop4; // ��������� ��� � 4 ����������� �������� ��� ���������
	if =0 delayed return ;			// ������ ������� ������������� ���������� - ������
		ar1 = gr0;
		nul;
		nul;
	//---------- �������� 4 ������� �� ������������� ������� 
	//ar1 = timer_replaced;	// ����� ����� ������� �������� 4 ���������� ������������� �������
	ar2,gr2 = [ar0++];		// ������ ��� ������ �������� ���������� �� ������������� �������
	[ar1++]=ar2,gr2;		// �������� �� � ��������� 
	ar1+=2;					// ���������� ������� "delayed goto funcname+4" � ����� ���������
	ar2,gr2 = [ar0++];		// ������ �������� ��� �������� ���������� �� ������������� �������
	[ar1++]=ar2,gr2;		// �������� �� � ���������
	
	//---------- ����������� 4 ������� � ������ ������������� ������� � ��������� �� ��������
	//gr2 = 0d0100000h;		// nul cop
	gr2 = [nop_cop];		// nul cop
	ar2 = gr2;
	[--ar0] = ar2,gr2;		// nul,nul 
	//ar2 = 0c8270000h;		// goto cop
	ar2 = [goto_cop];
	delayed return;	
		gr2 = gr1;			// master_enter;	 ����� ��������� ������� (����)
		[--ar0] = ar2,gr2;	// goto <funcname>
		nul;
	
global _nmprofiler_info_size:label;
<_nmprofiler_info_size>
	delayed return;
		nul;
		gr7 = [_nmprofiler_infosize];
			
		
	

			
end ".text_nmprofiler";



	
