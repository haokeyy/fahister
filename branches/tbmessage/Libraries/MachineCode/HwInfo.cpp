#include "StdAfx.h"
#include "HwInfo.h"


int WinxpSMBiosReader::Checksum(const BYTE *buf, int len)
{
	BYTE sum = 0;

	for (int i = 0; i < len; i++)
		sum += buf[i];

	return sum;
}

int WinxpSMBiosReader::GetSMBiosRawData(BYTE* buf, int* size)
{
    int result = 0;
    if (buf == NULL || *size < 0x10000)
    {
        *size = 0x10000;
        return result;
    }

    BYTE *mem = (BYTE *)malloc(0x10000);
    win_dump_mem(mem, 0xF0000, 0x10000);
    
    for (int offset = 0; offset <= 0x10000; offset += 16)
	{
		if (memcmp(mem + offset, "_SM_", 4) == 0 && offset <= 0x10000)
		{
            if (Checksum(mem + offset, *(mem + offset + 5)) == 0)
            {
                SMBIOSEntryPoint *entryPoint = (SMBIOSEntryPoint *)(mem + offset);

                // decode smbios entry point structure
                WORD len = entryPoint->totalStructureLength;
                DWORD startAddress = entryPoint->structureAddress;
                if (startAddress > 0x000F0000)
                {
                    DWORD addressOffset = startAddress - 0x000F0000;                
                    memcpy(buf, mem + addressOffset, len);
                }
                else
                {
                    BYTE *pbuf = (BYTE*)malloc(0x10000);
                    win_dump_mem(pbuf, startAddress, len);
                    memcpy(buf, pbuf, len);
                    free(pbuf);
                }
                result = len;
                break;
            }
		}
	}
    free(mem);

    return result;
}

BOOL WinxpSMBiosReader::LocateNtdllEntryPoints()
{
    HINSTANCE hMoudle1 = LoadLibrary("ntdll.dll");
    if (hMoudle1 == NULL)
    {
        return FALSE;
    }
    
    NtOpenSection = (NtOpenSectionProc)GetProcAddress(hMoudle1,"ZwOpenSection" );
     
    NtMapViewOfSection = (NtMapViewOfSectionProc) GetProcAddress(hMoudle1,"ZwMapViewOfSection" );
    
    NtUnmapViewOfSection = (NtUnmapViewOfSectionProc) GetProcAddress(hMoudle1,"ZwUnmapViewOfSection" );

    RtlInitUnicodeString = (RtlInitUnicodeStringProc) GetProcAddress(hMoudle1,"RtlInitUnicodeString" );

    RtlNtStatusToDosError = (RtlNtStatusToDosErrorProc) GetProcAddress(hMoudle1,"RtlNtStatusToDosError" );
    
	return TRUE;
}

void WinxpSMBiosReader::PrintError(char *message, long status)
{
	char *errMsg;

	FormatMessage( FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM,
			NULL, RtlNtStatusToDosError( status ), 
			MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), 
			(LPTSTR) &errMsg, 0, NULL );
	printf("%s: %s\n", message, errMsg );
	LocalFree( errMsg );
}

HANDLE WinxpSMBiosReader::OpenPhysicalMemory()
{
	long		status;
	HANDLE			physmem;
	UNICODE_STRING	physmemString;
	OBJECT_ATTRIBUTES attributes;
	WCHAR			physmemName[] = L"\\device\\physicalmemory";

	RtlInitUnicodeString( &physmemString, physmemName );	

	InitializeObjectAttributes( &attributes, &physmemString,
								0x00000040L, NULL, NULL );			
	status = NtOpenSection( &physmem, SECTION_MAP_READ, &attributes );

	if( !NT_SUCCESS( status )) {

		PrintError( "Could not open \\device\\physicalmemory", status );
		return NULL;
	}

	return physmem;
}

BOOL WinxpSMBiosReader::MapPhysicalMemory( HANDLE PhysicalMemory,PDWORD Address, PDWORD Length,PDWORD VirtualAddress )
{
    
	long			ntStatus;
	LARGE_INTEGER	viewBase;
	char				error[256];

	*VirtualAddress = 0;
	viewBase.QuadPart = (ULONGLONG) (*Address);
	ntStatus = NtMapViewOfSection (PhysicalMemory,
                               (HANDLE) -1,
                               (PVOID*) VirtualAddress,
                               0L,
                               *Length,
                               &viewBase,
                               Length,
                               1,
                               0,
                               PAGE_READONLY );

	if( !NT_SUCCESS( ntStatus )) {

		sprintf( error, "Could not map view of %X length %X",
				*Address, *Length );
		PrintError( error, ntStatus );
		return FALSE;					
	}

	*Address = viewBase.LowPart;
	return TRUE;
}

void WinxpSMBiosReader::UnmapPhysicalMemory(DWORD Address)
{
	long status;

	status = NtUnmapViewOfSection( (HANDLE) -1, (PVOID) Address );
	if( !NT_SUCCESS(status)) {

		PrintError("Unable to unmap view", status );
	}	
}

void WinxpSMBiosReader::win_dump_mem(BYTE *buf, size_t base, size_t len)
{
    LocateNtdllEntryPoints();//×÷ÓÃ??

	size_t mmoffset;
	SYSTEM_INFO sysinfo;
	HANDLE	physmem;
	DWORD paddress, vaddress, length;
    	
	//
	// Open physical memory
	//
	if( !(physmem = OpenPhysicalMemory())) {
		return;
	}

	GetSystemInfo(&sysinfo);
	mmoffset = base%sysinfo.dwPageSize;
	len += mmoffset;
    
	paddress = (DWORD)base;
	length = (DWORD)len;
	if(!MapPhysicalMemory( physmem, &paddress, &length, &vaddress )){
	    //free(p);
	    return;
	}
    
	//if((p=(BYTE*)malloc(length))==NULL){
	//	return NULL;
	//}
        
	memcpy(buf, (BYTE *)vaddress + mmoffset, length - mmoffset); 
    
	//
	// Unmap the view
	//
	UnmapPhysicalMemory( vaddress );  
	
	//
	// Close physical memory section
	//
	CloseHandle( physmem );	
}


int Win2k3SMBiosReader::GetSMBiosRawData(BYTE* buf, int* size)
{
    int result = 0;
    HINSTANCE hMoudle1 = GetModuleHandle("kernel32.dll");
    if (hMoudle1 == NULL)
    {
        printf("failed to load library kernel32.dll");
        return result;
    }
    
    GetSMBiosTable = (GetSMBiosTableProc)GetProcAddress(hMoudle1,"GetSystemFirmwareTable" );

    int size1 = GetSMBiosTable('RSMB', 0, NULL, 0);
    if (*size < size1 || buf == NULL)
    {
        *size = size1;
        return result;
    }

    RawSMBIOSData *pRawSMBiosData = (RawSMBIOSData*)malloc(size1);
    GetSMBiosTable('RSMB', 0, pRawSMBiosData, size1);

    result = pRawSMBiosData->Length;
    memcpy(buf, pRawSMBiosData->SMBIOSTableData, pRawSMBiosData->Length);
    free(pRawSMBiosData);

    return result;
}

CHwInfo::CHwInfo(BYTE* buf, int size)
{
    m_pBufBios = buf;
    m_nBufSize = size;
}


SMBiosStructHeader* CHwInfo::GetFirstStructure(BYTE type)
{    
    BYTE *p = m_pBufBios;

    while (p - m_pBufBios < m_nBufSize)
    {
        SMBiosStructHeader *header = (SMBiosStructHeader *)p;

        if (header->type == type)
        {
            return header;
        }
        else
        {
            p += header->length;

            while (TRUE)
            {
                BYTE SEPARATOR[2] = {0x00,0x00};

                if (memcmp(p, SEPARATOR, 2) == 0)
                {
                    p = p + 2;
                    break;
                }
                
                p++;
            }
        }
    }

    return NULL;
}


// read the string from the structure
//   the start address of the smbios structure
//   index is the string index base 1
char* CHwInfo::GetString(SMBiosStructHeader *structHeader, int index)
{
    //BYTE *p = (BYTE *)structHeader;
    BYTE *p = (BYTE *)structHeader;
    int savIndex = index;
    p += structHeader->length;

    int i = 1;
    while (i++ < index)
    {
        while (*p != 0)
        {
            p++;            
        }
        p++;

    }
    return (char*)p;
}

BOOL CHwInfo::IsWin2k3OrLater()
{    
    OSVERSIONINFO version;
    version.dwOSVersionInfoSize = sizeof(OSVERSIONINFO);
    GetVersionEx(&version);
   
    switch(version.dwPlatformId){        
        case VER_PLATFORM_WIN32_NT:
            if((version.dwMajorVersion >= 6) || (version.dwMajorVersion = 5 && version.dwMinorVersion >= 2)){
                return TRUE;
            }else{
                return FALSE;
            }            
        break;
        
        default:
        break;
    }
    return FALSE;
}

void CHwInfo::GetSMBiosData(BYTE* buf, int* size)
{
    BOOL bIs2k3 = IsWin2k3OrLater();
    if (bIs2k3)
    {
        Win2k3SMBiosReader *reader = new Win2k3SMBiosReader();
        reader->GetSMBiosRawData(buf, size);
    }
    else
    {
        WinxpSMBiosReader *reader = new WinxpSMBiosReader();
        reader->GetSMBiosRawData(buf, size);
    }
}


CString CHwInfo::GetMahineUuid()
{
    CString szUuid("");
    SMBiosStructHeader* structEnclosure = GetFirstStructure(1);
    
    if (structEnclosure)
    {
        BYTE arrUuid[16];
        memcpy(arrUuid, structEnclosure->data + 4, 16);

        for (int i = 0; i < 16; i++)
        {
            szUuid.AppendFormat("%02X", arrUuid[i]);
            if ((i+1)%4 == 0 && i < 15)
            {
                szUuid.Append("-");
            }
        }
    }

    return szUuid;
}

CString CHwInfo::GetMachineSn()
{
    CString szSN("");
    SMBiosStructHeader* structEnclosure = GetFirstStructure(1);
    if (structEnclosure)
    {
        int snIndex = structEnclosure->data[3];
        szSN = GetString(structEnclosure, snIndex);
    }

    return szSN;
}
