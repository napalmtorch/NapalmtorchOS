#include <kernel/hardware/common/serial.h>
#include <kernel/core/kernel.h>

// private methods
bool_t serial_recieved();
bool_t serial_send();
bool_t serial_validate();

// properties
const char* SERIAL_PORTNAMES[6] = { "DISABLED", "COM1", "COM2", "COM3", "COM4", "INVALID" };
SERIALPORT serial_port;

// set current serial port
void serial_setport(SERIALPORT port)
{
    // set current port value
    serial_port = port;

    // send port data to serial controller
    port_outb(serial_port + 1, 0x00);    // disable all interrupts
    port_outb(serial_port + 3, 0x80);    // set baud rate divisor
    port_outb(serial_port + 0, 0x03);    // set divisor to 3 (lo uint8_t) 38400 baud
    port_outb(serial_port + 1, 0x00);    // hi uint8_t
    port_outb(serial_port + 3, 0x03);    // 8 bits, no parity, one stop bit
    port_outb(serial_port + 2, 0xC7);    // enable fifo, clear them, with 14-uint8_t threshold
    port_outb(serial_port + 4, 0x0B);    // irqs enabled, rts/dsr set
    port_outb(serial_port + 4, 0x1E);    // set in loopback mode, test the serial chip
    port_outb(serial_port + 0, 0xAE);    // test serial chip (send uint8_t 0xAE and check if serial returns same uint8_t)

    // check if serial is faulty
    if (port_inb(serial_port) != 0xAE) { serial_port = SERIALPORT_NONE; return; }
    
    // ff serial is not faulty set it in normal operation mode
    port_outb(serial_port + 4, 0x0F);
}

// get current serial port
SERIALPORT serial_getport() { return serial_port; }

// get string representation of specified serial port
const char* serial_getportstr(SERIALPORT port)
{
    switch (port)
    {
        case SERIALPORT_NONE: { return SERIAL_PORTNAMES[0]; }
        case SERIALPORT_COM1: { return SERIAL_PORTNAMES[1]; }
        case SERIALPORT_COM2: { return SERIAL_PORTNAMES[2]; }
        case SERIALPORT_COM3: { return SERIAL_PORTNAMES[3]; }
        case SERIALPORT_COM4: { return SERIAL_PORTNAMES[4]; }
        default:              { return SERIAL_PORTNAMES[5]; }
    }
}

// read character from current serial port
char serial_read()
{
    if (!serial_validate()) { return 0; }
    while (!serial_recieved());
    return port_inb(serial_port);
}

// write character to current serial port
void serial_writechar(char c)
{
    if (!serial_validate()) { return; }
    while (!serial_send());
    port_outb(serial_port, c);
}

// write colored character to current serial port
void serial_writechar_col(char c, uint8_t color)
{
    serial_setcol(color);
    serial_writechar(c);
    serial_setcol(0xFF);
}

// write string to current serial port
void serial_write(const char* str)
{
    if (!serial_validate()) { return; }
    int i = 0;
    while (str[i] != 0) { serial_writechar(str[i]); i++; }
}

// write colored string to current serial port
void serial_write_col(const char* str, uint8_t color)
{
    serial_setcol(color);
    serial_write(str);
    serial_setcol(0xFF);
}

// write line to current serial port
void serial_writeln(const char* str)
{
    serial_write(str);
    serial_newline();
}

// write colored line to current serial port
void serial_writeln_col(const char* str, uint8_t color)
{
    serial_write_col(str, color);
    serial_newline();
}

// write a new line to current serial port
void serial_newline() { serial_writechar('\n'); }

// print formatted string with va_list of arguments to current serial port
int serial_vprintf(const char* str, va_list args)
{
    while (*str != 0)
    {
        if (*str == '%')
        {
            str++;
            if (*str == '%') { serial_writechar('%'); }
            if (*str == 'c') { serial_writechar((char)va_arg(args, int)); }
            else if (*str == 'd' || *str == 'u')
            {
                int dec = va_arg(args, int);
                char str[16];
                memset(str, 0, 16);
                serial_write(itoa(dec, str, 10));
            }
            else if (*str == 'u')
            {
                uint32_t dec = va_arg(args, uint32_t);
                char str[16];
                memset(str, 0, 16);
                serial_write(ltoa(dec, str, 10));
            }
            else if (*str == 'x')
            {
                uint32_t dec = va_arg(args, uint32_t);
                char str[16];
                memset(str, 0, 16);
                serial_write(ltoa(dec, str, 16));
            }
            else if (*str == '2')
            {
                str++;
                if (*str != 'x' && *str != 'X') { serial_writechar(*str); }
                else
                {
                    uint32_t num = va_arg(args, uint32_t);
                    char str[16];
                    memset(str, 0, 16);
                    serial_write(strhex(num, str, FALSE, 1));
                }
            }
            else if (*str == '4')
            {
                str++;
                if (*str != 'x' && *str != 'X') { serial_writechar(*str); }
                else
                {
                    uint32_t num = va_arg(args, uint32_t);
                    char str[16];
                    memset(str, 0, 16);
                    serial_write(strhex(num, str, FALSE, 2));
                }
            }
            else if (*str == '8')
            {
                str++;
                if (*str != 'x' && *str != 'X') { serial_writechar(*str); }
                else
                {
                    uint32_t num = va_arg(args, uint32_t);
                    char str[16];
                    memset(str, 0, 16);
                    serial_write(strhex(num, str, FALSE, 4));
                }
            }
            else if (*str == 'f')
            {
                /* TODO : implement float to string */
            }
            else if (*str == 's')
            {
                char* val = va_arg(args, char*);
                serial_write(val);
            }
            else { serial_writechar(*str); }
        }
        else { serial_writechar(*str); }
        str++;
    }

    return TRUE;
}

// print formatted string to current serial port
int serial_printf(const char* str, ...)
{
    va_list args;
    va_start(args, str);
    int res = serial_vprintf(str, args);
    va_end(args);
    return res;
}

// set color of current serial port
void serial_setcol(uint8_t color)
{
    if (!serial_validate()) { return; }
    switch (color)
    {
        case 0x0: { serial_write("\033[34");  break; }
        case 0x1: { serial_write("\033[34m"); break; }
        case 0x2: { serial_write("\033[32m"); break; }
        case 0x3: { serial_write("\033[36m"); break; }
        case 0x4: { serial_write("\033[31m"); break; }
        case 0x5: { serial_write("\033[35m"); break; }
        case 0x6: { serial_write("\033[33m"); break; }
        case 0x7: { serial_write("\033[37m"); break; }
        case 0x8: { serial_write("\033[37m"); break; }
        case 0x9: { serial_write("\033[34m"); break; }
        case 0xA: { serial_write("\033[32m"); break; }
        case 0xB: { serial_write("\033[36m"); break; }
        case 0xC: { serial_write("\033[31m"); break; }
        case 0xD: { serial_write("\033[35m"); break; }
        case 0xE: { serial_write("\033[33m"); break; }
        case 0xF: { serial_write("\033[37m"); break; }
        default:  { serial_write("\033[0m");  break; }
    }
}

// check if serial has recieved data
bool_t serial_recieved()
{
    if (!serial_validate()) { return FALSE; }
    return port_inb((uint32_t)(serial_port + 5)) & 1;
}

// check if serial is able to send data
bool_t serial_send()
{
    if (!serial_validate()) { return FALSE; }
    return port_inb((uint32_t)(serial_port + 5)) & 0x20;
}

// validate current serial port
bool_t serial_validate()
{
    if (serial_port == SERIALPORT_COM1) { return TRUE; }
    if (serial_port == SERIALPORT_COM2) { return TRUE; }
    if (serial_port == SERIALPORT_COM3) { return TRUE; }
    if (serial_port == SERIALPORT_COM4) { return TRUE; }
    return FALSE;
}