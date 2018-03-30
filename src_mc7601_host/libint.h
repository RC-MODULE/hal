//////////////////////////////////////////////////////////////////////////
//                                                                      //
//             ������� ����������� ����������� ���������������          //
//                                                                      //
//     ���������� ������� � ��������� � ����������� ����� ������� ELF   //
//                                                                      //
//                      ������������ ������ libint.h                    //
//                                                                      //
//     ������������ ���� ����������� ����, ������, � ������ �������     //
//     ��������� ��� ���������� libelf                                  //
//                                                                      //
//                                                                      //
// Copyright (c) 1997-2006 RC Module                                    //
//  If this code works, it was written by Alex Ruzavin                  //
//  If not, I don't know who wrote it.                                  //
//                                                                      //
// $Revision:: 2     $      $Date:: 6/02/06 3:37p    $                  //
//                                                                      //
//////////////////////////////////////////////////////////////////////////

#ifndef __LIBINT_H
#define __LIBINT_H

struct Elf32_Libhdr_f {
    public:
        Elf32_Word  lib_name;   // �������� ����� � ������� �����
        Elf32_Word  lib_date;   // ��������� ��� � Elf32_Libhdr
        Elf32_Off   lib_offset;
        Elf32_Word  lib_size;
        Elf32_Word  lib_mode;

        Elf32_Libhdr_f()
            : lib_date(0),
            lib_offset(0),
            lib_size(0),
            lib_mode(0),
            lib_name(0)
        {  }
};

struct Elf32_LibSym_f {
    public:
        Elf32_Word  ls_name;   // �������� ����� � ������� �����
        Elf32_Word  ls_index;  // ����� �������� � ������� ������ ���������
        Elf32_Sword ls_bind;   // ���������� ������� (GLOBAL,WEAK)
};

    // ���������� ����� libelf. ��������� ���� ������ ����������� � ������
    // ������������ � ������ ELF_FILE ��� �������� ������ ������ �����
class ELF_INTERNAL_BLOCK {
    private:
        char * Buffer;
        Elf32_Word Size;
        ELF_INTERNAL_BLOCK * Next;

    public:
        ELF_INTERNAL_BLOCK() : Buffer(0), Size(0), Next(0) {  }
        ELF_INTERNAL_BLOCK( Elf32_Word BufferSize );
        ~ELF_INTERNAL_BLOCK();

        char * GetBuffer() const { return Buffer; }
        Elf32_Word GetSize() const { return Size; }
        ELF_INTERNAL_BLOCK * GetNext() const { return Next; }

        void AddBlock( ELF_INTERNAL_BLOCK * Block )
        {
            assert( Next == 0 );
            Next = Block;
        }
};

/*
class Elf32_T_Const_libhdr_f {
    private:

        static Elf_T_Type * Object;

    public:
        operator Elf_T_Type * () const
        {
            if ( Object )
                return Object;

            Object = new Elf_T_Int_Type< Elf32_T_Const_libhdr_f >;
            return Object;
        }

    public:
        static Elf32_Word Size() { return 20; } // sizeof(Elf32_Libhdr_f);

        static void ToF(unsigned char *dest,unsigned char *src,unsigned e);
        static void ToM(unsigned char *dest,unsigned char *src,unsigned e)
        {
            ToF(dest,src,e);
        }
};

class Elf32_T_libhdr_f : public Elf_T_Int_Type< Elf32_T_Const_libhdr_f > {
};

class Elf32_T_Const_libsym_f {
    private:

        static Elf_T_Type * Object;

    public:
        operator Elf_T_Type * () const
        {
            if ( Object )
                return Object;

            Object = new Elf_T_Int_Type< Elf32_T_Const_libsym_f >;
            return Object;
        }

    public:
        static Elf32_Word Size() { return 12; } // sizeof(Elf32_LibSym_f);

        static void ToF(unsigned char *dest,unsigned char *src,unsigned e);
        static void ToM(unsigned char *dest,unsigned char *src,unsigned e)
        {
            ToF(dest,src,e);
        }
};

class Elf32_T_libsym_f : public Elf_T_Int_Type< Elf32_T_Const_libsym_f > {
};

static const Elf32_T_Const_libhdr_f ELF32_T_LIBHDR_F;
static const Elf32_T_Const_libsym_f ELF32_T_LIBSYM_F;
*/

#endif // __LIBINT_H
