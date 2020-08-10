//------------------------------------------------------------------------
//
//  $Workfile:: vStat.h  $
//
//  Векторно-матричная библиотека
//
//  Copyright (c) RC Module Inc.
//
//  $Revision: 1.1 $      $Date: 2004/11/22 13:50:02 $
//
//! \if file_doc
//!
//! \file   vMinMax.h
//! \author Сергей Мушкаев
//! \brief  Определение функций поиска минимума и максимума
//!
//! \endif
//!
//------------------------------------------------------------------------

 
#ifndef _HALCRC_H_INCLUDED_
#define _HALCRC_H_INCLUDED_

#ifdef __cplusplus
		extern "C" {
#endif

    /**
	\internal
    \defgroup halCrc_ halCrc_
    \ingroup vStat
		\brief 
		    \ru Подсчет циклического кода.
		    \en Cyclic code calculation.

		\~
	  \~
		\param pSrcVec
			\ru Входной вектор.
			\en Input vec.
		\~
		\param nSize
			\ru Размер входного вектора.
			\en Input vec size.

		\~
		\retval nCrc
			\ru Циклический код.
			\en Cyclic code.

		\~
    \par
    \xmlonly
				<testperf>
          <param name="pSrcVec"> im0 im1 		</param>
          <param name="crcAccumulator"> im2 		</param>
          <param name="nSize"> 2048 	</param>
				</testperf>
				<testperf>
          <param name="pSrcVec"> im0 		</param>
          <param name="crcAccumulator"> im2 		</param>
          <param name="nSize"> 8 128 1024 2048 	</param>
				</testperf>

    \endxmlonly
    */
    //! \{
	unsigned halCrc_32u		(const unsigned int* pSrcVec, int nSize);
	unsigned halCrcAcc_32u	(const unsigned int* pSrcVec, int nSize, unsigned int* crcAccumulator);
	
	
   
__INLINE__ unsigned halCrc_64s(nm64s* pSrcVec, int nSize) {  return halCrc_32u((unsigned*)pSrcVec, nSize<<1); }
__INLINE__ unsigned halCrc_32s(nm32s* pSrcVec, int nSize) {  return halCrc_32u((unsigned*)pSrcVec, nSize   ); }           
__INLINE__ unsigned halCrc_16s(nm16s* pSrcVec, int nSize) {  return halCrc_32u((unsigned*)pSrcVec, nSize>>1); }
__INLINE__ unsigned halCrc_8s (nm8s*  pSrcVec, int nSize) {  return halCrc_32u((unsigned*)pSrcVec, nSize>>2); }
__INLINE__ unsigned halCrc_4s (nm4s*  pSrcVec, int nSize) {  return halCrc_32u((unsigned*)pSrcVec, nSize>>3); }
__INLINE__ unsigned halCrc_64u(nm64u* pSrcVec, int nSize) {  return halCrc_32u((unsigned*)pSrcVec, nSize<<1); }
                                                                
//__INLINE__ unsigned  halCrc_32f(nm32f* pSrcVec, int numBitsToClear, int nSize) {  return halCrcMask_32u((unsigned*)pSrcVec,(-1<<numBitsToClear),nSize>>1); }
																
__INLINE__ unsigned halCrc_16u(nm16u* pSrcVec, int nSize) {  return halCrc_32u((unsigned*)pSrcVec, nSize>>1); }
__INLINE__ unsigned halCrc_8u (nm8u*  pSrcVec, int nSize) {  return halCrc_32u((unsigned*)pSrcVec, nSize>>2); }
           unsigned halCrc_32f(float* pSrcVec, int nSize);

__INLINE__ unsigned halCrcAcc_64s(nm64s* pSrcVec, int nSize, unsigned int* crcAccumulator) {  return halCrcAcc_32u((unsigned*)pSrcVec, nSize<<1, crcAccumulator); }
__INLINE__ unsigned halCrcAcc_32s(nm32s* pSrcVec, int nSize, unsigned int* crcAccumulator) {  return halCrcAcc_32u((unsigned*)pSrcVec, nSize,    crcAccumulator); }           
__INLINE__ unsigned halCrcAcc_16s(nm16s* pSrcVec, int nSize, unsigned int* crcAccumulator) {  return halCrcAcc_32u((unsigned*)pSrcVec, nSize>>1, crcAccumulator); }
__INLINE__ unsigned halCrcAcc_8s (nm8s*  pSrcVec, int nSize, unsigned int* crcAccumulator) {  return halCrcAcc_32u((unsigned*)pSrcVec, nSize>>2, crcAccumulator); }
__INLINE__ unsigned halCrcAcc_64u(nm64u* pSrcVec, int nSize, unsigned int* crcAccumulator) {  return halCrcAcc_32u((unsigned*)pSrcVec, nSize<<1, crcAccumulator); }


__INLINE__ unsigned halCrcAcc_16u(nm16u* pSrcVec, int nSize, unsigned int* crcAccumulator) {  return halCrcAcc_32u((unsigned*)pSrcVec, nSize>>1, crcAccumulator); }
__INLINE__ unsigned halCrcAcc_8u (nm8u*  pSrcVec, int nSize, unsigned int* crcAccumulator) {  return halCrcAcc_32u((unsigned*)pSrcVec, nSize>>2, crcAccumulator); }
           unsigned halCrcAcc_32f(float* pSrcVec, int nSize, unsigned *crcAccumulator);
	
    //! \}
	


//*****************************************************************************
	/**
	\internal
    \defgroup halSadV16_ halSadV16_
    \ingroup vStat
	\brief 
		\ru Сумма абсолютных разностей двух массивов (SAD) размером в 16 элементов 
		\en Sum of absolute differences (SAD) between two array with size of 16 numbers 

		\~
	\param pSrc1
		\ru Массив из 16 элементов. 
        \en array of 16 numbers
		\~
	\param pSrc2
		\ru Массив из 16 элементов. 
        \en array of 16 numbers
		\~
	\return int		
		\ru SAD. 
        \en SAD

		\~
  \par
	\xmlonly
	 <testperf> 
			<param name="pSrc1"> im0 im1 </param>
			<param name="pSrc2"> im0 im1 </param>
			<size> 1 </size>
	 </testperf>
	\endxmlonly
    */

    //! \{
int halSadV16_16s(nm16s* pSrc1,nm16s* pSrc2);
int halSadV16_8s(nm8s*  pSrc1,nm8s*  pSrc2);
    //! \}

	
#ifdef __cplusplus
		};
#endif

#endif // _VSTAT_H_INCLUDED_