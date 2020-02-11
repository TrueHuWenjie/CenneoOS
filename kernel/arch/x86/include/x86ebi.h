// Loader for Cenneo OS
// /kernel/arch/x86/include/x86ebi.h
// Explorer Boot Information on x86 Architecture

#ifndef X86EBI_H_
#define X86EBI_H_

#define X86EBI_CHECK { \
    if (boot_info_ptr->flag[0] != 'E' | \
        boot_info_ptr->flag[1] != 'B' | \
        boot_info_ptr->flag[2] != 'I' | \
        boot_info_ptr->size != sizeof(struct boot_info)) \
        reset(); \
}

/**在_start.asm中放置了boot_info_ptr指针指向boot_info*/
extern struct boot_info *boot_info_ptr;

/**实模式Segment:Offset地址转换成保护模式地址*/
// #define real_addr_in_pm(Segment, Offset)	(Segment * 0x10 + Offset)

#pragma pack(push)					//保存当前对齐信息
#pragma pack(1)						//设定结构体以一个字节为单位对齐

/**地址范围描述符结构体*/
#define ARDS_FREE	1
#define ARDS_RESV	2
struct Address_Range_Descriptor_Structure
{
	unsigned int BaseAddrLow;
	unsigned int BaseAddrHigh;
	unsigned int LengthLow;
	unsigned int LengthHigh;
	unsigned int Type;
};

/**VBE信息块结构体*/
#define VBE_SIGN_NUM	4
struct VbeInfoBlock
{
	unsigned char VbeSignature[VBE_SIGN_NUM];
	unsigned short VbeVersion;
	unsigned short OemStringPtr_Off;
	unsigned short OemStringPtr_Seg;
	unsigned char Capabilities[4];
	unsigned short VideoModePtr_Off;
	unsigned short VideoModePtr_Seg;
	unsigned short TotalMemory;
	unsigned short OemSoftwareRev;
	unsigned int OemVendorNamePtr;
	unsigned int OemProductNamePtr;
	unsigned int OemProductRevPtr;
	unsigned char Reserved[222];
	unsigned char OemData[256];
};

struct ModeInfoBlock
{
	/**Mandatory information for all VBE revisions*/
	unsigned short ModeAttributes;
	unsigned char WinAAttributes;
	unsigned char WinBAttributes;
	unsigned short WinGranularity;
	unsigned short WinSize;
	unsigned short WinASegment;
	unsigned short WinBSegment;
	unsigned int WinFuncPtr;
	unsigned short BytesPerScanLine;

	/**Mandatory information for VBE 1.2 and above*/
	unsigned short XResolution;
	unsigned short YResolution;
	unsigned char XCharSize;
	unsigned char YCharSize;
	unsigned char NumberOfPlanes;
	unsigned char BitsPerPixel;
	unsigned char NumberOfBanks;
	unsigned char MemoryModel;
	unsigned char BankSize;
	unsigned char NUmberOfImagePages;
	unsigned char Reserved1;

	/**Direct Color fields(required for direct/6 and YUV/7 memory models)*/
	unsigned char RedMaskSize;
	unsigned char RedFieldPosition;
	unsigned char GreenMaskSize;
	unsigned char GreenFieldPosition;
	unsigned char BlueMaskSize;
	unsigned char BlueFieldPosition;
	unsigned char RsvdMaskSize;
	unsigned char RsvdFieldPosition;
	unsigned char DirectColorModeInfo;

	/**Mandatory infomation for VBE 2.0 and above*/
	unsigned int PhysBasePtr;
	unsigned int Reserved2;
	unsigned short Reserved3;

	/**Mandatory information for VBE 3.0 and above*/
	unsigned short LinBytesPerScanLine;
	unsigned char BnkNumberOfImagePages;
	unsigned char LinNumberOfImagePages;
	unsigned char LinRedMaskSize;
	unsigned char LinRedFieldPosition;
	unsigned char LinGreenMaskSize;
	unsigned char LinGreenFieldPosition;
	unsigned char LinBlueMaskSize;
	unsigned char LinBlueFieldPosition;
	unsigned char LinRsvdMaskSize;
	unsigned char LinRsvdFieldPosition;
	unsigned int MaxPixelClock;
	unsigned char Reserved4[189];
	unsigned char Reserved5;
};

#define BOOT_FLAG_MAX	4
#define BOOT_ARDS_NUM	8

// Boot Information structure
struct boot_info
{
	/**信息一定要是"EBI"(Explorer Boot Information)*/
	char flag[BOOT_FLAG_MAX];

	/**启动信息长度*/
	unsigned int size;

	/**保护模式 32位 0-4GB 代码段段选择子*/
	unsigned short code_sel;

	/**保护模式 32位 0-4GB 数据段段选择子*/
	unsigned short data_sel;

	/**内存分布信息*/
	struct Address_Range_Descriptor_Structure ARDS[BOOT_ARDS_NUM];

	/**VBE信息块结构体*/
	struct VbeInfoBlock VbeInfoBlock;

	/**VBE模式信息结构体*/
	struct ModeInfoBlock ModeInfoBlock;
};

#pragma pack(pop)

#endif
