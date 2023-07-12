/**
 ****************************************************************************************************
 * @file        atk_md0280_gpio.c
 * @author      ����ԭ���Ŷ�(ALIENTEK)
 * @version     V1.0
 * @date        2022-06-21
 * @brief       ATK-MD0280ģ��GPIO�ӿ���������
 * @license     Copyright (c) 2020-2032, ������������ӿƼ����޹�˾
 ****************************************************************************************************
 * @attention
 *
 * ʵ��ƽ̨:����ԭ�� MiniSTM32 V4������
 * ������Ƶ:www.yuanzige.com
 * ������̳:www.openedv.com
 * ��˾��ַ:www.alientek.com
 * �����ַ:openedv.taobao.com
 *
 ****************************************************************************************************
 */

#include "atk_md0280_gpio.h"

/**
 * @brief       ATK-MD0280ģ��GPIO�ӿڳ�ʼ��
 * @param       ��
 * @retval      ��
 */
void atk_md0280_gpio_init(void)
{
    GPIO_InitTypeDef gpio_init_struct = {0};
    
    /* ʹ��ʱ�� */
    ATK_MD0280_GPIO_RS_GPIO_CLK_ENABLE();
    ATK_MD0280_GPIO_CS_GPIO_CLK_ENABLE();
    ATK_MD0280_GPIO_RD_GPIO_CLK_ENABLE();
    ATK_MD0280_GPIO_WR_GPIO_CLK_ENABLE();
    ATK_MD0280_GPIO_DATA_GPIO_CLK_ENABLE();
    
    /* ��ʼ��RS���� */
    gpio_init_struct.Pin    = ATK_MD0280_GPIO_RS_GPIO_PIN;
    gpio_init_struct.Mode   = GPIO_MODE_OUTPUT_PP;
    gpio_init_struct.Pull   = GPIO_PULLUP;
    gpio_init_struct.Speed  = GPIO_SPEED_FREQ_HIGH;
    HAL_GPIO_Init(ATK_MD0280_GPIO_RS_GPIO_PORT, &gpio_init_struct);
    
    /* ��ʼ��CS���� */
    gpio_init_struct.Pin    = ATK_MD0280_GPIO_CS_GPIO_PIN;
    gpio_init_struct.Mode   = GPIO_MODE_OUTPUT_PP;
    gpio_init_struct.Pull   = GPIO_PULLUP;
    gpio_init_struct.Speed  = GPIO_SPEED_FREQ_HIGH;
    HAL_GPIO_Init(ATK_MD0280_GPIO_CS_GPIO_PORT, &gpio_init_struct);
    
    /* ��ʼ��RD���� */
    gpio_init_struct.Pin    = ATK_MD0280_GPIO_RD_GPIO_PIN;
    gpio_init_struct.Mode   = GPIO_MODE_OUTPUT_PP;
    gpio_init_struct.Pull   = GPIO_PULLUP;
    gpio_init_struct.Speed  = GPIO_SPEED_FREQ_HIGH;
    HAL_GPIO_Init(ATK_MD0280_GPIO_RD_GPIO_PORT, &gpio_init_struct);
    
    /* ��ʼ��WR���� */
    gpio_init_struct.Pin    = ATK_MD0280_GPIO_WR_GPIO_PIN;
    gpio_init_struct.Mode   = GPIO_MODE_OUTPUT_PP;
    gpio_init_struct.Pull   = GPIO_PULLUP;
    gpio_init_struct.Speed  = GPIO_SPEED_FREQ_HIGH;
    HAL_GPIO_Init(ATK_MD0280_GPIO_WR_GPIO_PORT, &gpio_init_struct);
    
    /* ��ʼ��DATA���� */
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
 * @brief       ATK-MD0280ģ��ͨ��GPIO�ӿ�д����
 * @param       cmd: ��д����
 * @retval      ��
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
 * @brief       ATK-MD0280ģ��ͨ��GPIO�ӿ�д����
 * @param       dat: ��д����
 * @retval      ��
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
 * @brief       ATK-MD0280ģ��ͨ��GPIO�ӿ�д�Ĵ���
 * @param       reg: �������Ĵ���
 *              dat: ��д����
 * @retval      ��
 */
void atk_md0280_gpio_write_reg(uint16_t reg, uint16_t dat)
{
    atk_md0280_gpio_write_cmd(reg);
    atk_md0280_gpio_write_dat(dat);
}

/**
 * @brief       ATK-MD0280ģ��ͨ��GPIO�ӿڶ�����
 * @param       ��
 * @retval      ��ȡ��������
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
