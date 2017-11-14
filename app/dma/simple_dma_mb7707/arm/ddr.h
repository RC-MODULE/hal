#define PL341_MEMC_STATUS             0x000
#define PL341_MEMC_CMD                0x004
#define PL341_DIRECT_CMD              0x008
#define PL341_MEMORY_CFG              0x00C 
#define PL341_REFRESH_PRD             0x010
#define PL341_CAS_LATENCY             0x014
#define PL341_WRITE_LATENCY           0x018
#define PL341_T_MRD                   0x01C
#define PL341_T_RAS                   0x020
#define PL341_T_RC                    0x024
#define PL341_T_RCD                   0x028
#define PL341_T_RFC                   0x02C
#define PL341_T_RP                    0x030
#define PL341_T_RRD                   0x034
#define PL341_T_WR                    0x038
#define PL341_T_WTR                   0x03C
#define PL341_T_XP                    0x040
#define PL341_T_XSR                   0x044
#define PL341_T_ESR                   0x048
#define PL341_MEMORY_CFG2             0x04C
#define PL341_MEMORY_CFG3             0x050
#define PL341_T_FAW                   0x054
#define PL341_TRD_EN                  0x05C


#define PL341_CHIP_0_CFG              0X200
#define PL341_CHIP_1_CFG              0X204
#define PL341_CHIP_2_CFG              0X208
#define PL341_CHIP_3_CFG              0X20C

#define EM0_Based				      0x40000000
#define EM1_Based				      0xC0000000

#define DMC0_APB_Based                0x20032000
#define DMC1_APB_Based                0x20031000

#define DDR_INITED 2
#define DDR_NOT_INITED 1

void Init_DDR_EM0();
void Init_DDR_EM1();
void Init_TSP_DDR_Data();

#define INIT_DDR_EM0 Init_DDR_EM0
#define INIT_DDR_EM1 Init_DDR_EM1

