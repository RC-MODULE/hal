//----------------------------------------------------------------
//
// cache.asm,              1997-2016 (c) RC Module Ltd.
// nm6407, clock function realisation file
//
//----------------------------------------------------------------


begin text

global _halInstrCacheEnable : label;
<_halInstrCacheEnable>
    delayed return;
        vnul;
        pcr_6405 set 0000_0080h;



global _halInstrCacheDisable : label;	
<_halInstrCacheDisable>
    delayed return;
		vnul;
        pcr_6405 clear 0000_0080h;
	
end text;




