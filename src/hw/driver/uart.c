#include "uart.h"
#include "qbuffer.h"

#include "tusb.h"
#include "pico/stdio/driver.h"

#ifdef _USE_HW_UART


extern stdio_driver_t stdio_usb;


#define UART_RX_BUF_SIZE    512

typedef struct
{
  bool is_init;
  uint32_t baud;


  uint8_t rx_buf[UART_RX_BUF_SIZE];
  qbuffer_t *qbuffer;
} uart_tbl_t;


uart_tbl_t uart_tbl[UART_MAX_CH];


bool uartInit(void)
{
  bool ret = true;
  for (int i = 0; i < UART_MAX_CH; i++)
  {
    uart_tbl[i].is_init = false;
    uart_tbl[i].baud = 38400;
  }
  return ret;
}


bool uartOpen(uint8_t ch, uint32_t baud)
{
  bool ret = false;
  
  uart_tbl_t *p_uart = &uart_tbl[ch];


  switch (ch)
  {
  case _DEF_UART1:
    p_uart->is_init = true;
    p_uart->baud    = baud;

    ret = true;
    break;
  case _DEF_UART2:



    break;
  }

  return ret;
}

uint32_t uartAvailable(uint8_t ch)
{
  uint32_t ret = 0;

  switch (ch)
  {
  case _DEF_UART1:
    ret = tud_cdc_available();  
    break;
  case _DEF_UART2:
    break;
  }
  return ret;
}

uint8_t uartRead(uint8_t ch)
{
  uint32_t ret = 0;

  switch (ch)
  {
  case _DEF_UART1:
    stdio_usb.in_chars((char *)&ret, 1);
    break;
  case _DEF_UART2:
    
    break;
  }
  return ret;
}

uint32_t uartWrite(uint8_t ch, uint8_t *p_data, uint32_t length)
{
  uint32_t ret = 0;

  switch (ch)
  {
  case _DEF_UART1:
    stdio_usb.out_chars((char *)p_data, length);
    break;
  case _DEF_UART2:
    
    
    break;
  }
  return ret;
}

uint32_t uartPrintf(uint8_t ch, char *fmt, ...)
{
  char buf[256];
	va_list args;
	int len;
	uint32_t ret;

	va_start(args, fmt);
	len = vsnprintf(&buf[0], 256, fmt, args);

	ret = uartWrite(ch, (uint8_t *)&buf[0], len);

	va_end(args);
	return ret;
}

uint32_t uartGetBaud(uint8_t ch)
{
  uint32_t ret = 0;

  switch (ch)
  {
  case _DEF_UART1:
    ret = uart_tbl[ch].baud;
    break;
  case _DEF_UART2:
    
    break;
  }
  return ret;
}

bool uartFlush(uint8_t ch)
{
  bool ret = true;

  while(uartAvailable(ch))
  {
    uartRead(ch);
  }
  
  return ret;
}


#endif