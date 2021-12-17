#include <kernel/runtime/elf.h>
#include <kernel/core/kernel.h>

bool_t method_table_loaded = FALSE; 
void** method_table = NULL;

void* elf_map_methods()
{
	if (method_table_loaded) { return method_table; }

	method_table = tcalloc(4096 * sizeof(void*), MEMSTATE_PTRARRAY);
	
	method_table[0] = (uint32_t)mm_allocate;
	method_table[1] = (uint32_t)mm_free;
	method_table[2] = (uint32_t)mm_amount_installed;
	method_table[3] = (uint32_t)mm_amount_reserved;
	method_table[4] = (uint32_t)mm_amount_used;

	method_table[5] = (uint32_t)term_clear_col;
	method_table[6] = (uint32_t)term_writechar_col;
	method_table[7] = (uint32_t)term_write_col;

	return method_table;
}

bool_t elf_identify(elf_header_t* header)
{
    if (header->e_ident[0] == 0x7F && header->e_ident[1] == 'E' && header->e_ident[2] == 'L' && header->e_ident[3] == 'F') { return TRUE; }
    return FALSE;
}

bool_t elf_probe(uint8_t* buffer)
{
    elf_header_t* header = (elf_header_t*)buffer;
    if (elf_identify(header))
    {
        debug_info("Validated ELF");
        return TRUE;
    }
    return FALSE;
}

uint8_t elf_start(uint8_t* buffer, uint32_t buffer_size, elf_priv_data* priv)
{
    if (!elf_probe(buffer)) { return FALSE; }
    elf_header_t *header = (elf_header_t *)buffer;
	debug_info("Type: %s%s%s\n",
		header->e_ident[4] == 1 ? "32bit ":"64 bit",
		header->e_ident[5] == 1 ? "Little Endian ":"Big endian ",
		header->e_ident[6] == 1 ? "True ELF ":"buggy ELF ");

	if(header->e_type != 2)
	{
		debug_info("File is not executable!, type = 0x%2x", header->e_type);
		return 0;
	}

	uint8_t* prog = tcalloc(buffer_size + 0x1000, MEMSTATE_ARRAY);

	elf_program_header_t *ph = (elf_program_header_t *)(buffer + header->e_phoff);
	for(int i = 0; i < header->e_phnum; i++, ph++)
	{
		switch(ph->p_type)
		 {
		 	case 0: { break; }
		 	case 1:
            {
                uint8_t* phys_loc = tcalloc(ph->p_filesz, MEMSTATE_ARRAY);
		 		debug_info("LOAD: offset 0x%x vaddr 0x%x paddr 0x%x filesz 0x%x memsz 0x%x",
		 				ph->p_offset, ph->p_vaddr, ph->p_paddr, ph->p_filesz, ph->p_memsz);
		 		paging_map(ph->p_vaddr, prog);
		 		memcpy(ph->p_vaddr, buffer + ph->p_offset, ph->p_filesz);
		 		break;
            }
		 	default:
            {
                debug_error("Unsupported p_type! Bail out!");
                return 0;
            }
		 }
	}

    thread_t* thread = thread_create_ext("program", (uint8_t*)header->e_entry, ph->p_filesz, 0x20000, 0);
    term_writeln("Running external program");
    taskmgr_ready_thread(thread);
    return TRUE;
}