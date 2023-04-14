#include "I2CSniff.hpp"

#define PIN_SDA 22
#define PIN_SCL 19

#define I2C_IDLE 0
//#define I2C_START 1
#define I2C_TRX 2
//#define I2C_RESP 3
//#define I2C_STOP 4

static volatile byte i2cStatus = I2C_IDLE;//Status of the I2C BUS
static uint32_t lastStartMillis = 0;//stoe the last time
static volatile byte dataBuffer[9600];//Array for storing data of the I2C communication
static volatile uint16_t bufferPoiW=0;//points to the first empty position in the dataBufer to write
static volatile uint16_t bufferPoiR=0;//points to the position where to start read from
static volatile byte bitCount = 0;//counter of bit appeared on the BUS
static volatile uint16_t byteCount =0;//counter of bytes were writen in one communication.
static volatile byte i2cBitD =0;//Container of the actual SDA bit
static volatile byte i2cBitD2 =0;//Container of the actual SDA bit
static volatile byte i2cBitC =0;//Container of the actual SDA bit
static volatile byte i2cBitC2 =0;//Container of the actual SDA bit
static volatile byte i2cClk =0;//Container of the actual SCL bit
static volatile byte i2cClk2 =0;//Container of the actual SCL bit
static volatile byte i2cAck =0;//Container of the last ACK value
static volatile byte i2cCase =0;//Container of the last ACK value
static volatile uint16_t falseStart = 0;//Counter of false start events
//static volatile byte respCount =0;//Auxiliary variable to help detect next byte instead of STOP
//these variables just for statistic reasons
static volatile uint16_t sclUpCnt = 0;//Auxiliary variable to count rising SCL
static volatile uint16_t sdaUpCnt = 0;//Auxiliary variable to count rising SDA
static volatile uint16_t sdaDownCnt = 0;//Auxiliary variable to count falling SDA

static void SA_iRestore(const  uint32_t *__s)
{
    XTOS_RESTORE_INTLEVEL(*__s);
}

// Note value can be 0-15, 0 = Enable all interrupts, 15 = no interrupts
#define SA_ATOMIC_RESTORESTATE uint32_t _sa_saved              \
    __attribute__((__cleanup__(SA_iRestore))) = XTOS_DISABLE_LOWPRI_INTERRUPTS


/*************** MACRO **********************/
#define ATOMIC()                                            \
for ( SA_ATOMIC_RESTORESTATE, _sa_done =  1;                   \
    _sa_done; _sa_done = 0 )

static int IRAM_ATTR write_byte(byte v) {
  uint16_t rp = bufferPoiR;  // must be atomic
  if ( ( bufferPoiW + 1 ) == sizeof(dataBuffer) )
  {
    if ( 0 == rp ) {
      return 0; // no space    
    }
  }
  else
  {
    if ( ( bufferPoiW + 1 ) == rp ) {
      return 0; // no space    
    }
  }
  dataBuffer[bufferPoiW] = v;
  if ( (bufferPoiW + 1) == sizeof(dataBuffer) )
  {
    bufferPoiW = 0;  // must be atomic
  }
  else
  {
    bufferPoiW++;  // must be atomic
  }
  return 1;
}

static int read_byte(byte *v) {
  uint16_t wp = 0;
  ATOMIC()
  {
    wp = bufferPoiW; // must be atomic
  }
  if ( wp == bufferPoiR ) {
    return 0; // no data    
  }
  *v = dataBuffer[bufferPoiR];
  if ( (bufferPoiR + 1) == sizeof(dataBuffer) )
  {
    ATOMIC()
    {
      bufferPoiR = 0;  // must be atomic
    }
  }
  else
  {
    ATOMIC()
    {
      bufferPoiR++;  // must be atomic
    }
  }
  return 1;
}

void IRAM_ATTR i2cTriggerOnRaisingSCL() 
{
  ATOMIC()
  {
	sclUpCnt++;
	
	//is it a false trigger?
	if(i2cStatus==I2C_IDLE)
	{
		falseStart++;
		//return;//this is not clear why do we have so many false START
	}


	//get the value from SDA
	i2cBitC =  digitalRead(PIN_SDA);

	//decide wherewe are and what to do with incoming data
	i2cCase = 0;//normal case

	if(bitCount==8)//ACK case
		i2cCase = 1;

	if(bitCount==7 && byteCount==0 )// R/W if the first address byte
		i2cCase = 2;

	bitCount++;

	switch (i2cCase)
	{
		case 0: //normal case
			write_byte('0' + i2cBitC);//dataBuffer[bufferPoiW++] = '0' + i2cBitC;//48
		break;//end of case 0 general
		case 1://ACK
			if(i2cBitC)//1 NACK SDA HIGH
				{
					write_byte('-');//dataBuffer[bufferPoiW++] = '-';//45
				}
				else//0 ACK SDA LOW
				{
					write_byte('+');//dataBuffer[bufferPoiW++] = '+';//43
				}	
			byteCount++;
			bitCount=0;
		break;//end of case 1 ACK
		case 2:
			if(i2cBitC)
			{
				write_byte('R');//dataBuffer[bufferPoiW++] = 'R';//82
			}
			else
			{
				write_byte('W');//dataBuffer[bufferPoiW++] = 'W';//87
			}
		break;//end of case 2 R/W

	}//end of switch
  }//end of ATOMIC

}//END of i2cTriggerOnRaisingSCL()

void IRAM_ATTR i2cTriggerOnChangeSDA()
{
  ATOMIC()
  {
	//make sure that the SDA is in stable state
	do
	{
		i2cBitD =  digitalRead(PIN_SDA);
		i2cBitD2 =  digitalRead(PIN_SDA);
	} while (i2cBitD!=i2cBitD2);
	do
	{
		i2cClk =  digitalRead(PIN_SCL);
		i2cClk2 =  digitalRead(PIN_SCL);
	} while (i2cClk!=i2cClk2);
	do
	{
		i2cBitD =  digitalRead(PIN_SDA);
		i2cBitD2 =  digitalRead(PIN_SDA);
	} while (i2cBitD!=i2cBitD2);
	do
	{
		i2cClk =  digitalRead(PIN_SCL);
		i2cClk2 =  digitalRead(PIN_SCL);
	} while (i2cClk!=i2cClk2);

	//i2cBitD =  digitalRead(PIN_SDA);

	if(i2cBitD)//RISING if SDA is HIGH (1)
	{
		
	    //digitalRead(PIN_SCL);
		if(i2cStatus=!I2C_IDLE && i2cClk==1)//If SCL still HIGH then it is a STOP sign
		{			
			//i2cStatus = I2C_STOP;
			i2cStatus = I2C_IDLE;
			bitCount = 0;
			byteCount = 0;
			bufferPoiW--;
			write_byte('s');//dataBuffer[bufferPoiW++] = 's';//115
			write_byte('\n');//dataBuffer[bufferPoiW++] = '\n'; //10
		}
		sdaUpCnt++;
	}
	else //FALLING if SDA is LOW
	{
	    //digitalRead(PIN_SCL);
		if(i2cStatus==I2C_IDLE && i2cClk)//If SCL still HIGH than this is a START
		{
			i2cStatus = I2C_TRX;
			//lastStartMillis = millis();//takes too long in an interrupt handler and caused timeout panic and CPU restart
			bitCount = 0;
			byteCount =0;
			write_byte('S');//dataBuffer[bufferPoiW++] = 'S';//83 STOP
			//i2cStatus = START;		
		}
		sdaDownCnt++;
	}
  }//end of ATOMIC
}//END of i2cTriggerOnChangeSDA()

void resetI2cVariable()
{
	i2cStatus = I2C_IDLE;
	bufferPoiW=0;
	bufferPoiR=0;
	bitCount =0;
	falseStart = 0;
}//END of resetI2cVariable()


void I2CSniff::begin()
{
    TextView::set_title("Reading");
    TextView::begin();

	//Define pins for SCL, SDA
	pinMode(PIN_SCL, INPUT_PULLUP);   
    pinMode(PIN_SDA, INPUT_PULLUP);
	//pinMode(PIN_SCL, INPUT);   
    //pinMode(PIN_SDA, INPUT);

    //reset variables
    resetI2cVariable();

    //Atach interrupt handlers to the interrupts on GPIOs
    attachInterrupt(PIN_SCL, i2cTriggerOnRaisingSCL, RISING); //trigger for reading data from SDA
	attachInterrupt(PIN_SDA, i2cTriggerOnChangeSDA, CHANGE); //for I2C START and STOP

    pause_ = false;
    last_parse_ = millis();
}

void I2CSniff::key_event(uint8_t key)
{
    if ( APP_KEY_PLAY == key )
    {
        pause_ = !pause_;
        if ( pause_ ) {
            TextView::set_title("Paused");
        }
        else
        {
            TextView::set_title("Reading");
        }
    }
    TextView::key_event(key);
}

void I2CSniff::loop()
{
    char buf[512]; // TODO: this is too little, need to allocate larger
    size_t size = 0;
    uint8_t v = 0;

    if ( (millis() - last_parse_) > 500 )
    {
        last_parse_ = millis();
        while ( read_byte(&v) )
        {
            size = 0;
            buf[size] = v;
            size++;
            while ( (size < sizeof(buf)) && read_byte(&v) )
            {
                buf[size] = v;
                size++;
            }
            if ( size > 0 )
            {
                if ( !pause_ )
                {
                    TextView::append_text(std::string(buf, size), 0x1000);
                }
            }
        }
    }
}

void I2CSniff::end()
{
    detachInterrupt(PIN_SCL);
    detachInterrupt(PIN_SDA);
    TextView::clear();
    TextView::end();
}
