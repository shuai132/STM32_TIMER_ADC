/**
 * 重定向c库printf 请确保此文件参与编译
 */

#include <stdio.h>
#include <usart.h>
#include <usbd_cdc_if.h>

/**
 * for armcc (勾选MicroLIB)
 */
int fputc(int ch, FILE *f) {
    HAL_UART_Transmit(&huart1, (uint8_t*)&ch, 1, 0xFFFF);
    CDC_Transmit_FS((uint8_t*)&ch, 1);
    return ch;
}

/**
 * for gcc
 */
int _write(int fd, char *pBuffer, int size) {
    HAL_UART_Transmit(&huart1, (uint8_t*)pBuffer, size, 0xFFFF);
    CDC_Transmit_FS((uint8_t*)pBuffer, size);
    return size;
}
