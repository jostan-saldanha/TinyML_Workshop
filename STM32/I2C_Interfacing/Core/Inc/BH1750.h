#ifndef INC_BH1750_H_
#define INC_BH1750_H_

#include "stdio.h"

// BH1750 I2C Address
#define BH1750_ADDR 0x23 // BH1750 I2C address

// BH1750 Commands
#define BH1750_POWER_ON       0x01
#define BH1750_CONT_H_RES_MODE 0x10

// Function prototypes
void BH1750_Init(I2C_HandleTypeDef *hi2c);
float BH1750_ReadLux(I2C_HandleTypeDef *hi2c);

// Error Status
HAL_StatusTypeDef Transmit_Err, Receive_Err;

// BH1750 initialization
void BH1750_Init(I2C_HandleTypeDef *hi2c) {
    uint8_t pwr_on = BH1750_POWER_ON;
    Transmit_Err = HAL_I2C_Master_Transmit(hi2c, BH1750_ADDR << 1, &pwr_on, 1, HAL_MAX_DELAY);
    HAL_Delay(10);

    uint8_t cmd = BH1750_CONT_H_RES_MODE;
    Transmit_Err = HAL_I2C_Master_Transmit(hi2c, BH1750_ADDR << 1, &cmd, 1, HAL_MAX_DELAY);

    // First measurement in H-Res mode takes up to ~180ms to become valid
    HAL_Delay(180);
}

// Reading Light Intensity from BH1750 sensor
float BH1750_ReadLux(I2C_HandleTypeDef *hi2c) {
    uint8_t data[2];
    Receive_Err = HAL_I2C_Master_Receive(hi2c, BH1750_ADDR << 1, data, sizeof(data), HAL_MAX_DELAY);

    uint16_t raw = (data[0] << 8) | data[1];
    return (float)raw / 1.2f;
}
#endif
