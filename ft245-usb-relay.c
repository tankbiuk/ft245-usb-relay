/**
  ******************************************************************************
  * FT245 chip control fo Quimat 5V USB relay module (and similar products)
  ******************************************************************************
  *
  * COPYRIGHT(c) 2017 Davide Fogliano <tankbiuk@gmail.com>
  *
  *	THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
  *	IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
  *	FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
  *	AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
  *	LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
  *	OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
  *	SOFTWARE.
  * 
  ******************************************************************************
  */
  
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <getopt.h>
#include <stdarg.h>
#include <ftdi.h>
#include <unistd.h>
#include <getopt.h>
#include <errno.h>

bool 	debug_enable;	// Debug enabled (-v) DEFUALT = false

void debug(const char *fmt, ...)
{
	va_list args;
	
	if(debug_enable)
	{
		va_start(args, fmt);
		vprintf(fmt, args);
		va_end(args);
	}
}

bool read_hex_byte(const char *str, unsigned char *val, char option)
{
	bool valid = false;
	int tmp = -1;
	
	if(sscanf(str,"%02x",&tmp) == 1)
	{
		if(tmp>=0 && tmp<256)
		{
			*val = (unsigned char)tmp;
			valid = true;
		}
	}
	
	if(!valid)
	{
		fprintf(stderr, "'-%c' value '%s' not valid\n", option, str);
		fprintf(stderr, "'-%c' provide valid byte [00-FF]\n", option);
	}
	
	return valid;
}

int main(int argc, char **argv)
{
	/* Ftdi context */
	struct ftdi_context *ftdi;

	/* Command line options */
	bool	print_only;		// Print status only (no '-o' or '-p' option)
	int		port;			// Selected ouput port (-p) [0-7]
	bool	state;			// State of the ouput (-e) [0-1] DEFUALT = 1
	unsigned char output;	// Ouput value (-o)
	unsigned char mask;		// Ouput mask (-m) DEFUALT = 0xFF
	bool	single_port;	// single(-p) or multiport (-o)
	
	int ret;
    unsigned char data = 0;
    
    int i;
    int tmp;
    int chipid;
    
    /*initialize defualt values */
    print_only = false;
    debug_enable = false;
    port = -1;
    state = true;
    output = 0x00;
    mask = 0xFF;
    single_port = true;
    
    if(argc == 1)
    { 
		print_only = true;
	}
	else
	{
		/* Get the command line options */
		while ((i = getopt(argc, argv, "vp:e:o:m:")) != -1)
		{
			switch (i)
			{
				case 'v':
					debug_enable = true;
					break;
				case 'p':

					port = strtoul(optarg, NULL, 0);
					
					if (port > 7 || port < 0)
					{
						fprintf(stderr, "'-p' value '%s' not valid\n",optarg);
						fprintf(stderr, "'-p' provide valid port number [0-7]\n");
						exit(-1);
					}
					
					break;
				case 'e':
					
					if(optarg[0] != '0' && optarg[0] != '1')
					{
						fprintf(stderr, "'-e' value '%s' not valid\n",optarg);
						fprintf(stderr, "'-e' provide valid value [0-1]\n");
						exit(-1);
					} 
					
					tmp = strtoul(optarg, NULL, 0);
					
					if(tmp)
					{
						state = 1;
					}
					else
					{
						state = 0;
					}
					break;
				
				case 'o':
					if(!read_hex_byte(optarg, &output, i))
						exit(-1);
					break;
					
				case 'm':
					if(!read_hex_byte(optarg, &mask, i))
						exit(-1);
					break;
					
				default:
					fprintf(stderr, "usage: %s [-p 0-7 [-e 0-1]]|[-o HH [-m HH]] [-v]\n", *argv);
					exit(-1);
			}
		}
	}
	
	if ((ftdi = ftdi_new()) == 0)
	{
        fprintf(stderr, "ERROR: ftdi_new failed\n");
        return -1;
    }
    
     if ((ret = ftdi_usb_open(ftdi, 0x0403, 0x6001)) < 0)
    {
        fprintf(stderr, "ERROR: unable to open ftdi device: %d (%s)\n", ret, ftdi_get_error_string(ftdi));
        ftdi_free(ftdi);
        return -1;
    }
	
	if(print_only)
	{
		if(ftdi_read_chipid(ftdi, &chipid) == 0)
		{
			printf("FTDI chipid: %X\n", chipid);
		}
		else
		{
			fprintf(stderr, "ERROR: Unable to read chip ID, errno:%d", errno);
			ftdi_free(ftdi);
			return -1;
		}
		
		
		if(ftdi_read_data(ftdi, &data, 1)  == 1)
		{
			printf("*** OUPUT ***\n");
			for(int b = 0 ; b < 8 ; b++)
			{
				printf("D%d:%d\n", b, ((data>>b)&0x01));
			}
			
			printf("*************\n");
		}
		else
		{
			fprintf(stderr, "ERROR: Unable to read output values, errno:%d\n", errno);
			ftdi_free(ftdi);
			return -1;
		}
		
	}
	else
	{
		if(ftdi_set_bitmode(ftdi, 0xFF, BITMODE_BITBANG ) != 0)
		{
			fprintf(stderr, "ERROR: Unable to  set bitmode, errno:%d", errno);
			ftdi_free(ftdi);
			return -1;
		}
		
		/* Calculate mask and ouput value based on parameters */
		if(port >=0)
		{
			mask = (0x01)<<port;
			output = state<<port;
		}
		
		if(ftdi_read_data(ftdi, &data, 1)  == 1)
		{

			data &= ~mask;
			data |= output;
			
			if(ftdi_write_data(ftdi, &data, 1)  != 1)
			{
				fprintf(stderr, "ERROR: Unable to write output values, errno:%d", errno);
				ftdi_free(ftdi);
				return -1;
			}
			
		}
		else
		{
			fprintf(stderr, "ERROR: Unable to read output values, errno:%d", errno);
			ftdi_free(ftdi);
			return -1;
		}
		
	}

    ftdi_free(ftdi);

    return EXIT_SUCCESS;
}
