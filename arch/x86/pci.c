
#include "x86/pci.h"
#include "x86/io.h"
#include "kernel/types.h"
#include "kernel/console.h"
#include "lib/itoa.h"

#define PCIBIOS_READ_CONFIG_BYTE	0xB108
#define PCIBIOS_READ_CONFIG_WORD	0xB109
#define PCIBIOS_READ_CONFIG_DWORD	0xB10A

#define PCI_SERVICE	(('$'<<0)+('P'<<8)+('C'<<16)+('I'<<24))
#define PCI_SIGNATURE	(('P'<<0)+('C'<<8)+('I'<<16)+(' '<<24))
#define PCIBIOS_PCI_BIOS_PRESENT	0xB101

struct long_addr {
    unsigned long offset;
    unsigned short selector;
};

static int PCIType = -1;
static int DeviceNumber = 0;
static int PCIBusses = 0;
struct long_addr pci_entry = { 0, 10};

int findPCI( void ) {
    unsigned int temp;

    /* Check if configuraton type 1 works */
    iowrite8( 0xCFB, 0x01 );
    temp = ioread32( 0xCF8 );
    iowrite32( 0xCF8, 0x80000000 );
    if ( ioread32( 0xCF8 ) == 0x80000000 ) {
	iowrite32( 0xCF8, temp );
	PCIType = 1;
	return PCI_PRESENT;
    }
    iowrite32( 0xCF8, temp );

    /* Check if configuration type 2 works */
    iowrite8( 0xCFB, 0x00 );
    iowrite8( 0xCF8, 0x00 );
    iowrite8( 0xCFA, 0x00 );
    if ( ioread8( 0xCF8 ) == 0x00 && ioread8( 0xCFA ) == 0x00 ) {
	PCIType = 2;
	return PCI_PRESENT;
    }
    return PCI_NOT_PRESENT;
}

unsigned long ReadPCIConfig( int bus, int device, int function, int offset, int size ) {
    //unsigned long flags;
    unsigned long value = 0;

    if ( size == 2 || size == 4 || size == 1 ) {
	 if ( PCIType & 1 ) {
	    iowrite32( 0xCF8, 0x80000000 | ( bus << 16 ) | ( device << 11 )
		| ( function << 8 ) | ( offset & ~3 ) );
	    switch ( size ) {
		case 1:
		    value = ioread8( 0xCFC + ( offset & 3 ) );
		    break;
		case 2:
		    value = ioread16( 0xCFC + ( offset & 2 ) );
		    break;
		case 4:
		    value = ioread32( 0xCFC );
		    break;
		default:
		    break;
	    }
	    return value;
	}else {
	    //kPrint( "[%s]: called with PCI present!\n", __FUNCTION__ );
	}
    } else {
	//kPrint( "[%s]: Invalid size %d passed\n", __FUNCTION__, size );
    }
    return 0;
}

unsigned long WritePCIConfig( int bus, int device, int function, int offset, int size, unsigned long value ) {
    //unsigned long flags;

    if ( size == 2 || size == 4 || size == 1 ) {
	if ( PCIType & 1 ) {
	    iowrite32( 0xCF8, 0x80000000 | ( bus << 16 ) | ( device << 11 )
		| ( function << 8 ) | ( offset & ~3 ) );
	    switch ( size ) {
		case 1:
		    iowrite8( 0xCFC + ( offset & 3 ), value );
		    break;
		case 2:
		    iowrite16( 0xCFC + ( offset & 2 ), value );
		    break;
		case 4:
		    iowrite32( 0xCFC, value );
		    break;
		default:
		    return -1;
	    }
	    return 0;
	} else {
	    //kPrint( "[%s]: called with PCI present!\n", __FUNCTION__ );
	}
    } else {
	//kPrint( "[%s]: Invalid size %d passed\n", __FUNCTION__, size );
    }
    return -1;
}

int ReadPCIHeader( PCIEntry *pciEntry, int bus, int device, int function ) {
    unsigned int temp2;

    pciEntry -> Bus = bus;
    pciEntry -> Device = device;
    pciEntry -> Function = function;

    pciEntry -> VendorID = ReadPCIConfig( bus, device, function, PCI_VENDOR_ID, 2 );
    pciEntry -> DeviceID = ReadPCIConfig( bus, device, function, PCI_DEVICE_ID, 2 );
    //kPrint( "vendor %x, device %x\n", pciEntry -> VendorID, pciEntry -> DeviceID );
    pciEntry -> Command = ReadPCIConfig( bus, device, function, PCI_COMMAND, 2 );
    pciEntry -> Status = ReadPCIConfig( bus, device, function, PCI_STATUS, 2 );
    pciEntry -> RevisionID = ReadPCIConfig( bus, device, function, PCI_REVISION, 2 );
    pciEntry -> Interface = ReadPCIConfig( bus, device, function, PCI_INTERFACE, 1 );
    pciEntry -> ClassCode = ReadPCIConfig( bus, device, function, PCI_CLASS_CODE, 1 );
    pciEntry -> SubClass = ReadPCIConfig( bus, device, function, PCI_SUB_CLASS, 1 );
    pciEntry -> u.NonBridge.BaseAddress0 = ReadPCIConfig( bus, device, function, PCI_BASE_REGISTERS + 0, 4 );
    if ( pciEntry -> u.NonBridge.BaseAddress0 ) {
	WritePCIConfig( bus, device, function, PCI_BASE_REGISTERS + 0, 4, 0xFFFFFFFF );
	temp2 = ReadPCIConfig( bus, device, function, PCI_BASE_REGISTERS + 0, 4 ) & 0xFFFFFFF0;
	WritePCIConfig( bus, device, function, PCI_BASE_REGISTERS + 0, 4, pciEntry -> u.NonBridge.BaseAddress0 );
	temp2 = 1 + ~temp2;
	//dprint( "%x -> %x\n", pciEntry -> u.NonBridge.BaseAddress0, temp2 );
	pciEntry -> u.NonBridge.BaseAddressSize0 = temp2;
    }
    pciEntry -> u.NonBridge.BaseAddress1 = ReadPCIConfig( bus, device, function, PCI_BASE_REGISTERS + 4, 4 );
    if ( pciEntry -> u.NonBridge.BaseAddress1 ) {
	WritePCIConfig( bus, device, function, PCI_BASE_REGISTERS + 4, 4, 0xFFFFFFFF );
	temp2 = ReadPCIConfig( bus, device, function, PCI_BASE_REGISTERS + 4, 4 ) & 0xFFFFFFF0;
	WritePCIConfig( bus, device, function, PCI_BASE_REGISTERS + 4, 4, pciEntry -> u.NonBridge.BaseAddress1 );
	temp2 = 1 + ~temp2;
	//dprint( "%x -> %x\n", pciEntry -> u.NonBridge.BaseAddress1, temp2 );
	pciEntry -> u.NonBridge.BaseAddressSize1 = temp2;
    }
    pciEntry -> u.NonBridge.BaseAddress2 = ReadPCIConfig( bus, device, function, PCI_BASE_REGISTERS + 8, 4 );
    if ( pciEntry -> u.NonBridge.BaseAddress2 ) {
	WritePCIConfig( bus, device, function, PCI_BASE_REGISTERS + 8, 4, 0xFFFFFFFF );
	temp2 = ReadPCIConfig( bus, device, function, PCI_BASE_REGISTERS + 8, 4 ) & 0xFFFFFFF0;
	WritePCIConfig( bus, device, function, PCI_BASE_REGISTERS + 8, 4, pciEntry -> u.NonBridge.BaseAddress2 );
	temp2 = 1 + ~temp2;
	//dprint( "%x -> %x\n", pciEntry -> u.NonBridge.BaseAddress2, temp2 );
	pciEntry -> u.NonBridge.BaseAddressSize2 = temp2;
    }
    pciEntry -> u.NonBridge.BaseAddress3 = ReadPCIConfig( bus, device, function, PCI_BASE_REGISTERS + 12, 4 );
    if ( pciEntry -> u.NonBridge.BaseAddress3 ) {
	WritePCIConfig( bus, device, function, PCI_BASE_REGISTERS + 12, 4, 0xFFFFFFFF );
	temp2 = ReadPCIConfig( bus, device, function, PCI_BASE_REGISTERS + 12, 4 ) & 0xFFFFFFF0;
	WritePCIConfig( bus, device, function, PCI_BASE_REGISTERS + 12, 4, pciEntry -> u.NonBridge.BaseAddress3 );
	temp2 = 1 + ~temp2;
	//dprint( "%x -> %x\n", pciEntry -> u.NonBridge.BaseAddress3, temp2 );
	pciEntry -> u.NonBridge.BaseAddressSize3 = temp2;
    }
    pciEntry -> u.NonBridge.BaseAddress4 = ReadPCIConfig( bus, device, function, PCI_BASE_REGISTERS + 16, 4 );
    if ( pciEntry -> u.NonBridge.BaseAddress4 ) {
	WritePCIConfig( bus, device, function, PCI_BASE_REGISTERS + 16, 4, 0xFFFFFFFF );
	temp2 = ReadPCIConfig( bus, device, function, PCI_BASE_REGISTERS + 16, 4 ) & 0xFFFFFFF0;
	WritePCIConfig( bus, device, function, PCI_BASE_REGISTERS + 16, 4, pciEntry -> u.NonBridge.BaseAddress4 );
	temp2 = 1 + ~temp2;
	//dprint( "%x -> %x\n", pciEntry -> u.NonBridge.BaseAddress4, temp2 );
	pciEntry -> u.NonBridge.BaseAddressSize4 = temp2;
    }
    pciEntry -> u.NonBridge.BaseAddress5 = ReadPCIConfig( bus, device, function, PCI_BASE_REGISTERS + 20, 4 );
    if ( pciEntry -> u.NonBridge.BaseAddress5 ) {
	WritePCIConfig( bus, device, function, PCI_BASE_REGISTERS + 20, 4, 0xFFFFFFFF );
	temp2 = ReadPCIConfig( bus, device, function, PCI_BASE_REGISTERS + 20, 4 ) & 0xFFFFFFF0;
	WritePCIConfig( bus, device, function, PCI_BASE_REGISTERS + 20, 4, pciEntry -> u.NonBridge.BaseAddress5 );
	temp2 = 1 + ~temp2;
	//dprint( "%x -> %x\n", pciEntry -> u.NonBridge.BaseAddress5, temp2 );
	pciEntry -> u.NonBridge.BaseAddressSize5 = temp2;
    }
    pciEntry -> u.NonBridge.InterruptLine = ReadPCIConfig( bus, device, function, PCI_INTERRUPT_LINE, 1 );
    pciEntry -> u.NonBridge.InterruptPIN = ReadPCIConfig( bus, device, function, PCI_INTERRUPT_PIN, 1 );
    pciEntry -> u.NonBridge.MinDMATime = ReadPCIConfig( bus, device, function, PCI_MIN_GRANT, 1 );
    pciEntry -> u.NonBridge.MaxDMALatency = ReadPCIConfig( bus, device, function, PCI_MAX_LATENCY, 1 );

    return 0;
}

void scanBus( int bus ) {
    PCIEntry pciEntry;
    int devicesPerBus = ( PCIType & 1 ) ? 32 : 16;
    int device, function;
    unsigned long vendorID;
    unsigned char headerType = 0;

    PCIBusses++;

    for ( device = 0; device < devicesPerBus; device++ ) {
	for ( function = 0; function < 8; function++ ) {
	    vendorID = ReadPCIConfig( bus, device, function, PCI_VENDOR_ID, 2 );

	    if(vendorID != 0xFFFF && vendorID != 0x0000 ){
		    if ( function == 0 ) {
			    headerType = ReadPCIConfig( bus, device, function, PCI_HEADER_TYPE, 1 );
		    } else {
			    if ( ( headerType & PCI_MULTIFUNCTION ) == 0 ) {
				    continue;
			    }
		    } 

		    ReadPCIHeader( &pciEntry, bus, device, function );

		    if ( DeviceNumber < MAX_PCI_DEVICES ) {
			console_puts("[scanBus] vendorID = "); console_puts(itoa(vendorID,16)); 
			console_puts(" Function = "); console_puts(itoa(pciEntry.Function,16));
			console_puts(" DeviceID = "); console_puts(itoa(pciEntry.DeviceID,16)); 
			console_puts("\n");
			console_puts(" InterruptLine = "); console_puts(itoa(pciEntry.u.NonBridge.InterruptLine,16));
			console_puts(" InterruptPIN = "); console_puts(itoa(pciEntry.u.NonBridge.InterruptPIN,16)); 
			console_puts("\n");				
		    } else {
			console_puts( "[scanBUS]: To many PCI devices!\n");
		    }

		    if ( headerType & PCI_HEADER_BRIDGE ) {
			// scan next bus
		    }
	    }
	}
    }
}


int lspci( void )
{

	if ( PCIType == -1 && findPCI() == PCI_NOT_PRESENT ) {
		console_puts( "[lspci]: PCI bus not present\n");
		return -1;
	}
	console_puts( "[lspci]: PCI bus present (Type: ");
	console_puts( itoa(PCIType,10) );
	console_puts( " )\n" );

	scanBus( 0 );

	return 0;
}
