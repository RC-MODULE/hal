//////////////////////////////////////////////////////////////////////////
//                                                                      //
// NeuroMatrix SDK. ELF library.                                        //
//                                                                      //
// liblib.cpp -                                                         //
//                                                                      //
//      Файл реализации методов и функций для работы с библиотеками     //
//      объектных файлов формата ELF                                    //
//                                                                      //
//                                                                      //
// Copyright (c) 1997-2006 RC Module                                    //
//  If this code works, it was written by Alex Ruzavin                  //
//  If not, I don't know who wrote it.                                  //
//                                                                      //
// $Revision:: 2     $      $Date:: 6/06/06 4:20p    $                  //
//                                                                      //
//////////////////////////////////////////////////////////////////////////

#include <stddef.h>
#include <iostream>
#include <fstream>
#include <string.h>
#include <assert.h>
#include <time.h>

#include "libelf.h"
#include "libint.h"
#include "elf_set.h"

using namespace std;

ELF_INTERNAL_BLOCK::ELF_INTERNAL_BLOCK( Elf32_Word BufferSize )
    : Size( BufferSize ), Next(0), Buffer(0)
{
    Buffer = new char[ Size + 1];
    Buffer[Size] = '\0';
}

ELF_INTERNAL_BLOCK::~ELF_INTERNAL_BLOCK()
{
    if ( Next )
        delete Next;
    if ( Buffer )
        delete[] Buffer;
}


ELF_FILE::ELF_FILE( const char *name,ELF_CMD c ) throw(ELF_ERROR)
    : listelf(0),
    lhdr(0),
    libelem_hdr(0),
    kindfile( ELF_K_NONE ),
    StringBuffer(0),
    LastStringBuffer(0),
    StringSize(0),
    SymbolTable(0),
    SymbolSize(0)
{
    filename = name;
    if (c == ELF_C_READ)
        f.open(name,ios::in | ios::binary);

    else
        if (c == ELF_C_WRITE) {
            f.open(name,ios::out | ios::in | ios::binary | ios::trunc);
        }
        else
            if ( c == ELF_C_RDWR )
                f.open( name, ios::out | ios::in | ios::binary );
            else
                throw ELF_ERROR_UNKNOWN("Unknown file open command ");

    cmd = c;

    if (!f)
        throw ELF_ERROR_FILE("Elf error - Could not open file ",name);

    if (cmd != ELF_C_WRITE) {
        kindfile = Kind();
        switch(kindfile) {
            case ELF_K_LIB:
                break;
            case ELF_K_ELF:
                break;
            case ELF_K_NONE:
            default:
                throw ELF_ERROR_TYPE("Elf error - unknown file type");
        }
    }
    else  // cmd == ELF_C_WRITE
    {
        kindfile = ELF_K_LIB;
            // заполняем l_ident
    }
}

ELF_FILE::~ELF_FILE() throw(ELF_ERROR)
{
    if ( cmd != ELF_C_READ )
        Update( ELF_C_WRITE );

    ELF * ptr = listelf;
    while( ptr ) {
        ELF * nextptr = ptr->nextelf;
        delete ptr;
        ptr = nextptr;
    }

    if ( lhdr )
        delete lhdr;

    if ( libelem_hdr )
        delete[] libelem_hdr;

    if ( StringBuffer ) delete StringBuffer;
    if ( SymbolTable ) delete[] SymbolTable;

    f.flush();
    f.close();
}

// выдает заголовок библиотеки
Elf32_Lhdr * ELF_FILE::GetLhdr() throw(ELF_ERROR)
{
    if ( lhdr ) return lhdr;

    if ( kindfile != ELF_K_LIB )
        throw ELF_ERROR_TYPE("ElfLib error - not a library");

    if ( cmd == ELF_C_WRITE )
        throw ELF_ERROR_EXIST("ElfLib error - library header not exist");

    try {
        lhdr = new Elf32_Lhdr;
        f.seekg( 0 );
        f.read( (char *)lhdr, ELF32_T_LHDR_FSIZE );
        if (!f)
            throw ELF_ERROR_FILE("Elf error - error read lib header");

        if ( lhdr->l_ident[ EI_LIB_CLASS ] != ELFCLASS32 )
            throw ELF_ERROR_CLASS("Elf error - cannot read not 32 bit class library");

        //ELF32_T_LHDR.ToM((unsigned char *)(lhdr),(unsigned char *)(lhdr),
        //                                        lhdr->l_ident[EI_DATA]);
        ELF::ToM(ELF32_T_LHDR,(unsigned char *)(lhdr),(unsigned char *)(lhdr),
                                                lhdr->l_ident[EI_DATA], 1);
        return lhdr;
    }
    catch(ELF_ERROR) {
        if ( lhdr ) delete lhdr;
        lhdr = 0;
        throw;
    }

    return lhdr;
}

// выдает таблицу заголовков элементов библиотеки
Elf32_Libhdr *ELF_FILE::GetMemHdr() throw(ELF_ERROR)
{
        // если таблица заголовков уже прочитана, то возвращаем ее
    if ( libelem_hdr )
        return libelem_hdr;

        // если заголовок библиотеки еще не прочитан, то читаем его
    if ( lhdr == 0 )
        GetLhdr();

        // если ни одного элемента нет, то возвращает 0
    if ( lhdr->l_lhnum == 0 )
        return 0;

    Elf32_Libhdr *Libptr = 0;       // массив в котором собирается
                                    // таблица заголовков библиотеки
                                    // (представление в памяти)
    Elf32_Libhdr_f *HdrBuffer = 0;  // массив в который читается таблица
                                    // заголовков библиотеки
                                    // (представление в файле)

    Elf32_Word SymNdx = lhdr->l_lhsymndx;
    Elf32_Word StrNdx = lhdr->l_lhstrndx;

        // проверяем корректность индексов таблиц строк и символов
    if ( SymNdx > lhdr->l_lhnum )
        throw ELF_ERROR_LHDR("Elf error - invalid symbol index");

    if ( StrNdx > lhdr->l_lhnum )
        throw ELF_ERROR_LHDR("Elf error - invalid string index");

        // создаем буфера для хранения таблиц
    Libptr = new Elf32_Libhdr[lhdr->l_lhnum];
    HdrBuffer = new Elf32_Libhdr_f[lhdr->l_lhnum];
    if( Libptr == NULL || HdrBuffer == NULL )
    {
        if( Libptr ) delete[] Libptr;
        if( HdrBuffer ) delete[] HdrBuffer;
        throw;
    }

        // читаем таблицу заголовков из файла
    f.seekp(lhdr->l_lhoff);

    /*
    for (Elf32_Half i = 0; i < lhdr->l_lhnum; i++) {
        f.read( (char *)&HdrBuffer[i] , ELF32_T_LIBHDR_F_FSIZE );
        ELF32_T_LIBHDR_F.ToM( (unsigned char *)(&HdrBuffer[i]),
                              (unsigned char *)(&HdrBuffer[i]),
                              lhdr->l_ident[EI_LIB_DATA]);
    }
    */
    f.read( (char *)HdrBuffer, ELF32_T_LIBHDR_F_FSIZE * lhdr->l_lhnum );
    ELF::ToM( ELF32_T_LIBHDR_F,
            (unsigned char *)HdrBuffer,
            (unsigned char *)HdrBuffer,
            lhdr->l_ident[EI_LIB_DATA],
            lhdr->l_lhnum );

    if (!f) {
        if ( Libptr ) delete[] Libptr;
        if ( HdrBuffer ) delete[] HdrBuffer;
        throw ELF_ERROR_FILE("Elf error loading library header's table");
    }
        // получаем размер таблицы строк библиотеки
    StringSize = HdrBuffer[ StrNdx ].lib_size;

        // размер, для того, чтобы добавить 0 байт в конец
        // для надежности
    StringBuffer = new ELF_INTERNAL_BLOCK( StringSize );
    if( StringBuffer == NULL )
    {
        LastStringBuffer = 0;
        throw;
    }
    LastStringBuffer = StringBuffer;

        // читаем таблицу строк из файла
    f.seekg( HdrBuffer[ StrNdx ].lib_offset );
    f.read( StringBuffer->GetBuffer(), StringSize );
    if (!f) {
        if ( Libptr ) delete[] Libptr;
        if ( HdrBuffer ) delete[] HdrBuffer;
        throw ELF_ERROR_FILE("Elf error loading library string table");
    }

        // переводим таблицу заголовков в представление в памяти
    for ( Elf32_Half j = 0; j < lhdr->l_lhnum; j++ ) {
        if ( HdrBuffer[j].lib_name >= StringSize )
            throw ELF_ERROR_EXIST("Elf error - invalid offset in string table");

        Libptr[j].lib_name_offset = HdrBuffer[j].lib_name;
        Libptr[j].lib_name = StringBuffer->GetBuffer() + HdrBuffer[j].lib_name;
        Libptr[j].lib_date = HdrBuffer[j].lib_date;
        Libptr[j].lib_offset = HdrBuffer[j].lib_offset;
        Libptr[j].lib_size = HdrBuffer[j].lib_size;
        Libptr[j].lib_mode = HdrBuffer[j].lib_mode;

    }

        // удаляем временный буффер
    delete[] HdrBuffer;

        // все готово. возвращаем указатель на таблицу
    libelem_hdr = Libptr;

    return libelem_hdr;
}

// внутреняя функция. читает элементы библиотеки
// создает список дескрипторов ELF файлов
void ELF_FILE::readelf() throw (ELF_ERROR)
{
    if ( lhdr == 0)
        GetLhdr();

    Elf32_Libhdr * Libhdr = GetMemHdr();

    listelf = 0;
    ELF * pELFprevelf;
    try {
        for ( Elf32_Sword i = lhdr->l_lhnum - 1; i >= 0; i-- ) {

                // пропускаем специальные элементы библиотеки
            if ( i == lhdr->l_lhstrndx || i == lhdr->l_lhsymndx )
                continue;

            pELFprevelf = listelf;

            listelf = new ELF(
                    &f,                     // файл
                    Libhdr[i].lib_name,     // имя файла
                    Libhdr[i].lib_offset,   // смещение файла
                    cmd,                    // команда открытия
                    listelf );              // следующий элемент библиотеки
            if( listelf == NULL )
            {
                throw ELF_ERROR("Elf error - readelf - cann't create ELF list");
            }
        }
    }
    catch( ... )
    {
        listelf = pELFprevelf;
        while( listelf )
        {
            pELFprevelf = listelf->nextelf;
            delete listelf;
            listelf = pELFprevelf;
        }
        throw;
    }
}

// функция выдает таблицу символов библиотеки
// ЗАМЕЧАНИЕ: функция предполагает, что вариант, когда необходимо
// прочитать таблицу символов из файла и в тоже время таблица строк
// состоит из более одного буфера невозможен.
// В самом деле, если таблица строк состоит из более чем одного буфера, то
// к библиотеке добавлялись новый файлы и тогда таблица символов должна
// быть уже в памяти. Так, что такое предположение должно быть всегда верно

Elf32_LibSym *ELF_FILE::GetLibSym() throw(ELF_ERROR)
{
    if ( SymbolTable )
        return SymbolTable;

    if ( cmd == ELF_C_WRITE )
        throw ELF_ERROR_EXIST("Elf error - symbol table not exist");

    if ( lhdr == 0 )
        GetLhdr();

    Elf32_Libhdr *Libhdr = GetMemHdr();
        // если в библиотеке не элементов, то таблицы символов тоже нет.
    if ( Libhdr == 0 )
        throw ELF_ERROR_EXIST("Elf error - symbol table not exist");

        // проверяем предположение о наличии только одного буфера строк
    assert( StringBuffer == LastStringBuffer );

    Elf32_Word SymNdx = lhdr->l_lhsymndx;

        // размер таблицы символов в файле ( байтов )
    Elf32_Word SymElemSize = Libhdr[ lhdr->l_lhsymndx ].lib_size;

    if ( SymElemSize == 0 )
        return 0;

    if ( SymElemSize < ELF32_T_LIBSYM_F_FSIZE )
        throw ELF_ERROR_SIZE("Elf error - invalid size of library symbol table");

        // размер таблицы символов в памяти ( элементов )
    Elf32_Word SymSize = SymElemSize / ELF32_T_LIBSYM_F_FSIZE ;

    Elf32_LibSym_f * SymBuffer = 0;
    Elf32_LibSym * Symptr = 0;

        // создаем буфера для хранения таблиц
    Symptr = new Elf32_LibSym[ SymSize ];
    SymBuffer = new Elf32_LibSym_f[ SymElemSize ];
    if( Symptr == NULL || SymBuffer == NULL ) {
        if( Symptr ) delete[] Symptr;
        if( SymBuffer ) delete[] SymBuffer;
        throw;
    }

        // читаем таблицу строк из файла
    f.seekg( Libhdr[ SymNdx ].lib_offset );
    f.read( (char *)SymBuffer, SymElemSize );
    if (!f) {
        if ( Symptr ) delete[] Symptr;
        if ( SymBuffer ) delete[] SymBuffer;
        throw ELF_ERROR_FILE("Elf error loading library symbol table");
    }

        // переводим таблицу символов в представление в памяти
    ELF::ToM( ELF32_T_LIBSYM_F,
        (unsigned char *)SymBuffer,
        (unsigned char *)SymBuffer,
        lhdr->l_ident[EI_LIB_DATA],
        SymSize );

    for ( Elf32_Half j = 0; j < SymSize; j++ ) {
        if ( SymBuffer[j].ls_name >= StringSize ) {
            if ( Symptr ) delete[] Symptr;
            if ( SymBuffer ) delete[] SymBuffer;
            throw ELF_ERROR_EXIST("Elf error - invalid offset in string table");
        }

        Symptr[j].ls_name_offset = SymBuffer[j].ls_name;
        Symptr[j].ls_name = StringBuffer->GetBuffer() + SymBuffer[j].ls_name;
        Symptr[j].ls_index = Elf32_Half(SymBuffer[j].ls_index);
        Symptr[j].ls_bind = SymBuffer[j].ls_bind;
        Symptr[j].ls_hash = ElfHash( (unsigned char *)Symptr[j].ls_name );
    }

        // удаляем временный буффер
    delete[] SymBuffer;

        // все готово. возвращаем указатель на таблицу
    SymbolTable  = Symptr;
    SymbolSize = SymSize;

    return SymbolTable;
}

Elf32_Word  ELF_FILE::LibSymSize() throw(ELF_ERROR)
{
    Elf32_LibSym * LibSymptr = GetLibSym();

    return SymbolSize;
}

ELF *ELF_FILE::NextElf(ELF *elf) throw(ELF_ERROR)
{
    if ( kindfile == ELF_K_ELF ) {
        if ( elf ) return 0;
        if ( listelf == 0 ) {
            listelf = new ELF( &f, filename, 0, cmd, 0);
        }

        return listelf;
    }

    if ( lhdr == 0 )
        GetLhdr();

    if ( elf == 0 ) {
        if ( listelf == 0 ) {
            if ( lhdr->l_lhnum == 0 || cmd == ELF_C_WRITE ) {
                return 0;
                throw ELF_ERROR_EXIST("Elf error - no elements in library to get");
            }

            readelf();
        }
        return listelf;
    }

    return elf->nextelf;
}

ELF *ELF_FILE::GetElf(Elf32_Word Index) throw(ELF_ERROR)
{
    if ( lhdr == 0 )
        GetLhdr();

    if ( listelf == 0 ) {
            // список элементов пустой
        if ( lhdr->l_lhnum == 0 || cmd == ELF_C_WRITE ) {
                // элементов нет, возвращаем 0 или порождаем исключение
            return 0;
            throw ELF_ERROR_EXIST("Elf error - no elements in library to get");
        }

            // читаем элементы с диска
        readelf();
    }

    if (Index >= lhdr->l_lhnum)
        throw ELF_ERROR_EXIST("Elf error - element with given index not exist");

    if ( Index == lhdr->l_lhstrndx || Index == lhdr->l_lhsymndx )
        throw ELF_ERROR_TYPE("Elf error - cannot get special library elements");

        // берем элемент с нужным индексом
    ELF *ptr = listelf;
    for ( Elf32_Word i = 0; i < Index; i ++ ) {
            // пропускаем специальные элементы
        if ( i == lhdr->l_lhstrndx || i == lhdr->l_lhsymndx )
            continue;

        ptr = ptr->nextelf;
    }
    return ptr;
}

Elf32_Word ELF_FILE::LibHdrSize() throw(ELF_ERROR)
{
    if ( lhdr == 0 )
        GetLhdr();

    return lhdr->l_lhnum;
}

ELF_KIND ELF_FILE::Kind() throw(ELF_ERROR)
{
    if ( kindfile != ELF_K_NONE )
        return kindfile;

    char p[4];

    if ( cmd != ELF_C_WRITE ) {
        f.seekg(0);
        f.read(p,4);
        if (!f) throw ELF_ERROR_FILE("Elf Error loadind file type bytes");

        if ((p[EI_MAG0] == ELFMAG0) && (p[EI_MAG1] == ELFMAG1) &&
                (p[EI_MAG2] == ELFMAG2) && (p[EI_MAG3] == ELFMAG3))
                        return ELF_K_ELF;

        if ((p[EI_LIB_MAG0] == LIBMAG0) && (p[EI_LIB_MAG1] == LIBMAG1) &&
                (p[EI_LIB_MAG2] == LIBMAG2) && (p[EI_LIB_MAG3] == LIBMAG3))
                        return ELF_K_LIB;
    }

    return ELF_K_NONE;
}

// записывает элемент библиотеки Elf в файл name
void ELF_FILE::ExtractToFile( ELF * Elf, const char * name )
    throw(ELF_ERROR)
{
    char * Buffer = 0;
    try {
        fstream f( name, ios::out | ios::binary | ios::trunc);
        if ( !f )
            throw ELF_ERROR_FILE("Elf error - can't open file ", name);

        Elf32_Word FileSize;
        if ( Elf->cmd != ELF_C_READ )
            FileSize = Elf->Update( ELF_C_WRITE );
        else
            FileSize = Elf->Update( ELF_C_NULL );

        Buffer = new char[ FileSize ];
        if( Buffer == NULL ) throw;

        Elf->pfile->seekg( Elf->base_offset );
        Elf->pfile->read( Buffer, FileSize );
        if ( !(*(Elf->pfile)) )
            throw ELF_ERROR_FILE("Elf error read file ", Elf->filename);

        f.seekp( 0 );
        f.write( Buffer, FileSize );
        if ( !f )
            throw ELF_ERROR_FILE("Elf error write file ", name );

        f.flush();
        f.close();

        delete[] Buffer;

        return;
    }
    catch ( ... ) {
        if ( Buffer ) delete[] Buffer;
        throw;
    }
}

    //------------------------------------------//
    // Интерфейс создания библиотеки            //
    // функции работающие только с библиотеками //
    //------------------------------------------//

// создает новый заголовок библиотеки
Elf32_Lhdr * ELF_FILE::NewLhdr() throw(ELF_ERROR)
{
    if (lhdr) return lhdr;
    if (cmd != ELF_C_WRITE)
        return GetLhdr();

    lhdr = new Elf32_Lhdr;

    return lhdr;
}

// внутреняя функция. строит таблицы строк и символов для файла
// также проверяет на неоднозначность имена новых символов
// в случае неоднозначности порождает ELF_ERROR_AMBIGUITY
void ELF_FILE::BuildTable(
        ELF * Elf,                  // добавляемый файл
        Elf32_Word StringSize,      // размер прежней таблицы строк
        Elf32_Word FileIndex,       // индекс файла в библиотеке
        ELF_INTERNAL_BLOCK *& Block,// возвращаемый буфер строк
        Elf32_LibSym *& Sym,        // возвращаемая таблица символов
        Elf32_Word& SymSize,        // возвращаемое число символов
        Elf32_Word& FileNameOffset  // возвращаемое смещение имени
                                    // файла в новом буфере строк
    ) throw (ELF_ERROR)
{
    Elf32_Word BlockSize = 1; // первый всегда 0
    Elf32_Word SymbolCount = 0;

    BlockSize += strlen( Elf->GetName() ) + 1;   // имя файла

    ELF_SCN * Scn = Elf->NextScn(0);
    ELF_SCN * SymSect = NULL;
    ELF_SCN * StrSect = NULL;
    while( Scn ) {
        if ( Scn->GetShdr()->sh_type == SHT_SYMTAB &&
             strcmp( Scn->GetName(), ".symtab" ) == 0 ) {
            SymSect = Scn;
        }
        if ( Scn->GetShdr()->sh_type == SHT_STRTAB &&
             strcmp( Scn->GetName(), ".strtab" ) == 0 ) {
            StrSect = Scn;
        }
        Scn = Elf->NextScn( Scn );
    }

    if ( SymSect == NULL || StrSect == NULL )
        throw ELF_ERROR_EXIST("Elf error - symbol or string table not found");

    ELF_DATA * Data = SymSect->GetData(0);
    Elf32_Sym * Symbols = (Elf32_Sym *)Data->d_buf;
    Elf32_Word Size = Data->d_size / ELF32_T_SYM_FSIZE;

        // первый проход по символам
    for ( Elf32_Word i = 0; i < Size; i++ ) {
        if ( Symbols[i].st_shndx == SHN_UNDEF ||
                    Symbols[i].st_shndx >= SHN_LORESERVE )
             continue;

            // в таблицу символов библиотеки помещаются только
            // символы со связыванием STB_GLOBAL и STB_WEAK
        unsigned char Bind = ELF32_ST_BIND( Symbols[i].st_info );
        if ( Bind != STB_GLOBAL && Bind != STB_WEAK )
            continue;

        char * Name = Elf->StrPtr( StrSect->Index(), Symbols[i].st_name );

            // проверку совпадения имен делаем только для символом с
            // сильным глобальным связыванием, т.к. только они могут
            // вызвать конфликт
        if ( Bind == STB_GLOBAL ) {

                // При поиске символов с совпадающими имени, ищем только
                // символы с сильным глобальным связыванием
                // символы со слабым связыванием не ищем, т.к. они не
                // вызывают конфликта
            Elf32_Word Index = IntSearchSymbolBind( Name, STB_GLOBAL );

                // если найден символ с совпадающим именем, то
                // сигнализируем об ошибке
            if ( Index != (~0UL) )
                throw ELF_ERROR_AMBIGUITY(
                        "Elf error ",
                        "ambiguity between old symbols",
                        " and new symbols",
                        Name );
        }

        BlockSize += strlen( Name ) + 1;
        SymbolCount += 1;
    }

    Block = new ELF_INTERNAL_BLOCK( BlockSize );
    Sym = new Elf32_LibSym[ SymbolCount ];

        // второй проход по символам
    char * ptr = Block->GetBuffer();
    *ptr++ = '\0';      // всегда первым элементом ложим 0

        // заносим имя файла
    FileNameOffset = 1; // всегда со второго символа
    strcpy ( ptr , Elf->GetName() );
    ptr += strlen( Elf->GetName() );
    *ptr++ = '\0';

    Elf32_Word CurrSymbol = 0;  // номер текущего символа
    for ( Elf32_Word j = 0; j < Size; j++ ) {
        if ( Symbols[j].st_shndx == SHN_UNDEF ||
                    Symbols[j].st_shndx >= SHN_LORESERVE )
             continue;

        unsigned char Bind = ELF32_ST_BIND( Symbols[j].st_info );
        if ( Bind != STB_GLOBAL && Bind != STB_WEAK )
            continue;

        char * Name = Elf->StrPtr( StrSect->Index(), Symbols[j].st_name );

//          Elf32_Word  ls_name_offset; // смещение имени символа в таблице
//          char    *   ls_name;        // имя символа
//          Elf32_Half  ls_index;       // номер элемента, содержащего
//                                      // определение данного символа
//          Elf32_Sword ls_bind;        // связывание символа
//          Elf32_Word  ls_hash;        // значение хеш-функции Elf_Hash для
//                                      // имени символа

        Sym[ CurrSymbol ].ls_name_offset = ptr - Block->GetBuffer() + StringSize;
        Sym[ CurrSymbol ].ls_name  = ptr;
        Sym[ CurrSymbol ].ls_index = Elf32_Half(FileIndex);
        Sym[ CurrSymbol ].ls_bind = Bind;
        Sym[ CurrSymbol ].ls_hash  = ElfHash( (unsigned char *)Name );
        CurrSymbol += 1;

        strcpy( ptr, Name );
        ptr += strlen( Name );
        *ptr++ = '\0';
    }

    assert( (ptr - Block->GetBuffer()) == (Elf32_Sword)BlockSize );
    assert( CurrSymbol == SymbolCount );
    SymSize = SymbolCount;

    return;
}


// добавляет элемент Elf к библиотеке. Возвращает
// дескриптор нового элемента библиотеки
// date задает время создания файла
ELF * ELF_FILE::AddElf( ELF * Elf, time_t date ) throw (ELF_ERROR)
{
    if ( cmd == ELF_C_READ )
        throw ELF_ERROR_TYPE("Elf error - can't add elements for read-only library");

    Elf32_Lhdr * Lhdr = GetLhdr();
    Elf32_Libhdr * Libhdr = GetMemHdr();
    try {
        GetLibSym();
    }
    catch( ELF_ERROR_EXIST ) {
        // еще нет таблицы символов, все нормально
    }

    {
        ELF * TmpElf = NextElf(0);
        while( TmpElf ) {
                // записываем все изменения ELF файла на диск
            TmpElf->Update( ELF_C_NULL );
            TmpElf = NextElf( TmpElf );
        }
    }

    Elf32_Word FileSize = 0;

        // записываем все изменения ELF файла на диск
    if ( Elf->cmd == ELF_C_READ )
        FileSize = Elf->Update( ELF_C_NULL );
    else
        FileSize = Elf->Update( ELF_C_WRITE );

    ELF * NewElf = 0;

        // буфер данных для хранения строк
    ELF_INTERNAL_BLOCK * Block = 0;
        // буфер для хранения символов
    Elf32_LibSym * AddSym = 0;
        // число символов в буфере
    Elf32_Word SymSize = 0;
        // смещение имени файла в таблице строк
    Elf32_Word FileNameOffset = 0;

        // новое число файлов
    Elf32_Word NewFilesNum = lhdr->l_lhnum +1;

        // если файлов в библиотеке не было, то добавляем два специальных
        // элемента: таблицу строк и таблицу символов
    if ( listelf == 0 )
        NewFilesNum += 2;

        // построили таблицы для нового файла
    BuildTable(
        Elf,                // обрабатываемый файл
        StringSize,         // размер старой таблицы строк
        NewFilesNum - 1,    // индекс нового файла
        Block,
        AddSym,
        SymSize,
        FileNameOffset
        );

    assert( FileNameOffset < Block->GetSize() );

        // указатель на имя файла в буфере строк
    char * FileName = Block->GetBuffer() + FileNameOffset;

        // смещение файла в файле библиотеки
    Elf32_Word FileOffset = lhdr->l_lhoff;

        // копируем старую таблицу заголовков файлов
    Elf32_Libhdr * NewLibHdr = new Elf32_Libhdr[ NewFilesNum ];
    for ( Elf32_Word i = 0; i < lhdr->l_lhnum; i++ ) {
        NewLibHdr[i].lib_name_offset = libelem_hdr[i].lib_name_offset;
        NewLibHdr[i].lib_name   = libelem_hdr[i].lib_name;
        NewLibHdr[i].lib_date   = libelem_hdr[i].lib_date;
        NewLibHdr[i].lib_offset = libelem_hdr[i].lib_offset;
        NewLibHdr[i].lib_size   = libelem_hdr[i].lib_size;
        NewLibHdr[i].lib_mode   = libelem_hdr[i].lib_mode;
    }

        // заголовок нового файла
    NewLibHdr[ NewFilesNum - 1 ].lib_name_offset = FileNameOffset + StringSize;
    NewLibHdr[ NewFilesNum - 1 ].lib_name   = Block->GetBuffer() + FileNameOffset;
    NewLibHdr[ NewFilesNum - 1 ].lib_date   = date;
    NewLibHdr[ NewFilesNum - 1 ].lib_size   = FileSize;
    NewLibHdr[ NewFilesNum - 1 ].lib_offset = FileOffset;
    NewLibHdr[ NewFilesNum - 1 ].lib_mode   = 0;

    delete[] libelem_hdr;
    libelem_hdr = NewLibHdr;

    iostream * SavedFile = Elf->pfile;
    Elf32_Word SavedOffset = Elf->base_offset;
    ELF_CMD SavedCmd = Elf->cmd;

    try {
            // переключаем файл на новый поток
        Elf->pfile = &f;
        Elf->base_offset = FileOffset;
        Elf->cmd = ELF_C_WRITE;
            // и записываем его
        Elf->Update( ELF_C_WRITE );

            // переключаем обратно
        Elf->pfile = SavedFile;
        Elf->base_offset = SavedOffset;
        Elf->cmd = SavedCmd;
    }
    catch( ... ) {
        if ( Block ) delete Block;
        if ( AddSym ) delete[] AddSym;
        throw ELF_ERROR_FILE("Elf error can't copy file ", Elf->filename);
    }

        // поток из которого будет читаться файл
    if ( listelf == 0 ) {
            // таблицы должны быть пусты
        assert( StringBuffer == 0 );
        assert( SymbolTable == 0 );
        assert( StringSize == 0 );
        assert( SymbolSize == 0 );

            // новая таблица строк
        StringBuffer = Block;
        LastStringBuffer = Block;
        StringSize += Block->GetSize();

            // новая таблица символов
        SymbolTable = AddSym;
        SymbolSize = SymSize;

        lhdr->l_lhnum += 2;
        lhdr->l_lhstrndx = 0;
        lhdr->l_lhsymndx = 1;

        NewElf = new ELF (
                    &f,                 // файл
                    FileName,           // имя файла
                    FileOffset,         // смещение файла
                    ELF_C_READ,         // команда открытия
                    NULL                // следующий элемент библиотеки
            );
        listelf = NewElf;
    }
    else {
        assert( StringBuffer != 0 && LastStringBuffer != 0 );

            // добавляем строки
        LastStringBuffer->AddBlock( Block );
        LastStringBuffer = Block;
        StringSize += Block->GetSize();

            // добавляем символы
        Elf32_LibSym * NewSym = new Elf32_LibSym[ SymbolSize + SymSize ];
        for ( Elf32_Word i = 0; i < SymbolSize; i++ ) {
            NewSym[i].ls_name_offset = SymbolTable[i].ls_name_offset;
            NewSym[i].ls_name = SymbolTable[i].ls_name;
            NewSym[i].ls_index = SymbolTable[i].ls_index;
            NewSym[i].ls_hash = SymbolTable[i].ls_hash;
            NewSym[i].ls_bind = SymbolTable[i].ls_bind;
        }
        for ( Elf32_Word j = 0; j < SymSize; j++ ) {
            NewSym[ j + SymbolSize ].ls_name_offset = AddSym[j].ls_name_offset;
            NewSym[ j + SymbolSize ].ls_name = AddSym[j].ls_name;
            NewSym[ j + SymbolSize ].ls_index = AddSym[j].ls_index;
            NewSym[ j + SymbolSize ].ls_hash = AddSym[j].ls_hash;
            NewSym[ j + SymbolSize ].ls_bind = AddSym[j].ls_bind;
        }
        delete[] AddSym;
        delete[] SymbolTable;
        SymbolTable = NewSym;
        SymbolSize += SymSize;

            // находим последний элемент списка
        ELF * lastelf = listelf;
        while( lastelf->nextelf )
            lastelf = lastelf->nextelf;

        NewElf = new ELF(
                &f,                 // файл
                FileName,           // имя файла
                FileOffset,         // смещение файла
                ELF_C_READ,         // команда открытия
                NULL                // следующий элемент библиотеки
            );

        lastelf->nextelf = NewElf;
    }

        // заставлет ELF файл прочитать все в память
    NewElf->Update( ELF_C_NULL );
    NewElf->cmd = cmd;      // устанавливаем нужную команду открытия

//        // устанавливает для нового файла нужный поток
//    NewElf->pfile = &f;
//    NewElf->base_offset = FileOffset;

    lhdr->l_lhnum += 1;

    Update( ELF_C_NULL );
    return NewElf;
}

// Update( ELF_C_NULL ) - вычисляет смещения всех объектов
// в файле. физической записи не производит
// Update( ELF_C_WRITE ) - вычисляет смещения всех объектов
// в файле. записывает файл на диск,
// возвращает размер файла

Elf32_Off ELF_FILE::Update(ELF_CMD c) throw(ELF_ERROR)
{
    if ( Kind() != ELF_K_LIB )
        throw ELF_ERROR_TYPE("Elf error - not library");

    if ( cmd == ELF_C_READ && c == ELF_C_WRITE )
        throw ELF_ERROR_TYPE("Elf error - attempt write read only library");

        // заголовок должен существовать всегда
    GetLhdr();

        // если библиотека существовала ранее, то загружаем ее
        // полностью в память
    if ( cmd != ELF_C_WRITE ) {
        GetMemHdr();
        try {
            GetLibSym();
        }
        catch( ELF_ERROR_EXIST ) {
            // еще нет таблицы символов, все нормально
        }
        ELF * TmpElf = NextElf(0);
        while( TmpElf ) {
                // записываем все изменения ELF файла на диск
            if ( TmpElf->cmd == ELF_C_READ )
                TmpElf->Update( ELF_C_NULL );
            else
                TmpElf->Update( ELF_C_WRITE );

            TmpElf = NextElf( TmpElf );
        }
    }

        // если записываем файл на диск, то уменьшаем длину файла
    if ( cmd != ELF_C_READ && c == ELF_C_WRITE ) {
        fstream tmpf;
        tmpf.open(filename , ios::out | ios::trunc | ios::binary);
        tmpf << '\0';
        tmpf.close();
    }

        // устанавливаем в правильные значения заголовок библиотеки

    if (lhdr->l_ident[EI_LIB_CLASS] == ELFCLASS64)
        throw ELF_ERROR_TYPE("Elf error - unimplemented class 64 bit");

    if (lhdr->l_ident[EI_LIB_CLASS] != ELFCLASS32)
        throw ELF_ERROR_TYPE("Elf error - unknown class");

        // Здесь указывается порядок байтов в слове на данной машине.
    if ( lhdr->l_ident[EI_LIB_DATA] == ELFDATANONE )
        lhdr->l_ident[EI_LIB_DATA] = _TARG_ENCODING;

    if ( lhdr->l_ident[EI_LIB_VERSION] > EV_CURRENT )   // || (ehdr->e_ident[EI_VERSION] < EV_NONE))
        throw ELF_ERROR_TYPE("Elf error - unknown version");

    if (lhdr->l_ident[EI_LIB_VERSION] == EV_NONE)
        lhdr->l_ident[EI_LIB_VERSION] = EV_CURRENT;

    lhdr->l_ident[EI_LIB_MAG0] = LIBMAG0;
    lhdr->l_ident[EI_LIB_MAG1] = LIBMAG1;
    lhdr->l_ident[EI_LIB_MAG2] = LIBMAG2;
    lhdr->l_ident[EI_LIB_MAG3] = LIBMAG3;

    lhdr->l_lhentsize = (Elf32_Half)ELF32_T_LIBHDR_F_FSIZE;

    Elf32_Word Offset = 0;
    Elf32_Lhdr FileLhdr;

        // записываем заголовок библиотеки
    //ELF32_T_LHDR.ToF( (unsigned char *)(&FileLhdr),(unsigned char *)(lhdr),
    //                                            lhdr->l_ident[EI_LIB_DATA]);
    ELF::ToF( ELF32_T_LHDR, (unsigned char *)(&FileLhdr),(unsigned char *)(lhdr),
                                                lhdr->l_ident[EI_LIB_DATA], 1);

    if ( c == ELF_C_WRITE ) {
        f.seekp( 0 );
        f.write( (char *) &FileLhdr, ELF32_T_LHDR_FSIZE );
    }
    if ( !f )
        throw ELF_ERROR_FILE("Elf error write library bytes");
    Offset += ELF32_T_LHDR_FSIZE;
#if _DEBUG
    if ( c == ELF_C_WRITE ) {
        assert( streampos(Offset) == f.tellp() );
    }
#endif

        // записываем таблицу строк
    if ( c == ELF_C_WRITE ) {
        ELF_INTERNAL_BLOCK * Block = StringBuffer;
        while( Block ) {
            f.write( Block->GetBuffer(), Block->GetSize() );
            Block = Block->GetNext();
        }
    }
    if ( !f )
        throw ELF_ERROR_FILE("Elf error write library bytes");

    if ( libelem_hdr == 0 ) {
            // у нас нет таблицы заголовков элементов,
            // значит ничего не записываем
    }
    else {
        lhdr->l_lhstrndx = 0;
        libelem_hdr[0].lib_size = StringSize;
        libelem_hdr[0].lib_offset = Offset;
        libelem_hdr[0].lib_name = 0;
        libelem_hdr[0].lib_name_offset = 0;
        libelem_hdr[0].lib_date = time(0);
        Offset += StringSize;
    }
#if _DEBUG
    if ( c == ELF_C_WRITE ) {
        assert( streampos(Offset) == f.tellp() );
    }
#endif

        // записываем таблицу символов
    if ( c == ELF_C_WRITE ) {
        Elf32_LibSym_f * FileSymTable = new Elf32_LibSym_f[ SymbolSize ];
        for ( Elf32_Word i = 0; i < SymbolSize; i++ ) {
            FileSymTable[i].ls_name = SymbolTable[i].ls_name_offset;
            FileSymTable[i].ls_index = SymbolTable[i].ls_index;
            FileSymTable[i].ls_bind = SymbolTable[i].ls_bind;
            //ELF32_T_LIBSYM_F.ToF( (unsigned char *)&(FileSymTable[i]),
            //                      (unsigned char *)&(FileSymTable[i]),
            //                      lhdr->l_ident[EI_LIB_DATA] );
        }

        Elf32_LibSym_f * TmpTable = FileSymTable;
        ELF::ToF( ELF32_T_LIBSYM_F,
                (unsigned char *)TmpTable,
                (unsigned char *)TmpTable,
                lhdr->l_ident[EI_LIB_DATA],
                SymbolSize );

        f.write ( (char *)FileSymTable, SymbolSize * ELF32_T_LIBSYM_F_FSIZE );

        delete[] FileSymTable;
    }
    if ( !f )
        throw ELF_ERROR_FILE("Elf error write library bytes");
    if ( libelem_hdr == 0 ) {
            // у нас нет таблицы заголовков элементов,
            // значит ничего не записываем
    }
    else {
        lhdr->l_lhsymndx = 1;
        libelem_hdr[1].lib_size = SymbolSize * ELF32_T_LIBSYM_F_FSIZE;
        libelem_hdr[1].lib_offset = Offset;
        libelem_hdr[1].lib_name = 0;
        libelem_hdr[1].lib_name_offset = 0;
        libelem_hdr[1].lib_date = time(0);
        Offset += SymbolSize * ELF32_T_LIBSYM_F_FSIZE;
    }
#if _DEBUG
    if ( c == ELF_C_WRITE ) {
        assert( streampos(Offset) == f.tellp() );
    }
#endif

    // теперь возможен вариант, когда lhdr->l_lhnum == 0
//    if ( lhdr->l_lhnum < 2 )
//        lhdr->l_lhnum = 2;

        // записываем элементы библиотеки
    Elf32_Word Index = 2;
    ELF * Elf = listelf;
    while( Elf ) {
        libelem_hdr[Index].lib_offset = Offset;
        Elf->base_offset = Offset;

        Elf32_Word FileSize = Elf->Update( c );

        libelem_hdr[Index].lib_size = FileSize;
        Offset += FileSize;

        Elf = Elf->nextelf;
        Index+=1;
    }
#if _DEBUG
    if ( c == ELF_C_WRITE ) {
        f.seekp( 0, ios::end );
        assert( streampos(Offset) == f.tellp() );
    }
#endif

        // записываем таблицу заголовков
    lhdr->l_lhoff = Offset;
    if ( c == ELF_C_WRITE ) {
        f.seekp( Offset );
        Elf32_Libhdr_f * Libhdr = new Elf32_Libhdr_f[ lhdr->l_lhnum ];
        for ( Elf32_Word i = 0; i < lhdr->l_lhnum; i++ ) {
            Libhdr[i].lib_name = libelem_hdr[i].lib_name_offset;
            Libhdr[i].lib_offset = libelem_hdr[i].lib_offset;
            Libhdr[i].lib_size = libelem_hdr[i].lib_size;
            Libhdr[i].lib_date = libelem_hdr[i].lib_date;
            Libhdr[i].lib_mode = libelem_hdr[i].lib_mode;

            //ELF32_T_LIBHDR_F.ToF( (unsigned char *)&(Libhdr[i]),
            //                      (unsigned char *)&(Libhdr[i]),
            //                      lhdr->l_ident[EI_LIB_DATA] );
        }
        ELF::ToF( ELF32_T_LIBHDR_F,
                (unsigned char *)Libhdr,
                (unsigned char *)Libhdr,
                lhdr->l_ident[EI_LIB_DATA],
                lhdr->l_lhnum );

        f.write ( (char *)Libhdr, ( lhdr->l_lhnum ) * ELF32_T_LIBHDR_F_FSIZE );

        delete[] Libhdr;
    }
    Offset += ( lhdr->l_lhnum ) * ELF32_T_LIBHDR_F_FSIZE;
#if _DEBUG
    if ( c == ELF_C_WRITE ) {
        f.seekp( 0, ios::end );
        assert( streampos(Offset) == f.tellp() );
    }
#endif

        // записываем вновь заголовок библиотеки
        // записываем заголовок библиотеки
    //ELF32_T_LHDR.ToF( (unsigned char *)(&FileLhdr),(unsigned char *)(lhdr),
    //                                            lhdr->l_ident[EI_LIB_DATA]);
    ELF::ToF( ELF32_T_LHDR,
            (unsigned char *)(&FileLhdr),
            (unsigned char *)(lhdr),
            lhdr->l_ident[EI_LIB_DATA],
            1 );

    if ( c == ELF_C_WRITE ) {
        f.seekp( 0 );
        f.write( (char *) &FileLhdr, ELF32_T_LHDR_FSIZE );
    }
    if ( !f )
        throw ELF_ERROR_FILE("Elf error write library bytes");

    return Offset;
}

/*
void Elf32_T_Const_lhdr::ToF(
        unsigned char *dest,
        unsigned char *src,
        unsigned e)
{
    for (int i =0;i < EI_NIDENT;i++)
        *dest++ = *src++;
    Elf_Internal_tr_4byte(dest,src,e); // l_lhoff
    Elf_Internal_tr_2byte(dest,src,e); // l_lhnum
    Elf_Internal_tr_2byte(dest,src,e); // l_lhentsize
    Elf_Internal_tr_2byte(dest,src,e); // l_lhsymndx
    Elf_Internal_tr_2byte(dest,src,e); // l_lhstrndx

}

void Elf32_T_Const_libhdr_f::ToF(
        unsigned char *dest,
        unsigned char *src,
        unsigned e)
{
    Elf_Internal_tr_4byte(dest,src,e); // l_lhoff
    Elf_Internal_tr_4byte(dest,src,e); // l_lhnum
    Elf_Internal_tr_4byte(dest,src,e); // l_lhentsize
    Elf_Internal_tr_4byte(dest,src,e); // l_lhsymndx
    Elf_Internal_tr_4byte(dest,src,e); // l_lhstrndx
}

void Elf32_T_Const_libsym_f::ToF(
        unsigned char *dest,
        unsigned char *src,
        unsigned e)
{
    Elf_Internal_tr_4byte(dest,src,e); // ls_name
    Elf_Internal_tr_4byte(dest,src,e); // ls_index
    Elf_Internal_tr_4byte(dest,src,e); // ls_bind
}
*/

// внутренняя функция поиска символа со связыванием Bind
// и именем Name в таблице символов библиотеки
// возвращает номер найденного элемента в таблице символов
// в случае отсутствия символа возвращает (~0)
Elf32_Word ELF_FILE::IntSearchSymbolBind(
        const char * Name,
        unsigned char Bind
    ) throw(ELF_ERROR)
{
    Elf32_LibSym * LibSym = 0;

        // Проверяем наличие таблицы символов.
        // Оптимизация поиска символов.
    if ( SymbolTable == NULL && cmd == ELF_C_WRITE ) {
        return (~0UL);
    }

    try {
        LibSym = GetLibSym();
    }
    catch( ELF_ERROR_EXIST ) {
            // таблицы символов нет, соответственно символ не найден
        return (~0UL);
    }

    Elf32_Word SymbolCount = LibSymSize();
    Elf32_Word Hash = ElfHash( (unsigned char *)Name );

    for ( Elf32_Word i = 0; i < SymbolCount; i++ ) {
        if ( LibSym[i].ls_hash != Hash )
            continue;   // не совпадает хеш-функция имени

        if ( LibSym[i].ls_bind != Bind )
            continue;   // не совпадает связывание

        if ( strcmp( Name, LibSym[i].ls_name ) == 0 )
            return i;   // все совпадает
    }

    return (~0UL);
}

// возвращает индекс элемента содержащего данный символ
// если символ не найден, возвращает (~0)
Elf32_Word ELF_FILE::SearchSymbol( const char * Name )
{
    Elf32_LibSym * LibSym = 0;
    try {
        LibSym = GetLibSym();
    }
    catch( ELF_ERROR_EXIST ) {
            // таблицы символов нет, соответственно символ не найден
        return (~0UL);
    }

    Elf32_Word SymbolCount = LibSymSize();
    Elf32_Word Hash = ElfHash( (unsigned char *)Name );

    Elf32_Word SymbolIndex = ~0UL;  // здесь храниться индекс элемента
                                    // где был найден weak-символ.

    for ( Elf32_Word i = 0; i < SymbolCount; i++ ) {
        if ( Hash != LibSym[i].ls_hash )
            continue;

        if ( strcmp( Name, LibSym[i].ls_name ) == 0 ) {
                // если найденный символ
                // глобальный, возвращаем его
            if ( LibSym[i].ls_bind == STB_GLOBAL )
                return LibSym[i].ls_index;

                // если найденный символ weak, то запоминаем его в
                // переменной SymbolIndex
            if ( LibSym[i].ls_bind == STB_WEAK ) {

                    // запоминаем всегда первый weak символ,
                    // по стандарту можно любой,
                    // но мы возвращаем всегда первый
                if ( SymbolIndex != (~0UL) )
                    SymbolIndex = LibSym[i].ls_index;
            }
        }
    }

        // если символ не был найден, то в переменной содержиться ~0UL
        // если был найден weak символ, то там нужное число
    return SymbolIndex;
}
