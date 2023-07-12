/**
 ****************************************************************************************************
 * @file        atk_md0280_touch.h
 * @author      正点原子团队(ALIENTEK)
 * @version     V1.0
 * @date        2022-06-21
 * @brief       ATK-MD0280模块触摸驱动代码
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

#ifndef __ATK_MD0280_TOUCH_H
#define __ATK_MD0280_TOUCH_H

#include "atk_md0280.h"

#if (ATK_MD0280_USING_TOUCH != 0)

#define ATK_MD0280_TOUCH_READ_TIMES             5   /* 定义单次获取坐标读取的次数 */
#define ATK_MD0280_TOUCH_READ_DISCARD           1   /* 定义单次获取坐标丢弃的最大最小值的数量 */
#define ATK_MD0280_TOUCH_READ_RANGE             50  /* 定义单次获取坐标读取的两次值之间误差的最大值 */

/* 引脚定义 */
#define ATK_MD0280_TOUCH_PEN_GPIO_PORT          GPIOC
#define ATK_MD0280_TOUCH_PEN_GPIO_PIN           GPIO_PIN_1
#define ATK_MD0280_TOUCH_PEN_GPIO_CLK_ENABLE()  do{ __HAL_RCC_GPIOC_CLK_ENABLE(); }while(0)

/* IO操作 */
#define ATK_MD0280_TOUCH_READ_PEN()              HAL_GPIO_ReadPin(ATK_MD0280_TOUCH_PEN_GPIO_PORT, ATK_MD0280_TOUCH_PEN_GPIO_PIN)

/* 错误代码 */
#define ATK_MD0280_TOUCH_EOK                    0   /* 没有错误 */
#define ATK_MD0280_TOUCH_ERROR                  1
#define ATK_MD0280_TOUCH_EMPTY                  2   /* 空 */

/* 操作函数 */
void atk_md0280_touch_init(void);                        /* ATK-MD0280模块触摸初始化 */
uint8_t atk_md0280_touch_scan(uint16_t *x, uint16_t *y);    /* ATK-MD0280模块触摸扫描 */

#endif /* ATK_MD0280_USING_TOUCH */

#endif
