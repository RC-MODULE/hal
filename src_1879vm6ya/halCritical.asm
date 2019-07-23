
import from "critical";
begin ".text_hal"

DECLARE_CS_CONST();

global _halEnterCritical:label;
<_halEnterCritical>
	ar5 = ar7-2;
	push ar5,gr5;
	gr7 = [--ar5];
	HAL_ENTER_CRITICAL_SECTION(gr7);
	pop ar5,gr5;
return;

global _halExitCritical:label;
<_halExitCritical>
	ar5 = ar7-2;
	push ar5,gr5;
	gr7 = [--ar5];
	HAL_EXIT_CRITICAL_SECTION(gr7);
	pop ar5,gr5;
	return;
	

end ".text_hal";