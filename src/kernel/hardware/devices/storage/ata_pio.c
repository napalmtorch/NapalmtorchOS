#include <kernel/hardware/devices/storage/ata_pio.h>
#include <kernel/core/kernel.h>

// ports
#define ATA_PRIMARY_DATA         0x1F0
#define ATA_PRIMARY_ERR          0x1F1
#define ATA_PRIMARY_SECCOUNT     0x1F2
#define ATA_PRIMARY_LBA_LO       0x1F3
#define ATA_PRIMARY_LBA_MID      0x1F4
#define ATA_PRIMARY_LBA_HI       0x1F5
#define ATA_PRIMARY_DRIVE_HEAD   0x1F6
#define ATA_PRIMARY_COMM_REGSTAT 0x1F7
#define ATA_PRIMARY_ALTSTAT_DCR  0x3F6

// status flags
#define ATA_STAT_ERR  (1 << 0) 
#define ATA_STAT_DRQ  (1 << 3)
#define ATA_STAT_SRV  (1 << 4)
#define ATA_STAT_DF   (1 << 5) 
#define ATA_STAT_RDY  (1 << 6)
#define ATA_STAT_BSY  (1 << 7)

// service instance
service_t* ata_service;

void ata_callback(registers_t* regs)
{
    UNUSED(regs);
}

void ata_start(service_t* service)
{
    DEBUGMODE old = debug_getmode();
    debug_setmode(DEBUGMODE_ALL);
    irq_register(IRQ14, ata_callback);
    if (!atapio_identify()) { debug_warn("No hard disk detected"); }
    else { debug_info("Detected hard disk"); }
    debug_setmode(old);
}

void ata_stop(service_t* service)
{
    irq_unregister(IRQ14);
}

void atapio_init()
{
    ata_service = service_create("ATA", SERVICETYPE_DRIVER, ata_start, ata_stop);
    if (!service_register(ata_service)) { return; }

    ata_service->initialized = TRUE;
    if (!service_start(ata_service)) { return; }
}

bool_t atapio_identify()
{
    if (!ata_service->started) { return FALSE; }
    port_inb (ATA_PRIMARY_COMM_REGSTAT);
    port_outb(ATA_PRIMARY_DRIVE_HEAD, 0xA0);
    port_inb (ATA_PRIMARY_COMM_REGSTAT);
    port_outb(ATA_PRIMARY_SECCOUNT, 0);
    port_inb (ATA_PRIMARY_COMM_REGSTAT);
    port_outb(ATA_PRIMARY_LBA_LO, 0);
    port_inb (ATA_PRIMARY_COMM_REGSTAT);
    port_outb(ATA_PRIMARY_LBA_MID, 0);
    port_inb (ATA_PRIMARY_COMM_REGSTAT);
    port_outb(ATA_PRIMARY_LBA_HI, 0);
    port_inb (ATA_PRIMARY_COMM_REGSTAT);
    port_outb(ATA_PRIMARY_COMM_REGSTAT, 0xEC);
    port_outb(ATA_PRIMARY_COMM_REGSTAT, 0xE7);

    // Read the status port. If it's zero, the drive does not exist.
    uint8_t status = port_inb(ATA_PRIMARY_COMM_REGSTAT);

    while (status & ATA_STAT_BSY) 
    {
        uint32_t i = 0;
        while(1) { i++; }
        for(i = 0; i < 0x0FFFFFFF; i++) { }
        status = port_inb(ATA_PRIMARY_COMM_REGSTAT);
    }
    
    if (status == 0) { asm volatile("sti"); return FALSE; }

    while (status & ATA_STAT_BSY) { status = port_inb(ATA_PRIMARY_COMM_REGSTAT); }

    uint8_t mid = port_inb(ATA_PRIMARY_LBA_MID);
    uint8_t hi = port_inb(ATA_PRIMARY_LBA_HI);
    if (mid || hi) { asm volatile("sti"); return FALSE; }

    // Wait for ERR or DRQ
    while (!(status & (ATA_STAT_ERR | ATA_STAT_DRQ))) { status = port_inb(ATA_PRIMARY_COMM_REGSTAT); }

    if (status & ATA_STAT_ERR) { asm volatile("sti"); return FALSE; }

    uint8_t buff[256 * 2];
    port_instr(ATA_PRIMARY_DATA, buff, 256);
    return TRUE;
}

bool_t atapio_read(uint64_t sector, uint32_t count, uint8_t* buffer)
{
    if (!ata_service->started) { return FALSE; }
    port_outb(ATA_PRIMARY_DRIVE_HEAD, 0x40);                     // Select master
    port_outb(ATA_PRIMARY_SECCOUNT, (count >> 8) & 0xFF );     // sectorcount high
    port_outb(ATA_PRIMARY_LBA_LO, (sector >> 24) & 0xFF);           // LBA4
    port_outb(ATA_PRIMARY_LBA_MID, (sector >> 32) & 0xFF);          // LBA5
    port_outb(ATA_PRIMARY_LBA_HI, (sector >> 40) & 0xFF);           // LBA6
    port_outb(ATA_PRIMARY_SECCOUNT, count & 0xFF);             // sectorcount low
    port_outb(ATA_PRIMARY_LBA_LO, sector & 0xFF);                   // LBA1
    port_outb(ATA_PRIMARY_LBA_MID, (sector >> 8) & 0xFF);           // LBA2
    port_outb(ATA_PRIMARY_LBA_HI, (sector >> 16) & 0xFF);           // LBA3
    port_outb(ATA_PRIMARY_COMM_REGSTAT, 0x24);                   // READ SECTORS EXT

    for (uint32_t i = 0; i < count; i++) 
    {
        while (TRUE)
        {
            uint8_t status = port_inb(ATA_PRIMARY_COMM_REGSTAT);
            if(status & ATA_STAT_DRQ) { break; }
            if (status & ATA_STAT_ERR) { panic(EXCEPTION_DISKREAD, NULL); return FALSE; }
        }
        port_instr(ATA_PRIMARY_DATA, (unsigned char *)buffer, 256);
        buffer += 256;
    }
    return TRUE;
}

bool_t atapio_write(uint64_t sector, uint32_t count, uint8_t* buffer)
{
    if (!ata_service->started) { return FALSE; }
    port_outb(ATA_PRIMARY_DRIVE_HEAD, 0x40);                     // Select master
    port_outb(ATA_PRIMARY_SECCOUNT, (count >> 8) & 0xFF );     // sectorcount high
    port_outb(ATA_PRIMARY_LBA_LO, (sector >> 24) & 0xFF);           // LBA4
    port_outb(ATA_PRIMARY_LBA_MID, (sector >> 32) & 0xFF);          // LBA5
    port_outb(ATA_PRIMARY_LBA_HI, (sector >> 40) & 0xFF);           // LBA6
    port_outb(ATA_PRIMARY_SECCOUNT, count & 0xFF);             // sectorcount low
    port_outb(ATA_PRIMARY_LBA_LO, sector & 0xFF);                   // LBA1
    port_outb(ATA_PRIMARY_LBA_MID, (sector >> 8) & 0xFF);           // LBA2
    port_outb(ATA_PRIMARY_LBA_HI, (sector >> 16) & 0xFF);           // LBA3
    port_outb(ATA_PRIMARY_COMM_REGSTAT, 0x34);                   // READ SECTORS EXT

    for (uint8_t i = 0; i < count; i++) 
    {
        while (TRUE) 
        {
            uint8_t status = port_inb(ATA_PRIMARY_COMM_REGSTAT);
            if(status & ATA_STAT_DRQ) { break; }
            else if(status & ATA_STAT_ERR) { panic(EXCEPTION_DISKWRITE, NULL); break; }
        }
        port_outstr(ATA_PRIMARY_DATA, (unsigned char *)buffer, 256);
        buffer += 256;
    }

    // Flush the cache.
    port_outb(ATA_PRIMARY_COMM_REGSTAT, 0xE7);
    // Poll for BSY.
    while (port_inb(ATA_PRIMARY_COMM_REGSTAT) & ATA_STAT_BSY);
    return TRUE;
}

bool_t atapio_fill(uint64_t sector, uint32_t count, uint8_t* buffer)
{
    for (uint32_t i = 0; i < count; i++)
    {
        if (!atapio_write(sector + i, 1, buffer + (ATA_SECTOR_SIZE * i))) { return FALSE; }
    }
    return TRUE;
}