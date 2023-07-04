/**
 * @file     bootram_port.c
 * @author   BSP Team
 * @brief    This file provides bootram port function.
 * @version  0.0.0.1
 * @date     2020-12-29
 *
 * @copyright Copyright (c) 2020 Shanghai Lightning Semiconductor Technology Co. Ltd
 *
 */

/* Includes ------------------------------------------------------------------*/

#include "ramcode_port.h"
#include "serial/serial.h"
#include "hal/hal_flash.h"
#include "hal/hal_misc.h"

#include "hal/hal_gpio.h"
#include "hal/hal_uart.h"
#include "hal/hal_flash.h"
#include "hal/hal_dma.h"
#include "hal/hal_efuse.h"
/*****************************************  variables *********************************************/

static Serial_t bootram_fd;

// buffer for sector alignment.

#define UART_RX_BUF_LEN (1024*(16*2+2))

static uint8_t uart_rx_buf[UART_RX_BUF_LEN];
static uint8_t temp_4k_buffer[SIZE_4KB] = {0};

volatile uint8_t  uart_rx_over_flag = 0;
volatile uint32_t cur_pos           = 0;


/*****************************************  functions *********************************************/
int bootram_flash_uid(uint8_t *uid)
{
    hal_flash_read_unique_id(uid);
    return 0;
}
int bootram_flash_info(void)
{
    return hal_flash_read_id();
}

int bootram_flash_read(uint32_t offset, uint32_t len, void* buf)
{
    hal_flash_read(offset, len, buf);
    return 0;
}

int bootram_flash_write(uint32_t offset, uint32_t len, const void* buf)
{
    hal_flash_program(offset, len, (uint8_t*)buf);
    return 0;
}

int bootram_flash_erase(uint32_t offset, uint32_t len)
{
    if (offset % SIZE_4KB == 0) {  // 4KB aligned.
        hal_flash_erase(offset, len);
    }
    else {  // NOT 4KB aligned.
        uint32_t base = (offset / SIZE_4KB) * SIZE_4KB;
        uint32_t head = offset - base;
        uint32_t tail = SIZE_4KB - head;

        // read first sector
        memset(temp_4k_buffer, 0, SIZE_4KB);
        hal_flash_read(base, SIZE_4KB, temp_4k_buffer);
        memset(temp_4k_buffer + head, 0xFF, tail);

        // erase
        uint32_t erase_length = len + head;
        hal_flash_erase(base, erase_length);

        // program
        hal_flash_program(base, SIZE_4KB, temp_4k_buffer);
    }

    return 0;
}

int bootram_flash_chiperase(void)
{
    hal_flash_chip_erase();
    return 0;
}

void bootram_user_reboot(void)
{
    hal_misc_reset_all();
}

void bootram_serial_init(void)
{
    hal_gpio_pin_afio_select(GPIOA_BASE,GPIO_PIN_3,UART0_RX);
    hal_gpio_pin_afio_select(GPIOA_BASE,GPIO_PIN_2,UART0_TX);
    hal_gpio_pin_afio_en(GPIOA_BASE,GPIO_PIN_2,HAL_ENABLE);
    hal_gpio_pin_afio_en(GPIOA_BASE,GPIO_PIN_3,HAL_ENABLE);

    uart_init_t_def uart_init_struct;
    memset(&uart_init_struct,0,sizeof(uart_init_struct));
    
    uart_init_struct.baudrate   = 115200;                 
    uart_init_struct.parity     = UART_PARITY_NONE;        
    uart_init_struct.stop_bits  = UART_STOP_BITS_1;     
    uart_init_struct.word_len   = UART_WORD_LEN_8;       
    uart_init_struct.over_sampl = UART_OVER_SAMPL_8;
    
    hal_uart_init(UART0_BASE,&uart_init_struct);         
    hal_uart_rx_mode_en(UART0_BASE,HAL_ENABLE);          
    hal_uart_tx_mode_en(UART0_BASE,HAL_ENABLE);          
    hal_uart_en(UART0_BASE,HAL_ENABLE);                
    
    hal_uart_it_en(UART0_BASE,USART_IT_ERR);
    NVIC_EnableIRQ(UART0_IRQn); 
    
    dma_init_t_def dma_init;
    memset(&dma_init,0,sizeof(dma_init));
    dma_init.dma_data_num = UART_RX_BUF_LEN;                    
    dma_init.dma_mem_inc_en = DMA_MEM_INC_EN;            
    dma_init.dma_mem_addr = (uint32_t)uart_rx_buf;       
    dma_init.dma_dir = DMA_READ_FORM_P;                  
    dma_init.dma_p_addr = UART0_RX_DATA_REG;
    dma_init.dma_circ_mode_en = DMA_CIRC_MODE_EN;
    hal_dma_init(DMA_CH_5,&dma_init);
    hal_dma_en(DMA_CH_5,HAL_ENABLE);
    hal_uart_dma_en(UART0_BASE,USART_DMA_REQ_RX,HAL_ENABLE);      
}

size_t bootram_serial_write(const void* buf, size_t size)
{
    for(int i = 0; i < size; i ++){
        while (hal_uart_flag_get(UART0_BASE, USART_FLAG_TXE) == HAL_RESET) {};
        hal_uart_send_data(UART0_BASE, ((uint8_t*)buf)[i]);
    }
    return size;
}

unsigned char bootram_serial_setbaudrate(uint32_t baudrate)
{
    hal_uart_en(UART0_BASE,HAL_DISABLE);      
    hal_uart_baudrate_set(UART0_BASE, baudrate);
    hal_uart_en(UART0_BASE,HAL_ENABLE);      
    return 1;
}

int bootram_serial_flush(void)
{
    cur_pos = 0;
    uart_rx_over_flag = 0;
    hal_dma_en(DMA_CH_5,HAL_DISABLE);
    hal_dma_set_data_num(DMA_CH_5,UART_RX_BUF_LEN);
    hal_dma_en(DMA_CH_5,HAL_ENABLE);
    
    return 0;
}

size_t bootram_serial_read(void* buf, size_t size)
{
    volatile uint32_t len = 0;
  
    if(cur_pos >= UART_RX_BUF_LEN){
        cur_pos = 0;
        uart_rx_over_flag = 0;
    }
    
    len = UART_RX_BUF_LEN - hal_dma_get_data_num(DMA_CH_5);
    
    if(cur_pos > len){
        uart_rx_over_flag = 1;
    }
    
    
    if(uart_rx_over_flag == 0){
        if(cur_pos < len){
            len -= cur_pos;
            if(size > len){
                memcpy(buf,uart_rx_buf + cur_pos,len);
                cur_pos += len;
                return len;
            }else if(size <= len){
                memcpy(buf,uart_rx_buf + cur_pos,size);
                cur_pos += size;
                return size;
            }
        }else{
            return 0;
        }
    }else{
        len = UART_RX_BUF_LEN - cur_pos;
        if(size > len){
            memcpy(buf,uart_rx_buf + cur_pos,len);
            cur_pos += len;
            return len;
        }else if(size <= len){
            memcpy(buf,uart_rx_buf + cur_pos,size);
            cur_pos += size;
            return size;
        }
    }
}

void UART0_IRQHandler(void)
{
    //clear the ORE error.
    hal_uart_en(UART0_BASE,HAL_DISABLE);      
    while (hal_uart_flag_get(UART0_BASE, USART_FLAG_TX_FIFO_FULL) == HAL_SET) {
        hal_uart_recv_data(UART0_BASE);
    };
    hal_uart_it_flag_clear(UART0_BASE,USART_IT_FLAG_CLEAR_NOISE);
    hal_uart_it_flag_clear(UART0_BASE,USART_IT_FLAG_CLEAR_FE);
    hal_uart_it_flag_clear(UART0_BASE,USART_IT_FLAG_CLEAR_PE);
    bootram_serial_flush();
    hal_uart_en(UART0_BASE,HAL_ENABLE);   
}
