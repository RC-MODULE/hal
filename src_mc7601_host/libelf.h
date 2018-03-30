//////////////////////////////////////////////////////////////////////////
//                                                                      //
//             ������� ����������� ����������� ���������������          //
//                                                                      //
//     ���������� ������� � ��������� � ����������� ����� ������� ELF   //
//                                                                      //
//                      ������������ ������ libelf.h                    //
//                                                                      //
//     ������������ ���� ����������� ����, ������, � ������ �������     //
//     ������������ ��� ������ � ������� ������� ELF                    //
//                                                                      //
//                                                                      //
// Copyright (c) 1997-2008 RC Module                                    //
//  If this code works, it was written by Alex Ruzavin                  //
//  If not, I don't know who wrote it.                                  //
//                                                                      //
// $Revision:: 8     $      $Date:: 2.12.11 13:35    $                  //
//                                                                      //
//////////////////////////////////////////////////////////////////////////

#ifndef __LIBELF_H
#define __LIBELF_H

#include <iostream>
#include <fstream>
#include <sys/types.h>

#ifndef MY_MSC_VER
#define MY_MSC_VER 1400
#endif

#if _MSC_VER <= MY_MSC_VER && defined(_MSC_VER)
// ��������� Microsoft Visual C++ �������� �� ���� ����������
#pragma warning( disable : 4290 )
#pragma warning( disable : 4996 )  // This function or variable may be unsafe.(VS8)
#endif

    // ���� ������������ � ����������

typedef unsigned long       Elf32_Addr;     // ����� �� ������� ������
typedef unsigned short int  Elf32_Half;     // �������� ����� �� ������� ������
typedef unsigned long       Elf32_Off;      // �������� � �����
typedef long                Elf32_Sword;    // ����� �� ������ �� ������� ������
typedef unsigned long       Elf32_Word;     // ����� ��� ����� �� ������� ������

    //  �������� � ������ ������������� �����

const int EI_MAG0   = 0;
const int EI_MAG1   = 1;
const int EI_MAG2   = 2;
const int EI_MAG3   = 3;
const int EI_CLASS  = 4;
const int EI_DATA   = 5;
const int EI_VERSION= 6;
const int EI_PAD    = 7;
const int EI_NIDENT = 16;

    // ���������� ������ �������������

    // ��������� ��� ����������� ���� Elf
const unsigned char ELFMAG0 = '\x7f';
const unsigned char ELFMAG1 = 'E';
const unsigned char ELFMAG2 = 'L';
const unsigned char ELFMAG3 = 'F';

    // ���������� ��������� ������ Elf
const unsigned char LIBMAG0 = '\x7f';
const unsigned char LIBMAG1 = 'L';
const unsigned char LIBMAG2 = 'I';
const unsigned char LIBMAG3 = 'B';


    //  ��������� ������ �����

const unsigned char ELFCLASSNONE= 0;
const unsigned char ELFCLASS32  = 1;
const unsigned char ELFCLASS64  = 2;
const unsigned char ELFCLASSNUM = 3;

    // ��������� ����������� ������

const unsigned char ELFDATANONE = 0;
const unsigned char ELFDATA2LSB = 1;
const unsigned char ELFDATA2MSB = 2;
const unsigned char ELFDATANUM  = 3;

    //  ��������� ����� ������

const Elf32_Half    ET_NONE = 0;
const Elf32_Half    ET_REL  = 1;

const Elf32_Half    ET_EXEC = 2;
                    // � ����� ������, ���������� ����������� ����� ���������

const Elf32_Half    ET_DYN      = 3;
const Elf32_Half    ET_CORE     = 4;
const Elf32_Half    ET_NUM      = 5;
const Elf32_Half    ET_LOPROC   = 0xff00;

const Elf32_Half    ET_EXEREL   = 0xff00;
                    // ����������� ������������ ����
                    // ���������� ��� ���������������

const Elf32_Half    ET_HIPROC= 0xffff;

    // ��������� ����� �����

const Elf32_Half  EM_NONE           = 0;
const Elf32_Half  EM_M32            = 1;    // AT&T WE 32100
const Elf32_Half  EM_SPARC          = 2;    // SPARC
const Elf32_Half  EM_386            = 3;    // Intel Architecture
const Elf32_Half  EM_68K            = 4;    // Motorola 68000
const Elf32_Half  EM_88K            = 5;    // Motorola 88000
const Elf32_Half  EM_860            = 7;    // Intel 80860
const Elf32_Half  EM_MIPS           = 8;    // MIPS RS3000 Big-Endian
const Elf32_Half  EM_MIPS_RS4_BE    = 10;   // MIPS RS4000 Big-Endian
//                RESERVED          = 11-16;// Reserved for future use
const Elf32_Half  EM_NEUROCHIP      = 17;   // NM6403
const Elf32_Half  EM_NEUROCORE01    = 18;   // NMC3
const Elf32_Half  EM_NM6405	        = 64050;// NM6405
const Elf32_Half  EM_SOC            = 64051;// NMC3 soc
const Elf32_Half  EM_CURRENTPROC    = EM_NEUROCHIP;

    // ��������� ������ Elf

const unsigned int EV_NONE      = 0;    //
const unsigned int EV_CURRENT   = 1;    // ������� ������ Elf
const unsigned int EV_NUM       = 2;    //

    //  ��������� ����������� �������� ������

const Elf32_Half    SHN_UNDEF       = 0;
const Elf32_Half    SHN_LORESERVE   = 0xff00;
const Elf32_Half    SHN_LOPROC      = 0xff00;
const Elf32_Half    SHN_HIPROC      = 0xff1f;
const Elf32_Half    SHN_ABS         = 0xfff1;
const Elf32_Half    SHN_COMMON      = 0xfff2;
const Elf32_Half    SHN_HIRESERVE   = 0xffff;

    //  ��������� ����� ������

const Elf32_Word    SHT_NULL    = 0;
const Elf32_Word    SHT_PROGBITS= 1;
const Elf32_Word    SHT_SYMTAB  = 2;
const Elf32_Word    SHT_STRTAB  = 3;
const Elf32_Word    SHT_RELA    = 4;
const Elf32_Word    SHT_HASH    = 5;
const Elf32_Word    SHT_DYNAMIC = 6;
const Elf32_Word    SHT_NOTE    = 7;
const Elf32_Word    SHT_NOBITS  = 8;
const Elf32_Word    SHT_REL     = 9;
const Elf32_Word    SHT_SHLIB   = 10;
const Elf32_Word    SHT_DYNSYM  = 11;
const Elf32_Word    SHT_NUM     = 12;
const Elf32_Word    SHT_LOPROC  = 0x70000000L;
const Elf32_Word    SHT_HIPROC  = 0x7fffffffL;
const Elf32_Word    SHT_LOUSER  = 0x80000000L;
const Elf32_Word    SHT_HIUSER  = 0xffffffffL;
//	custom ������ ��� �������� ���������� � ������ ������
const Elf32_Word    SHT_BANKMAP = 0xBA6405L;
//	��� �������, ����� ������� ������ �������
const unsigned int BankInfoMarker = 0x21212121;	


    // ����� ��������� ������

const Elf32_Word    SHF_WRITE       = 0x1;
const Elf32_Word    SHF_ALLOC       = 0x2;
const Elf32_Word    SHF_EXECINSTR   = 0x4;
const Elf32_Word    SHF_MASKPROC    = 0xf0000000L;

    //  ��������� ����� ����������� ���������

const Elf32_Word    PT_NULL     = 0;
const Elf32_Word    PT_LOAD     = 1;
const Elf32_Word    PT_DYNAMIC  = 2;
const Elf32_Word    PT_INTERP   = 3;
const Elf32_Word    PT_NOTE     = 4;
const Elf32_Word    PT_SHLIB    = 5;
const Elf32_Word    PT_PHDR     = 6;
const Elf32_Word    PT_NUM      = 7;
const Elf32_Word    PT_LOPROC   = 0x70000000L;
const Elf32_Word    PT_HIPROC   = 0x7fffffffL;

struct Elf32_Phdr {
    public:
        Elf32_Word  p_type;		//	��� ��������, PT_...
        Elf32_Off   p_offset;	//	�������� � �����, � ������
        Elf32_Addr  p_vaddr;	//	����������� ����� � ������ �� ����
        Elf32_Addr  p_paddr;	//	���������� ����� � ������ �� ����
        Elf32_Word  p_filesz;	//	������ � ������ � �����
        Elf32_Word  p_memsz;	//	������ � ������ � ������ �� ����
        Elf32_Word  p_flags;	//	PF_... �����
        Elf32_Word  p_align;	//	������������ p_vaddr � p_offset

        Elf32_Phdr() :  p_type(PT_NULL),
                        p_offset(0),
                        p_vaddr(0),
                        p_paddr(0),
                        p_filesz(0),
                        p_memsz(0),
                        p_flags(0),
                        p_align(0)
        { }
};

//	������� ������� ���������� ������

struct Elf32_Shdr {
    public:
        Elf32_Word  sh_name;	//	������ ����� ������
        Elf32_Word  sh_type;	//	��� ������, SHT_...
        Elf32_Word  sh_flags;	//	SHF_... �����
        Elf32_Addr  sh_addr;	//	����� ������������ � ������, ��� �� �����������-0 
        Elf32_Off   sh_offset;	//	����� � ����-�����
        Elf32_Word  sh_size;	//	������ � ����-����� � ������
        Elf32_Word  sh_link;	//	������ �� ��������� ������, ����� ������� �� ����
        Elf32_Word  sh_info;	//	���. ����, ����� ������� �� ����
        Elf32_Word  sh_addralign;//	������������ sh_addr
        Elf32_Word  sh_entsize;	//	��� ��������� ������ - ������ ������ � ������

        Elf32_Shdr() :  sh_name(0),
                        sh_type(SHT_NULL),
                        sh_flags(0),
                        sh_addr(0),
                        sh_offset(0),
                        sh_size(0),
                        sh_link(SHN_UNDEF),
                        sh_info(0),
                        sh_addralign(0),
                        sh_entsize(0)
      { }

};

//	��������� ELF-�����

struct Elf32_Ehdr {
    public:
        unsigned char   e_ident[EI_NIDENT];	//	���������, ���������, ��� ���� ���� ����
        Elf32_Half      e_type;				//	���������, obj ���, exe ��� dll (������ �����)
        Elf32_Half      e_machine;			//	������� ���������
        Elf32_Word      e_version;			//	1 ��� ��������� �������� (������)
        Elf32_Addr      e_entry;			//	����� �����, ����� �������� ����������
        Elf32_Off       e_phoff;			//	�������� ������� ��������� (obj-�������������) ���������
        Elf32_Off       e_shoff;			//	�������� ������� ������ (exe-�������������) ���������
        Elf32_Word      e_flags;			//	����������-����������� �����
        Elf32_Half      e_ehsize;			//	������ ������ ��������� (� �����)
        Elf32_Half      e_phentsize;		//	������ ������ � ������� ������
        Elf32_Half      e_phnum;			//	���������� ������� � ������� ������
        Elf32_Half      e_shentsize;		//	������ ������ � ������� ���������
        Elf32_Half      e_shnum;			//	���������� ������� � ������� ���������
        Elf32_Half      e_shstrndx;			//	������ ������, �������� ����� ������

        Elf32_Ehdr();
		Elf32_Ehdr(Elf32_Half TargetProc);
};

    // ��������� ��� ������ ������������� ������ Elf

const int EI_LIB_MAG0   = 0;
const int EI_LIB_MAG1   = 1;
const int EI_LIB_MAG2   = 2;
const int EI_LIB_MAG3   = 3;
const int EI_LIB_CLASS  = 4;
const int EI_LIB_DATA   = 5;
const int EI_LIB_VERSION= 6;
const int EI_LIB_PAD    = 7;
const int EI_LIB_NIDENT = 16;

    //
    // ��������� ��������� ����������
    //

struct Elf32_Lhdr {
    public:
        unsigned char   l_ident[EI_LIB_NIDENT];
        Elf32_Off       l_lhoff;        // �������� ������� ����������
        Elf32_Half      l_lhnum;        // ����� ��������� ����������
        Elf32_Half      l_lhentsize;    // ������ ��������� � �������
        Elf32_Half      l_lhsymndx;     // ������ ������������ ��������
                                        // ����������� ������� ����������
        Elf32_Half      l_lhstrndx;     // ������ ������������ ��������
                                        // ����������� �����

        Elf32_Lhdr()
            : l_lhoff(0),
            l_lhnum(0),
            l_lhentsize(0),
            l_lhsymndx(0),
            l_lhstrndx(0)
        {
            l_ident[EI_LIB_MAG0] = LIBMAG0;
            l_ident[EI_LIB_MAG1] = LIBMAG1;
            l_ident[EI_LIB_MAG2] = LIBMAG2;
            l_ident[EI_LIB_MAG3] = LIBMAG3;
            l_ident[EI_LIB_CLASS] = ELFCLASS32;
            l_ident[EI_LIB_DATA] = ELFDATA2LSB;
            l_ident[EI_LIB_VERSION] = EV_CURRENT;

            for( int i = EI_LIB_PAD; i < EI_LIB_NIDENT; i++ )
                l_ident[i] = '\0';
        }
};

    //
    // ��������� ��������� �������� ����������
    //

struct Elf32_Libhdr {
    private:
        friend class ELF_FILE;

        Elf32_Word      lib_name_offset;    // �������� ����� ��������
                                            // � ������� �����
    public:
        char        *   lib_name;   // ��� ��������
        time_t          lib_date;   // ���� �������� ����� ��������
        Elf32_Off       lib_offset; // �������� � �������� � ���������
        Elf32_Word      lib_size;   // ������ �������� ����������
        Elf32_Word      lib_mode;   // ��������� �����

        Elf32_Libhdr()
            : lib_name(0),
            lib_date(0),
            lib_offset(0),
            lib_size(0),
            lib_mode(0)
        { }
};

const Elf32_Word    LHF_NONE    = 0;
const Elf32_Word    LHF_SPECIAL = 0x1;  // ���� ������������ ��������
                                        // ����������
const Elf32_Word    LHF_MASKPROC  = 0xffff0000L;
                                        // ����� ��� ����������������
                                        // ������

    //
    // ��������� �������� ������� ��������
    //

struct Elf32_LibSym {
    private:
        friend class ELF_FILE;

        Elf32_Word  ls_name_offset; // �������� ����� ������� � �������
    public:
        char    *   ls_name;    // ��� �������
        Elf32_Half  ls_index;   // ����� ��������, �����������
                                // ����������� ������� �������
        Elf32_Sword ls_bind;    // ���������� ������� ��������� STB_???
        Elf32_Word  ls_hash;    // �������� ���-������� Elf_Hash ���
                                // ����� �������

        Elf32_LibSym()
            : ls_name(0),
            ls_index(0),
            ls_hash(0)
        {  }
};

struct Elf32_Sym {
    public:
        Elf32_Word      st_name;
        Elf32_Addr      st_value;
        Elf32_Word      st_size;
        unsigned char   st_info;
        unsigned char   st_other;
        Elf32_Half      st_shndx;

        Elf32_Sym() :   st_name(0),
                        st_value(0),
                        st_size(0),
                        st_info(0),
                        st_other(0),
                        st_shndx(SHN_UNDEF)
      { }
};

    //  ����������� ������� ��� ����������� ���� � ���������� � ����������
    // ��� ������� � ���� st_info ��������� Elf32_Sym

inline unsigned char ELF32_ST_BIND(unsigned char i)
{
    return (unsigned char)(i >> 4);
}

inline unsigned char ELF32_ST_TYPE(unsigned char i)
{
    return (unsigned char)((i) & char(0xf));
}

inline unsigned char ELF32_ST_INFO(unsigned char b,unsigned char t)
{
    return (unsigned char)(((b)<<char(4)) + ((t)&char(0xf)));
}

    // ��������� ����� ���������� ��������

const unsigned char STB_LOCAL   = 0;
const unsigned char STB_GLOBAL  = 1;
const unsigned char STB_WEAK    = 2;
const unsigned char STB_NUM     = 3;
const unsigned char STB_LOPROC  = 13;
const unsigned char STB_HIPROC  = 15;

    // ��������� ����� ��������

const unsigned char STT_NOTYPE  = 0;
const unsigned char STT_OBJECT  = 1;
const unsigned char STT_FUNC    = 2;
const unsigned char STT_SECTION = 3;
const unsigned char STT_FILE    = 4;
const unsigned char STT_NUM     = 5;
const unsigned char STT_LOPROC  = 13;

const unsigned char STT_HIPROC  = 15;

    //  ��� �������� ������� �����������

struct Elf32_Rel {
    public:
        Elf32_Addr  r_offset;
        Elf32_Word  r_info;

        Elf32_Rel() : r_offset(0), r_info(0) { }
};

struct Elf32_Rela {
    public:
        Elf32_Addr  r_offset;
        Elf32_Word  r_info;
        Elf32_Sword r_addend;

        Elf32_Rela() : r_offset(0), r_info(0),r_addend(0) { }
};

    // ����������� ������� ��� ����������� ���������� � ���� ����������� �
    // ������� ������� � ���� r_info �������� Elf32_Rel � Elf32_Rela

inline Elf32_Word ELF32_R_SYM(Elf32_Word i) { return Elf32_Half((i)>>8); }

inline unsigned char ELF32_R_TYPE(Elf32_Word i) { return (unsigned char)(i); }

inline Elf32_Word ELF32_R_INFO(Elf32_Word b,unsigned char t)
{
    return ( (Elf32_Word(b)<<8) + Elf32_Word(t) );
}

    // ��������� ����� �����������
    // ��� ��������� ���������� ��� ���������������

const unsigned char R_NC_NONE   = 0;  // ���������� �����������
const unsigned char R_NC_ADDR   = 1;  // ����������� ��� ����������� ������
const unsigned char R_NC_PC     = 2;  // ����������� ��� �������������� ������
const unsigned char R_NC_BYTEPTR= 3;
//  ��������, ��������� ����� ���� ���������� ������� ar = ar-Label, ar-=Label
//  � ��� ��� ����� (� �������� ������ ����������� � ar+=Label)
const unsigned char R_NC_MINUSADDR= 4;// ��������� ����� ���������� �����
                           // ����������� ��� �������� � ������ � ������ �����


    // ��������� ���������� �������� � ������ ������������

struct Elf32_Nhdr {
    public:
        Elf32_Word  n_namesz;       // ����� ����� �����������
        Elf32_Word  n_descsz;       // ����� ���������� �����������
        Elf32_Word  n_type;         // ��� �����������
};

    // ��������� ����� ������

enum ELF_KIND { ELF_K_NONE = 0,ELF_K_ELF = 1,ELF_K_LIB = 2,ELF_K_NUM };

    // ��������� ������ ����������
enum ELF_CMD {  ELF_C_NULL, ELF_C_READ, ELF_C_RDWR, ELF_C_WRITE, ELF_C_NUM };


    //------------------------------------------------------//
    //                                                      //
    //         ������ ������, ����������� �����������       //
    //                                                      //
    //------------------------------------------------------//

   // ������� ����� ��� ������� ������, ����������� �����������
class ELF_ERROR {
    protected:
        const char  *   errormsg;
        const char  *   msg1;
        const char  *   msg2;
        const char  *   msg3;
    public:
        ELF_ERROR (const char *s, const char *s1 = 0, const char *s2 = 0,
                  const char *s3 = 0) : errormsg(s),msg1(s1),msg2(s2),msg3(s3)
        { }

        friend std::ostream& operator<< (std::ostream& s,ELF_ERROR& e);
};

    // ������ ��������� � �����
class ELF_ERROR_FILE : public ELF_ERROR {
    public:
        ELF_ERROR_FILE (const char *s, const char *s1 = 0, const char *s2 = 0,
            const char *s3 = 0) : ELF_ERROR(s,s1,s2,s3)
        { }
};

    // ������������ ������ ( ����� ��� � ����� ) � �����
class ELF_ERROR_CLASS : public ELF_ERROR {
    public:
        ELF_ERROR_CLASS (const char *s, const char *s1 = 0, const char *s2 = 0,
            const char *s3 = 0) : ELF_ERROR(s,s1,s2,s3)
        { }
};

    // ������ � ��������� ELF �����
class ELF_ERROR_EHDR : public ELF_ERROR {
    public:
        ELF_ERROR_EHDR(const char *s, const char *s1 = 0, const char *s2 = 0,
            const char *s3 = 0) : ELF_ERROR(s,s1,s2,s3)
        { }
};

    // ������ � ��������� ���������� ELF ������
class ELF_ERROR_LHDR : public ELF_ERROR {
    public:
        ELF_ERROR_LHDR(const char *s, const char *s1 = 0, const char *s2 = 0,
            const char *s3 = 0) : ELF_ERROR(s,s1,s2,s3)
        { }
};

    // ������ ���������� ����� � ����������: ������������� ��� �������
    // ������� GetAmbiguityName ������ ��� �������������� �������
class ELF_ERROR_AMBIGUITY : public ELF_ERROR {
    protected:
        char *   eeaAmName;      // ��� �������
    public:
        ELF_ERROR_AMBIGUITY(
            const char *s,
            const char *s1 = 0,
            const char *s2 = 0,
            char *AmName = 0);

            // ����������� ����������� � �������� ����������� �������
            // ���������� ��� ����������� ������ - ����� �������.
        ELF_ERROR_AMBIGUITY( const ELF_ERROR_AMBIGUITY& ErrAm );
        ELF_ERROR_AMBIGUITY& operator=( const ELF_ERROR_AMBIGUITY& ErrAm );

        ~ELF_ERROR_AMBIGUITY()
        {
            if ( eeaAmName ) {
                delete[] eeaAmName;
                eeaAmName = NULL;
            }
        }

        const char * GetAmbiguityName() const
        {
            return eeaAmName;
        }
};

    // �� ���������� ����� ���� ������� �����
class ELF_ERROR_EXIST : public ELF_ERROR {
    public:
        ELF_ERROR_EXIST(const char *s, const char *s1 = 0, const char *s2 = 0,
            const char *s3 = 0) : ELF_ERROR(s,s1,s2,s3)
        { }
};

    // ������������ ����� �����-���� ���������
class ELF_ERROR_TYPE : public ELF_ERROR {
    public:
        ELF_ERROR_TYPE(const char *s, const char *s1 = 0, const char *s2 = 0,
            const char *s3 = 0) : ELF_ERROR(s,s1,s2,s3)
        { }
};

    // ������������ �������� �����-���� ��������� �����
class ELF_ERROR_SIZE : public ELF_ERROR {
    public:
        ELF_ERROR_SIZE(const char *s, const char *s1 = 0, const char *s2 = 0,
            const char *s3 = 0) : ELF_ERROR(s,s1,s2,s3)
        { }
};

    // ����������� �������� ��� ������� �����
class ELF_ERROR_UNKNOWN : public ELF_ERROR {
    public:
        ELF_ERROR_UNKNOWN(const char *s,const char *s1 = 0,const char *s2 = 0,
            const char *s3 = 0) : ELF_ERROR(s,s1,s2,s3)
        { }
};

    // ������������ ��������� ������
class ELF_ERROR_OVERFLOW : public ELF_ERROR {
    public:
        ELF_ERROR_OVERFLOW(const char *s,const char *s1 = 0,const char *s2 = 0,
            const char *s3 = 0) : ELF_ERROR(s,s1,s2,s3)
        { }
};

    // ��������� ������ ����������
class ELF_ERROR_INTERNAL : public ELF_ERROR {
    public:
        ELF_ERROR_INTERNAL(const char *s,const char *s1 = 0,const char *s2 = 0,
            const char *s3 = 0) : ELF_ERROR(s,s1,s2,s3)
        { }
};

    //------------------------------------------------------//
    //                                                      //
    //      ������ ��� ������������� ����� ����������       //
    //                                                      //
    //------------------------------------------------------//

enum Elf_Type {
    ELF32_T_NONE,
    ELF32_T_ADDR,
    ELF32_T_OFF,
    ELF32_T_BYTE,
    ELF32_T_HALF,
    ELF32_T_WORD,
    ELF32_T_SWORD,
    ELF32_T_EHDR,
    ELF32_T_PHDR,
    ELF32_T_SHDR,
    ELF32_T_SYM,
    ELF32_T_REL,
    ELF32_T_RELA,
    ELF32_T_LHDR,
    ELF32_T_LIBHDR_F,
    ELF32_T_LIBSYM_F
};

    // ������� ����� ������ � �����.
const int ELF32_T_NONE_FSIZE = 0;
const int ELF32_T_ADDR_FSIZE = 4;
const int ELF32_T_OFF_FSIZE = 4;
const int ELF32_T_BYTE_FSIZE = 1;
const int ELF32_T_HALF_FSIZE = 2;
const int ELF32_T_WORD_FSIZE = 4;
const int ELF32_T_SWORD_FSIZE = 4;
const int ELF32_T_EHDR_FSIZE = 52; // sizeof(Elf32_Ehdr)
const int ELF32_T_PHDR_FSIZE = 32; // sizeof(Elf32_Phdr)
const int ELF32_T_SHDR_FSIZE = 40; // sizeof(Elf32_Shdr)
const int ELF32_T_SYM_FSIZE = 16;  // sizeof(Elf32_Sym)
const int ELF32_T_REL_FSIZE = 8;   // sizeof(Elf32_Rel)
const int ELF32_T_RELA_FSIZE = 12; // sizeof(Elf32_Rela)
const int ELF32_T_LHDR_FSIZE = 28; // sizeof(Elf32_Lhdr)
const int ELF32_T_LIBHDR_F_FSIZE = 20; // sizeof(Elf32_Libhdr_f)
const int ELF32_T_LIBSYM_F_FSIZE = 12; // sizeof(Elf32_LibSym_f)


    //  ����� ��� ������������

enum ELF_FLAGS {
        ELF_F_NONE = 0 ,
        ELF_F_INMEM = 0x01 ,
        ELF_F_DIRTY = 0x02 ,
        ELF_F_DISKVIEW = 0x04,
        ELF_F_NOTDISCARD = 0x08,
        ELF_F_NOSHDR = 0x10
};

    //  ������ ���������� ����������

class ELF;        // ���������� ���������� ����� (����� ���� � ������� ����������)
class ELF_SCN;    // ���������� ������ ���������� �����
class ELF_DATA;   // ���������� ������ � ������
class ELF_FILE;   // ���� � ����� (����� ���� �����������)


class ELF {
    private:

        friend class ELF_SCN;
        friend class ELF_FILE;

    private:

        Elf32_Ehdr  *   ehdr;
        Elf32_Phdr  *   phdr;

        ELF_SCN     *   listscn;
        ELF_CMD         cmd;
        unsigned int    flags;      // ����� ����� � �����
        unsigned int    eflags;     // ����� ��������� Elf
        unsigned int    pflags;     // ����� ������� ����������� ���������
        const char  *   filename;

        int FileExist;      // ���� ������ �� ����.
                            // ������������ ������, ��� cmd == ELF_C_WRITE
        int DeleteFile;     // ���� ����������� �� ���� ������� �������
                            // ������������ ��� ������ � ������������

        int Fill_Character;

    protected:
        ELF     *   nextelf;        // ��������� �� ��������� �������.
                                    // ������������ � ����������.
//        Elf32_Word  UsageCount;     // ����� ��������, ����������� ������
//                                    // ELF ����
        Elf32_Off   base_offset;    // �������� ������ ���������� �����
                                    // �� ���� �����. ��� ���������

        std::iostream *   pfile;     // ����� ��� ������� � �����

        ELF(
            std::iostream * pf,       // ��������� �� ����� ����� ����������
            const char * name,        // ��� �����
            Elf32_Off offset,   // �������� ����� � ����������
            ELF_CMD c,          // ������� ��������
            ELF *next           // ��������� �� ��������� ELF ����
            )
            throw(ELF_ERROR);
            // ������������ � ������ ELF_FILE

    private:

        Elf32_Shdr *buildshdr() throw(ELF_ERROR);
            // ������ ����� ������� ���������� ������

        void readshdr() throw(ELF_ERROR);

        void checkflags();

        std::ostream& write(Elf32_Ehdr& ehdr) throw(ELF_ERROR);

        std::ostream& write(Elf32_Phdr& phdr) throw(ELF_ERROR);

        std::ostream& write(Elf32_Shdr& shdr) throw(ELF_ERROR);

    public:

            // ��������� ���� ��� ������� ����� � �����������
            // �� ��������� c.
            // � == ELF_C_READ - ���� ����������� �� ������
            // c == ELF_C_RDWR - ���� ����������� �� ���������
            // c == ELF_C_WRITE - ���� ����������� �� ������
            // ��� ����� �������� ���������� name.
        ELF( const char *name, ELF_CMD c)
            throw(ELF_ERROR);

            // ��������� ����. ���������� ���� �� ���� ���� ����������.
        ~ELF()
            throw(ELF_ERROR);

            // Update( ELF_C_NULL ) - ��������� �������� ���� ��������
            // � �����. ���������� ������ �� ����������
            // Update( ELF_C_WRITE ) - ��������� �������� ���� ��������
            // � �����. ���������� ���� �� ����,
            // ���������� ������ �����
        Elf32_Off       Update(ELF_CMD c) throw(ELF_ERROR);

            // ������ ������� ���������� ����������� ���������
        Elf32_Phdr  *   GetPhdr() throw(ELF_ERROR);

            // ������� ����� ������� ����������� ���������
        Elf32_Phdr  *   NewPhdr(Elf32_Word count) throw(ELF_ERROR);

            // ������ ELF ��������� �����
        Elf32_Ehdr  *   GetEhdr(Elf32_Half TargetProc = EM_CURRENTPROC) throw(ELF_ERROR);

            // ������� ��������� ELF �����
        Elf32_Ehdr  *   NewEhdr(Elf32_Half TargetProc = EM_CURRENTPROC) throw(ELF_ERROR);

            // ������ ����� ������������� �����
        char        *   GetIdent(unsigned char *ptr) throw(ELF_ERROR);

            // ������ ������ � ������� index
        ELF_SCN     *   GetScn(Elf32_Word index) throw(ELF_ERROR);

            // ������� ����� ������
        ELF_SCN     *   NewScn() throw(ELF_ERROR);

            // ������ ��������� ������ �� scn
        ELF_SCN     *   NextScn(ELF_SCN *scn) throw(ELF_ERROR);

            // ������� ������ ".shstrtab". ��������� � ��� ��� ����� ����
            // ������.
        ELF_SCN     *   NewSHSTRTAB() throw(ELF_ERROR);

			//	������� ����� �� �����
		static Elf32_Word sectionFlagsFromName( const char* name );

            //  ������ ��� �����
        ELF_KIND        Kind() throw(ELF_ERROR);

            // ������ ��� �����
        const char  *   GetName() const { return filename; }

            // ���������� ��������� �� ���� �� ��������� offset
            // � ������ � �������� section
        char        *   StrPtr(Elf32_Word section, Elf32_Word offset)
            throw(ELF_ERROR);

            // ������������� ���� ����������
        void            Fill(int fill) throw();

            // ������� ������ ����������� ������� � ������.
            // �������� Phdr ������ ����������� �������.
            // �������� Buffer ������ ����� � ������� �������� �������
            // ���� �������� SkipZeros ����� 0, �� ������� ����������� 0 ��
            // ������� ������� � ������ (������ Phdr.p_memsz).
            // ���� �������� SkipZeros �� ����� 0, �� ���������� 0 ��
            // ������������, � ����� ����������� ������ �������
            // �� ����� (������ Phdr.p_filesz).
            // ����� ������ ���� ������������ ������� ��� ����, ����� ��������
            // ������ ��������.
        void  LoadProgramSegment( Elf32_Phdr *Phdr , char *Buffer, int SkipZeros = 0)
            throw (ELF_ERROR);

        unsigned int    SetFlag(unsigned int f);

        unsigned int    ClearFlag(unsigned int f);

        unsigned int    SetFlagPhdr(unsigned int f);

        unsigned int    ClearFlagPhdr(unsigned int f);

        unsigned int    SetFlagEhdr(unsigned int f);

        unsigned int    ClearFlagEhdr(unsigned int f);

            // ��������� ������ ������ � ������������� � ������.
        static void ToM(
                Elf_Type type,          // ��� ��������� �������.
                unsigned char *dest,    // �������� ������.
                unsigned char *src,     // ������ ����������.
                unsigned encode,        // ��� �����������.
                unsigned size);         // ����� ��������� � �������.

            // ��������� ������ ������ � ������������� � �����.
        static void ToF(
                Elf_Type type,
                unsigned char *dest,
                unsigned char *src,
                unsigned encode,
                unsigned size);
};

inline unsigned int ELF::SetFlag(unsigned int f)
{
    return (this->flags |= f);
}

inline unsigned int ELF::ClearFlag(unsigned int f)
{
    return (this->flags &= (~f));
}

inline unsigned int ELF::SetFlagPhdr(unsigned int f)
{
    return (this->pflags |= f);
}

inline unsigned int ELF::ClearFlagPhdr(unsigned int f)
{
    return (this->pflags &= (~f));
}

inline unsigned int ELF::SetFlagEhdr(unsigned int f)
{
    return (this->eflags |= f);
}

inline unsigned int ELF::ClearFlagEhdr(unsigned int f)
{
    return (this->eflags &= (~f));
}

class ELF_SCN {
    private:
        friend class ELF;

        char        *   name;
        Elf32_Word      ndex;
        unsigned int    flags;
        unsigned int    sflags;
        ELF_SCN     *   nextScn;
        ELF_DATA    *   listdata;
        Elf32_Shdr  *   shdr;

        ELF *elf;

        std::ostream& write(ELF_CMD c,Elf32_Off& filesize) throw(ELF_ERROR);

        void checkflags();
        void elf_internal_load_section(Elf_Type t) throw(ELF_ERROR);


    public:

        ELF_SCN (
                ELF *e,
                Elf32_Word d,
                unsigned int f,
                unsigned int sf,
                ELF_SCN *next,
                ELF_DATA *data,
                Elf32_Shdr *sh)
            throw(ELF_ERROR);

        ~ELF_SCN()
            throw(ELF_ERROR);

            // ������ ��������� ������
        Elf32_Shdr * GetShdr() const { return shdr; }

            // ������ ������ ������
        Elf32_Word Index() const { return ndex; }

            // ������ ���� ������ ��������� �� data ��� 0, ����
            // ���� data ���������. ���� �������� data ����� 0,
            // �� ������ ������ ���� ������.
        ELF_DATA * GetData(ELF_DATA *data) throw(ELF_ERROR);

            // ������� ����� ���� ������ � ������
        ELF_DATA * NewData() throw(ELF_ERROR);

            // ����������� ��� ������
        char * AttachName(const char *ptr) throw(ELF_ERROR);

            // ������ ��� ������
        char * GetName() throw(ELF_ERROR);

            // ������� ���������/������ ������
        unsigned int    SetFlag(unsigned int f);
        unsigned int    ClearFlag(unsigned int f);
        unsigned int    SetFlagShdr(unsigned int f);
        unsigned int    ClearFlagShdr(unsigned int f);
};

inline unsigned int ELF_SCN::SetFlag(unsigned int f)
{
    return (flags |= f);
}

inline unsigned int ELF_SCN::ClearFlag(unsigned int f)
{
    return (flags &= (~f));
}

inline unsigned int ELF_SCN::SetFlagShdr(unsigned int f)
{
    return (sflags |= f);
}

inline unsigned int ELF_SCN::ClearFlagShdr(unsigned int f)
{
    return (sflags &= (~f));
}

class ELF_DATA {
    private:

        friend class ELF_SCN;
        friend std::ostream& operator<< (std::ostream& f,ELF_SCN *scn) throw(ELF_ERROR);


    private:
        ELF_SCN     *   section;
        ELF_DATA    *   next;
        int             nrefs;
        unsigned int    flags;

    public:

        void        *   d_buf;
        Elf_Type        d_type;
        Elf32_Word      d_size;
        Elf32_Off       d_offset;
        Elf32_Word      d_align;
        unsigned int    d_version;

    public:

        ELF_DATA() :    section(0),
                        next(0L),
                        nrefs(0L),
                        flags(ELF_F_INMEM),
                        d_buf(0L),
                        d_type(ELF32_T_NONE),
                        d_size(0L),
                        d_offset(0L),
                        d_align(0L),
                        d_version(EV_CURRENT)
        { }

        ~ELF_DATA()
            throw(ELF_ERROR);


        ELF_DATA *XlateToF(ELF_DATA *dest,unsigned int encode)
            throw(ELF_ERROR);


        ELF_DATA *XlateToM(ELF_DATA *dest,unsigned int encode)
            throw(ELF_ERROR);


        unsigned int SetFlag(unsigned int f);
        unsigned int ClearFlag(unsigned int f);
};

inline unsigned int ELF_DATA::SetFlag(unsigned int f)
{
    return (flags |= f);
}

inline unsigned int ELF_DATA::ClearFlag(unsigned int f)
{
    return (flags &= (~f));
}

extern unsigned int ElfVersion(unsigned int ver);

Elf32_Word Elf32Fsize(Elf_Type type,Elf32_Word count,unsigned int ver);

unsigned long ElfHash(const unsigned char *name);

    // ���������� ������ libelf
class ELF_INTERNAL_BLOCK;

class ELF_FILE {
    private:

        std::fstream     f;
        ELF_KIND    kindfile;   // ��� �����

        ELF     *   listelf;   // ������ ��������� ����������

        ELF_CMD     cmd;        // ������� �������� �����
        const char* filename;   // ��� �����
        Elf32_Lhdr *lhdr;       // ��������� ����������
        Elf32_Libhdr * libelem_hdr; // ������� ���������� ����������

            // ������ ����� ����� ����������
        ELF_INTERNAL_BLOCK * StringBuffer;
            // ��������� ����� ����� ����������
        ELF_INTERNAL_BLOCK * LastStringBuffer;
        Elf32_Word StringSize;  // ��������� ������ ������� �����

        Elf32_LibSym *  SymbolTable;    // ������� �������� ����������
        Elf32_Word      SymbolSize;     // ������ ������� ��������


            // ��������� �������. ������ �������� ����������
        void readelf() throw (ELF_ERROR);

            // ��������� �������. ������ ������� ����� � �������� ��� �����
            // ����� ��������� �� ��������������� ����� ����� ��������
            // � ������ ��������������� ��������� ELF_ERROR_AMBIGUITY
        void BuildTable(
                ELF * Elf,                  // ����������� ����
                Elf32_Word StringSize,      // ������ ������� ������� �����
                Elf32_Word FileIndex,       // ������ ����� � ����������
                ELF_INTERNAL_BLOCK *& Block,// ������������ ����� �����
                Elf32_LibSym *& Sym,        // ������������ ������� ��������
                Elf32_Word& SymSize,        // ������������ ����� ��������
                Elf32_Word& FileNameOffset  // ������������ �������� �����
                                            // ����� � ����� ������ �����
            ) throw (ELF_ERROR);

            // ���������� ������� ������ ������� �� ����������� Bind
            // � ������ Name � ������� �������� ����������
            // ���������� ����� ���������� �������� � ������� ��������
            // � ������ ���������� ������� ���������� (~0)
        Elf32_Word IntSearchSymbolBind( const char * Name, unsigned char Bind )
            throw(ELF_ERROR);

    public:
        ELF_FILE( const char *name,ELF_CMD c)
            throw(ELF_ERROR);

        ~ELF_FILE()
            throw(ELF_ERROR);

            // ������ ��������� ELF-���� �� elf. ���� �������� elf==NULL,
            // �� ������ ������ ELF ����. ���� elf ��������� �������
            // ����������, �� ���������� NULL.
        ELF * NextElf(ELF *elf) throw(ELF_ERROR);

            // ������ ��� �����
            // ��� ����� ����� ����
            // ELF_K_LIB - ���������� ELF ������.
            // ELF_K_ELF - ELF ����.
            // ELF_K_NONE - ����������� ���.
        ELF_KIND Kind() throw(ELF_ERROR);

            // ������ ��� ����� ����������
        const char * GetName() const { return filename; }

            //------------------------------------------//
            // ��������� ������ ����������              //
            // ������� ���������� ������ � ������������ //
            //------------------------------------------//

            // ������ ELF ���� � ������� index. ���� ������ ��
            // ������������� ������ ���� �����, ����������� ����������
            // ELF_ERROR_EXIST.
        ELF * GetElf(Elf32_Word index) throw(ELF_ERROR);

            // ������ ��������� ����������
        Elf32_Lhdr * GetLhdr() throw(ELF_ERROR);

            // ������ ������� ���������� ��������� ����������
            // ���� �������� ���� �� �������� �����������, ��
            // ��������� ���������� ELF_ERROR_TYPE
        Elf32_Libhdr * GetMemHdr() throw(ELF_ERROR);

            // ������ ����� ��������� ������� ���������� ����������
            // ���������� ��� GetLhdt()->l_lhnum
        Elf32_Word LibHdrSize() throw(ELF_ERROR);

            // ������ ������� �������� ����������
            // ���� ������� ���� �� �������� �����������,
            // ��������� ���������� ELF_ERROR_TYPE
        Elf32_LibSym    *   GetLibSym() throw(ELF_ERROR);

            // ������ ������ ������� �������� ����������
            // ���� ������� ���� �� �������� �����������,
            // ��������� ���������� ELF_ERROR_TYPE
        Elf32_Word          LibSymSize() throw(ELF_ERROR);

            // ���������� ������ �������� ����������� ������ ������
            // ���� ������ �� ������, ���������� (~0)
        Elf32_Word SearchSymbol( const char * Name );

            // ���������� ������� ���������� Elf � ���� name
        void ExtractToFile( ELF * Elf, const char * name ) throw(ELF_ERROR);

            //------------------------------------------//
            // ��������� �������� ����������            //
            // ������� ���������� ������ � ������������ //
            //------------------------------------------//

            // ������� ����� ��������� ����������
        Elf32_Lhdr * NewLhdr() throw(ELF_ERROR);

            // ��������� ������� Elf � ����������. ����������
            // ���������� ������ �������� ����������
            // date ������ ����� �������� �����
            // ����� ���������� ����� � ���������� �� ������� �����������
            // ��������� � ELF �����, ������ � ������� �������� �����
            // �.�. ��� ��������� �� ����� ������� � ����������
        ELF * AddElf( ELF * Elf, time_t date ) throw (ELF_ERROR);

            // ����������� ���� �� ����������
            // ���������� ���������� ELF �����.
            //
        ELF * DetachElf( ELF * Elf ) throw (ELF_ERROR);

            // Update( ELF_C_NULL ) - ��������� �������� ���� ��������
            // � �����. ���������� ������ �� ����������
            // Update( ELF_C_WRITE ) - ��������� �������� ���� ��������
            // � �����. ���������� ���� �� ����,
            // ���������� ������ �����
        Elf32_Off Update(ELF_CMD c) throw(ELF_ERROR);
};

#endif  // __LIBELF_H
