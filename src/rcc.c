#include "rcc.h"

static const uint16_t ahb_prescalars[8] = { 2, 4, 8, 16, 64, 128, 256, 512 };
static const uint8_t apb_prescalars[4] = { 2, 4, 8, 16 };

rcc_clock_t rcc_system_clock()
{
    // 0-HSI, 1-HSE, 2-PLL_P
    return (rcc_clock_t)((RCC->cfgr >> RCC_CFGR_SWS) & 0x3U);
}

uint32_t rcc_system_clock_hz()
{
    rcc_clock_t clock = rcc_system_clock();

    switch(clock)
    {
        case RCC_CLOCK_HSI: return HSI_CLOCK_HZ;
        case RCC_CLOCK_HSE: return HSE_CLOCK_HZ;
        case RCC_CLOCK_PLL_P:
        {
            // VCO input freq = PLL input clock freq / PLLM with 2 <= PLLM <= 63
            uint32_t pllm = (RCC->pll_cfgr >> RCC_PLLCFGR_PLLM) & 0x3FU;
            // VCO output freq = VCO input freq * PLLN with 50 <= PLLN <= 432
            uint32_t plln = (RCC->pll_cfgr >> RCC_PLLCFGR_PLLN) & 0x1FFU;
            // PLL output clock freq = VCO freq / PLLP with PLLP = 2, 4, 6, or 8
            uint32_t pllp_bits = (RCC->pll_cfgr >> RCC_PLLCFGR_PLLP) & 0x3U;
            uint32_t pllp = (pllp_bits + 1U) * 2U;

            uint8_t pll_src = (RCC->pll_cfgr >> RCC_PLLCFGR_PLLSRC) & 0x1U;

            uint32_t vco_input = (pll_src == 0) ? HSI_CLOCK_HZ : HSE_CLOCK_HZ;
            vco_input /= pllm;

            uint32_t vco_output = vco_input * plln;
            return vco_output / pllp;
        }
        case RCC_CLOCK_PLL_R:
        default: return 0;
    }
}

uint16_t rcc_ahb_prescalar()
{
    uint8_t temp = (RCC->cfgr >> RCC_CFGR_HPRE) & 0xFU;
    return (temp > 7) ? ahb_prescalars[temp - 8] : 1U;
}

uint8_t rcc_apb1_prescalar()
{
    uint8_t temp = (RCC->cfgr >> RCC_CFGR_PPRE1) & 0x7U;
    return (temp > 3) ? apb_prescalars[temp - 4] : 1U;
}

uint8_t rcc_apb2_prescalar()
{
    uint8_t temp = (RCC->cfgr >> RCC_CFGR_PPRE2) & 0x7U;
    return (temp > 3) ? apb_prescalars[temp - 4] : 1U;
}

uint32_t rcc_apb1_clock_hz()
{
    uint32_t clock_hz = rcc_system_clock_hz();

    uint16_t ahb_prescalar = rcc_ahb_prescalar();
    uint8_t apb1_prescalar = rcc_apb1_prescalar();
    return (clock_hz / ahb_prescalar) / apb1_prescalar;
}

uint32_t rcc_apb2_clock_hz()
{
    uint32_t clock_hz = rcc_system_clock_hz();

    uint16_t ahb_prescalar = rcc_ahb_prescalar();
    uint8_t apb2_prescalar = rcc_apb2_prescalar();
    return (clock_hz / ahb_prescalar) / apb2_prescalar;
}
