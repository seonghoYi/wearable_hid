#include "hc06.h"
#include "uart.h"

#include "cli.h"

#ifdef _USE_HW_HC06



#ifdef _USE_HW_CLI
static void cliHC06(cli_args_t *args);
#endif

bool HC06Init(void)
{
  bool ret = true;

#ifdef _USE_HW_CLI
	cliAdd("HC06", cliHC06);
#endif

  return ret;
}

bool HC06Begin(HC06_baud_t baud)
{
  bool ret = true;
  uint32_t baud_hz = 9600;

  switch (baud)
  {
  case HC06_BAUD_1200:
    baud_hz = 1200;
    break;
  case HC06_BAUD_2400:
    baud_hz = 2400;
    break;
  case HC06_BAUD_4800:
    baud_hz = 4800;
    break;
  case HC06_BAUD_9600:
    baud_hz = 9600;
    break;
  case HC06_BAUD_19200:
    baud_hz = 19200;
    break;
  case HC06_BAUD_38400:
    baud_hz = 38400;
    break;
  case HC06_BAUD_57600:
    baud_hz = 57600;
    break;
  case HC06_BAUD_115200:
    baud_hz = 115200;
    break;
  case HC06_BAUD_230400:
    baud_hz = 230400;
    break;
  case HC06_BAUD_460800:
    baud_hz = 460800;
    break;
  case HC06_BAUD_921600:
    baud_hz = 921600;
    break;
  case HC06_BAUD_1382400:
    baud_hz = 1382400;
    break;
  default:
    ret &= false;
    break;
  }


  ret &= uartOpen(_DEF_HC06, baud_hz);

  return ret;
}

uint32_t HC06Available(void)
{
  uint32_t ret = 0;
  ret = uartAvailable(_DEF_HC06);
  return ret;
}

uint8_t HC06Read(void)
{
  uint8_t ret = 0;
  ret = uartRead(_DEF_HC06);
  return ret;
}

uint32_t HC06Write(uint8_t *p_data, uint32_t length)
{
  uint32_t ret = 0;
  ret = uartWrite(_DEF_HC06, p_data, length);
  return ret;
} 

uint32_t HC06Printf(char *fmt, ...)
{
  char buf[256];
	va_list args;
	int len;
	uint32_t ret;

	va_start(args, fmt);
	len = vsnprintf(&buf[0], 256, fmt, args);

	ret = uartWrite(_DEF_HC06, (uint8_t *)&buf[0], len);

	va_end(args);
	return ret;
}





#ifdef _USE_HW_CLI
void cliHC06(cli_args_t *args)
{
  bool ret = true;
  char *command, *params;
  char send_command[100];
  uint8_t uart_baud, bt_baud = 0;

  if (args->argc == 3)
  {
    if (args->isStr(0, "begin") == true)
    {
        uart_baud = args->getData(1);
        bt_baud = args->getData(2);

        switch(uart_baud)
        {
          case 0x1:
          case 0x2:
          case 0x3:
          case 0x4:
          case 0x5:
          case 0x6:
          case 0x7:
          case 0x8:
          case 0x9:
          case 0xA:
          case 0xB:
          case 0xC:
            break;
          default:
            uart_baud = HC06_BAUD_9600;
            cliPrintf("Wrong baud parameter. It will be set default (4: 9600Hz)\n");
            break;
        }

        switch(bt_baud)
        {
          case 0x1:
          case 0x2:
          case 0x3:
          case 0x4:
          case 0x5:
          case 0x6:
          case 0x7:
          case 0x8:
          case 0x9:
          case 0xA:
          case 0xB:
          case 0xC:
            break;
          default:
            bt_baud = HC06_BAUD_9600;
            cliPrintf("Wrong baud parameter. It will be set default (4: 9600Hz)\n");
            break;
        }

        HC06Begin(uart_baud);

        sprintf(send_command, "AT+BAUD%X", bt_baud);
        HC06Write((uint8_t *)send_command, strlen(send_command));
        delay(1000);

        while(HC06Available())
        {
          uint8_t rx = HC06Read();
          cliPrintf("%c", (char)rx);
        }
        cliPrintf("\n");
        HC06Begin(bt_baud);
        
    }
    else if(args->isStr(0, "AT") == true)
    {
      command = args->getStr(1);
      params = args->getStr(2);

      if(command == NULL && params == NULL)
      {
        sprintf(send_command, "AT");
        HC06Write((uint8_t *)send_command, strlen(send_command));
        delay(1000);
      }
      else
      {
        ret = false;
      }
      if (ret)
      {
        sprintf(send_command, "AT+%s%s", command, params);
        HC06Write((uint8_t *)send_command, strlen(send_command));
        delay(1000);
      }

      while(HC06Available())
      {
        uint8_t rx = HC06Read();
        cliPrintf("%c", (char)rx);
      }
      cliPrintf("\n");
    }
  }
  else if(args->argc == 1)
  {
    if(args->isStr(0, "read") == true)
    {
      while(HC06Available())
      {
        uint8_t rx = HC06Read();
        cliPrintf("%c", (char)rx);
      }
      cliPrintf("\n");
    }
  }
  else if(args->argc == 2)
  {
    if(args->isStr(0, "write") == true)
    {
      sprintf(send_command, "%s", args->getStr(1));
      HC06Write((uint8_t *)send_command, strlen(send_command));
    }
  }
  else
  {
    ret = false;
  }


  if (ret == false)
  {
    cliPrintf("HC06 begin uartbaud[1~C] setbaud[1~C]\n");
    cliPrintf("HC06 AT NAME param\n");
    cliPrintf("             set: string\n");
    //cliPrintf("                get: ?\n");
    cliPrintf("HC06 AT PIN param\n");
    cliPrintf("            set: 4-digit password\n");
    cliPrintf("HC06 read\n");
    cliPrintf("HC06 write data[string]\n");
  }

}  
#endif

#endif