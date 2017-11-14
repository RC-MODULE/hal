//----------------------------------------------------------------
//
// cache.asm,              1997-2016 (c) RC Module Ltd.
// nm6407, clock function realisation file
//
//----------------------------------------------------------------

const CSR    =4000_0C00h;	//CSR	ЧТ/ЗП	Регистр управления кэш-памятью команд
const CSR_SET=4000_0C02h;	//CSR_SET	ЧТ/ЗП	Побитовая установка регистра CSR
const CSR_CLR=4000_0C04h;	//CSR_CLR	ЧТ/ЗП	Побитовый сброс регистра CSR
const PDA    =4000_0C08h;  //PDA	ЧТ/ЗП	Регистр адреса гиперстраницы
const CEN    =1;

begin text

global _halInstrCacheEnable : label;
<_halInstrCacheEnable>
	gr7 = CEN;
    delayed return;
		vnul;
        [CSR_SET]=gr7;

global _halInstrCacheDisable : label;	
<_halInstrCacheDisable>
	gr7 = CEN;
    delayed return;
		vnul;
        [CSR_CLR]=gr7;
	
end text;