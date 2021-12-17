#include <stdlib.h>
#include <stdint.h>
#include <stddef.h>
#include <stdarg.h>
#include <stdio.h>
#include <string.h>

typedef struct
{
    char     name[64];
    uint8_t  reserved[60];
    uint32_t signature;
} __attribute__((packed)) header_t;

uint8_t* output_data;
uint32_t output_size;

void parse_program(const char* filename, header_t header);
void add_header(uint8_t* elf_data, uint32_t size, header_t header);
void save_output(const char* filename);

int main(int argc, char** argv)
{
    header_t header;
    memset(&header, 0, sizeof(header_t));
    strcpy(header.name, "Demo Program");
    parse_program(argv[1], header);
    save_output(argv[2]);
    return 0;
}

void parse_program(const char* filename, header_t header)
{
    // attempt to load file
    FILE* fptr = fopen(filename, "rb");
    if (fptr == NULL) { printf("Unable to locate file '%s'", filename); return; }
    fseek(fptr, 0, SEEK_END);

    // get file size
    uint32_t size = ftell(fptr);
    fseek(fptr, 0, SEEK_SET);
    if (size == 0) { printf("Invalid file '%s'", filename); return; }

    // load data
    uint8_t* elf_data = malloc(size + 1);
    memset(elf_data, 0, size + 1);
    fread(elf_data, size, 1, fptr);

    // add header 
    add_header(elf_data, size + 1, header);
}

void add_header(uint8_t* elf_data, uint32_t size, header_t header)
{
    if (output_data != NULL) { free(output_data); }
    output_data = malloc(size + sizeof(header_t));
    memset(output_data, 0, size + sizeof(header_t));
    memset(header.reserved, 0, sizeof(header.reserved));
    header.signature = 0xBAD0A515;

    memcpy(output_data + sizeof(header_t), elf_data, size);
}

void save_output(const char* filename)
{
    FILE* fptr = fopen(filename, "wb");
    if (fptr == NULL) { printf("Unable to save output to '%s'\n", filename); return; }
    fwrite(output_data, output_size, 1, fptr);
    fclose(fptr);
}