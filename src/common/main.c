#include "main.h"

static ram_t *ram;
rv32i_csr_t g_rv32i_csr;
rv32i_ctx_t *g_rv32i_ctx;
static rv_elf_section_info sections;

static rv32_err_t rv32_ram_attach();
static rv32_err_t rv32_ram_detach();
static rv32_err_t rv32_ram_store_elf(char const *file_path, uint32_t *imem_addr);
static void rv32_ram_dump(char const *asm_name);
static void rv32_cpu_reg_dump(char const *asm_name);
static rv32_err_t check_args(int argc, char const *argv[]);

int main(int argc, char const *argv[])
{
    rv32_err_t err = RV32_SUCCESS;
    uint32_t entry_point;

    printf("Slash-Sim Startup\n");
    err = check_args(argc, argv);
    RV32_ASSERT(err);

    /* Initialise the CPU Context */
    g_rv32i_ctx = (rv32i_ctx_t *)malloc(sizeof(rv32i_ctx_t));
    if (!g_rv32i_ctx)
    {
        return RV32_ERR_MALLOC;
    }
    memset(g_rv32i_ctx, 0, sizeof(rv32i_ctx_t));

    err = rv32_ram_attach();
    RV32_ASSERT_GOTO(err, exit);
    /* Read instruction and data binary and save it to the ram instance */
    err = rv32_ram_store_elf(argv[1], &entry_point);
    RV32_ASSERT_GOTO(err, exit);

    printf("\n-------------- Execution Start --------------\n");
    rv32_fetch(ram, entry_point);
    printf("-------------- Execution End ----------------\n\n");

exit:
    rv32_ram_dump(argv[2]);
    rv32_cpu_reg_dump(argv[2]);
    rv32_ram_detach();
    free(g_rv32i_ctx);
    return RV32_SUCCESS;
}

static rv32_err_t rv32_ram_attach()
{
    ram = init_ram(RAM_SIZE);
    if (ram)
    {
        printf("RAM Init done\n");
    }
    else
    {
        printf("[E: ] RAM Init Failed\n");
        return RV32_ERR_RAM_INIT;
    }
    return RV32_SUCCESS;
}

static rv32_err_t rv32_ram_detach()
{
    printf("RAM De-Init done\n");
    deinit_ram();
    return RV32_SUCCESS;
}

static rv32_err_t rv32_ram_store_elf(char const *file_path, uint32_t *imem_addr)
{
    FILE *elf = fopen(file_path, "rb");
    if (!elf)
    {
        printf("[E] : File not found \n");
        return RV32_ERR_ELF_FILE;
    }

    if (EXIT_SUCCESS != read_elf(elf, (size_t *)imem_addr, ram))
    {
        printf("[E] : Read ELF Error\n");
        return RV32_ERR_ELF_FILE;
    }

    printf("Reading binary Success. PC: 0x%0x\n", *imem_addr);
    return RV32_SUCCESS;
}

static void rv32_ram_dump(char const *asm_name)
{
    uint32_t buff;
    uint32_t imem_addr = 0;
    char *out_file_path = malloc(strlen(asm_name) + 20);
    sprintf(out_file_path, "out/%s/ram_dump.bin", asm_name);
    printf("Saving RAM Dump: %s\n", out_file_path);
    FILE *mem = fopen(out_file_path, "wb");
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
    for (size_t i = 0; i < sizeof(rv32i_ctx_t) / sizeof(uint32_t); i++)
    {
        sprintf(line, "%4s: 0x%08x\n", reg_name_list[i], *((uint32_t *)g_rv32i_ctx + i));
        fputs(line, mem);
    }
    free(line);
    free(out_file_path);
    fclose(mem);
}

static rv32_err_t check_args(int argc, char const *argv[])
{
    if (argc < 2)
    {
        printf("Instuction Binary not provided!!\n");
        return RV32_ERR_ASSERT_PARAM;
    }
    if (argc < 3)
    {
        printf("ASM name not provided\n");
        return RV32_ERR_ASSERT_PARAM;
    }
    return RV32_SUCCESS;
}
