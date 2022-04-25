
#ifndef ELF_H_
#define ELF_H_

// 32-Bit Data Types
typedef unsigned int Elf32_Addr;
typedef unsigned short Elf32_Half;
typedef unsigned int Elf32_Off;
typedef int Elf32_Sword;
typedef unsigned int Elf32_Word;

// File Header
#define EI_NIDENT 16
typedef struct {
	unsigned char e_ident[EI_NIDENT];
	Elf32_Half e_type;
	Elf32_Half e_machine;
	Elf32_Word e_version;
	Elf32_Addr e_entry;
	Elf32_Off e_phoff;
	Elf32_Off e_shoff;
	Elf32_Word e_flags;
	Elf32_Half e_ehsize;
	Elf32_Half e_phentsize;
	Elf32_Half e_phnum;
	Elf32_Half e_shentsize;
	Elf32_Half e_shnum;
	Elf32_Half e_shstrndx;
} Elf32_Ehdr;

// e_type
#define ET_NONE		0x0000		// No file type
#define ET_REL		0x0001		// Relocatable file
#define ET_EXEC		0x0002		// Executable file
#define ET_DYN		0x0003		// Shared object file
#define ET_CORE		0x0004		// Core file
#define ET_LOPROC	0xff00		// Processor-specific
#define ET_HIPROC	0xffff		// Processor-specific

// e_machine
#define EM_NONE			ET_NONE	// No machine
#define EM_M32			1		// AT&T WE 32100
#define EM_SPARC		2		// SPARC
#define EM_386			3		// Intel Architecture
#define EM_68K			4		// Motorola 68000
#define EM_88K			5		// Motorola 88000
#define EM_860			7		// Intel 80860
#define EM_MIPS			8		// MIPS RS3000 Big-Endian
#define EM_MIPS_RS4_BE	10		// MiPS RS4000 Big-Endian

// e_version
#define EV_NONE			0		// Invalid version
#define EV_CURRENT		1		// Current version

// e_ident
#define EI_MAG0			0		// File identification
#define EI_MAG1			1		// File identification
#define EI_MAG2			2		// File identification
#define EI_MAG3			3		// File identification
#define EI_CLASS		4		// File class
#define EI_DATA			5		// Data encoding
#define EI_VERSION		6		// File version
#define EI_PAD			7		// Start of padding bytes
#define EI_NIDENT		16		// Size of e_ident[]

// e_ident[EI_CLASS]
#define ELFCLASSNONE	0		// Invalid class
#define ELFCLASS32		1		// 32-bit objects
#define ELFCLASS64		2		// 64-bit objects

// e_ident[EI_DATA]
#define ELFDATANONE		0		// Invalid data encoding
#define ELFDATA2LSB		1
#define ELFDATA2MSB		2

// Section Header
typedef struct {
	Elf32_Word sh_name;
	Elf32_Word sh_type;
	Elf32_Word sh_flags;
	Elf32_Addr sh_addr;
	Elf32_Off sh_offset;
	Elf32_Word sh_size;
	Elf32_Word sh_link;
	Elf32_Word sh_info;
	Elf32_Word sh_addralign;
	Elf32_Word sh_entsize;
} Elf32_Shdr;

#define ELF32_ST_BIND(i) ((i)>>4)
#define ELF32_ST_TYPE(i) ((i)&0xf)
#define ELF32_ST_INFO(b,t) (((b)<<4)+((t)&0xf))

#endif
