/*****************************************************************************************
 * SLASH-SIM LICENSE
 * Copyrights (C) <2024>, Pranjal Chanda
 *
 * @file    main.c
 * @brief   Entry point and core logic of the Slash Sim RV32 simulator.
 *          This file initializes the context, handles memory, and loads ELF binaries.
 *****************************************************************************************/

/*****************************************************************************************
 * INCLUDES
 *****************************************************************************************/
#include <main.h>

/*****************************************************************************************
 * GLOBALS
 *****************************************************************************************/
rv32i_csr_t g_rv32i_csr;
rv32i_ctx_t *g_rv32i_ctx;
slash_app_config_t g_app_cfg;

/*****************************************************************************************
 * STATICS
 *****************************************************************************************/
static rv_elf_section_info sections;

/*****************************************************************************************
 * FUNCTION DECLARATION
 *****************************************************************************************/

static rv32_err_t rv32_ram_store_elf(char const *file_path, uint32_t *imem_addr);
static void rv32_ram_dump(char const *asm_name, size_t ram_base_addr);
static void rv32_cpu_reg_dump(char const *asm_name);
static rv32_err_t parse_args(int argc, char const *argv[]);

/*****************************************************************************************
 * FUNCTION DEFINATION
 *****************************************************************************************/

/*******************************************************************************************
 * @brief  Entry point of the simulator. Initializes the context and loads the ELF binary.
 *
 * @param[in] argc  Argument count.
 * @param[in] argv  Argument values.
 *
 * @return Exit status of the program.
 ******************************************************************************************/
int main(int argc, char const *argv[])
{
    rv32_err_t err = RV32_SUCCESS;
    uint32_t entry_point;

    LOG_DEBUG("Slash-Sim Startup");
    err = parse_args(argc, argv);
    RV32_ASSERT(err);

    /* Initialise the CPU Context */
    g_rv32i_ctx = (rv32i_ctx_t *)calloc(sizeof(rv32i_ctx_t), 1);
    if (!g_rv32i_ctx)
    {
        return RV32_ERR_MALLOC;
    }

    err = device_tree_register();
    RV32_ASSERT_GOTO(err, ramdump_exit);
    /* Read instruction and data binary and save it to the ram instance */
    err = rv32_ram_store_elf(g_app_cfg.elf_path, &entry_point);
    RV32_ASSERT_GOTO(err, elf_exit);

    printf("\n-------------- Execution Start --------------\n");
    rv32_fetch(entry_point);
    printf("-------------- Execution End ----------------\n\n");

elf_exit:
    rv32_cpu_reg_dump(g_app_cfg.elf_name);
ramdump_exit:
    err = device_tree_deregister();
    free(g_rv32i_ctx);
    return err;
}

/*******************************************************************************************
 * @brief   Read an ELF file, extract binary from it and store it in different sections
 *
 * @param[in] file_path     Path to the ELF file
 * @param[out] imem_addr    VMA of the entry point
 * @return rv32_err_t
 ******************************************************************************************/
static rv32_err_t rv32_ram_store_elf(char const *file_path, uint32_t *imem_addr)
{
    FILE *elf = fopen(file_path, "rb");
    if (!elf)
    {
        LOG_ERROR("File not found");
        return RV32_ERR_ELF_FILE;
    }

    if (EXIT_SUCCESS != read_elf(elf, (size_t *)imem_addr))
    {
        LOG_ERROR("Read ELF Error");
        return RV32_ERR_ELF_FILE;
    }

    LOG_DEBUG("Reading binary Success. PC: 0x%0x", *imem_addr);
    return RV32_SUCCESS;
}

/*******************************************************************************************
 * @brief Save the reg dump of the current CPU state
 *
 * @param[in] asm_name  Name of the program running on sim
 ******************************************************************************************/
static void rv32_cpu_reg_dump(char const *asm_name)
{
    char *out_file_path = malloc(strlen(asm_name) + 20);
    char *line = malloc(100);
    sprintf(out_file_path, "out/%s/reg_dump.txt", asm_name);
    LOG_INFO("Saving REG Dump: %s", out_file_path);
    FILE *mem = fopen(out_file_path, "w");
    for (size_t i = 0; i < sizeof(rv32i_ctx_t) / sizeof(uint32_t); i++)
    {
        sprintf(line, "%s: 0x%08x\n", reg_name_list[i], *((uint32_t *)g_rv32i_ctx + i));
        fputs(line, mem);
    }
    free(line);
    free(out_file_path);
    fclose(mem);
}

/*******************************************************************************************
 * @brief Check the command line args provided to the sim
 *
 * @param argc  No of args
 * @param argv  Pointer to args
 * @return rv32_err_t
 ******************************************************************************************/
static rv32_err_t parse_args(int argc, char const *argv[])
{
    if (argc < 2)
    {
        LOG_ERROR("ELF not provided!!");
        return RV32_ERR_ASSERT_PARAM;
    }
    if (argc < 3)
    {
        LOG_ERROR("ELF name not provided");
        return RV32_ERR_ASSERT_PARAM;
    }
    g_app_cfg.elf_path = argv[1];
    g_app_cfg.elf_name = argv[2];

    return RV32_SUCCESS;
}
