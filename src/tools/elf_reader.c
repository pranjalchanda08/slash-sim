/*****************************************************************************************
 * SLASH-SIM LICENSE
 * Copyrights (C) <2024>, Pranjal Chanda
 *
 * @file    elf_reader.c
 * @brief   Provides definations related to ELF file readings
 *****************************************************************************************/

/*****************************************************************************************
 * INCLUDES
 *****************************************************************************************/
#include "elf_reader.h"
#include "logging.h"

/*****************************************************************************************
 * FUNCTION DEFINATIONS
 *****************************************************************************************/
/******************************************************************************************
 * @brief Prints the section details in tabulated format
 *
 * @param[in] sections      Array of predecoded sections
 * @param[in] num_sections  No of sections in the array
 *****************************************************************************************/
static void print_section_details(rv_elf_section_info *sections, int num_sections)
{
    printf("==================================================\n");
    printf("  Section Name   |  Offset  |  Size   | Address \n");
    printf("==================================================\n");
    for (int i = 0; i < num_sections; i++)
    {
        if (sections[i].loadable)
        {
            printf("%-16s | 0x%06lx | 0x%05lx | 0x%08lx\n",
                   sections[i].name,
                   sections[i].offset,
                   sections[i].size,
                   sections[i].addr);
        }
    }
    printf("==================================================\n");
}

/******************************************************************************************
 * @brief Function to extract binary data of all loadable sections
 *
 * @param[in] file          File Descripter of the ELF file
 * @param[in] sections      Array of predecoded sections
 * @param[in] num_sections  No of sections in the array
 * @return int
 *****************************************************************************************/
static int extract_binary_data(FILE *file, rv_elf_section_info *sections, int num_sections)
{
    rv32_err_t err;
    for (int i = 0; i < num_sections; i++)
    {
        if (sections[i].loadable)
        {
            uint32_t *data = (uint32_t *)malloc(sections[i].size);
            if (!data)
            {
                LOG_DEBUG("Failed to allocate memory for section data");
                return -1;
            }
            fseek(file, sections[i].offset, SEEK_SET);
            fread(data, 1, sections[i].size, file);
            for (size_t j = 0; j < (sections[i].size / sizeof(uint32_t)); j++)
            {
                err = peripheral_exec_store((sections[i].addr + (j * sizeof(uint32_t))), sizeof(uint32_t), (uint8_t *)(data + j));
                RV32_ASSERT(err);
            }
            free(data);
            LOG_DEBUG("Extracted binary data for section: %s", sections[i].name);
        }
    }
    return 0; // Success
}

/******************************************************************************************
 * @brief Function to read ELF32 sections
 *
 * @param[in] file                  File Descripter of the ELF file
 * @param[in] elf_header            Pre Read ELF Header
 * @param[out] sections             Space holder for sections array allocation
 * @param[out] total_memory_size    Total memory size (Loadable)
 * @return int  No of section entry in the ELF table header
 *****************************************************************************************/
static int read_elf32_sections(FILE *file, Elf32_Ehdr *elf_header, rv_elf_section_info **sections, size_t *total_memory_size)
{
    fseek(file, elf_header->e_shoff, SEEK_SET);
    Elf32_Shdr *section_headers = malloc(elf_header->e_shnum * sizeof(Elf32_Shdr));
    if (!section_headers)
    {
        LOG_DEBUG("Failed to allocate memory for section headers");
        return -1;
    }
    fread(section_headers, elf_header->e_shnum, sizeof(Elf32_Shdr), file);

    // Read section name string table
    Elf32_Shdr shstrtab_header = section_headers[elf_header->e_shstrndx];
    char *shstrtab = malloc(shstrtab_header.sh_size);
    if (!shstrtab)
    {
        LOG_DEBUG("Failed to allocate memory for section names");
        free(section_headers);
        return -1;
    }
    fseek(file, shstrtab_header.sh_offset, SEEK_SET);
    fread(shstrtab, 1, shstrtab_header.sh_size, file);

    *sections = malloc(elf_header->e_shnum * sizeof(rv_elf_section_info));
    if (!*sections)
    {
        LOG_DEBUG("Failed to allocate memory for rv_elf_section_info");
        free(section_headers);
        free(shstrtab);
        return -1;
    }

    size_t first_addr = (size_t)-1, last_addr = 0;

    // Populate rv_elf_section_info array and track memory layout size
    for (int i = 0; i < elf_header->e_shnum; i++)
    {
        (*sections)[i].name = strdup(&shstrtab[section_headers[i].sh_name]);
        (*sections)[i].offset = section_headers[i].sh_offset;
        (*sections)[i].size = section_headers[i].sh_size;
        (*sections)[i].addr = section_headers[i].sh_addr;
        (*sections)[i].loadable = section_headers[i].sh_flags & SHF_ALLOC;

        if ((*sections)[i].loadable)
        {
            if (section_headers[i].sh_addr < first_addr)
                first_addr = section_headers[i].sh_addr;
            if (section_headers[i].sh_addr + section_headers[i].sh_size > last_addr)
                last_addr = section_headers[i].sh_addr + section_headers[i].sh_size;
        }
    }

    *total_memory_size = last_addr - first_addr;

    free(section_headers);
    free(shstrtab);
    return elf_header->e_shnum;
}

/******************************************************************************************
 * @brief Function to read ELF64 sections
 *
 * @param[in] file                  File Descripter of the ELF file
 * @param[in] elf_header            Pre Read ELF Header
 * @param[out] sections             Space holder for sections array allocation
 * @param[out] total_memory_size    Total memory size (Loadable)
 * @return int  No of section entry in the ELF table header
 *****************************************************************************************/
static int read_elf64_sections(FILE *file, Elf64_Ehdr *elf_header, rv_elf_section_info **sections, size_t *total_memory_size)
{
    fseek(file, elf_header->e_shoff, SEEK_SET);
    Elf64_Shdr *section_headers = malloc(elf_header->e_shnum * sizeof(Elf64_Shdr));
    if (!section_headers)
    {
        LOG_DEBUG("Failed to allocate memory for section headers");
        return -1;
    }
    fread(section_headers, elf_header->e_shnum, sizeof(Elf64_Shdr), file);

    // Read section name string table
    Elf64_Shdr shstrtab_header = section_headers[elf_header->e_shstrndx];
    char *shstrtab = malloc(shstrtab_header.sh_size);
    if (!shstrtab)
    {
        LOG_DEBUG("Failed to allocate memory for section names");
        free(section_headers);
        return -1;
    }
    fseek(file, shstrtab_header.sh_offset, SEEK_SET);
    fread(shstrtab, 1, shstrtab_header.sh_size, file);

    *sections = malloc(elf_header->e_shnum * sizeof(rv_elf_section_info));
    if (!*sections)
    {
        LOG_DEBUG("Failed to allocate memory for rv_elf_section_info");
        free(section_headers);
        free(shstrtab);
        return -1;
    }

    size_t first_addr = (size_t)-1, last_addr = 0;

    // Populate rv_elf_section_info array and track memory layout size
    for (int i = 0; i < elf_header->e_shnum; i++)
    {
        (*sections)[i].name = strdup(&shstrtab[section_headers[i].sh_name]);
        (*sections)[i].offset = section_headers[i].sh_offset;
        (*sections)[i].size = section_headers[i].sh_size;
        (*sections)[i].addr = section_headers[i].sh_addr;
        (*sections)[i].loadable = section_headers[i].sh_flags & SHF_ALLOC;

        if ((*sections)[i].loadable)
        {
            if (section_headers[i].sh_addr < first_addr)
                first_addr = section_headers[i].sh_addr;
            if (section_headers[i].sh_addr + section_headers[i].sh_size > last_addr)
                last_addr = section_headers[i].sh_addr + section_headers[i].sh_size;
        }
    }

    *total_memory_size = last_addr - first_addr;

    free(section_headers);
    free(shstrtab);
    return elf_header->e_shnum;
}

/******************************************************************************************
 * @brief Read ELF and save the data into RAM corresponding address
 *
 * @param[in] file          File Descripter of the ELF file
 * @param[out] entry_point  PC entry address
 * @return int
 *****************************************************************************************/
int read_elf(FILE *file, size_t *entry_point)
{
    unsigned char e_ident[EI_NIDENT];
    fread(e_ident, 1, EI_NIDENT, file);

    rv_elf_section_info *sections = NULL;
    int num_sections = 0;
    size_t total_memory_size = 0;

    fseek(file, 0, SEEK_SET);
    if (e_ident[EI_CLASS] == ELFCLASS32)
    {
        Elf32_Ehdr elf_header;
        fread(&elf_header, 1, sizeof(elf_header), file);
        num_sections = read_elf32_sections(file, &elf_header, &sections, &total_memory_size);
        *entry_point = elf_header.e_entry;
    }
    else if (e_ident[EI_CLASS] == ELFCLASS64)
    {
        Elf64_Ehdr elf_header;
        fread(&elf_header, 1, sizeof(elf_header), file);
        num_sections = read_elf64_sections(file, &elf_header, &sections, &total_memory_size);
        *entry_point = elf_header.e_entry;
    }
    else
    {
        fprintf(stderr, "Unsupported ELF class\n");
        fclose(file);
        return EXIT_FAILURE;
    }

    if (num_sections < 0)
    {
        fprintf(stderr, "Failed to read sections\n");
        fclose(file);
        return EXIT_FAILURE;
    }

    print_section_details(sections, num_sections);

    if (extract_binary_data(file, sections, num_sections) < 0)
    {
        fprintf(stderr, "Failed to extract binary data\n");
        fclose(file);
        return EXIT_FAILURE;
    }

    LOG_DEBUG("Total Memory Layout Size: 0x%lx bytes", total_memory_size);

    for (int i = 0; i < num_sections; i++)
    {
        free(sections[i].name);
    }
    free(sections); // Free the sections array
    fclose(file);   // Close the ELF file

    return EXIT_SUCCESS;
}
