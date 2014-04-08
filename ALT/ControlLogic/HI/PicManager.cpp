#include "PicManager.h"

PicManager::PicManager(int spiDevice)
{
	//----- SET SPI MODE -----
    //SPI_MODE_0 (0,0) 	CPOL=0 (Clock Idle low level), CPHA=0 (SDO transmit/change edge active to idle)
    //SPI_MODE_1 (0,1) 	CPOL=0 (Clock Idle low level), CPHA=1 (SDO transmit/change edge idle to active)
    //SPI_MODE_2 (1,0) 	CPOL=1 (Clock Idle high level), CPHA=0 (SDO transmit/change edge active to idle)
    //SPI_MODE_3 (1,1) 	CPOL=1 (Clock Idle high level), CPHA=1 (SDO transmit/change edge idle to active)
	_gearReductionRatio = 16;
	_shaftDegreesPerPulse = 0.7;
	_spiMode = SPI_MODE_0;
	_bitsPerWord = 8;
	_spiSpeed = 500000;
	_spiDevice = spiDevice;
	openPort();
}

PicManager::~PicManager()
{
	closePort();
}

bool PicManager::MoveDegrees(int degreesToMove)
{
	uint16_t pulseCount = roundf(abs(degreesToMove * _gearReductionRatio / _shaftDegreesPerPulse));
	unsigned char lowerByte = pulseCount & 0x00FF;
	unsigned char upperByte = 0x00;
	if (degreesToMove < 0)
	{
		upperByte = 0x7F & (pulseCount >> 8);
		upperByte = 0x40 | upperByte;
	}
	else
	{
		upperByte = 0x3F & (pulseCount >> 8);
	}
	writeAndRead( &lowerByte );
	usleep(100);
	writeAndRead( &upperByte );
	return true;
}

void PicManager::Stop()
{
	writeAndRead(0x00);
	usleep(100);
	writeAndRead(0x00);
}

int PicManager::openPort ()
{
	int status_value = -1;
    int *spi_cs_fd;

	spi_cs_fd = &_spiFD;

    if (_spiDevice)
    	*spi_cs_fd = open(std::string("/dev/spidev0.1").c_str(), O_RDWR);
    else
    	*spi_cs_fd = open(std::string("/dev/spidev0.0").c_str(), O_RDWR);

    if (*spi_cs_fd < 0)
    {
        perror("Error - Could not open SPI device");
    }

    status_value = ioctl(*spi_cs_fd, SPI_IOC_WR_MODE, &_spiMode);
    if(status_value < 0)
    {
        perror("Could not set SPIMode (WR)...ioctl fail");
    }

    status_value = ioctl(*spi_cs_fd, SPI_IOC_RD_MODE, &_spiMode);
    if(status_value < 0)
    {
      perror("Could not set SPIMode (RD)...ioctl fail");
    }

    status_value = ioctl(*spi_cs_fd, SPI_IOC_WR_BITS_PER_WORD, &_bitsPerWord);
    if(status_value < 0)
    {
      perror("Could not set SPI bitsPerWord (WR)...ioctl fail");
    }

    status_value = ioctl(*spi_cs_fd, SPI_IOC_RD_BITS_PER_WORD, &_bitsPerWord);
    if(status_value < 0)
    {
      perror("Could not set SPI bitsPerWord(RD)...ioctl fail");
    }

    status_value = ioctl(*spi_cs_fd, SPI_IOC_WR_MAX_SPEED_HZ, &_spiSpeed);
    if(status_value < 0)
    {
      perror("Could not set SPI speed (WR)...ioctl fail");
    }

    status_value = ioctl(*spi_cs_fd, SPI_IOC_RD_MAX_SPEED_HZ, &_spiSpeed);
    if(status_value < 0)
    {
      perror("Could not set SPI speed (RD)...ioctl fail");
    }
    return(status_value);
}

int PicManager::closePort()
{
	int status_value = -1;
    int *spi_cs_fd;

    if (_spiDevice)
    	spi_cs_fd = &_spiFD;
    else
    	spi_cs_fd = &_spiFD;


    status_value = close(*spi_cs_fd);
    if(status_value < 0)
    {
    	perror("Error - Could not close SPI device");
    }
    return(status_value);
}


//data		Bytes to write.  Contents is overwritten with bytes read.
int PicManager::writeAndRead( unsigned char *data)
{
	struct spi_ioc_transfer spi[1];
	int i = 0;
	int retVal = -1;
    int *spi_cs_fd;

    if (_spiDevice)
    	spi_cs_fd = &_spiFD;
    else
    	spi_cs_fd = &_spiFD;

	for (i = 0 ; i < 1 ; i++)
	{
		spi[i].tx_buf        = (unsigned long)(data + i); // transmit from "data"
		spi[i].rx_buf        = (unsigned long)(data + i) ; // receive into "data"
		spi[i].len           = sizeof(*(data + i)) ;
		spi[i].delay_usecs   = 0 ;
		spi[i].speed_hz      = _spiSpeed ;
		spi[i].bits_per_word = _bitsPerWord ;
		spi[i].cs_change = 0;
	}

	retVal = ioctl(*spi_cs_fd, SPI_IOC_MESSAGE(1), &spi) ;

	if(retVal < 0)
	{
		perror("Error - Problem transmitting spi data..ioctl");
		exit(1);
	}

	return retVal;
}