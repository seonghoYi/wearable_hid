#include "uart.h"
#include "qbuffer.h"


#include "hardware/uart.h"
#include "hardware/irq.h"

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


static qbuffer_t qbuffer[UART_MAX_CH];




void uart0ReceiveISR(void)
{
  while(uart_is_readable(uart0))
  {
    uint8_t rx = uart_getc(uart0);
    qbufferWrite(uart_tbl[_DEF_UART2].qbuffer, &rx, 1);
  }
}




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
    p_uart->is_init = true;
    p_uart->baud    = baud;

    p_uart->qbuffer = &qbuffer[ch];
    qbufferCreate(p_uart->qbuffer, (uint8_t *)&(p_uart->rx_buf[0]), UART_RX_BUF_SIZE);

    uart_deinit(uart0);

    uart_init(uart0, baud);

    gpio_set_function(16, GPIO_FUNC_UART);
    gpio_set_function(17, GPIO_FUNC_UART);

    uart_set_hw_flow(uart0, false, false);
    uart_set_fifo_enabled(uart0, false);

    irq_set_exclusive_handler(UART0_IRQ, uart0ReceiveISR);
    irq_set_enabled(UART0_IRQ, true);

    uart_set_irq_enables(uart0, true, false);


    ret = true;
    break;
  }

  return ret;
}

uint32_t uartAvailable(uint8_t ch)
{
  uint32_t ret = 0;

  uart_tbl_t *p_uart = &uart_tbl[ch];

  switch (ch)
  {
  case _DEF_UART1:
    ret = tud_cdc_available();  
    break;
  case _DEF_UART2:
  ret = qbufferAvailable(p_uart->qbuffer);
    break;
  }
  return ret;
}

uint8_t uartRead(uint8_t ch)
{
  uint32_t ret = 0;

  uart_tbl_t *p_uart = &uart_tbl[ch];

  switch (ch)
  {
  case _DEF_UART1:
    stdio_usb.in_chars((char *)&ret, 1);
    break;
  case _DEF_UART2:
    qbufferRead(p_uart->qbuffer, (uint8_t *)&ret, 1);
    break;
  }
  return ret;
}

uint32_t uartWrite(uint8_t ch, uint8_t *p_data, uint32_t length)
{
  uint32_t ret = 0;
  uint32_t transferred = 0;
  
  switch (ch)
  {
  case _DEF_UART1:
    stdio_usb.out_chars((char *)p_data, length);
    break;
  case _DEF_UART2:
    while(length - transferred)
    {
      uart_putc_raw(uart0, (char)p_data[transferred++]);
    }
    ret = length;
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
    ret = uart_tbl[ch].baud;
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