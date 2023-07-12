/**
 ****************************************************************************************************
 * @file        atk_md0280_gpio.c
 * @author      正点原子团队(ALIENTEK)
 * @version     V1.0
 * @date        2022-06-21
 * @brief       ATK-MD0280模块GPIO接口驱动代码
 * @license     Copyright (c) 2020-2032, 广州市星翼电子科技有限公司
 ****************************************************************************************************
 * @attention
 *
 * 实验平台:正点原子 MiniSTM32 V4开发板
 * 在线视频:www.yuanzige.com
 * 技术论坛:www.openedv.com
 * 公司网址:www.alientek.com
 * 购买地址:openedv.taobao.com
 *
 ****************************************************************************************************
 */

#include "atk_md0280_gpio.h"

/**
 * @brief       ATK-MD0280模块GPIO接口初始化
 * @param       无
 * @retval      无
 */
void atk_md0280_gpio_init(void)
{
    GPIO_InitTypeDef gpio_init_struct = {0};
    
    /* 使能时钟 */
    ATK_MD0280_GPIO_RS_GPIO_CLK_ENABLE();
    ATK_MD0280_GPIO_CS_GPIO_CLK_ENABLE();
    ATK_MD0280_GPIO_RD_GPIO_CLK_ENABLE();
    ATK_MD0280_GPIO_WR_GPIO_CLK_ENABLE();
    ATK_MD0280_GPIO_DATA_GPIO_CLK_ENABLE();
    
    /* 初始化RS引脚 */
    gpio_init_struct.Pin    = ATK_MD0280_GPIO_RS_GPIO_PIN;
    gpio_init_struct.Mode   = GPIO_MODE_OUTPUT_PP;
    gpio_init_struct.Pull   = GPIO_PULLUP;
    gpio_init_struct.Speed  = GPIO_SPEED_FREQ_HIGH;
    HAL_GPIO_Init(ATK_MD0280_GPIO_RS_GPIO_PORT, &gpio_init_struct);
    
    /* 初始化CS引脚 */
    gpio_init_struct.Pin    = ATK_MD0280_GPIO_CS_GPIO_PIN;
    gpio_init_struct.Mode   = GPIO_MODE_OUTPUT_PP;
    gpio_init_struct.Pull   = GPIO_PULLUP;
    gpio_init_struct.Speed  = GPIO_SPEED_FREQ_HIGH;
    HAL_GPIO_Init(ATK_MD0280_GPIO_CS_GPIO_PORT, &gpio_init_struct);
    
    /* 初始化RD引脚 */
    gpio_init_struct.Pin    = ATK_MD0280_GPIO_RD_GPIO_PIN;
    gpio_init_struct.Mode   = GPIO_MODE_OUTPUT_PP;
    gpio_init_struct.Pull   = GPIO_PULLUP;
    gpio_init_struct.Speed  = GPIO_SPEED_FREQ_HIGH;
    HAL_GPIO_Init(ATK_MD0280_GPIO_RD_GPIO_PORT, &gpio_init_struct);
    
    /* 初始化WR引脚 */
    gpio_init_struct.Pin    = ATK_MD0280_GPIO_WR_GPIO_PIN;
    gpio_init_struct.Mode   = GPIO_MODE_OUTPUT_PP;
    gpio_init_struct.Pull   = GPIO_PULLUP;
    gpio_init_struct.Speed  = GPIO_SPEED_FREQ_HIGH;
    HAL_GPIO_Init(ATK_MD0280_GPIO_WR_GPIO_PORT, &gpio_init_struct);
    
    /* 初始化DATA引脚 */
    gpio_init_struct.Pin    = ATK_MD0280_GPIO_DATA_GPIO_PIN;
    gpio_init_struct.Mode   = GPIO_MODE_OUTPUT_PP;
    gpio_init_struct.Pull   = GPIO_PULLUP;
    gpio_init_struct.Speed  = GPIO_SPEED_FREQ_HIGH;
    HAL_GPIO_Init(ATK_MD0280_GPIO_DATA_GPIO_PORT, &gpio_init_struct);
    
    ATK_MD0280_GPIO_RS(1);
    ATK_MD0280_GPIO_CS(1);
    ATK_MD0280_GPIO_RD(1);
    ATK_MD0280_GPIO_WR(1);
    ATK_MD0280_GPIO_DATA(0xFFFF);
}

/**
 * @brief       ATK-MD0280模块通过GPIO接口写命令
 * @param       cmd: 待写命令
 * @retval      无
 */
void atk_md0280_gpio_write_cmd(uint16_t cmd)
{
    ATK_MD0280_GPIO_RS(0);
    ATK_MD0280_GPIO_CS(0);
    ATK_MD0280_GPIO_DATA(cmd);
    ATK_MD0280_GPIO_WR(0);
    ATK_MD0280_GPIO_WR(1);
    ATK_MD0280_GPIO_CS(1);
}

/**
 * @brief       ATK-MD0280模块通过GPIO接口写数据
 * @param       dat: 待写数据
 * @retval      无
 */
void atk_md0280_gpio_write_dat(uint16_t dat)
{
    ATK_MD0280_GPIO_RS(1);
    ATK_MD0280_GPIO_CS(0);
    ATK_MD0280_GPIO_DATA(dat);
    ATK_MD0280_GPIO_WR(0);
    ATK_MD0280_GPIO_WR(1);
    ATK_MD0280_GPIO_CS(1);
}

/**
 * @brief       ATK-MD0280模块通过GPIO接口写寄存器
 * @param       reg: 待操作寄存器
 *              dat: 待写数据
 * @retval      无
 */
void atk_md0280_gpio_write_reg(uint16_t reg, uint16_t dat)
{
    atk_md0280_gpio_write_cmd(reg);
    atk_md0280_gpio_write_dat(dat);
}

/**
 * @brief       ATK-MD0280模块通过GPIO接口读数据
 * @param       无
 * @retval      读取到的数据
 */
uint16_t atk_md0280_gpio_read_dat(void)
{
    GPIO_InitTypeDef gpio_init_struct = {0};
    uint16_t dat;
    
    gpio_init_struct.Pin    = ATK_MD0280_GPIO_DATA_GPIO_PIN;
    gpio_init_struct.Mode   = GPIO_MODE_INPUT;
    gpio_init_struct.Pull   = GPIO_PULLUP;
    gpio_init_struct.Speed  = GPIO_SPEED_FREQ_HIGH;
    HAL_GPIO_Init(ATK_MD0280_GPIO_DATA_GPIO_PORT, &gpio_init_struct);
    
    ATK_MD0280_GPIO_RS(1);
    ATK_MD0280_GPIO_CS(0);
    ATK_MD0280_GPIO_RD(0);
    __nop();
    __nop();
    dat = ATK_MD0280_GPIO_READ_DATA();
    ATK_MD0280_GPIO_RD(1);
    ATK_MD0280_GPIO_CS(1);
    
    gpio_init_struct.Pin    = ATK_MD0280_GPIO_DATA_GPIO_PIN;
    gpio_init_struct.Mode   = GPIO_MODE_OUTPUT_PP;
    gpio_init_struct.Pull   = GPIO_PULLUP;
    gpio_init_struct.Speed  = GPIO_SPEED_FREQ_HIGH;
    HAL_GPIO_Init(ATK_MD0280_GPIO_DATA_GPIO_PORT, &gpio_init_struct);
    
    return dat;
}
