#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include <ctype.h>
#include "libcpm.h"

void print(const char* s) __z88dk_fastcall
{
    for (;;)
    {
        uint8_t b = *s++;
        if (!b)
            return;
        putchar(b);
    }
}

void crlf(void)
{
    print("\r\n");
}

void printx(const char* s) __z88dk_fastcall
{
    print(s);
    crlf();
}

void printhex4(uint8_t nibble) __z88dk_fastcall
{
	nibble &= 0x0f;
    if (nibble < 10)
        nibble += '0';
    else
        nibble += 'a' - 10;
    putchar(nibble);
}

void printhex8(uint8_t b) __z88dk_fastcall
{
    printhex4(b >> 4);
    printhex4(b);
}

void printhex24(uint32_t n) __z88dk_fastcall
{
    printhex8(n >> 16);
    printhex8(n >> 8);
    printhex8(n);
}

void main(void)
{
    cpm_fcb.cr = 0;
    if (cpm_open_file(&cpm_fcb) == 0xff)
    {
        printx("Cannot open source file");
        return;
    }

    uint32_t address = 0;
    for (;;)
    {
        if (cpm_read_sequential(&cpm_fcb) != 0)
            break;
        
        const uint8_t* data = (const uint8_t*)0x80;
        for (uint8_t i=0; i<8; i++)
        {
			printhex24(address);
			print(" : ");
            address += 16;

            for (uint8_t j=0; j<16; j++)
            {
                uint8_t b = *data++;
                printhex8(b);
                putchar(' ');
            }
            data -= 16;

            print(": ");
            for (uint8_t j=0; j<16; j++)
            {
                uint8_t b = *data++;
                putchar(isprint(b) ? b : '.');
            }

            crlf();
        }
    }
}
