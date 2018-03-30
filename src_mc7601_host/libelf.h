//////////////////////////////////////////////////////////////////////////
//                                                                      //
//             Базовое программное обеспечение нейропроцессора          //
//                                                                      //
//     Библиотека доступа к объектным и исполняемым файла формата ELF   //
//                                                                      //
//                      Интерфейсный модуль libelf.h                    //
//                                                                      //
//     Интерфейсный файл описывающий типы, классы, и методы классов     //
//     используемые при работе с файлами формата ELF                    //
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
// запрещаем Microsoft Visual C++ ругаться на наши исключения
#pragma warning( disable : 4290 )
#pragma warning( disable : 4996 )  // This function or variable may be unsafe.(VS8)
#endif

    // Типы используемые в библиотеке

typedef unsigned long       Elf32_Addr;     // адрес на целевой машине
typedef unsigned short int  Elf32_Half;     // половина слова на целевой машине
typedef unsigned long       Elf32_Off;      // смещение в файле
typedef long                Elf32_Sword;    // слово со знаком на целевой машине
typedef unsigned long       Elf32_Word;     // слово без знака на целевой машине

    //  смешения в байтах идентификации файла

const int EI_MAG0   = 0;
const int EI_MAG1   = 1;
const int EI_MAG2   = 2;
const int EI_MAG3   = 3;
const int EI_CLASS  = 4;
const int EI_DATA   = 5;
const int EI_VERSION= 6;
const int EI_PAD    = 7;
const int EI_NIDENT = 16;

    // содержание байтов идентификации

    // объектный или исполняемый файл Elf
const unsigned char ELFMAG0 = '\x7f';
const unsigned char ELFMAG1 = 'E';
const unsigned char ELFMAG2 = 'L';
const unsigned char ELFMAG3 = 'F';

    // библиотека объектных файлов Elf
const unsigned char LIBMAG0 = '\x7f';
const unsigned char LIBMAG1 = 'L';
const unsigned char LIBMAG2 = 'I';
const unsigned char LIBMAG3 = 'B';


    //  константы класса файла

const unsigned char ELFCLASSNONE= 0;
const unsigned char ELFCLASS32  = 1;
const unsigned char ELFCLASS64  = 2;
const unsigned char ELFCLASSNUM = 3;

    // константы кодирования данных

const unsigned char ELFDATANONE = 0;
const unsigned char ELFDATA2LSB = 1;
const unsigned char ELFDATA2MSB = 2;
const unsigned char ELFDATANUM  = 3;

    //  константы типов файлов

const Elf32_Half    ET_NONE = 0;
const Elf32_Half    ET_REL  = 1;

const Elf32_Half    ET_EXEC = 2;
                    // в нашем случае, абсолютный исполняемый образ программы

const Elf32_Half    ET_DYN      = 3;
const Elf32_Half    ET_CORE     = 4;
const Elf32_Half    ET_NUM      = 5;
const Elf32_Half    ET_LOPROC   = 0xff00;

const Elf32_Half    ET_EXEREL   = 0xff00;
                    // исполняемый перемещаемый файл
                    // специфично для нейропроцессора

const Elf32_Half    ET_HIPROC= 0xffff;

    // константы типов машин

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

    // константы версии Elf

const unsigned int EV_NONE      = 0;    //
const unsigned int EV_CURRENT   = 1;    // текущая версия Elf
const unsigned int EV_NUM       = 2;    //

    //  константы специальных индексов секций

const Elf32_Half    SHN_UNDEF       = 0;
const Elf32_Half    SHN_LORESERVE   = 0xff00;
const Elf32_Half    SHN_LOPROC      = 0xff00;
const Elf32_Half    SHN_HIPROC      = 0xff1f;
const Elf32_Half    SHN_ABS         = 0xfff1;
const Elf32_Half    SHN_COMMON      = 0xfff2;
const Elf32_Half    SHN_HIRESERVE   = 0xffff;

    //  константы типов секций

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
//	custom секция для хранения информации о банках памяти
const Elf32_Word    SHT_BANKMAP = 0xBA6405L;
//	для отладки, чтобы увидеть секцию глазами
const unsigned int BankInfoMarker = 0x21212121;	


    // флаги атрибутов секций

const Elf32_Word    SHF_WRITE       = 0x1;
const Elf32_Word    SHF_ALLOC       = 0x2;
const Elf32_Word    SHF_EXECINSTR   = 0x4;
const Elf32_Word    SHF_MASKPROC    = 0xf0000000L;

    //  константы типов программных сегментов

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
        Elf32_Word  p_type;		//	тип сегмента, PT_...
        Elf32_Off   p_offset;	//	смещение в эльфе, в байтах
        Elf32_Addr  p_vaddr;	//	виртуальный адрес в памяти на цели
        Elf32_Addr  p_paddr;	//	физический адрес в памяти на цели
        Elf32_Word  p_filesz;	//	размер в байтах в эльфе
        Elf32_Word  p_memsz;	//	размер в байтах в памяти на цели
        Elf32_Word  p_flags;	//	PF_... флаги
        Elf32_Word  p_align;	//	выравнивание p_vaddr и p_offset

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

//	элемент таблицы заголовков секций

struct Elf32_Shdr {
    public:
        Elf32_Word  sh_name;	//	индекс имени секции
        Elf32_Word  sh_type;	//	тип секции, SHT_...
        Elf32_Word  sh_flags;	//	SHF_... флаги
        Elf32_Addr  sh_addr;	//	адрес расположения в памяти, для не загружаемых-0 
        Elf32_Off   sh_offset;	//	адрес в эльф-файле
        Elf32_Word  sh_size;	//	размер в эльф-файле в байтах
        Elf32_Word  sh_link;	//	ссылка на связанную секцию, смысл зависит от типа
        Elf32_Word  sh_info;	//	доп. инфо, смысл зависит от типа
        Elf32_Word  sh_addralign;//	выравнивание sh_addr
        Elf32_Word  sh_entsize;	//	для табличных секций - размер записи в байтах

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

//	заголовок ELF-файла

struct Elf32_Ehdr {
    public:
        unsigned char   e_ident[EI_NIDENT];	//	константа, указывает, что файл есть эльф
        Elf32_Half      e_type;				//	указывает, obj это, exe или dll (говоря грубо)
        Elf32_Half      e_machine;			//	целевая платформа
        Elf32_Word      e_version;			//	1 для настоящих индейцев (эльфов)
        Elf32_Addr      e_entry;			//	точка входа, адрес передачи управления
        Elf32_Off       e_phoff;			//	смещение таблицы сегментов (obj-интерпретация) программы
        Elf32_Off       e_shoff;			//	смещение таблицы секций (exe-интерпретация) программы
        Elf32_Word      e_flags;			//	платформно-специфичные флаги
        Elf32_Half      e_ehsize;			//	размер данной структуры (в файле)
        Elf32_Half      e_phentsize;		//	размер записи в таблице секций
        Elf32_Half      e_phnum;			//	количество записей в таблице секций
        Elf32_Half      e_shentsize;		//	размер записи в таблице сегментов
        Elf32_Half      e_shnum;			//	количество записей в таблице сегментов
        Elf32_Half      e_shstrndx;			//	индекс секции, хранящей имена секций

        Elf32_Ehdr();
		Elf32_Ehdr(Elf32_Half TargetProc);
};

    // константы для байтов идентификации файлов Elf

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
    // структура заголовка библиотеки
    //

struct Elf32_Lhdr {
    public:
        unsigned char   l_ident[EI_LIB_NIDENT];
        Elf32_Off       l_lhoff;        // смещение таблицы заголовков
        Elf32_Half      l_lhnum;        // число элементов библиотеки
        Elf32_Half      l_lhentsize;    // размер заголовка в таблице
        Elf32_Half      l_lhsymndx;     // индекс специального элемента
                                        // содержащего символы библиотеки
        Elf32_Half      l_lhstrndx;     // индекс специального элемента
                                        // содержащего имена

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
    // Структура заголовка элемента библиотеки
    //

struct Elf32_Libhdr {
    private:
        friend class ELF_FILE;

        Elf32_Word      lib_name_offset;    // смещение имени элемента
                                            // в таблице строк
    public:
        char        *   lib_name;   // имя элемента
        time_t          lib_date;   // дата создания файла элемента
        Elf32_Off       lib_offset; // смещение в элемента в библиотке
        Elf32_Word      lib_size;   // размер элемента библиотеки
        Elf32_Word      lib_mode;   // различные флаги

        Elf32_Libhdr()
            : lib_name(0),
            lib_date(0),
            lib_offset(0),
            lib_size(0),
            lib_mode(0)
        { }
};

const Elf32_Word    LHF_NONE    = 0;
const Elf32_Word    LHF_SPECIAL = 0x1;  // флаг специального элемента
                                        // библиотеки
const Elf32_Word    LHF_MASKPROC  = 0xffff0000L;
                                        // маска для пользовательских
                                        // флагов

    //
    // Структура элемента таблицы символов
    //

struct Elf32_LibSym {
    private:
        friend class ELF_FILE;

        Elf32_Word  ls_name_offset; // смещение имени символа в таблице
    public:
        char    *   ls_name;    // имя символа
        Elf32_Half  ls_index;   // номер элемента, содержащего
                                // определение данного символа
        Elf32_Sword ls_bind;    // связывание символа константы STB_???
        Elf32_Word  ls_hash;    // значение хеш-функции Elf_Hash для
                                // имени символа

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

    //  специальные функции для объединения типа и информации о связывании
    // для символа в поле st_info структуры Elf32_Sym

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

    // константы типов связывания символов

const unsigned char STB_LOCAL   = 0;
const unsigned char STB_GLOBAL  = 1;
const unsigned char STB_WEAK    = 2;
const unsigned char STB_NUM     = 3;
const unsigned char STB_LOPROC  = 13;
const unsigned char STB_HIPROC  = 15;

    // константы типов символов

const unsigned char STT_NOTYPE  = 0;
const unsigned char STT_OBJECT  = 1;
const unsigned char STT_FUNC    = 2;
const unsigned char STT_SECTION = 3;
const unsigned char STT_FILE    = 4;
const unsigned char STT_NUM     = 5;
const unsigned char STT_LOPROC  = 13;

const unsigned char STT_HIPROC  = 15;

    //  тип элемента таблицы перемещений

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

    // специальные функции для объединения информации о типе перемещения и
    // индексе символа в поле r_info структур Elf32_Rel и Elf32_Rela

inline Elf32_Word ELF32_R_SYM(Elf32_Word i) { return Elf32_Half((i)>>8); }

inline unsigned char ELF32_R_TYPE(Elf32_Word i) { return (unsigned char)(i); }

inline Elf32_Word ELF32_R_INFO(Elf32_Word b,unsigned char t)
{
    return ( (Elf32_Word(b)<<8) + Elf32_Word(t) );
}

    // константы типов перемещения
    // все константы определены для нейропроцессора

const unsigned char R_NC_NONE   = 0;  // отсутствие перемещения
const unsigned char R_NC_ADDR   = 1;  // перемещение как абсолютного адреса
const unsigned char R_NC_PC     = 2;  // перемещение как относительного адреса
const unsigned char R_NC_BYTEPTR= 3;
//  Добавлен, поскольку иначе надо выкидывать команды ar = ar-Label, ar-=Label
//  а они уже живут (и вызывают ошибки превращаясь в ar+=Label)
const unsigned char R_NC_MINUSADDR= 4;// константа минус абсолютный адрес
                           // перемещение как смещения в байтах в секции файла


    // структура заголовока элемента в секции комментариев

struct Elf32_Nhdr {
    public:
        Elf32_Word  n_namesz;       // длина имени комментария
        Elf32_Word  n_descsz;       // длина информации комментария
        Elf32_Word  n_type;         // тип комментария
};

    // Константы типов файлов

enum ELF_KIND { ELF_K_NONE = 0,ELF_K_ELF = 1,ELF_K_LIB = 2,ELF_K_NUM };

    // константы команд библиотеки
enum ELF_CMD {  ELF_C_NULL, ELF_C_READ, ELF_C_RDWR, ELF_C_WRITE, ELF_C_NUM };


    //------------------------------------------------------//
    //                                                      //
    //         Классы ошибок, порождаемые библиотекой       //
    //                                                      //
    //------------------------------------------------------//

   // базовый класс для классов ошибок, порождаемых библиотекой
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

    // ошибка обращения к файлу
class ELF_ERROR_FILE : public ELF_ERROR {
    public:
        ELF_ERROR_FILE (const char *s, const char *s1 = 0, const char *s2 = 0,
            const char *s3 = 0) : ELF_ERROR(s,s1,s2,s3)
        { }
};

    // Несовпадение класса ( числа бит в слове ) в файле
class ELF_ERROR_CLASS : public ELF_ERROR {
    public:
        ELF_ERROR_CLASS (const char *s, const char *s1 = 0, const char *s2 = 0,
            const char *s3 = 0) : ELF_ERROR(s,s1,s2,s3)
        { }
};

    // ошибка в заголовке ELF файла
class ELF_ERROR_EHDR : public ELF_ERROR {
    public:
        ELF_ERROR_EHDR(const char *s, const char *s1 = 0, const char *s2 = 0,
            const char *s3 = 0) : ELF_ERROR(s,s1,s2,s3)
        { }
};

    // ошибка в заголовке библиотеки ELF файлов
class ELF_ERROR_LHDR : public ELF_ERROR {
    public:
        ELF_ERROR_LHDR(const char *s, const char *s1 = 0, const char *s2 = 0,
            const char *s3 = 0) : ELF_ERROR(s,s1,s2,s3)
        { }
};

    // ошибка добавления файла к библиотеке: неоднозначное имя символа
    // функция GetAmbiguityName выдает имя неоднозначного символа
class ELF_ERROR_AMBIGUITY : public ELF_ERROR {
    protected:
        char *   eeaAmName;      // имя символа
    public:
        ELF_ERROR_AMBIGUITY(
            const char *s,
            const char *s1 = 0,
            const char *s2 = 0,
            char *AmName = 0);

            // конструктор копирования и оператор копирования объекта
            // необходимы для копирования строки - имени символа.
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

    // не существует какой либо элемент файла
class ELF_ERROR_EXIST : public ELF_ERROR {
    public:
        ELF_ERROR_EXIST(const char *s, const char *s1 = 0, const char *s2 = 0,
            const char *s3 = 0) : ELF_ERROR(s,s1,s2,s3)
        { }
};

    // несовпадение типов каких-либо элементов
class ELF_ERROR_TYPE : public ELF_ERROR {
    public:
        ELF_ERROR_TYPE(const char *s, const char *s1 = 0, const char *s2 = 0,
            const char *s3 = 0) : ELF_ERROR(s,s1,s2,s3)
        { }
};

    // несовпадение размеров каких-либо элементов файла
class ELF_ERROR_SIZE : public ELF_ERROR {
    public:
        ELF_ERROR_SIZE(const char *s, const char *s1 = 0, const char *s2 = 0,
            const char *s3 = 0) : ELF_ERROR(s,s1,s2,s3)
        { }
};

    // неизвестный параметр или элемент файла
class ELF_ERROR_UNKNOWN : public ELF_ERROR {
    public:
        ELF_ERROR_UNKNOWN(const char *s,const char *s1 = 0,const char *s2 = 0,
            const char *s3 = 0) : ELF_ERROR(s,s1,s2,s3)
        { }
};

    // переполнение приемного буфера
class ELF_ERROR_OVERFLOW : public ELF_ERROR {
    public:
        ELF_ERROR_OVERFLOW(const char *s,const char *s1 = 0,const char *s2 = 0,
            const char *s3 = 0) : ELF_ERROR(s,s1,s2,s3)
        { }
};

    // внутреняя ошибка библиотеки
class ELF_ERROR_INTERNAL : public ELF_ERROR {
    public:
        ELF_ERROR_INTERNAL(const char *s,const char *s1 = 0,const char *s2 = 0,
            const char *s3 = 0) : ELF_ERROR(s,s1,s2,s3)
        { }
};

    //------------------------------------------------------//
    //                                                      //
    //      Классы для представления типов библиотеки       //
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

    // Размеры типов данных в файле.
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


    //  Флаги для десктипторов

enum ELF_FLAGS {
        ELF_F_NONE = 0 ,
        ELF_F_INMEM = 0x01 ,
        ELF_F_DIRTY = 0x02 ,
        ELF_F_DISKVIEW = 0x04,
        ELF_F_NOTDISCARD = 0x08,
        ELF_F_NOSHDR = 0x10
};

    //  классы интерфейса библиотеки

class ELF;        // дескриптор объектного файла (может быть в составе библиотеки)
class ELF_SCN;    // дескриптор секции объектного файла
class ELF_DATA;   // дескриптор данных в секции
class ELF_FILE;   // файл в целом (может быть библиотекой)


class ELF {
    private:

        friend class ELF_SCN;
        friend class ELF_FILE;

    private:

        Elf32_Ehdr  *   ehdr;
        Elf32_Phdr  *   phdr;

        ELF_SCN     *   listscn;
        ELF_CMD         cmd;
        unsigned int    flags;      // флаги файла в целом
        unsigned int    eflags;     // флаги заголовка Elf
        unsigned int    pflags;     // флаги таблицы программных сегментов
        const char  *   filename;

        int FileExist;      // флаг создан ли файл.
                            // используется только, при cmd == ELF_C_WRITE
        int DeleteFile;     // флаг принадлежит ли файл данному объекту
                            // используется при работе с библиотеками

        int Fill_Character;

    protected:
        ELF     *   nextelf;        // указатель на следующий элемент.
                                    // используется в библиотеке.
//        Elf32_Word  UsageCount;     // число объектов, разделяющих данный
//                                    // ELF файл
        Elf32_Off   base_offset;    // смещение начала объектного файла
                                    // во всем файле. для библиотек

        std::iostream *   pfile;     // поток для доступа к файлу

        ELF(
            std::iostream * pf,       // указатель на поток файла библиотеки
            const char * name,        // имя файла
            Elf32_Off offset,   // смещение файла в библиотеке
            ELF_CMD c,          // команда открытия
            ELF *next           // указатель на следующий ELF файл
            )
            throw(ELF_ERROR);
            // используется в классе ELF_FILE

    private:

        Elf32_Shdr *buildshdr() throw(ELF_ERROR);
            // строит новую таблицу заголовков секций

        void readshdr() throw(ELF_ERROR);

        void checkflags();

        std::ostream& write(Elf32_Ehdr& ehdr) throw(ELF_ERROR);

        std::ostream& write(Elf32_Phdr& phdr) throw(ELF_ERROR);

        std::ostream& write(Elf32_Shdr& shdr) throw(ELF_ERROR);

    public:

            // открывает файл или создает новый в зависимости
            // от параметра c.
            // с == ELF_C_READ - файл открывается на чтение
            // c == ELF_C_RDWR - файл открывается на изменение
            // c == ELF_C_WRITE - файл открывается на запись
            // имя файла задается параметром name.
        ELF( const char *name, ELF_CMD c)
            throw(ELF_ERROR);

            // закрывает файл. записывает файл на диск если необходимо.
        ~ELF()
            throw(ELF_ERROR);

            // Update( ELF_C_NULL ) - вычисляет смещения всех объектов
            // в файле. физической записи не производит
            // Update( ELF_C_WRITE ) - вычисляет смещения всех объектов
            // в файле. записывает файл на диск,
            // Возвращает размер файла
        Elf32_Off       Update(ELF_CMD c) throw(ELF_ERROR);

            // выдает таблицу заголовков программных сегментов
        Elf32_Phdr  *   GetPhdr() throw(ELF_ERROR);

            // создает новую таблицу программных сегментов
        Elf32_Phdr  *   NewPhdr(Elf32_Word count) throw(ELF_ERROR);

            // выдает ELF заголовок файла
        Elf32_Ehdr  *   GetEhdr(Elf32_Half TargetProc = EM_CURRENTPROC) throw(ELF_ERROR);

            // создает заголовок ELF файла
        Elf32_Ehdr  *   NewEhdr(Elf32_Half TargetProc = EM_CURRENTPROC) throw(ELF_ERROR);

            // выдает байты идентификации файла
        char        *   GetIdent(unsigned char *ptr) throw(ELF_ERROR);

            // выдает секцию с номером index
        ELF_SCN     *   GetScn(Elf32_Word index) throw(ELF_ERROR);

            // создает новую секцию
        ELF_SCN     *   NewScn() throw(ELF_ERROR);

            // выдает следующую секцию за scn
        ELF_SCN     *   NextScn(ELF_SCN *scn) throw(ELF_ERROR);

            // создает секцию ".shstrtab". добавляет в нее имя самой этой
            // секции.
        ELF_SCN     *   NewSHSTRTAB() throw(ELF_ERROR);

			//	выводит флаги по имени
		static Elf32_Word sectionFlagsFromName( const char* name );

            //  выдает тип файла
        ELF_KIND        Kind() throw(ELF_ERROR);

            // выдает имя файла
        const char  *   GetName() const { return filename; }

            // возвращает указатель на байт со смещением offset
            // в секции с индексом section
        char        *   StrPtr(Elf32_Word section, Elf32_Word offset)
            throw(ELF_ERROR);

            // устанавливает байт заполнения
        void            Fill(int fill) throw();

            // функция читает программный сегмент в память.
            // параметр Phdr задает программный сегмент.
            // параметр Buffer задает буфер в который читается сегмент
            // если параметр SkipZeros равен 0, то сегмент дополняется 0 до
            // полного размера в памяти (размер Phdr.p_memsz).
            // Если параметр SkipZeros не равен 0, то дополнение 0 не
            // производится, и буфер заполняется только данными
            // из файла (размер Phdr.p_filesz).
            // буфер должен быть достаточного размера для того, чтобы вместить
            // данные сегмента.
        void  LoadProgramSegment( Elf32_Phdr *Phdr , char *Buffer, int SkipZeros = 0)
            throw (ELF_ERROR);

        unsigned int    SetFlag(unsigned int f);

        unsigned int    ClearFlag(unsigned int f);

        unsigned int    SetFlagPhdr(unsigned int f);

        unsigned int    ClearFlagPhdr(unsigned int f);

        unsigned int    SetFlagEhdr(unsigned int f);

        unsigned int    ClearFlagEhdr(unsigned int f);

            // Переводит массив данных к представлению в памяти.
        static void ToM(
                Elf_Type type,          // тип элементов массива.
                unsigned char *dest,    // исходный массив.
                unsigned char *src,     // массив результата.
                unsigned encode,        // тип кодирования.
                unsigned size);         // число элементов в массиве.

            // Переводит массив данных к представлению в файле.
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

            // Выдает заголовок секции
        Elf32_Shdr * GetShdr() const { return shdr; }

            // Выдает индекс секции
        Elf32_Word Index() const { return ndex; }

            // Выдает блок данных следующий за data или 0, если
            // блок data последний. Если параметр data равен 0,
            // то выдает первый блок данных.
        ELF_DATA * GetData(ELF_DATA *data) throw(ELF_ERROR);

            // Создает новый блок данных в секции
        ELF_DATA * NewData() throw(ELF_ERROR);

            // Присваивает имя секции
        char * AttachName(const char *ptr) throw(ELF_ERROR);

            // Выдает имя секции
        char * GetName() throw(ELF_ERROR);

            // Функции установки/снятия флагов
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

    // внутренние классы libelf
class ELF_INTERNAL_BLOCK;

class ELF_FILE {
    private:

        std::fstream     f;
        ELF_KIND    kindfile;   // тип файла

        ELF     *   listelf;   // список элементов библиотеки

        ELF_CMD     cmd;        // команда открытия файла
        const char* filename;   // имя файла
        Elf32_Lhdr *lhdr;       // заголовок библиотеки
        Elf32_Libhdr * libelem_hdr; // таблица заголовков библиотеки

            // первый буфер строк библиотеки
        ELF_INTERNAL_BLOCK * StringBuffer;
            // последний буфер строк библиотеки
        ELF_INTERNAL_BLOCK * LastStringBuffer;
        Elf32_Word StringSize;  // суммарный размер буферов строк

        Elf32_LibSym *  SymbolTable;    // таблица символов библиотеки
        Elf32_Word      SymbolSize;     // размер таблицы символов


            // внутреняя функция. читает элементы библиотеки
        void readelf() throw (ELF_ERROR);

            // внутреняя функция. строит таблицы строк и символов для файла
            // также проверяет на неоднозначность имена новых символов
            // в случае неоднозначности порождает ELF_ERROR_AMBIGUITY
        void BuildTable(
                ELF * Elf,                  // добавляемый файл
                Elf32_Word StringSize,      // размер прежней таблицы строк
                Elf32_Word FileIndex,       // индекс файла в библиотеке
                ELF_INTERNAL_BLOCK *& Block,// возвращаемый буфер строк
                Elf32_LibSym *& Sym,        // возвращаемая таблица символов
                Elf32_Word& SymSize,        // возвращаемое число символов
                Elf32_Word& FileNameOffset  // возвращаемое смещение имени
                                            // файла в новом буфере строк
            ) throw (ELF_ERROR);

            // внутренняя функция поиска символа со связыванием Bind
            // и именем Name в таблице символов библиотеки
            // возвращает номер найденного элемента в таблице символов
            // в случае отсутствия символа возвращает (~0)
        Elf32_Word IntSearchSymbolBind( const char * Name, unsigned char Bind )
            throw(ELF_ERROR);

    public:
        ELF_FILE( const char *name,ELF_CMD c)
            throw(ELF_ERROR);

        ~ELF_FILE()
            throw(ELF_ERROR);

            // выдает следующий ELF-файл за elf. если параметр elf==NULL,
            // то выдает первый ELF файл. Если elf последний элемент
            // библиотеки, то возвращает NULL.
        ELF * NextElf(ELF *elf) throw(ELF_ERROR);

            // выдает тип файла
            // тип файла может быть
            // ELF_K_LIB - библиотека ELF файлов.
            // ELF_K_ELF - ELF файл.
            // ELF_K_NONE - неизвестный тип.
        ELF_KIND Kind() throw(ELF_ERROR);

            // выдает имя файла библиотеки
        const char * GetName() const { return filename; }

            //------------------------------------------//
            // Интерфейс чтения библиотеки              //
            // функции работающие только с библиотеками //
            //------------------------------------------//

            // выдает ELF файл с номером index. Если индекс не
            // соответствует какому либо файлу, порождается исключение
            // ELF_ERROR_EXIST.
        ELF * GetElf(Elf32_Word index) throw(ELF_ERROR);

            // выдает заголовок библиотеки
        Elf32_Lhdr * GetLhdr() throw(ELF_ERROR);

            // выдает таблицу заголовков элементов библиотеки
            // если открытый файл не является библиотекой, то
            // порождает исключение ELF_ERROR_TYPE
        Elf32_Libhdr * GetMemHdr() throw(ELF_ERROR);

            // выдает число элементов таблицы заголовков библиотеки
            // сокращение для GetLhdt()->l_lhnum
        Elf32_Word LibHdrSize() throw(ELF_ERROR);

            // выдает таблицу символов библиотеки
            // если открыты файл не является библиотекой,
            // порождает исключение ELF_ERROR_TYPE
        Elf32_LibSym    *   GetLibSym() throw(ELF_ERROR);

            // выдает размер таблицы символов библиотеки
            // если открыты файл не является библиотекой,
            // порождает исключение ELF_ERROR_TYPE
        Elf32_Word          LibSymSize() throw(ELF_ERROR);

            // возвращает индекс элемента содержащего данный символ
            // если символ не найден, возвращает (~0)
        Elf32_Word SearchSymbol( const char * Name );

            // записывает элемент библиотеки Elf в файл name
        void ExtractToFile( ELF * Elf, const char * name ) throw(ELF_ERROR);

            //------------------------------------------//
            // Интерфейс создания библиотеки            //
            // функции работающие только с библиотеками //
            //------------------------------------------//

            // создает новый заголовок библиотеки
        Elf32_Lhdr * NewLhdr() throw(ELF_ERROR);

            // добавляет элемент Elf к библиотеке. Возвращает
            // дескриптор нового элемента библиотеки
            // date задает время создания файла
            // После добавления файла к библиотеке не следует производить
            // изменений в ELF файле, точнее в таблице символов файла
            // т.к. эти изменения не будут внесены в библиотеку
        ELF * AddElf( ELF * Elf, time_t date ) throw (ELF_ERROR);

            // отсоединяет файл от библиотеки
            // возвращает дескриптор ELF файла.
            //
        ELF * DetachElf( ELF * Elf ) throw (ELF_ERROR);

            // Update( ELF_C_NULL ) - вычисляет смещения всех объектов
            // в файле. физической записи не производит
            // Update( ELF_C_WRITE ) - вычисляет смещения всех объектов
            // в файле. записывает файл на диск,
            // возвращает размер файла
        Elf32_Off Update(ELF_CMD c) throw(ELF_ERROR);
};

#endif  // __LIBELF_H
