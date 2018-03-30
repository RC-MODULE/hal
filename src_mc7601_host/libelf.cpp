//////////////////////////////////////////////////////////////////////////
//                                                                      //
//             Базовое программное обеспечение нейропроцессора          //
//                                                                      //
//     Библиотека доступа к объектным и исполняемым файла формата ELF   //
//                                                                      //
//                     Реализационный модуль libelf.cpp                 //
//                                                                      //
//      Файл реализации методов и функций библиотеки доступа с файлам   //
//                             формата ELF                              //
//                                                                      //
//                                                                      //
// Copyright (c) 1997-2008 RC Module                                    //
//  If this code works, it was written by Alex Ruzavin                  //
//  If not, I don't know who wrote it.                                  //
//                                                                      //
// $Revision:: 9     $      $Date:: 21.06.10 15:40   $                  //
//                                                                      //
//////////////////////////////////////////////////////////////////////////

#include <iostream>
#include <fstream>
#include <string.h>

#include <strstream>

#include "libelf.h"

    // define and initialize host (_HOST_ENCODING)
    // and target (_TARG_ENCODING) machine data encoding
    // ELFDATA2LSB - native Intel, etc.; ELFDATA2MSB - E90, etc.
#include "elf_set.h"

using namespace std;


static const Elf32_Off E_PHOFF = EI_NIDENT + 12;
                                        // + ELF32_T_ADDR.Size()
                                        // + ELF32_T_WORD.Size()
                                        // + 2 * ELF32_T_HALF.Size();

static const Elf32_Off E_SHOFF = E_PHOFF + 4; // ELF32_T_OFF.Size();

Elf32_Ehdr::Elf32_Ehdr()
{
    e_ident[EI_MAG0]  = ELFMAG0;
    e_ident[EI_MAG1]  = ELFMAG1;
    e_ident[EI_MAG2]  = ELFMAG2;
    e_ident[EI_MAG3]  = ELFMAG3;
    e_ident[EI_CLASS]  = ELFCLASS32;
    e_ident[EI_DATA] = ELFDATANONE;
    e_ident[EI_VERSION] = EV_CURRENT;
    for (int i = EI_PAD; i < EI_NIDENT;i++) e_ident[i] = 0;
    e_type = ET_NONE;
    e_machine = EM_NEUROCHIP;
    e_version = EV_CURRENT;
    e_entry = Elf32_Addr(0);
    e_phoff = Elf32_Off(0);
    e_shoff = Elf32_Off(0);
    e_flags = Elf32_Word(0);
    e_ehsize = Elf32_Half(ELF32_T_EHDR_FSIZE);
    e_phentsize = Elf32_Half(ELF32_T_PHDR_FSIZE);
    e_phnum = Elf32_Half(0);
    e_shentsize = Elf32_Half(ELF32_T_SHDR_FSIZE);
    e_shnum = Elf32_Half(0);
    e_shstrndx = SHN_UNDEF;
}

Elf32_Ehdr::Elf32_Ehdr(Elf32_Half TargetProc)
{
	e_ident[EI_MAG0]  = ELFMAG0;
	e_ident[EI_MAG1]  = ELFMAG1;
	e_ident[EI_MAG2]  = ELFMAG2;
	e_ident[EI_MAG3]  = ELFMAG3;
	e_ident[EI_CLASS]  = ELFCLASS32;
	e_ident[EI_DATA] = ELFDATANONE;
	e_ident[EI_VERSION] = EV_CURRENT;
	for (int i = EI_PAD; i < EI_NIDENT;i++) e_ident[i] = 0;
	e_type = ET_NONE;
	if((TargetProc == EM_NEUROCHIP) || (TargetProc == EM_NEUROCORE01) ||
				(TargetProc == EM_NM6405) || (TargetProc == EM_SOC))
	{
		e_machine = TargetProc;
	}
	else
	{
		throw (ELF_ERROR_EHDR("Unknown target processor.\n"));
	}
	e_version = EV_CURRENT;
	e_entry = Elf32_Addr(0);
	e_phoff = Elf32_Off(0);
	e_shoff = Elf32_Off(0);
	e_flags = Elf32_Word(0);
	e_ehsize = Elf32_Half(ELF32_T_EHDR_FSIZE);
	e_phentsize = Elf32_Half(ELF32_T_PHDR_FSIZE);
	e_phnum = Elf32_Half(0);
	e_shentsize = Elf32_Half(ELF32_T_SHDR_FSIZE);
	e_shnum = Elf32_Half(0);
	e_shstrndx = SHN_UNDEF;
}

ostream& operator<< (ostream& s,ELF_ERROR& e)
{
    if (e.errormsg) s << e.errormsg;
    if (e.msg1) s << e.msg1;
    if (e.msg2) s << e.msg2;
    if (e.msg3) s << e.msg3;

    return s;
}

ELF::ELF ( const char *name, ELF_CMD c )
    throw (ELF_ERROR)
    : ehdr(0),
    phdr(0),
    listscn(0),
    cmd(c),
    filename(name),
    Fill_Character(0),
    flags(0),
    eflags(0),
    pflags(0),
    base_offset(0),
    nextelf(0),
    FileExist( 1 ),
    DeleteFile( 1 )
{
    fstream * file = NULL;

    try {
        file = new fstream;
        if( file == NULL )
            throw ELF_ERROR("Elf error - Cann't create fstream");

        if (c == ELF_C_READ)
            file->open(filename,ios::in | ios::binary);
        else if (c == ELF_C_WRITE) {
                FileExist = 0;
                // не создаем файл.
                // создаем его в ELF::Update().
                //f.open(filename,ios::out | ios::binary | ios::trunc);

                // Изменение - не ставим флаг DIRTY для того чтобы не
                // форсировать создание файла
                //flags = ELF_F_DIRTY;
            }
            else {
                if (c == ELF_C_RDWR)
                    file->open(filename,ios::in | ios::out | ios::binary);
                else
                    throw ELF_ERROR_UNKNOWN("Elf error - Unknown file open command");
            }

        if (!(*file))
            throw ELF_ERROR_FILE("Elf error - Could not open file ",filename);

        pfile = file;
        if (cmd != ELF_C_WRITE) {
            if (Kind() == ELF_K_NONE)
                throw ELF_ERROR_TYPE("Elf error - unknown file type");
            GetEhdr();
        }
    }
	catch( ELF_ERROR & e )
	{
		if ( file ) delete file;
		throw e;
	}
    catch( ... ) {
        if ( file ) delete file;
        throw;
    }
}

// protected constructor for use in derived class

ELF::ELF( iostream * pf,const char * name,Elf32_Off offset, ELF_CMD c,ELF *next = 0) throw(ELF_ERROR)
    : ehdr(0),
    phdr(0),
    listscn(0),
    filename(name),
    Fill_Character(0),
    flags(0),
    eflags(0),
    pflags(0),
    pfile(pf),
    nextelf(next),
    FileExist( 1 ),
    DeleteFile( 0 )
{
//  if ((c != ELF_C_READ) && (c != ELF_C_WRITE))
//      throw ELF_ERROR_UNKNOWN("Elf error - command ELF_C_RDWR not allowed here");
    cmd = c;
    if (!(*pfile)) throw ELF_ERROR_FILE("Elf error - error open file");
    base_offset = offset;

    if (c == ELF_C_READ) {
        if (Kind() != ELF_K_ELF)
            throw ELF_ERROR_TYPE("Elf error - unknown file type");
        GetEhdr();
    }
}

//

void ELF::checkflags()
{
    flags |= (eflags & ELF_F_DIRTY);
    flags |= (pflags & ELF_F_DIRTY);

    ELF_SCN *ptr = listscn;
    while (ptr) {
        ptr -> checkflags();
        flags |= ((ptr->flags) & ELF_F_DIRTY);
        ptr = ptr->nextScn;
    }
}

void ELF_SCN::checkflags()
{
    flags |= (sflags & ELF_F_DIRTY);

    ELF_DATA *ptr = listdata;
    while (ptr) {
        flags |= ((ptr->flags) & ELF_F_DIRTY);
        ptr = ptr->next;
    }
}

ELF::~ELF() throw (ELF_ERROR)
{
    if (cmd == ELF_C_WRITE || cmd == ELF_C_RDWR) {
        checkflags();
            // ИЗМЕНЕНО 15.10.97 Ч.А.
            // теперь файл записывается только по Update( ELF_C_WRITE );
        if ( flags & ELF_F_DIRTY ) {
        //  Update(ELF_C_WRITE);
        }
    }

    if ( FileExist ) {
        pfile->flush();
        if ( DeleteFile ) ((fstream *)pfile)->close();
    }

    if ( DeleteFile && pfile ) delete pfile;

    if (ehdr) delete ehdr;          ehdr = 0;
    if (listscn) delete listscn;    listscn = 0;
    if (phdr) delete[] phdr;        phdr = 0;
}

Elf32_Off ELF::Update(ELF_CMD c) throw (ELF_ERROR)
{
    if (c != ELF_C_WRITE && c != ELF_C_NULL)
        throw ELF_ERROR_UNKNOWN("Elf error - unknown file Update command");
    if (cmd == ELF_C_READ && c == ELF_C_WRITE)
        throw ELF_ERROR_TYPE("Elf error - attempt write read only file");
    if (cmd == ELF_C_WRITE && ehdr == 0)
        throw ELF_ERROR_EHDR("Elf error - elf header not exist");

    if (Kind() != ELF_K_ELF) throw ELF_ERROR_TYPE("Elf error - not ELF file");

    if (cmd != ELF_C_WRITE) {
            // полностью загружаем файл в память
        if (ehdr == 0) ehdr = GetEhdr();
        if ((phdr == 0) && ehdr->e_phnum)
            phdr = GetPhdr();

        ELF_SCN * ptr = NextScn( NULL );

            // перебираем секции. если данные секции
            // не были еще загружены, то загружаем их
            // При этом секции с флагом ELF_F_DIRTY не изменяются
        while ( ptr ) {
            if ( (ptr->listdata == 0) && ((ptr->flags & ELF_F_DIRTY) == 0))
                (void)(ptr->GetData(0));
            ptr = NextScn( ptr );
        }

        if (cmd == ELF_C_RDWR && c == ELF_C_WRITE && DeleteFile ) {
            fstream tmpf;               // attempt truncate file before write.
            tmpf.open(filename , ios::out | ios::trunc | ios::binary); // ??
            tmpf << '\0';
            tmpf.close();
        }
    }

        // если файл еще не создан, то создаем его здесь
    if ( cmd == ELF_C_WRITE && c == ELF_C_WRITE && FileExist == 0 ) {
        ((fstream *)pfile)->open(filename,ios::out | ios::binary | ios::trunc);
        FileExist = 1;
    }

    if (ehdr->e_ident[EI_CLASS] == ELFCLASS64)
        throw ELF_ERROR_TYPE("Elf error - unimplemented class 64 bit");

    if (ehdr->e_ident[EI_CLASS] != ELFCLASS32)
        throw ELF_ERROR_TYPE("Elf error - unknown class");

        // Byte e_ident[EI_DATA] specifies the data encoding of the
        // processor-specific data in the object file.
    if ( ehdr->e_ident[EI_DATA] == ELFDATANONE )
        ehdr->e_ident[EI_DATA] = _TARG_ENCODING;

    if ( ehdr->e_ident[EI_VERSION] > EV_CURRENT )   // || (ehdr->e_ident[EI_VERSION] < EV_NONE))
        throw ELF_ERROR_TYPE("Elf error - unknown version");

    if (ehdr->e_ident[EI_VERSION] == EV_NONE)
        ehdr->e_ident[EI_VERSION] = EV_CURRENT;

    ehdr->e_ident[EI_MAG0] = ELFMAG0;
    ehdr->e_ident[EI_MAG1] = ELFMAG1;
    ehdr->e_ident[EI_MAG2] = ELFMAG2;
    ehdr->e_ident[EI_MAG3] = ELFMAG3;

    ehdr->e_ehsize = (Elf32_Half)ELF32_T_EHDR_FSIZE;

    Elf32_Off phdroff = 0;
    Elf32_Off shdroff = 0;
    Elf32_Off filesize = 0;

    if (c == ELF_C_WRITE) {
        if (!(*pfile)) throw ELF_ERROR_FILE("Elf file ",filename," error");
        pfile->seekp(0 + base_offset);
        write(*ehdr);
        ClearFlagEhdr(ELF_F_DIRTY);
    }
    filesize += ELF32_T_EHDR_FSIZE;

    if (phdr) {
        phdroff = (c == ELF_C_WRITE) ? ((Elf32_Off)pfile->tellp() - base_offset) : filesize;
        if (c == ELF_C_WRITE) {
            for (Elf32_Word i = 0;i < ehdr->e_phnum; i++) {  // to know < or <= ?
                write(phdr[i]);
                ClearFlagPhdr(ELF_F_DIRTY);
            }
        }
        filesize += ELF32_T_PHDR_FSIZE * ehdr->e_phnum;
        ehdr->e_phentsize = (Elf32_Half) ELF32_T_PHDR_FSIZE;
        SetFlagEhdr(ELF_F_DIRTY);
    }

    if (listscn)
        listscn->write( c , filesize); // write section

    Elf32_Shdr *sptr = NULL;

        // если не установлен флаг ELF_F_NOSHDR, то записываем таблицу
        // заголовков секций
        // пока игнорируем данный флаг
//    if ( (flags & ELF_F_NOSHDR) == 0)
        sptr = buildshdr();
    // else; не используется

    if (sptr) {
        shdroff = (c == ELF_C_WRITE) ? ((Elf32_Off)pfile->tellp() - base_offset)
                                     : filesize;
        if (c == ELF_C_WRITE) {
            if ( ehdr->e_shnum != 0 ) {     // если есть секции для записи
                for (Elf32_Word i = 0; i < ehdr->e_shnum; i++) {
                    write(sptr[i]);
                    GetScn(i)->ClearFlagShdr(ELF_F_DIRTY);
                }
            }
        }
        if ( ehdr->e_shnum != 0 )
            filesize += ELF32_T_SHDR_FSIZE * ( ehdr->e_shnum );
                                            // Don't add SHN_UNDEF section !

        ehdr->e_shentsize = (Elf32_Half)ELF32_T_SHDR_FSIZE;
        SetFlagEhdr(ELF_F_DIRTY);

        delete[] sptr;
    }

    if (c == ELF_C_WRITE)
        if ( filesize != ((Elf32_Off)pfile->tellp() - base_offset) )
            throw ELF_ERROR_INTERNAL("Elf internal error - different file size");

    ehdr->e_phoff = phdroff;
    ehdr->e_shoff = shdroff;

    if (c == ELF_C_WRITE) {
        pfile->seekp(0 + base_offset);  // write Elf header with correct offsets
        write(*ehdr);
        ClearFlagEhdr(ELF_F_DIRTY);

        pfile->flush();
        if (!(*pfile)) throw ELF_ERROR_FILE("Elf error saving file",filename);
        ClearFlag(ELF_F_DIRTY);
    }

    return filesize;
}

ostream& ELF::write(Elf32_Ehdr& ehdr_data) throw(ELF_ERROR)
{
    if (ehdr == 0)
        GetEhdr();
//      throw ELF_ERROR_EHDR("Elf error - Elf header not loaded or created");

    int e = ehdr->e_ident[EI_DATA];

    ELF::ToF(ELF32_T_EHDR,(unsigned char *)&ehdr_data,(unsigned char *)&ehdr_data,e,1);
    pfile->write( (char *)&ehdr_data, ELF32_T_EHDR_FSIZE );
    ELF::ToM(ELF32_T_EHDR,(unsigned char *)&ehdr_data,(unsigned char *)&ehdr_data,e,1);
    if (!(pfile)) throw ELF_ERROR_FILE("Elf error saving elf header");
    return *pfile;
}

ostream& ELF::write(Elf32_Phdr& phdr_data) throw(ELF_ERROR)
{
    if (ehdr == 0)
        GetEhdr();
//      throw ELF_ERROR_EHDR("Elf error - Elf header not loaded or created");

    int e = ehdr->e_ident[EI_DATA];

    //ELF32_T_PHDR.ToF((unsigned char *)&phdr_data,(unsigned char *)&phdr_data,e);
    ELF::ToF( ELF32_T_PHDR, (unsigned char *)&phdr_data,(unsigned char *)&phdr_data,e,1);
    pfile->write( (char *)&phdr_data, ELF32_T_PHDR_FSIZE );
    ELF::ToM( ELF32_T_PHDR, (unsigned char *)&phdr_data,(unsigned char *)&phdr_data,e,1);
    if (!(*pfile)) throw ELF_ERROR_FILE("Elf error saving elf program table");
    return (*pfile);
}

ostream& ELF::write(Elf32_Shdr& shdr_data) throw(ELF_ERROR)
{
    if (ehdr == 0)
        GetEhdr();
//      throw ELF_ERROR_EHDR("Elf error - Elf header not loaded or created");

    int e = ehdr->e_ident[EI_DATA];

    //ELF32_T_SHDR.ToF((unsigned char *)&shdr_data,(unsigned char *)&shdr_data,e);
    ELF::ToF( ELF32_T_SHDR, (unsigned char *)&shdr_data,(unsigned char *)&shdr_data,e,1);
    pfile->write( (char *)&shdr_data,ELF32_T_SHDR_FSIZE );
    ELF::ToM( ELF32_T_SHDR, (unsigned char *)&shdr_data,(unsigned char *)&shdr_data,e,1);
    if (!(*pfile)) throw ELF_ERROR_FILE("Elf error saving elf section table ");
    return (*pfile);
}

Elf32_Shdr *ELF::buildshdr() throw(ELF_ERROR)
{
    Elf32_Shdr *sptr,*ptr1 = new Elf32_Shdr[(ehdr->e_shnum)];
    ELF_SCN *ptr = listscn;
    sptr = ptr1;

    while(ptr) {
        *ptr1 = *(ptr->shdr);
        ptr1++;
        ptr = ptr->nextScn;
    }
    return sptr;
}

Elf32_Phdr *ELF::GetPhdr() throw(ELF_ERROR)
{
    if (ehdr == 0)
        this->GetEhdr();

    if ( phdr ) return phdr;    // if program segment table already loaded

    if ( ehdr->e_phoff == 0 )
        throw ELF_ERROR_EXIST("Elf error program header table not exist");

    if ( cmd == ELF_C_WRITE ) {
            // Очень странно. Файл создается заново в памяти, а таблицы
            // секций в памяти нет, хотя ehdr->e_phoff указывает, что
            // на диске она есть.
            // Единственно возможный вариант - пользователь сам установил
            // поле ehdr->e_phoff.
            // Вывод: таблица сегментов не существует. Ошибка.
        throw ELF_ERROR_EXIST("Elf error program header table not exist");
    }

    try {
        phdr = new Elf32_Phdr[ehdr -> e_phnum];
        pfile->seekg(ehdr -> e_phoff + base_offset);
        pfile->read((char *)(phdr), (ehdr -> e_phentsize) * (ehdr->e_phnum));

        ELF::ToM( ELF32_T_PHDR,
            (unsigned char *)phdr,
            (unsigned char *)phdr,
            ehdr->e_ident[EI_DATA],
            ehdr->e_phnum );

        if (!(*pfile))
            throw (ELF_ERROR_FILE("Elf error reading file",filename));
        return phdr;
    }
    catch(ELF_ERROR) {
            delete[] phdr;
            phdr = 0;
            throw;
    }
}

Elf32_Phdr *ELF::NewPhdr(Elf32_Word count) throw (ELF_ERROR)
{
    if (ehdr == 0)
        GetEhdr();

    if (phdr) {
            // Здесь мы не разделяем случаи, когда таблица сегментов
            // не существует и когда еще не загружена, т.к. в любом
            // случае удалять в памяти нечего.
        delete[] phdr;
        phdr = 0;
        ehdr->e_phnum = 0;  // для надежности
    }

        // создаем новую таблицу сегментов
    phdr = new Elf32_Phdr[count];
    ehdr->e_phnum = Elf32_Half(count);
    ehdr->e_phentsize = (Elf32_Half)ELF32_T_PHDR_FSIZE;
    pflags |= ELF_F_DIRTY;
    return phdr;
}

Elf32_Ehdr *ELF::GetEhdr(Elf32_Half TargetProc) throw (ELF_ERROR)
{
    if (ehdr) return ehdr;
    if (cmd == ELF_C_WRITE)
        throw (ELF_ERROR_EHDR("Elf error - Elf header not exist"));
    if (Kind() != ELF_K_ELF)
        throw ELF_ERROR_TYPE("Elf error - not ELF file");
    try {
        ehdr = new Elf32_Ehdr(TargetProc);
        pfile->seekg(0 + base_offset);
        pfile->read((char *)ehdr, ELF32_T_EHDR_FSIZE );
        if (!(*pfile)) throw ELF_ERROR_FILE("Elf error - error read elf header");

        if ( ehdr->e_ident[ EI_CLASS ] != ELFCLASS32 )
            throw ELF_ERROR_CLASS("Elf error - cannot read 64 bit class file");

        ELF::ToM( ELF32_T_EHDR,
                (unsigned char *)(ehdr),
                (unsigned char *)(ehdr),
                ehdr->e_ident[EI_DATA],
                1);

        return ehdr;
    }
    catch(ELF_ERROR) {
        delete ehdr;
        ehdr = 0;
        throw;
    }
}

Elf32_Ehdr *ELF::NewEhdr(Elf32_Half TargetProc) throw (ELF_ERROR)
{
    if (ehdr) return ehdr;
    if (cmd != ELF_C_WRITE)
        return GetEhdr(TargetProc);

    ehdr = new Elf32_Ehdr(TargetProc);
    SetFlagEhdr(ELF_F_DIRTY);
    return ehdr;
}

char *ELF::GetIdent(unsigned char *ptr) throw (ELF_ERROR)
{
    char *sptr = 0;

    try {
        if (ptr == 0) sptr = new char[EI_NIDENT];
        else sptr = (char *)(ptr);
        if (ehdr) {
            memcpy(sptr,ehdr->e_ident,EI_NIDENT);
            return sptr;
        }
        pfile->seekg(0 + base_offset);
        pfile->read((char *)(sptr),EI_NIDENT);  // Normal !
        if (!(pfile)) throw (ELF_ERROR_FILE("Elf error reading file identification byte"));
        return sptr;
    }
    catch(ELF_ERROR) {
        if (ptr == 0 && sptr) delete sptr;
        throw;
    }
}

void ELF::readshdr() throw(ELF_ERROR)
{
    Elf32_Shdr *sptr = new Elf32_Shdr[ehdr->e_shnum];
    pfile->seekg(ehdr->e_shoff + base_offset);

    pfile->read( (char *)sptr , ELF32_T_SHDR_FSIZE * ehdr->e_shnum );
    ELF::ToM( ELF32_T_SHDR,
            (unsigned char *)sptr,
            (unsigned char *)sptr,
            ehdr->e_ident[EI_DATA],
            ehdr->e_shnum);

    if (!(*pfile)) {
        delete[] sptr;
        throw (ELF_ERROR_FILE("Elf error loading section header"));
    }

    for (long i = ehdr->e_shnum - 1;i >= 0; i--)
        listscn = new ELF_SCN( this, i, ELF_F_NONE,ELF_F_NONE,
            listscn, 0L, &sptr[i] );
    if( listscn == NULL )
    {
        if( sptr ) delete[] sptr;
        throw;
    }

    delete[] sptr;

    try {
        ELF_SCN *ptr = listscn;
        while(ptr) {
            ptr->name = (ehdr->e_shstrndx)
                            ? StrPtr(ehdr->e_shstrndx,ptr->shdr->sh_name)
                            :  0;

            ptr = ptr->nextScn;
        }
    }
    catch (ELF_ERROR) {
        throw;
    }

}

ELF_SCN *ELF::GetScn(Elf32_Word Index) throw (ELF_ERROR)
{
    if (ehdr == 0)
        GetEhdr();

    if (listscn == 0) {
        if (ehdr->e_shoff == 0 || ehdr->e_shnum == 0 || cmd == ELF_C_WRITE)
            throw ELF_ERROR_EXIST("Elf error - no sections in file");

        readshdr();
    }

    if (Index >= ehdr->e_shnum)
        throw (ELF_ERROR_EXIST("Elf error - section not exist"));
    if (Index >= SHN_LORESERVE)
        throw ELF_ERROR_EXIST("Elf error - request non-phisical section");

    ELF_SCN *ptr = listscn;
    while (ptr) {
        if (ptr->Index() == Index) return ptr;
        ptr = ptr->nextScn;
    }
    throw ELF_ERROR_INTERNAL("Elf internal error: section not found.");
#if _MSC_VER <= MY_MSC_VER && defined(_MSC_VER)
    return 0;
#endif
}

ELF_SCN *ELF::NewScn() throw(ELF_ERROR)
{
    Elf32_Shdr *sptr;
    ELF_SCN *ptr;
    if (ehdr == 0)
        GetEhdr();

    if (listscn == 0) {
        if (cmd != ELF_C_WRITE && ehdr->e_shoff) {
            readshdr();
        }
        else {
            try {
                sptr = 0;
                ptr = 0;
                sptr = new Elf32_Shdr[2];
                ptr  = new ELF_SCN(
                                this,
                                1,
                                ELF_F_INMEM | ELF_F_DIRTY,
                                ELF_F_DIRTY,
                                0,
                                0,
                                sptr + 1);

                listscn = new ELF_SCN(
                                this,
                                0,
                                ELF_F_INMEM | ELF_F_DIRTY,
                                ELF_F_DIRTY,
                                ptr,
                                0,
                                sptr);

                ehdr->e_shnum = 2;  // секции считаются вместе с пустой
                SetFlagEhdr(ELF_F_DIRTY);
                listscn->SetFlag(ELF_F_DIRTY);
                delete[] sptr;
                return ptr;
            }
            catch(ELF_ERROR) {
                if (sptr) delete[] sptr;
                if (ptr) delete ptr;
                if (listscn) delete listscn;
                throw;
            }
        }
    }

//  if (shdr) delete shdr;
    try {
        sptr = 0;
        ptr = listscn;
        while(ptr->nextScn) ptr = ptr->nextScn;
        sptr = new Elf32_Shdr;
        ptr->nextScn = new ELF_SCN(this,ehdr->e_shnum,
                        ELF_F_INMEM | ELF_F_DIRTY,ELF_F_DIRTY,0,0,sptr);

        ehdr->e_shnum++;    // увеличиваем число секций на одну
        delete sptr;
        ptr->nextScn->SetFlag(ELF_F_DIRTY);
        SetFlagEhdr(ELF_F_DIRTY);
        return ptr->nextScn;
    }
    catch(ELF_ERROR) {
        if (sptr) delete sptr;
        throw;
    }
}

ELF_SCN *ELF::NextScn(ELF_SCN *scn) throw(ELF_ERROR)
{
    if (ehdr == 0)
        GetEhdr();

    if (scn == 0) {
        if (listscn == 0) {
            if (ehdr->e_shoff == 0 || ehdr->e_shnum == 0 || cmd == ELF_C_WRITE)
                throw ELF_ERROR_EXIST("Elf error - no sections to get");

            readshdr();
        }
        return listscn->nextScn;
    }

    if (scn->elf != this)
        throw ELF_ERROR_TYPE("Elf error - elf does not own section");

    return scn->nextScn;
}

ELF_KIND ELF::Kind() throw (ELF_ERROR)
{
    unsigned char *p;
    char s[4];

    if (ehdr == 0) {
        pfile->seekg(0 + base_offset);
        pfile->read(s,4);
        if (!(*pfile)) throw ELF_ERROR_FILE("Elf Error loadind file type bytes");
        p = (unsigned char *)s;
    }
    else p = ehdr->e_ident;

    if ((p[EI_MAG0] == ELFMAG0) && (p[EI_MAG1] == ELFMAG1) &&
            (p[EI_MAG2] == ELFMAG2) && (p[EI_MAG3] == ELFMAG3))
                    return ELF_K_ELF;
    if ((p[EI_MAG0] == LIBMAG0) && (p[EI_MAG1] == LIBMAG1) &&
            (p[EI_MAG2] == LIBMAG2) && (p[EI_MAG3] == LIBMAG3))
                    return ELF_K_LIB;

    return ELF_K_NONE;
}

char *ELF::StrPtr(Elf32_Word section, Elf32_Word offset) throw(ELF_ERROR)
{
    if (ehdr == 0)
        GetEhdr();

    ELF_SCN *scn = GetScn(section);
    Elf32_Shdr *sptr = scn->GetShdr();
    if (sptr->sh_type != SHT_STRTAB)
        throw ELF_ERROR_TYPE("Elf error - not STRTAB section");
    if (offset > sptr->sh_size)
        throw ELF_ERROR_EXIST("Elf error - invalid offset in section SHT_STRTAB");

    ELF_DATA *data = scn->GetData(0);
    while(data) {
        if (offset < (data->d_offset + data->d_size))
            return ((char *)data->d_buf) + offset;
        data = scn->GetData(data);
    }
    throw ELF_ERROR_INTERNAL("Elf internal error - no such data buffer");

    return 0;
}

//----------------------------------------------------------------------//
// функция читает программный сегмент в память.
// параметр Phdr задает программный сегмент.
// параметр Buffer задает буфер в который читается сегмент
// буфер должен быть достаточного размера для того, чтобы вместить
// даные сегмента.
//----------------------------------------------------------------------//

void ELF::LoadProgramSegment( Elf32_Phdr *Phdr , char *Buffer, int SkipZeros )
    throw (ELF_ERROR)
{
    pfile->seekg( Phdr->p_offset );

    if (!(pfile))
        throw ELF_ERROR_FILE("Elf Error - can not load program segment");

    if ( Phdr->p_filesz != 0 ) {
        pfile->read(Buffer, Phdr->p_filesz );
        if (!(*pfile))
            throw ELF_ERROR_FILE("Elf Error - can not load program segment");
    }

    if ( Phdr->p_memsz < Phdr->p_filesz )
        throw ELF_ERROR_TYPE("Elf Error - invalid size of segment");

    if ( SkipZeros == 0 ) {
        if ( Phdr->p_memsz != Phdr->p_filesz ) {
            memset( Buffer+Phdr->p_filesz, '\0', Phdr->p_memsz-Phdr->p_filesz );
        }
    }
}

//----------------------------------------------------------------------//
//                                                                      //
// Функции рабатающие с именами секций                                  //
//                                                                      //
//----------------------------------------------------------------------//

//============================================================================
//  Функция sectionFlagsFromName находит стандартные значения 
//  флагов для секций с предопределенными именами 
//=============================================================================void LINKER::CreateInitialAttributes()

Elf32_Word
ELF::sectionFlagsFromName( const char* name )
{
	if ( !strncmp( name, ".bss", 4 ))	//	имена, _начинающиеся_ с .bss
		return SHF_ALLOC | SHF_WRITE;
	if ( !strcmp( name, ".data" ))
		return SHF_ALLOC | SHF_WRITE;
	if ( !strcmp( name, ".fini" ))
		return SHF_ALLOC | SHF_EXECINSTR;
	if ( !strncmp( name, ".heap", 5 ))
		return SHF_ALLOC | SHF_WRITE;
	if ( !strcmp( name, ".init" ))
		return SHF_ALLOC | SHF_EXECINSTR;
	if ( !strcmp( name, ".stack" ))
		return SHF_ALLOC | SHF_WRITE;
	if ( !strcmp( name, ".text" ))
		return SHF_ALLOC | SHF_EXECINSTR;
	return SHF_ALLOC;
}
    // создает секцию ".shstrtab". добавляет в нее имя самой этой
    // секции.
ELF_SCN * ELF::NewSHSTRTAB() throw(ELF_ERROR)
{
    if ( ehdr->e_shstrndx != 0 )
        throw ELF_ERROR_EXIST("Elf error - ",
        "section \".shstrtab\" already exist");

    ELF_SCN     *scn;
    ELF_DATA    *ptr;
    char *pzero;

    pzero = new char(0);
    if( pzero == NULL )
    {
        throw;
    }
    scn = NewScn();

    Elf32_Shdr *s = scn->GetShdr();

    s->sh_name = 0;
    s->sh_type = SHT_STRTAB;
    s->sh_addralign = 1;
    s->sh_entsize = 0;
    ehdr->e_shstrndx = Elf32_Half(scn->Index());
    // Add new data buffer with one (1) zero byte !
    ptr = scn->NewData();
    ptr->d_size = 1;
    ptr->d_buf = pzero;
    ptr->d_type = ELF32_T_BYTE;
    ptr->d_align = ELF32_T_BYTE_FSIZE;
    ptr->d_version = EV_CURRENT;

    scn->AttachName(".shstrtab");       // name for .shstrtab itself

    return scn;
}

char *ELF_SCN::GetName() throw(ELF_ERROR)
{
    if (name) return name;

    if (elf->ehdr->e_shstrndx == 0) return 0;
    if (this->shdr->sh_name == 0) return 0; // removed 31 may for error free

    name = elf->StrPtr(elf->ehdr->e_shstrndx,shdr->sh_name);
    return name;
}

char *ELF_SCN::AttachName(const char *sptr) throw(ELF_ERROR)
{
    ELF_SCN     *scn;
    ELF_DATA    *ptr;


    if (this->GetName())
        throw ELF_ERROR_EXIST("Elf error - name of section already exist");
    if (this->Index() == SHN_UNDEF)
    throw ELF_ERROR_TYPE("Elf error - SHN_UNDEF section can't have name");

    if (elf->ehdr->e_shstrndx == 0) {
        void *pzero = (void *) new char(0);
        scn = elf->NewScn();

        Elf32_Shdr *s = scn->GetShdr();

        s->sh_name = 0;
        s->sh_type = SHT_STRTAB;
        s->sh_addralign = 1;
        s->sh_entsize = 0;
        elf->ehdr->e_shstrndx = Elf32_Half(scn->Index());
            // Add new data buffer with one (1) zero byte !
        ptr = scn->NewData();
        ptr->d_size = 1;
        ptr->d_buf = pzero;
        ptr->d_type = ELF32_T_BYTE;
        ptr->d_align = ELF32_T_BYTE_FSIZE;
        ptr->d_version = EV_CURRENT;

        scn->AttachName(".shstrtab");       // name for .shstrtab itself
    }
    else    {
        scn = elf->GetScn(elf->ehdr->e_shstrndx);
//      s = scn->GetShdr();
    }
            //  Add new data buffer with name of section
    void *pzero = (void *) new char[strlen(sptr) + 1]; ;

    ptr = scn->NewData();
    ptr->d_size = strlen(sptr) + 1;
    ptr->d_buf = pzero;
    ptr->d_type = ELF32_T_BYTE;
    ptr->d_align = ELF32_T_BYTE_FSIZE;
    ptr->d_version = EV_CURRENT;

    strncpy((char *)ptr->d_buf,sptr,ptr->d_size - 1);
    ((char *)(ptr->d_buf))[ptr->d_size - 1 ] = '\0';
    name = (char *)ptr->d_buf;

    elf->Update(ELF_C_NULL);                    // ??
    shdr->sh_name = ptr->d_offset;              // ??

    return name;
}

ELF_SCN::ELF_SCN(ELF *e,Elf32_Word d,unsigned f,unsigned sf,ELF_SCN *next,
                ELF_DATA *data,Elf32_Shdr *sh) throw (ELF_ERROR)
    : elf(e), ndex(d),flags(f), sflags(sf), nextScn(next), listdata(data),name(0)
{
    shdr = 0;
    shdr = new Elf32_Shdr;
    shdr->sh_name = sh->sh_name;
    shdr->sh_type = sh->sh_type;
    shdr->sh_flags = sh->sh_flags;
    shdr->sh_addr = sh->sh_addr;
    shdr->sh_offset = sh->sh_offset;
    shdr->sh_size = sh->sh_size;
    shdr->sh_link = sh->sh_link;
    shdr->sh_info = sh->sh_info;
    shdr->sh_addralign = sh->sh_addralign;
    shdr->sh_entsize = sh->sh_entsize;
}

ELF_SCN::~ELF_SCN () throw(ELF_ERROR)
{
    if ((flags & ELF_F_DIRTY) ||
            ((sflags & ELF_F_DIRTY) && ((elf->flags & ELF_F_NOSHDR) == 0)) ) {
        if (elf->cmd != ELF_C_READ) {
            // Убрана проверка 15.10.97 Ч.А.
            // throw ELF_ERROR_EXIST("Elf error - discard dirty section"); // throw ?
        }
    }

    ELF_DATA *ptr = listdata;
    ELF_DATA *ptr1;
    while(ptr) {
        ptr1 = ptr->next;
        delete ptr;
        ptr = ptr1;
    }

    if (nextScn) delete nextScn;
    if (shdr) delete shdr;
}

void ELF_SCN::elf_internal_load_section(Elf_Type t) throw(ELF_ERROR)
{
    Elf32_Word typeSize = Elf32Fsize( t, 1, EV_CURRENT ); // t.Size();

    (elf->pfile)->seekg(shdr->sh_offset + (elf->base_offset));
    listdata->d_buf = (void *) new char[shdr->sh_size];
    listdata->d_type = t;
    listdata->d_size = shdr->sh_size;
    listdata->d_align = typeSize;
    listdata->d_offset = shdr->sh_offset;

        // если у нас служебные секции, то устанавливаем
        // выравнивание в два слова
    if ( t == ELF32_T_SYM || t == ELF32_T_REL || t == ELF32_T_RELA )
        listdata->d_align = 2 * ELF32_T_WORD_FSIZE;

    (elf->pfile)->read((char *)listdata->d_buf,listdata->d_size);
    if (!(*(elf->pfile)))
        throw ELF_ERROR_FILE("Elf error - error read file data");

    ELF::ToM( t,
            (unsigned char *)listdata->d_buf,
            (unsigned char *)listdata->d_buf,
            elf->ehdr->e_ident[EI_DATA],
            (listdata->d_size) / typeSize);
}

ELF_DATA *ELF_SCN::GetData(ELF_DATA *data) throw(ELF_ERROR)
{
    if (Index() == SHN_UNDEF)
        throw ELF_ERROR("Elf error - SHN_UNDEF section don't have data");

    if (elf->cmd != ELF_C_WRITE && listdata == 0) {

        try {
            listdata = 0;
            listdata = new ELF_DATA();
            listdata->section = this;
            listdata->nrefs = 1;
            listdata->d_version = EV_CURRENT;
            listdata->d_buf = 0;
            listdata->d_size = 0;
            listdata->next = 0;

            switch (shdr->sh_type) {
            case SHT_NULL:
                listdata->d_buf = 0;
                listdata->d_size = 0;
                listdata->d_type = ELF32_T_BYTE;
                listdata->d_align = 0;
                listdata->d_offset = 0;
                break;
            case SHT_NOBITS:                    // Add more section types !!! ??
                listdata->d_buf = 0;
                listdata->d_size = shdr->sh_size;
                listdata->d_type = ELF32_T_BYTE;
                listdata->d_align = 0;
                listdata->d_offset = 0;
                break;
            case SHT_PROGBITS:
                elf_internal_load_section(ELF32_T_BYTE);
                break;
            case SHT_REL:
                elf_internal_load_section(ELF32_T_REL);
                break;
            case SHT_RELA:
                elf_internal_load_section(ELF32_T_RELA);
                break;
            case SHT_STRTAB:
                elf_internal_load_section(ELF32_T_BYTE);
                break;
            case SHT_SYMTAB:
                elf_internal_load_section(ELF32_T_SYM);
                break;
    //      case SHT_NOTE:
    //          elf_internal_load_section(ELF32_T_NOTE);
    //          break;
            default:
                elf_internal_load_section(ELF32_T_BYTE);
                break;
            }
        }
        catch(ELF_ERROR) {
            if (listdata) {
                listdata->ClearFlag(ELF_F_DIRTY);
                delete listdata;
            }
            throw;
        }
    }

    if (data == 0) return  listdata;

    if (data->section != this)
        throw ELF_ERROR_TYPE("Elf error - section does not own data");

    return data->next;
}

ostream& ELF_SCN::write(ELF_CMD c, Elf32_Off &filesize) throw(ELF_ERROR)
{
    Elf32_Word  align;
    Elf32_Off   offset;
    ELF_SCN *scn = this;
    ELF_DATA *ptr;

    if (elf->ehdr == 0)
        throw ELF_ERROR_EHDR("Elf error - Elf header not loaded or created");

    while(scn) {
        if (scn->Index() == SHN_UNDEF) {
            if (c == ELF_C_WRITE) scn->ClearFlag(ELF_F_DIRTY);
            scn->shdr->sh_entsize = 0;

            scn->shdr->sh_offset = 0;
            scn->shdr->sh_size = 0;
            scn->shdr->sh_addralign = 0;
            scn->SetFlagShdr(ELF_F_DIRTY);
        }
        if (scn->shdr->sh_type == SHT_NULL) {
            if (c == ELF_C_WRITE) scn->ClearFlag(ELF_F_DIRTY);
            scn = scn->nextScn;
            continue;
        }
        if (scn->shdr->sh_type == SHT_NOBITS) {
            if (c == ELF_C_WRITE) scn->ClearFlag(ELF_F_DIRTY);

/* Теперь не увеличиваем (3.4.97)
                // если выравнивание секции меньше 4-х байт
                // то увеличиваем выравнивание секции
            align = (ELF32_T_WORD.Size()>scn->shdr->sh_addralign)
                ? ELF32_T_WORD.Size()
                : scn->shdr->sh_addralign;
*/
            align = scn->shdr->sh_addralign;
            if ( align == 0 )
                align = 1;

                // увеличиваем выравнивание секции в соответствии
                // с выравниванием буферов данных
            ptr = scn->listdata;
            while (ptr) {
                align = (ptr->d_align > align) ? ptr->d_align : align;
                ptr = ptr->next;
            }

            scn->shdr->sh_addralign = align;

                // выравниваем секцию в файле
            scn->shdr->sh_offset = filesize;

            offset = scn->shdr->sh_offset + align - 1;
            offset = offset - (offset % align);

            if (offset != scn->shdr->sh_offset) {
                if (c == ELF_C_WRITE) {
                    for(Elf32_Off i = scn->shdr->sh_offset;i < offset;i++)
                        (elf->pfile)->put((unsigned char)scn->elf->Fill_Character);
                }
                scn->shdr->sh_offset = offset;
                filesize = offset;
            }

                // теперь считаем размеры секции
            ptr = scn->listdata;
            offset = 0;
            while (ptr) {
                if (ptr->d_align == 0)
                    ptr->d_align = 1;

                Elf32_Word typeSize = Elf32Fsize( ptr->d_type, 1, EV_CURRENT );

                ptr->d_align = (ptr->d_align > typeSize) ? ptr->d_align : typeSize;

                offset = offset + (ptr->d_align - 1);
                offset = offset - (offset % ptr->d_align);
                ptr->d_offset = offset;
                offset += ptr->d_size;

                ptr = ptr->next;
            }

            if ( offset != 0 )
                scn->shdr->sh_size = offset;

            scn = scn->nextScn;
            continue;
        }
        scn->shdr->sh_offset = (c == ELF_C_WRITE) ?
            ((Elf32_Off)(elf->pfile)->tellp() - elf->base_offset) : filesize;

/* Теперь этого не делаем (3.4.97)
            // ставим минимум 4-х байтовое выравнивание
            // если выравнивание секции меньше 4-х байт
            // то увеличиваем выравнивание секции
        align = (ELF32_T_WORD.Size()>scn->shdr->sh_addralign)
            ? ELF32_T_WORD.Size()
            : scn->shdr->sh_addralign;
*/

        align = scn->shdr->sh_addralign;
        if ( align == 0 )
            align = 1;

            // увеличиваем выравнивание секции в соответствии
            // с выравниванием буферов данных
        ptr = scn->listdata;
        while (ptr) {
            align = (ptr->d_align > align) ? ptr->d_align : align;
            ptr = ptr->next;
        }

        scn->shdr->sh_addralign = align;

        switch(scn->shdr->sh_type) {
            case SHT_SYMTAB:
                scn->shdr->sh_entsize = ELF32_T_SYM_FSIZE;
                scn->shdr->sh_addralign = 2*ELF32_T_WORD_FSIZE;
                break;
            case SHT_REL:
                scn->shdr->sh_entsize = ELF32_T_REL_FSIZE;
                scn->shdr->sh_addralign = 2*ELF32_T_WORD_FSIZE;
                break;
            case SHT_RELA:
                scn->shdr->sh_entsize = ELF32_T_RELA_FSIZE;
                scn->shdr->sh_addralign = 2*ELF32_T_WORD_FSIZE;
                break;
            default:                    // ничего не делаем
                break;
        }

        ptr = scn->listdata;

            // выравниваем начало секции
        align = scn->shdr->sh_addralign;
        offset = scn->shdr->sh_offset + align - 1;
        offset = offset - (offset % align);
        if (offset != scn->shdr->sh_offset) {
            if (c == ELF_C_WRITE) {
                for(Elf32_Off i = scn->shdr->sh_offset;i < offset;i++)
                    (elf->pfile)->put((unsigned char)scn->elf->Fill_Character);
            }
            scn->shdr->sh_offset = offset;
            filesize = offset;
        }

        while(ptr) {
            ptr->d_offset = (c == ELF_C_WRITE) ?
                ((Elf32_Off)(elf->pfile)->tellp() - (elf->base_offset)) : filesize;
            if (ptr->d_align == 0)
                ptr->d_align = 1;
//          ptr->d_align = (ptr->d_align > ptr->d_type.Size()) ?
//                                          ptr->d_align : ptr->d_type.Size();

                // выравниваем буфера данных
            offset = ptr->d_offset + ptr->d_align - 1;
            offset = offset - (offset % ptr->d_align);

            if (offset != ptr->d_offset) {
                if (c == ELF_C_WRITE) {
                    for(Elf32_Off i = ptr->d_offset;i < offset;i++)
                        (elf->pfile)->put((unsigned char)scn->elf->Fill_Character);
                }
                ptr->d_offset = offset;
                filesize = offset;
            }

            ptr->d_offset = ptr->d_offset - scn->shdr->sh_offset;

            if (c == ELF_C_WRITE) {
                    // переводим в файловое представление
                if ((ptr->flags & ELF_F_DISKVIEW) == 0)
                    ptr->XlateToF(ptr,scn->elf->ehdr->e_ident[EI_DATA]);

                (elf->pfile)->write((const char *)(ptr->d_buf),ptr->d_size);

                    // переводим в представление в памяти
                if (ptr->flags & ELF_F_DISKVIEW)
                    ptr->XlateToM(ptr,scn->elf->ehdr->e_ident[EI_DATA]);

                ptr->ClearFlag(ELF_F_DIRTY);
            }
            filesize += ptr->d_size;
            ptr = ptr->next;
            if(!(*(elf->pfile))) throw ELF_ERROR_FILE("Elf error saving section");
        }

        scn->shdr->sh_size = filesize - scn->shdr->sh_offset;
        if (c == ELF_C_WRITE) scn->ClearFlag(ELF_F_DIRTY);
        scn = scn->nextScn;
    }

    return *(elf->pfile);
}

ELF_DATA *ELF_SCN::NewData() throw (ELF_ERROR)
{
    if (Index() == SHN_UNDEF)
        throw ELF_ERROR_TYPE("Elf error - SHN_UNDEF section can't have data");

    if ( listdata == 0 && elf->cmd != ELF_C_WRITE ) {
        GetData(0);
    }

    if (listdata == 0) {
        listdata = new ELF_DATA;
        listdata->section = this;
        listdata->nrefs = 1;
        listdata->SetFlag( ELF_F_DIRTY );   // ставим флаг обновления файла
        return listdata;
    }

    ELF_DATA *ptr = listdata;
    while(ptr->next) ptr = ptr->next;
    ptr->next = new ELF_DATA;
    ptr->next->section = this;
    ptr->next->nrefs = 1;
    ptr->SetFlag( ELF_F_DIRTY );   // ставим флаг обновления файла
    return ptr->next;
}

ELF_DATA::~ELF_DATA() throw (ELF_ERROR)
{
    if ( flags & ELF_F_DIRTY) {
        // Убрана проверка 15.10.97 Ч.А.
        // throw ELF_ERROR_EXIST("Elf internal error - discard dirty data buffer");
    }

    if (d_buf && ((flags & ELF_F_NOTDISCARD) == 0))
        delete [] (char*) d_buf;
}

ELF_DATA *ELF_DATA::XlateToF(ELF_DATA *dest,unsigned encode) throw(ELF_ERROR)
{
    if (dest == 0) throw ELF_ERROR_TYPE("Elf error - null xlate data");
    if (dest->d_buf == 0) throw ELF_ERROR_TYPE("Elf error - null xlate dest buffer");
    if (d_buf == 0) throw ELF_ERROR_TYPE("Elf error - null xlate src buffer");

    if (d_type != dest->d_type)
        throw ELF_ERROR_TYPE("Elf error - incompatible types in XlateToF");
    if (d_size > dest->d_size)
        throw ELF_ERROR_OVERFLOW("Elf error - destination buffer overflow");
    if (encode >= ELFDATANUM)
        throw ELF_ERROR_UNKNOWN("Elf error - unknown data encoding");
    if (d_version == EV_NONE || dest->d_version == EV_NONE ||
                d_version > EV_CURRENT || dest->d_version > EV_CURRENT )
        throw ELF_ERROR_UNKNOWN("Elf error - unknown version");
    if (flags & ELF_F_DISKVIEW)     // ??
        throw ELF_ERROR_TYPE("Elf error - attempting translate file view to file view");

    Elf_Type t = this->d_type;
    if (t == ELF32_T_NONE)
        t = ELF32_T_BYTE;

    unsigned char *ptr  = (unsigned char *)this->d_buf;
    unsigned char *ptr1 = (unsigned char *)dest->d_buf;

    unsigned long TSize = Elf32Fsize( t, 1, EV_CURRENT ); // t.Size();

    ELF::ToF( t, ptr1, ptr, encode, (this->d_size) / TSize);

    dest->d_size = this->d_size;
    dest->SetFlag(ELF_F_DISKVIEW);
    return dest;
}

ELF_DATA *ELF_DATA::XlateToM(ELF_DATA *dest,unsigned encode) throw(ELF_ERROR)
{
    if (dest == 0) throw ELF_ERROR_TYPE("Elf error - null xlate data");
    if (dest->d_buf == 0) throw ELF_ERROR_TYPE("Elf error - null xlate dest buffer");
    if (d_buf == 0) throw ELF_ERROR_TYPE("Elf error - null xlate src buffer");

    if (this->d_type != dest->d_type)
        throw ELF_ERROR_TYPE("Elf error - incompatible types in XlateToF");
    if (this->d_size > dest->d_size)
        throw ELF_ERROR_OVERFLOW("Elf error - destination buffer overflow");
    if (encode >= ELFDATANUM)
        throw ELF_ERROR_UNKNOWN("Elf error - unknown data encoding");
    if (d_version == EV_NONE || dest->d_version == EV_NONE ||
                d_version > EV_CURRENT || dest->d_version > EV_CURRENT )
        throw ELF_ERROR_UNKNOWN("Elf error - unknown version");
    if ((flags & ELF_F_DISKVIEW) == 0)  // ??
        throw ELF_ERROR_TYPE("Elf error - attempting translate mem view to mem view");


    Elf_Type t = this->d_type;
    if (t == ELF32_T_NONE)
        t = ELF32_T_BYTE;

    unsigned char *ptr  = (unsigned char *)this->d_buf;
    unsigned char *ptr1 = (unsigned char *)dest->d_buf;

    unsigned long TSize = Elf32Fsize( t, 1, EV_CURRENT );

    ELF::ToM( t, ptr1, ptr, encode, (this->d_size) / TSize);

    dest->d_size = this->d_size;
    dest->ClearFlag(ELF_F_DISKVIEW);
    return dest;
}

void Elf_Internal_tr_4byte(unsigned char *&dest,unsigned char *&src,unsigned e)
{
    char c[4];
    switch (e) {
        case ELFDATANONE:
        case _HOST_ENCODING:
            c[0] = src[0];
            c[1] = src[1];
            c[2] = src[2];
            c[3] = src[3];
            dest[0] = c[0];
            dest[1] = c[1];
            dest[2] = c[2];
            dest[3] = c[3];
            break;
        default:
            c[0] = src[0];
            c[1] = src[1];
            c[2] = src[2];
            c[3] = src[3];
            dest[0] = c[3];
            dest[1] = c[2];
            dest[2] = c[1];
            dest[3] = c[0];
            break;
    }
    dest +=4;
    src +=4;
}

void Elf_Internal_tr_2byte(unsigned char *&dest,unsigned char *&src,unsigned e)
{
    char c[2];
    switch (e) {
        case ELFDATANONE:
        case _HOST_ENCODING:
            c[0] = src[0];
            c[1] = src[1];
            dest[0] = c[0];
            dest[1] = c[1];
            break;
        default:
            c[0] = src[0];
            c[1] = src[1];
            dest[0] = c[1];
            dest[1] = c[0];
            break;
    }
    dest += 2;
    src += 2;
}


        // Переводит массив данных к представлению в памяти.
void ELF::ToM(
        Elf_Type type,          // тип элементов массива.
        unsigned char *dest,    // исходный массив.
        unsigned char *src,     // массив результата.
        unsigned encode,        // тип кодирования.
        unsigned size           // число элементов в массиве.
    )
{
    unsigned int j;
    unsigned int e = encode;

    if ( (dest == src) && (e == _HOST_ENCODING) ) {
            // Не требуется перекодировки.
        return;
    }

    switch( type ) {
        default: {
            throw ELF_ERROR_TYPE("Elf error - unknown type in ELF::ToM");
        }
        case ELF32_T_ADDR:
        case ELF32_T_OFF:
        case ELF32_T_WORD:
        case ELF32_T_SWORD: {
            for ( j = 0; j < size; j++ ) {
                Elf_Internal_tr_4byte( dest, src, e );
            }

            break;
        }
        case ELF32_T_BYTE: {
            if ( dest == src ) {
                return;
            }

            memcpy( dest, src, size );
            break;
        }
        case ELF32_T_HALF:{
            for ( j = 0; j < size; j++ ) {
                Elf_Internal_tr_2byte( dest, src, e );
            }
            break;
        }
        case ELF32_T_EHDR:{
            for ( j = 0; j < size; j++ ) {
                for (int i =0;i < EI_NIDENT;i++)
                    *dest++ = *src++;
                Elf_Internal_tr_2byte(dest,src,e);  // e_type
                Elf_Internal_tr_2byte(dest,src,e); // e_machine

                Elf_Internal_tr_4byte(dest,src,e);  // e_version
                Elf_Internal_tr_4byte(dest,src,e); // e_entry
                Elf_Internal_tr_4byte(dest,src,e); // e_phoff
                Elf_Internal_tr_4byte(dest,src,e); // e_shoff
                Elf_Internal_tr_4byte(dest,src,e); // e_flags

                Elf_Internal_tr_2byte(dest,src,e); // e_ehsize
                Elf_Internal_tr_2byte(dest,src,e); // e_phentsize
                Elf_Internal_tr_2byte(dest,src,e); // e_phnum
                Elf_Internal_tr_2byte(dest,src,e); // e_shentsize
                Elf_Internal_tr_2byte(dest,src,e); // e_shnum
                Elf_Internal_tr_2byte(dest,src,e); // e_shstrndx
            }
            break;
        }
        case ELF32_T_PHDR:{
            for ( j = 0; j < size; j++ ) {
                Elf_Internal_tr_4byte(dest,src,e);  // p_type
                Elf_Internal_tr_4byte(dest,src,e); // p_offset
                Elf_Internal_tr_4byte(dest,src,e); // p_vaddr
                Elf_Internal_tr_4byte(dest,src,e); // p_paddr
                Elf_Internal_tr_4byte(dest,src,e); // p_filesz
                Elf_Internal_tr_4byte(dest,src,e); // p_memsz
                Elf_Internal_tr_4byte(dest,src,e); // p_flags
                Elf_Internal_tr_4byte(dest,src,e); // p_align
            }
            break;
        }
        case ELF32_T_SHDR:{
            for ( j = 0; j < size; j++ ) {
                Elf_Internal_tr_4byte(dest,src,e);  // sh_name
                Elf_Internal_tr_4byte(dest,src,e); // sh_type
                Elf_Internal_tr_4byte(dest,src,e); // sh_flags
                Elf_Internal_tr_4byte(dest,src,e); // sh_addr
                Elf_Internal_tr_4byte(dest,src,e); // sh_offset

                Elf_Internal_tr_4byte(dest,src,e); // sh_size
                Elf_Internal_tr_4byte(dest,src,e); // sh_link
                Elf_Internal_tr_4byte(dest,src,e); // sh_info
                Elf_Internal_tr_4byte(dest,src,e); // sh_addralign
                Elf_Internal_tr_4byte(dest,src,e); // sh_entsize
            }
            break;
        }
        case ELF32_T_SYM:{
            for ( j = 0; j < size; j++ ) {
                Elf_Internal_tr_4byte(dest,src,e);  // st_name
                Elf_Internal_tr_4byte(dest,src,e);  // st_value
                Elf_Internal_tr_4byte(dest,src,e);  // st_size

                *dest++ = *src++;                   // st_info
                *dest++ = *src++;                   // st_other

                Elf_Internal_tr_2byte(dest,src,e);  // st_shndx
            }
            break;
        }
        case ELF32_T_REL:{
            for ( j = 0; j < size; j++ ) {
                Elf_Internal_tr_4byte(dest,src,e);  // r_offset
                Elf_Internal_tr_4byte(dest,src,e);  // r_info
            }
            break;
        }
        case ELF32_T_RELA:{
            for ( j = 0; j < size; j++ ) {
                Elf_Internal_tr_4byte(dest,src,e);  // r_offset
                Elf_Internal_tr_4byte(dest,src,e); // r_info
                Elf_Internal_tr_4byte(dest,src,e); // r_addend
            }
            break;
        }
        case ELF32_T_LHDR:{
            for ( j = 0; j < size; j++ ) {
                for (int i =0;i < EI_NIDENT;i++)
                    *dest++ = *src++;
                Elf_Internal_tr_4byte(dest,src,e); // l_lhoff
                Elf_Internal_tr_2byte(dest,src,e); // l_lhnum
                Elf_Internal_tr_2byte(dest,src,e); // l_lhentsize
                Elf_Internal_tr_2byte(dest,src,e); // l_lhsymndx
                Elf_Internal_tr_2byte(dest,src,e); // l_lhstrndx
            }
            break;
        }
        case ELF32_T_LIBHDR_F: {
            for ( j = 0; j < size; j++ ) {
                Elf_Internal_tr_4byte(dest,src,e); // l_lhoff
                Elf_Internal_tr_4byte(dest,src,e); // l_lhnum
                Elf_Internal_tr_4byte(dest,src,e); // l_lhentsize
                Elf_Internal_tr_4byte(dest,src,e); // l_lhsymndx
                Elf_Internal_tr_4byte(dest,src,e); // l_lhstrndx
            }
            break;
        }
        case ELF32_T_LIBSYM_F: {
            for ( j = 0; j < size; j++ ) {
                Elf_Internal_tr_4byte(dest,src,e); // ls_name
                Elf_Internal_tr_4byte(dest,src,e); // ls_index
                Elf_Internal_tr_4byte(dest,src,e); // ls_bind
            }
            break;
        }

    }
}

        // Переводит массив данных к представлению в файле.
void ELF::ToF(
        Elf_Type type,          // тип элементов массива.
        unsigned char *dest,    // исходный массив.
        unsigned char *src,     // массив результата.
        unsigned encode,        // тип кодирования.
        unsigned size           // число элементов в массиве.
    )
{
        // Перестановка байтов симметрична.
    ToM( type, dest, src, encode, size );
}

Elf32_Word Elf32Fsize(Elf_Type type,Elf32_Word count,unsigned int ver)
{
    if (ElfVersion(ver) == EV_NONE) return Elf32_Word(0);
    switch( type ) {
        default: {
            throw ELF_ERROR_TYPE("Elf error - unknown type in ELF::ToM");
        }
        case ELF32_T_ADDR:
            return ELF32_T_ADDR_FSIZE*count;

        case ELF32_T_OFF:
            return ELF32_T_OFF_FSIZE*count;

        case ELF32_T_WORD:
            return ELF32_T_WORD_FSIZE*count;

        case ELF32_T_SWORD:
            return ELF32_T_SWORD_FSIZE*count;

        case ELF32_T_BYTE:
            return ELF32_T_BYTE_FSIZE*count;

        case ELF32_T_HALF:
            return ELF32_T_HALF_FSIZE*count;

        case ELF32_T_EHDR:
            return ELF32_T_EHDR_FSIZE*count;

        case ELF32_T_PHDR:
            return ELF32_T_PHDR_FSIZE*count;

        case ELF32_T_SHDR:
            return ELF32_T_SHDR_FSIZE*count;

        case ELF32_T_SYM:
            return ELF32_T_SYM_FSIZE*count;

        case ELF32_T_REL:
            return ELF32_T_REL_FSIZE*count;

        case ELF32_T_RELA:
            return ELF32_T_RELA_FSIZE*count;

        case ELF32_T_LHDR:
            return ELF32_T_LHDR_FSIZE*count;

        case ELF32_T_LIBHDR_F:
            return ELF32_T_LIBHDR_F_FSIZE*count;

        case ELF32_T_LIBSYM_F:
            return ELF32_T_LIBSYM_F_FSIZE*count;
    }
}


static unsigned int _elf_version = EV_NONE;

unsigned int ElfVersion(unsigned ver)
{
    unsigned tmp;

     if (ver == EV_NONE) {
          return EV_CURRENT;
     }

     if (ver > EV_CURRENT)
          return EV_NONE;

     tmp = (_elf_version == EV_NONE) ? EV_CURRENT : _elf_version;
     _elf_version = ver;
     return tmp;
}

void ELF::Fill(int Fill) throw()
{
    Fill_Character = Fill;
}

unsigned long ElfHash(const unsigned char *name)
{
    unsigned long h = 0, g;
    while (*name) {
        h = (h << 4) + *name++;
        g = h & 0xf0000000L;
        if (g) h ^= g >> 24;
        h &= ~g;
    }
    return h;
}


ELF_ERROR_AMBIGUITY::ELF_ERROR_AMBIGUITY(
        const char *s,
        const char *s1,
        const char *s2,
        char *AmName
    ) : ELF_ERROR(s,s1,s2,0),
    eeaAmName( 0 )
{
    eeaAmName = new char[ strlen(AmName) + 1];
    strcpy( eeaAmName, AmName );
    msg3 = AmName;
}

ELF_ERROR_AMBIGUITY::ELF_ERROR_AMBIGUITY( const ELF_ERROR_AMBIGUITY& ErrAm )
    : ELF_ERROR( ErrAm )
{
    msg3 = 0;
    eeaAmName = new char[ strlen(ErrAm.eeaAmName) + 1];
    strcpy( eeaAmName, ErrAm.eeaAmName );
    msg3 = eeaAmName;
}

ELF_ERROR_AMBIGUITY& ELF_ERROR_AMBIGUITY::operator=( const ELF_ERROR_AMBIGUITY& ErrAm )
{
    if ( eeaAmName )
        delete[] eeaAmName;

    eeaAmName = new char[ strlen(ErrAm.eeaAmName) + 1];
    strcpy( eeaAmName, ErrAm.eeaAmName );
    msg3 = eeaAmName;

    return *this;
}
