
import from "critical";
begin ".text_hal"
extern _halCoreID             : word;

DECLARE_CS_CONST();

global _halEnterCriticalSectionCore:label;
<_halEnterCriticalSectionCore>
	ar5 = ar7-2;
	push ar5,gr5;
	gr7 = [--ar5];
	HAL_ENTER_CRITICAL_SECTION(gr7);
	pop ar5,gr5;
return;

global _halExitCriticalSectionCore:label;
<_halExitCriticalSectionCore>
	ar5 = ar7-2;
	push ar5,gr5;
	gr7 = [--ar5];
	HAL_EXIT_CRITICAL_SECTION(gr7);
	pop ar5,gr5;
	return;
	
global _halEnterCriticalSection:label;
<_halEnterCriticalSection>
	ar5 = ar7-2;
	gr7 = [_halCoreID];
	HAL_ENTER_CRITICAL_SECTION(gr7);
	pop ar5,gr5;
return;

global _halExitCriticalSection:label;
<_halExitCriticalSection>
	ar5 = ar7-2;
	push ar5,gr5;
	gr7 = [_halCoreID];
	HAL_EXIT_CRITICAL_SECTION(gr7);
	pop ar5,gr5;
	return;


end ".text_hal";