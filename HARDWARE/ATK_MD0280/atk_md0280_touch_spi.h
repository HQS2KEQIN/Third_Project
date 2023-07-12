/**
 ****************************************************************************************************
 * @file        atk_md0280_touch_spi.h
 * @author      正点原子团队(ALIENTEK)
 * @version     V1.0
 * @date        2022-06-21
 * @brief       ATK-MD0280模块触摸SPI接口驱动代码
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

#ifndef __ATK_MD0280_TOUCH_SPI_H
#define __ATK_MD0280_TOUCH_SPI_H

#include "atk_md0280.h"

#if (ATK_MD0280_USING_TOUCH != 0)

/* 引脚定义 */
#define ATK_MD0280_TOUCH_SPI_MI_GPIO_PORT           GPIOC
#define ATK_MD0280_TOUCH_SPI_MI_GPIO_PIN            GPIO_PIN_2
#define ATK_MD0280_TOUCH_SPI_MI_GPIO_CLK_ENABLE()   do{ __HAL_RCC_GPIOC_CLK_ENABLE(); }while(0)
#define ATK_MD0280_TOUCH_SPI_MO_GPIO_PORT           GPIOC
#define ATK_MD0280_TOUCH_SPI_MO_GPIO_PIN            GPIO_PIN_3
#define ATK_MD0280_TOUCH_SPI_MO_GPIO_CLK_ENABLE()   do{ __HAL_RCC_GPIOC_CLK_ENABLE(); }while(0)
#define ATK_MD0280_TOUCH_SPI_TCS_GPIO_PORT          GPIOC
#define ATK_MD0280_TOUCH_SPI_TCS_GPIO_PIN           GPIO_PIN_13
#define ATK_MD0280_TOUCH_SPI_TCS_GPIO_CLK_ENABLE()  do{ __HAL_RCC_GPIOC_CLK_ENABLE(); }while(0)
#define ATK_MD0280_TOUCH_SPI_CLK_GPIO_PORT          GPIOC
#define ATK_MD0280_TOUCH_SPI_CLK_GPIO_PIN           GPIO_PIN_0
#define ATK_MD0280_TOUCH_SPI_CLK_GPIO_CLK_ENABLE()  do{ __HAL_RCC_GPIOC_CLK_ENABLE(); }while(0)

/* IO操作 */
#define ATK_MD0280_TOUCH_SPI_READ_MI()              HAL_GPIO_ReadPin(ATK_MD0280_TOUCH_SPI_MI_GPIO_PORT, ATK_MD0280_TOUCH_SPI_MI_GPIO_PIN)
#define ATK_MD0280_TOUCH_SPI_MO(x)                  do{ x ?                                                                                                         \
                                                        HAL_GPIO_WritePin(ATK_MD0280_TOUCH_SPI_MO_GPIO_PORT, ATK_MD0280_TOUCH_SPI_MO_GPIO_PIN, GPIO_PIN_SET) :      \
                                                        HAL_GPIO_WritePin(ATK_MD0280_TOUCH_SPI_MO_GPIO_PORT, ATK_MD0280_TOUCH_SPI_MO_GPIO_PIN, GPIO_PIN_RESET);     \
                                                    }while(0)
#define ATK_MD0280_TOUCH_SPI_TCS(x)                 do{ x ?                                                                                                         \
                                                        HAL_GPIO_WritePin(ATK_MD0280_TOUCH_SPI_TCS_GPIO_PORT, ATK_MD0280_TOUCH_SPI_TCS_GPIO_PIN, GPIO_PIN_SET) :    \
                                                        HAL_GPIO_WritePin(ATK_MD0280_TOUCH_SPI_TCS_GPIO_PORT, ATK_MD0280_TOUCH_SPI_TCS_GPIO_PIN, GPIO_PIN_RESET);   \
                                                    }while(0)
#define ATK_MD0280_TOUCH_SPI_CLK(x)                 do{ x ?                                                                                                         \
                                                        HAL_GPIO_WritePin(ATK_MD0280_TOUCH_SPI_CLK_GPIO_PORT, ATK_MD0280_TOUCH_SPI_CLK_GPIO_PIN, GPIO_PIN_SET) :    \
                                                        HAL_GPIO_WritePin(ATK_MD0280_TOUCH_SPI_CLK_GPIO_PORT, ATK_MD0280_TOUCH_SPI_CLK_GPIO_PIN, GPIO_PIN_RESET);   \
                                                    }while(0)

/* 操作函数 */
void atk_md0280_touch_spi_init(void);               /* ATK-MD0280模块触摸SPI接口初始化 */
uint16_t atk_md0280_touch_spi_read(uint8_t cmd);    /* ATK-MD0280模块触摸SPI接口读数据 */

#endif /* ATK_MD0280_USING_TOUCH */

#endif
