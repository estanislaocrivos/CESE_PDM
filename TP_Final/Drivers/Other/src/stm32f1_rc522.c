/*
 * stm32f1_rc522.c
 *
 * This library contains the necessary functions for initializing, reading and writing RFID
 * cards using the RC522 RFID module.
 *
 * Author: -
 * Date: -
 *
 * Source: Internet
 *
 */

#include "stm32f1_rc522.h"
#include "string.h"
#include "stdio.h"
#include "stdint.h"
#include "API_SPI.h"

/**
  * @brief This function is used for polling the peripheral.
  *
  * @param uint8_t * RFID_ID Takes a pointer to a string where the ID will be stored.
  * @retval uint8_t Returns 0 in case of detecting a card.
  */
uint8_t RFID_Sense(uint8_t * RFID_ID)
{
	uint8_t status;
	uint8_t String_ID[6];

	MFRC522_Request(0x26, String_ID);
	status = MFRC522_Anticoll(String_ID);

	sprintf((char *)RFID_ID, "%02X%02X%02X%02X%02X", String_ID[0], String_ID[1], String_ID[2], String_ID[3], String_ID[4]);

	return status;
}

void Write_MFRC522(uint8_t addr, uint8_t val)
{

  HAL_GPIO_WritePin(GPIOA,GPIO_PIN_4,GPIO_PIN_RESET);
	SPI_Transcieve((addr<<1)&0x7E);
	SPI_Transcieve(val);
	HAL_GPIO_WritePin(GPIOA,GPIO_PIN_4,GPIO_PIN_SET);
}

uint8_t Read_MFRC522(uint8_t addr)
{
	uint8_t val;

	/* CS LOW */
	//GPIO_ResetBits(MFRC522_CS_GPIO, MFRC522_CS_PIN);
	HAL_GPIO_WritePin(GPIOA,GPIO_PIN_4,GPIO_PIN_RESET);

	//The address is located:1XXXXXX0
	SPI_Transcieve(((addr<<1)&0x7E) | 0x80);
	val = SPI_Transcieve(0x00);
	
	/* CS HIGH */
	//GPIO_SetBits(MFRC522_CS_GPIO, MFRC522_CS_PIN);
	HAL_GPIO_WritePin(GPIOA,GPIO_PIN_4,GPIO_PIN_SET);
	return val;	
	
}

void SetBitMask(uint8_t reg, uint8_t mask)  
{
    uint8_t tmp;
    tmp = Read_MFRC522(reg);
    Write_MFRC522(reg, tmp | mask);  // set bit mask
}

void ClearBitMask(uint8_t reg, uint8_t mask)  
{
    uint8_t tmp;
    tmp = Read_MFRC522(reg);
    Write_MFRC522(reg, tmp & (~mask));  // clear bit mask
} 

void AntennaOn(void)
{
	Read_MFRC522(TxControlReg);
	SetBitMask(TxControlReg, 0x03);
}

void AntennaOff(void)
{
	ClearBitMask(TxControlReg, 0x03);
}

void MFRC522_Reset(void)
{
	Write_MFRC522(CommandReg, PCD_RESETPHASE);
}

void MFRC522_Init(void)
{
	HAL_GPIO_WritePin(GPIOA,GPIO_PIN_4,GPIO_PIN_SET);
	HAL_GPIO_WritePin(GPIOB,GPIO_PIN_0,GPIO_PIN_SET);
	
	MFRC522_Reset();
	 	
	//Timer: TPrescaler*TreloadVal/6.78MHz = 24ms
	Write_MFRC522(TModeReg, 0x8D);		// auto=1; f(Timer) = 6.78MHz/TPreScaler
	Write_MFRC522(TPrescalerReg, 0x3E);	// TModeReg[3..0] + TPrescalerReg
	Write_MFRC522(TReloadRegL, 30);           
	Write_MFRC522(TReloadRegH, 0);
	
	Write_MFRC522(TxAutoReg, 0x40);
	Write_MFRC522(ModeReg, 0x3D);

	AntennaOn();
}

uint8_t MFRC522_ToCard(uint8_t command, uint8_t *sendData, uint8_t sendLen, uint8_t *backData, uint *backLen)
{
    uint8_t status = MI_ERR;
    uint8_t irqEn = 0x00;
    uint8_t waitIRq = 0x00;
    uint8_t lastBits;
    uint8_t n;
    uint i;

    switch (command)
    {
        case PCD_AUTHENT:		//Acknowledging the liver
		{
			irqEn = 0x12;
			waitIRq = 0x10;
			break;
		}
		case PCD_TRANSCEIVE:	// FIFO data collection
		{
			irqEn = 0x77;
			waitIRq = 0x30;
			break;
		}
		default:
			break;
    }
   
    Write_MFRC522(CommIEnReg, irqEn|0x80);	//Yeu cau ngat
    ClearBitMask(CommIrqReg, 0x80);			//Clear all the bits
    SetBitMask(FIFOLevelReg, 0x80);			//FlushBuffer=1, FIFO
    
	Write_MFRC522(CommandReg, PCD_IDLE);	//NO action; Huy bo lenh hien hanh	???

	// Record in FIFO
    for (i=0; i<sendLen; i++)
    {   
		Write_MFRC522(FIFODataReg, sendData[i]);    
	}

	//chay
	Write_MFRC522(CommandReg, command);
    if (command == PCD_TRANSCEIVE)
    {    
		SetBitMask(BitFramingReg, 0x80);		//StartSend=1,transmission of data starts  
	}   
    
	i = 2000;
    do 
    {
        n = Read_MFRC522(CommIrqReg);
        i--;
    }
    while ((i!=0) && !(n&0x01) && !(n&waitIRq));

    ClearBitMask(BitFramingReg, 0x80);
	
    if (i != 0)
    {    
        if(!(Read_MFRC522(ErrorReg) & 0x1B))
        {
            status = MI_OK;
            if (n & irqEn & 0x01)
            {   
				status = MI_NOTAGERR;
			}

            if (command == PCD_TRANSCEIVE)
            {
               	n = Read_MFRC522(FIFOLevelReg);
              	lastBits = Read_MFRC522(ControlReg) & 0x07;
                if (lastBits)
                {   
					*backLen = (n-1)*8 + lastBits;   
				}
                else
                {   
					*backLen = n*8;   
				}

                if (n == 0)
                {   
					n = 1;    
				}
                if (n > MAX_LEN)
                {   
					n = MAX_LEN;   
				}
				
				//FIFO doc in the received data
                for (i=0; i<n; i++)
                {   
					backData[i] = Read_MFRC522(FIFODataReg);    
				}
            }
        }
        else
        {   
			status = MI_ERR;  
		}
        
    }
    return status;
}

uint8_t MFRC522_Request(uint8_t reqMode, uint8_t *TagType)
{
	uint8_t status;  
	uint backBits;

	Write_MFRC522(BitFramingReg, 0x07);
	
	TagType[0] = reqMode;
	status = MFRC522_ToCard(PCD_TRANSCEIVE, TagType, 1, TagType, &backBits);

	if ((status != MI_OK) || (backBits != 0x10))
	{    
		status = MI_ERR;
	}
   
	return status;
}

uint8_t MFRC522_Anticoll(uint8_t *serNum)
{
    uint8_t status;
    uint8_t i;
	uint8_t serNumCheck=0;
    uint unLen;

	Write_MFRC522(BitFramingReg, 0x00);
 
    serNum[0] = PICC_ANTICOLL;
    serNum[1] = 0x20;
    status = MFRC522_ToCard(PCD_TRANSCEIVE, serNum, 2, serNum, &unLen);

    if (status == MI_OK)
	{
		//Check the serial number
		for (i=0; i<4; i++)
		{   
		 	serNumCheck ^= serNum[i];
		}
		if (serNumCheck != serNum[i])
		{   
			status = MI_ERR;    
		}
    }

    return status;
} 

void CalulateCRC(uint8_t *pIndata, uint8_t len, uint8_t *pOutData)
{
    uint8_t i, n;

    ClearBitMask(DivIrqReg, 0x04);			//CRCIrq = 0
    SetBitMask(FIFOLevelReg, 0x80);			//Con tro FIFO
    //Write_MFRC522(CommandReg, PCD_IDLE);

	//Record in FIFO
    for (i=0; i<len; i++)
    {   
		Write_MFRC522(FIFODataReg, *(pIndata+i));   
	}
    Write_MFRC522(CommandReg, PCD_CALCCRC);

	// Let the CRC computer complete
    i = 0xFF;
    do 
    {
        n = Read_MFRC522(DivIrqReg);
        i--;
    }
    while ((i!=0) && !(n&0x04));			//CRCIrq = 1

	//Doc results in CRC calculation
    pOutData[0] = Read_MFRC522(CRCResultRegL);
    pOutData[1] = Read_MFRC522(CRCResultRegM);
}

uint8_t MFRC522_SelectTag(uint8_t *serNum)
{
	uint8_t i;
	uint8_t status;
	uint8_t size;
	uint recvBits;
	uint8_t buffer[9]; 

	//ClearBitMask(Status2Reg, 0x08);			//MFCrypto1On=0

    buffer[0] = PICC_SElECTTAG;
    buffer[1] = 0x70;
    for (i=0; i<5; i++)
    {
    	buffer[i+2] = *(serNum+i);
    }
	CalulateCRC(buffer, 7, &buffer[7]);		//??
    status = MFRC522_ToCard(PCD_TRANSCEIVE, buffer, 9, buffer, &recvBits);
    
    if ((status == MI_OK) && (recvBits == 0x18))
    {   
		size = buffer[0]; 
	}
    else
    {   
		size = 0;    
	}

    return size;
}

uint8_t MFRC522_Auth(uint8_t authMode, uint8_t BlockAddr, uint8_t *Sectorkey, uint8_t *serNum)
{
    uint8_t status;
    uint recvBits;
    uint8_t i;
	uint8_t buff[12]; 

	//Confirmation + Address + password + quick number
    buff[0] = authMode;
    buff[1] = BlockAddr;
    for (i=0; i<6; i++)
    {    
		buff[i+2] = *(Sectorkey+i);   
	}
    for (i=0; i<4; i++)
    {    
		buff[i+8] = *(serNum+i);   
	}
    status = MFRC522_ToCard(PCD_AUTHENT, buff, 12, buff, &recvBits);

    if ((status != MI_OK) || (!(Read_MFRC522(Status2Reg) & 0x08)))
    {   
		status = MI_ERR;   
	}
    
    return status;
}

uint8_t MFRC522_Read(uint8_t blockAddr, uint8_t *recvData)
{
    uint8_t status;
    uint unLen;

    recvData[0] = PICC_READ;
    recvData[1] = blockAddr;
    CalulateCRC(recvData,2, &recvData[2]);
    status = MFRC522_ToCard(PCD_TRANSCEIVE, recvData, 4, recvData, &unLen);

    if ((status != MI_OK) || (unLen != 0x90))
    {
        status = MI_ERR;
    }
    
    return status;
}

uint8_t MFRC522_Write(uint8_t blockAddr, uint8_t *writeData)
{
    uint8_t status;
    uint recvBits;
    uint8_t i;
	uint8_t buff[18]; 
    
    buff[0] = PICC_WRITE;
    buff[1] = blockAddr;
    CalulateCRC(buff, 2, &buff[2]);
    status = MFRC522_ToCard(PCD_TRANSCEIVE, buff, 4, buff, &recvBits);

    if ((status != MI_OK) || (recvBits != 4) || ((buff[0] & 0x0F) != 0x0A))
    {   
		status = MI_ERR;   
	}
        
    if (status == MI_OK)
    {
        for (i=0; i<16; i++)		//16 FIFO bytes recorded
        {    
        	buff[i] = *(writeData+i);   
        }
        CalulateCRC(buff, 16, &buff[16]);
        status = MFRC522_ToCard(PCD_TRANSCEIVE, buff, 18, buff, &recvBits);
        
		if ((status != MI_OK) || (recvBits != 4) || ((buff[0] & 0x0F) != 0x0A))
        {   
			status = MI_ERR;   
		}
    }
    
    return status;
}

void MFRC522_Halt(void)
{
	uint unLen;
	uint8_t buff[4]; 

	buff[0] = PICC_HALT;
	buff[1] = 0;
	CalulateCRC(buff, 2, &buff[2]);
 
	MFRC522_ToCard(PCD_TRANSCEIVE, buff, 4, buff,&unLen);
}
