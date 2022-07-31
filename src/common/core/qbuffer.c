#include "qbuffer.h"


void qbufferInit(void)
{

}


bool qbufferCreate(qbuffer_t *p_node, uint8_t *p_buf, uint32_t length)
{
	bool ret = true;

	p_node->in    = 0;
	p_node->out   = 0;
	p_node->len   = length;
	p_node->p_buf = p_buf;
	p_node->size  = 1;

	return ret;
}

bool qbufferCreateBySize(qbuffer_t *p_node, uint8_t *p_buf, uint32_t size, uint32_t length)
{
	bool ret = true;

	p_node->in    = 0;
	p_node->out   = 0;
	p_node->len   = length;
	p_node->p_buf = p_buf;
	p_node->size  = size;

	return ret;
}

bool qbufferWrite(qbuffer_t *p_node, uint8_t *p_data, uint32_t length)
{
	bool ret = 0;
	uint32_t next_in;
	uint32_t size = p_node->size;

	if (p_data == NULL)
	{
		return ret;
	}

	for (int i=0; i<length; i++)
	{
		next_in = (p_node->in + 1) %p_node->len;

		if (next_in != p_node->out)
		{
			if (p_node->p_buf != NULL)
			{
				for (int j=0; j<size; j++)
				{
					p_node->p_buf[p_node->in*size+j] = p_data[i*size+j];
				}
				
			}
			p_node->in = next_in;
		}
		else
		{
			ret = false;
			break;
		}
	}
	return ret;
}


bool qbufferRead(qbuffer_t *p_node, uint8_t *p_data, uint32_t length)
{
	bool ret = true;
	uint32_t size = p_node->size;

	if (p_data == NULL)
	{
		return false;
	}

	for (int i=0; i<length; i++)
	{
		if (p_node->p_buf != NULL)
		{
			for (int j=0; j<size; j++)
			{
				p_data[i*size+j] = p_node->p_buf[p_node->out*size+j];
			}
		}

		if (p_node->out != p_node->in)
		{
			p_node->out = (p_node->out + 1) % p_node->len;
		}
		else
		{
			ret = false;
			break;
		}
	}
	return ret;
}

uint32_t qbufferAvailable(qbuffer_t *p_node)
{
	uint32_t ret;

	ret = (p_node->in - p_node->out) %p_node->len;

	return ret;
}


void qbufferFlush(qbuffer_t *p_node)
{
	p_node->in  = 0;
	p_node->out = 0;
}
