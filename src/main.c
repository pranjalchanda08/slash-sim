#include "main.h"

static ram_t *ram;
rv32i_csr_t g_rv32i_csr;
rv32i_ctx_t *g_rv32i_ctx;
static rv_elf_section_info sections;

static void rv32_ram_attach()
{
    ram = init_ram(RAM_SIZE);
    if (ram)
    {
        printf("RAM Init done\n");
    }
    else
    {
        printf("[E: ] RAM Init Failed\n");
        exit(EXIT_FAILURE);
    }
}

static void rv32_ram_detach()
{
    printf("RAM De-Init done\n");
    deinit_ram();
}

static uint32_t rv32_ram_store_elf(char const *file_path)
{
    FILE *elf = fopen(file_path, "rb");
    if (!elf)
    {
        printf("Error reading bin file \n");
        exit(1);
    }
    else
        printf("ELF found. Reading %s\n", file_path);

    uint32_t imem_addr = 0;

    if (EXIT_SUCCESS != read_elf(elf, (size_t *)&imem_addr, ram))
    {
        printf("[E] : Read ELF Error\n");
        exit(EXIT_FAILURE);
    }

    printf("Reading binary Success. PC: 0x%0x\n", imem_addr);
    return imem_addr;
}

static void rv32_ram_dump(char const *asm_name)
{
    char *out_file_path = malloc(strlen(asm_name) + 20);
    sprintf(out_file_path, "out/%s/ram_dump.bin", asm_name);
    printf("Saving RAM Dump: %s\n", out_file_path);
    FILE *mem = fopen(out_file_path, "wb");
    uint32_t buff;
    uint32_t imem_addr = 0;
    while (imem_addr < RAM_SIZE)
    {
        buff = ram_load(ram, imem_addr, 8);
        imem_addr++;
        fwrite(&buff, sizeof(uint8_t), 1, mem);
    }
    free(out_file_path);
    fclose(mem);
}

static void rv32_cpu_reg_dump(char const *asm_name)
{
    char *out_file_path = malloc(strlen(asm_name) + 20);
    char *line = malloc(100);
    sprintf(out_file_path, "out/%s/reg_dump.txt", asm_name);
    printf("Saving REG Dump: %s\n", out_file_path);
    FILE *mem = fopen(out_file_path, "w");
    for (size_t i = 0; i < sizeof(rv32i_ctx_t)/sizeof(uint32_t); i++)
    {
        sprintf(line, "%4s: 0x%08x\n", reg_name_list[i], *((uint32_t*)g_rv32i_ctx + i));
        fputs(line, mem);
    }
    free(line);
    free(out_file_path);
    fclose(mem);
}

int main(int argc, char const *argv[])
{
    if (argc < 2)
    {
        printf("Instuction Binary not provided!!\n");
        return 1;
    }
    if (argc < 3)
    {
        printf("ASM name not provided\n");
        return 1;
    }
    printf("Slash-Sim Startup \n");
    rv32_ram_attach();
    /* Read instruction and data binary and save it to the ram instance */
    uint32_t entry_point = rv32_ram_store_elf(argv[1]);
    g_rv32i_ctx = (rv32i_ctx_t *)malloc(sizeof(rv32i_ctx_t));
    memset(g_rv32i_ctx, 0, sizeof(rv32i_ctx_t));
    printf("\n-------------- Execution Start --------------\n");
    rv32_fetch(ram, entry_point);
    printf("-------------- Execution End ----------------\n\n");

    rv32_ram_dump(argv[2]);
    rv32_cpu_reg_dump(argv[2]);
    rv32_ram_detach();
    free(g_rv32i_ctx);
    return 0;
}
