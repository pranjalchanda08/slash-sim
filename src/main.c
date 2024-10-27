#include "main.h"

rv32i_ctx_t *g_rv32i_ctx;
ram_t g_dram_mem;
ram_t g_iram_mem;

static void rv32_ram_attach()
{
    init_ram(RAM_SIZE);
    printf("RAM Init done\n");
    ram_space_reg(&g_iram_mem, IRAM_SIZE, IRAM_BASE);
    printf("IRAM Space Allocation done: s:0x%x, b:0x%x \n", IRAM_SIZE, IRAM_BASE);
    ram_space_reg(&g_dram_mem, DRAM_SIZE, DRAM_BASE);
    printf("DRAM Space Allocation done: s:0x%x, b:0x%x \n", DRAM_SIZE, DRAM_BASE);
}

static void rv32_ram_detach()
{
    printf("RAM De-Init done\n");
    deinit_ram();
}

static uint32_t rv32_ram_store_ibin(char const *file_path)
{
    FILE *ibin = fopen(file_path, "rb");
    if (!ibin)
    {
        printf("Error reading bin file \n");
        exit(1);
    }
    else
        printf("ibin found. Reading %s\n", file_path);
    uint32_t iword = 0;
    uint32_t imem_addr = IRAM_BASE;

    while (fread(&iword, sizeof(uint32_t), 1, ibin))
    {
        ram_store(&g_iram_mem, imem_addr, 32, iword);
        imem_addr += sizeof(uint32_t);
    }
    printf("Reading binary Success. Max Addr: 0x%0x\n", imem_addr);
    fclose(ibin);
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
        buff = ram_load(&g_iram_mem, imem_addr, 8);
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
    printf("rv32I emu Startup \n");
    rv32_ram_attach();
    /* Read instruction and data binary and save it to the ram instance */
    rv32_ram_store_ibin(argv[1]);
    g_rv32i_ctx = (rv32i_ctx_t *) malloc(sizeof(rv32i_ctx_t));
    memset(g_rv32i_ctx, 0, sizeof(rv32i_ctx_t));
    printf("\n-------------- Execution Start --------------\n");
    rv32_fetch(&g_iram_mem);
    printf("-------------- Execution End ----------------\n\n");

    rv32_ram_dump(argv[2]);
    rv32_cpu_reg_dump(argv[2]);
    rv32_ram_detach();
    free(g_rv32i_ctx);
    return 0;
}
