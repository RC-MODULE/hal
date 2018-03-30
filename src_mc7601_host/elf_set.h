//////////////////////////////////////////////////////////////////////////
//                                                                      //
// elf_set.h -                                                          //
//                                                                      //
//      Settings for libelf and liblib libraries                        //
//                                                                      //
//                                                                      //
// Copyright (c) 2005 RC Module                                         //
//  If this code works, it was written by Alex Ruzavin                  //
//  If not, I don't know who wrote it.                                  //
//                                                                      //
// $Revision:: 2     $      $Date:: 1/14/08 4:46p    $                  //
//                                                                      //
//////////////////////////////////////////////////////////////////////////

#ifndef __ELF_SET
#define __ELF_SET

    // Здесь указывается порядок байтов в слове на данной машине.
    // intel - little-endian.
    // _X86 and/or __i386__ - устанавливается в gcc.
    // _M_IX86 - устанавливается в MSVC.
#if defined(_X86) || defined(__i386__) || defined(_M_IX86) || defined(_M_AMD64)
    // !!! native Intel coding !!!
    // little-endian
static const unsigned char _HOST_ENCODING = ELFDATA2LSB;

#else  // defined(_X86) || defined(__i386__) || defined(_M_IX86) || defined(_M_AMD64)

    // E90 - big endian
    // _E90 - defined in makefile
#if defined(_E90)
static const unsigned char _HOST_ENCODING = ELFDATA2MSB;

#else
#error "Needs change data encoding according with native host machine data encoding"
#endif  // defined(_E90)
#endif  // defined(_X86) || defined(__i386__) || defined(_M_IX86) || defined(_M_AMD64)

    // target machine data encoding
#if defined (NM6403) || defined (NM6404) || defined (DSM) || defined (NM6405)
static const unsigned char _TARG_ENCODING = ELFDATA2LSB;

#else
#error "Needs change data encoding according with target machine data encoding"
#endif  // defined (NM6403) || defined (NM6404) || defined (DSM) || defined (NM6405)


#endif  // __ELF_SET
