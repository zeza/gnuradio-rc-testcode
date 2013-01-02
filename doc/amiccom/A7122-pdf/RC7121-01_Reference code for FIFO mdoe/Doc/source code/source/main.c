/*********************************************************************                                                                   **
**  Device:     A7121                                                *
**  File:		main.c					                             *
**  Author:		JPH						                             *
**  Target:		Winbond W77LE58				                         *
**  Tools:		ICE                                   				 *
**  Created:	2006-12-01					                         *
**  Description:	                                                 *
**  This file is a sample code for your reference.				     *
**                                                                   *
**  Copyright (C) 2006 AMICCOM Corp.                                 *
**                                                                   *
**  Revision history:                                                *
**                                                                   *
**  Revision 0.0  2006/12/1		                                     *
**  Initial version.                                                 *
*********************************************************************/
#include "define.h"
#include "w77le58.h"
#include "a7121reg.h"
#include "Uti.h"

#define TIMEOUT     	20
#define t0hrel      	1000
#define Freq_Base       0x0960//2400MHz

/*********************************************************************
**  I/O Declaration
*********************************************************************/
#define SpiCs       P1_0
#define SpiClk      P1_1
#define SpiRxd      P1_4
#define SpiTxd      P1_5
#define CdTxenPin   P1_7

#define FIFOData	P3_0
#define FIFOClk		P3_1
#define Ms0Pin      P3_2
#define Ms1Pin      P3_3
#define FpRdy		P3_4
#define ResetPin	P3_5

/*********************************************************************
**  Global Variable Declaration
*********************************************************************/
Uint8		data 	timer;
Uint8		data 	TimeoutFlag;
Uint16		data	Err_BitCnt;
Uint16		idata   ModeCtlReg;
Uint16		idata	FifoCtlReg;
Uint8       xdata   FreqBank[84];

const Uint8 code BitCount_Tab[16] = {0,1,1,2,1,2,2,3,1,2,2,3,2,3,3,4};
const Uint8 AccessCode_Tab[9] = {0x54,0x75,0xC5,0x8C,0xC7,0x33,0x45,0xE7,0x2A};
const Uint8 PN9_Tab[]=
{   0xFF,0x83,0xDF,0x17,0x32,0x09,0x4E,0xD1,
    0xE7,0xCD,0x8A,0x91,0xC6,0xD5,0xC4,0xC4,
    0x40,0x21,0x18,0x4E,0x55,0x86,0xF4,0xDC,
    0x8A,0x15,0xA7,0xEC,0x92,0xDF,0x93,0x53,
    0x30,0x18,0xCA,0x34,0xBF,0xA2,0xC7,0x59,
    0x67,0x8F,0xBA,0x0D,0x6D,0xD8,0x2D,0x7D,
    0x54,0x0A,0x57,0x97,0x70,0x39,0xD2,0x7A,
    0xEA,0x24,0x33,0x85,0xED,0x9A,0x1D,0xE0
};	// This table are 64bytes PN9 pseudo random code.

/*********************************************************************
**  function Declaration
*********************************************************************/
void InitTimer0(void);
void Timer0ISR (void);
void Rst_Timer0(void);
void SpiWrite(Uint16 dataWord,Uint8 address);
Uint16 SpiRead(Uint8 address);
void FIFOWrite(Uint8 source);
Uint8 FIFORead(void);
void FIFOWr_En(void);
void FIFOWr_Di(void);
void FIFORd_En(void);
void FIFORd_Di(void);
void SYNth_En(void);
void SYNth_Di(void);
void SetFreq(Uint8 ch);
void SetTRC(Uint8 trc);
void ResetRF(void);
void initRF(void);
Uint8 Calibration_VCO(Uint8 ch);
void TxPacket(void);
void RxPacket(void);
void WaitRx(void);
Uint8 WaitAck(void);
void MeasureRSSI(void);

/*********************************************************************
* main loop
*********************************************************************/
void main(void)
{
    //initsw
    PMR |= 0x01;//set DME0

    //initHW
    P0 = 0xFF;
    P1 = 0xFF;
    P2 = 0xFF;
    P3 = 0xFF;
    P4 = 0x0F;

	InitTimer0();
	EA=1;

	initRF(); //init RF
	SYNth_En(); //synthesizer on

	if ((P4 & 0x04)==0x04) // 1=master
	{
		while(1)
		{
			//Tx state
			TxPacket();

			//Rx stste
			if (WaitAck())
				RxPacket();

    		//measure RSSI value
			MeasureRSSI();

			Delay10ms(5); //delay 50ms
    	}
	}
	else // 0=slave
	{
		while(1)
        {
        	//Rx state
        	WaitRx();
            RxPacket();

			//Tx state
            TxPacket();
        }
	}
}

/*********************************************************************
** initRF
*********************************************************************/
void initRF(void)
{
    Uint8 i;
    Uint8 a,b,c,RH_value,RL_value;
    Uint16 tmp;

    //init i/o pin
    Ms0Pin = 1; //standby mode
    Ms1Pin = 0;
    SpiCs  = 1;
    SpiClk = 0;
    SpiTxd = 1;
    SpiRxd = 1;
    CdTxenPin = 0; //disable CDTXEN
	FIFOClk = 0;

    //reset RF chip
    ResetRF();

    //init register
    SpiWrite(0x0960, SYNTH1_REG); //BNk=0,default freq=2400MHz
    SpiWrite(0x0B1B, SYNTH2_REG); //VTH=6,CP=500uA,default R=27
    SpiWrite(0x2231, SYSTEM_CLK_REG); //XBR=17,XDR=17,XIR=8

    ModeCtlReg = 0x03D3; //RSTN=1,CE=1,SYN=1,TRC=tx state,DR=1Mbps,TRD=1,EXBR=1,EXDR=1,EXIR=1
    SpiWrite(ModeCtlReg, MODE_CONTROL_REG);

    SpiWrite(0xFFD0, TX_CONTROL1_REG); //TXDI=normal,DEV=8(250kHz),GF=off,IA=31,QA=31
    SpiWrite(0xE23F, TX_CONTROL2_REG); //PC=63,IO=8,QO=8,IQC=3
    SpiWrite(0x06F6, RX_CONTROL1_REG); //RXDI=0,DPC=3,ETH=6,RCP=3,DS=rxsyn->off,SYNI=0
    SpiWrite(0x0030, RX_CONTROL2_REG); //VGA=0db,DFG=6

    FifoCtlReg = 0x0FC0; //fifo byte count = 64 bytes
    SpiWrite(FifoCtlReg, FIFO_CONTROL_REG);

    for (i=0;i<9;i++)
        SpiWrite((AccessCode_Tab[i]<<8) | AccessCode_Tab[i],ACCESS_CODE_REG); //access code

    SpiWrite(0x01F4, CALIBRATION_CONTROL1_REG); //RSS=after erss is active,MCAL=0,IFC=1,DFC=1,DEMC=1,RHC=1,RLC=1
    SpiWrite(0x0001, CALIBRATION_CONTROL2_REG);	//ETR=1,FPRS=0
    SpiWrite(0x0D83, ADC_REG); //ADC=3,AD[2:0]=0,AD[5:3]=3,AD[8:6]=3

    //rx cal
    SpiWrite(0x01D3, MODE_CONTROL_REG); //EXIF=0
    SpiWrite(0x0003, CALIBRATION_CONTROL2_REG);	//ECAL=1
	Delay10ms(2); //delay 20ms
    SpiWrite(0x03D3, MODE_CONTROL_REG);	//EXIF=1
    do
    {
        tmp = SpiRead(CALIBRATION_CONTROL2_REG);
        tmp &= 0x0002;
    }
    while (tmp); //check calibration ok?
	SpiWrite(0x0037, RX_CONTROL2_REG); //VGA=20db,DFG=6

    //read calibration value for check
    a=(SpiRead(IF_FILTER_REG) & 0xFE00)>>9;
    b=(SpiRead(DATA_FILTER_REG)& 0xFE00)>>9;
    c=(SpiRead(DEMODULATOR_REG) & 0xFF00)>>8;
    RH_value=(SpiRead(RH_REG) & 0xFF00)>>8;
    RL_value=(SpiRead(RL_REG)& 0xFF00)>>8;

    //vco cal
    for(i=0;i<84;i++)
        FreqBank[i]=Calibration_VCO(i);
}

/*********************************************************************
* Calibration_VCO
*********************************************************************/
Uint8 Calibration_VCO(Uint8 ch)
{
    Uint8 var_bank;
    Uint8 ackError;
    Uint8 i;
    Uint16 ack;
    Uint16 var_data;

    var_bank=3;//init var_bank value
    SYNth_En();//set SYN=1
    var_data = (Freq_Base + ch) & 0x1FFF;//mask Bank[2:0] bits

    for (i=1; i<=8 ;i++)//retry 8 times
    {
        var_data &= 0x1FFF;
        var_data |= (var_bank << 13);
        SpiWrite(var_data, SYNTH1_REG);//write synthesizer I

        Delay100us(3);//delay300us

        ack=SpiRead(SYNTH2_REG);

        //vt=01 calibration ok
        if((ack & 0xc000)==0x4000)
        {
            ackError=0;
            break;
        }

        //vt=00 freq too low
        if((ack & 0xc000)==0x0000)
        {
            if(var_bank==7)
            {
                ackError=1;
                break;
            }
            var_bank++;
        }
        //vt=11 freq too high
        if((ack & 0xc000)==0xc000)
        {
            if(var_bank==0)
            {
                ackError=1;
                break;
            }
            var_bank--;
        }
    }

	SYNth_Di();//set SYN=0
    if(ackError) return 0xFF;
    else return var_bank;
}

/*********************************************************************
** init Timer0
*********************************************************************/
void InitTimer0(void)
{
    TR0 = 0;
    TMOD =0x01;//timer0 mode=1
    TH0 = (65536-t0hrel)>>8;// Reload Timer0 high byte,low byte
    TL0 = 65536-t0hrel;
    TF0 = 0;// Clear any pending Timer0 interrupts
    ET0 = 1;// Enable Timer0 interrupt
}

/*********************************************************************
** timer0ISR
*********************************************************************/
void Timer0ISR (void) interrupt 1
{
    TF0 = 0;// Clear Timer0 interrupt
    TH0 = (65536-t0hrel)>>8;// Reload Timer0 high byte
    TL0 = 65536-t0hrel;// Reload Timer0 low byte
    timer++;
    if (timer == TIMEOUT)
    {
        TimeoutFlag=1;
        TR0 = 0;// Stop timer
        timer=0;
    }
}

/*********************************************************************
** Rst_Timer0
*********************************************************************/
void Rst_Timer0(void)
{
    TR0 = 0;
    TH0 = (65536-t0hrel)>>8; // Reload Timer0 high byte
    TL0 = 65536-t0hrel;
    TF0 = 0;
    timer = 0;
    TimeoutFlag = 0;
}

/*********************************************************************
** SpiWrite
*********************************************************************/
void SpiWrite(Uint16 dataWord,Uint8 address)
{
    Uint8 i;

    SpiCs  = 0;// Enable A7121 SPI
    SpiClk = 0;
    SpiRxd = 1;

    //send address code
    address=(address<<2) | 0x80;//fill wirte bit
    for(i = 0; i < 8; i++)
    {
        SpiClk = 1;

        if(address & 0x80)
            SpiRxd = 1;//bit=1
        else
            SpiRxd = 0;//bit=0

        _nop_();
        address = address << 1;
        SpiClk = 0;
    }

    _nop_();

    //send data code
    for(i = 0; i < 16; i++)
    {
        SpiClk = 1;

        if(dataWord & 0x8000)
            SpiRxd = 1;
        else
            SpiRxd = 0;

        dataWord = dataWord<< 1;
        _nop_();
        SpiClk = 0;
    }

    _nop_();
    SpiCs = 1; //Disable A7121 SPI
    _nop_();
    SpiRxd = 1;
}

/*********************************************************************
** SpiRead
*********************************************************************/
Uint16 SpiRead(Uint8 address)
{
    Uint8 i;
    Uint16 spiData;

    spiData=0;
    SpiClk=0;
    SpiTxd = 1;

    SpiCs  = 0; //Enable A7121 SPI
    _nop_();

    //send address code
    address=(address<<2) | 0x00;//fill read bit
    for(i = 0; i < 8; i++)
    {
        SpiClk = 1;

        if(address & 0x80)
            SpiRxd = 1;
        else
            SpiRxd = 0;

        _nop_();
        address = address << 1;
        SpiClk = 0;
    }

    _nop_();

    //read data code
    for(i = 0; i < 16; i++)
    {
        SpiClk = 1;
        _nop_();

        if(SpiTxd)
            spiData = (spiData << 1) | 0x01;  // SpiRxd: read "1"
        else
            spiData = spiData << 1;           // SpiRxd: read "0"

        SpiClk = 0;
    }

    _nop_();
    SpiCs = 1;
    SpiRxd = 1;

    return spiData;
}

/*********************************************************************
*FIFOWrite
*********************************************************************/
void FIFOWrite(Uint8 source)
{
    Uint8 i;

    for(i = 0; i < 8; i++)
    {
        FIFOClk = 1;

        if(source & 0x80)
            FIFOData = 1;
        else
            FIFOData = 0;

        _nop_();
        source = source << 1;
        FIFOClk = 0;
    }

    _nop_();
    FIFOData=1;
}

/*********************************************************************
*FIFORead
*********************************************************************/
Uint8 FIFORead(void)
{
    Uint8 i;
    Uint8 tmp=0;

    for(i = 0; i < 8; i++)
    {
        FIFOClk = 1;
        _nop_();
        if(FIFOData)
            tmp = (tmp << 1) | 0x01;
        else
            tmp  = tmp << 1;

        FIFOClk = 0;
     }

     return tmp;
}

/*********************************************************************
**  SYNth_En
*********************************************************************/
void SYNth_En(void)
{
    ModeCtlReg |= 0x0004; //enable synth bit
    SpiWrite(ModeCtlReg, MODE_CONTROL_REG);
    Delay10us(15); //delay 150us for pll settling time
}

/*********************************************************************
**  SYNth_Di
*********************************************************************/
void SYNth_Di(void)
{
    ModeCtlReg &= (~0x0004); //disable synth bit
    SpiWrite(ModeCtlReg, MODE_CONTROL_REG);
}

/*********************************************************************
** ResetRF
*********************************************************************/
void ResetRF(void)
{
	//hardware reset
    ResetPin = 0;
    _nop_();
    ResetPin = 1;

    // or register reset
    //SpiWrite(0x0002,MODE_CONTROL_REG);//reset enable
	//delay();
	//SpiWrite(0x0003,MODE_CONTROL_REG);//reset disable
}

/*********************************************************************
**  FIFOWr_En
*********************************************************************/
void FIFOWr_En(void)
{
    FifoCtlReg |= 0x0004;
    SpiWrite(FifoCtlReg, FIFO_CONTROL_REG);
}

/*********************************************************************
**  FIFOWr_Di
*********************************************************************/
void FIFOWr_Di(void)
{
    FifoCtlReg &= ~0x0004;
    SpiWrite(FifoCtlReg, FIFO_CONTROL_REG);
}

/*********************************************************************
**  FIFORd_En
*********************************************************************/
void FIFORd_En(void)
{
    FifoCtlReg |= 0x0010;
    SpiWrite(FifoCtlReg, FIFO_CONTROL_REG);
}

/*********************************************************************
**  FIFORd_Di
*********************************************************************/
void FIFORd_Di(void)
{
    FifoCtlReg &= ~0x0010;
    SpiWrite(FifoCtlReg, FIFO_CONTROL_REG);
}

/*********************************************************************
** setFreq
*********************************************************************/
void SetFreq(Uint8 ch)
{
    Uint16 tmp;

    tmp = Freq_Base + ch;		//freq 2400MHz + offset ch
    tmp |= FreqBank[ch]<<13;	//find BNK[2:0] value
    SpiWrite(tmp, SYNTH1_REG);
}

/*********************************************************************
** SetTRC
*********************************************************************/
void SetTRC(Uint8 trc)
{
	if (trc)
	{
    	ModeCtlReg |= 0x0008;
    	SpiWrite(ModeCtlReg, MODE_CONTROL_REG);//set TRC=1
    }
    else
	{
	    ModeCtlReg &= (~0x0008);
	    SpiWrite(ModeCtlReg, MODE_CONTROL_REG);//set TRC=0
	}
}

/*********************************************************************
** TxPacket
*********************************************************************/
void TxPacket(void)
{
    Uint8 i;

    //write data to fifo
	FIFOWr_En();
    for (i=0; i<64; i++)
        FIFOWrite(PN9_Tab[i]);
    FIFOWr_Di();

    SetFreq(52);						//freq 2452MHz
    SetTRC(1);							//set bit TRC=1
    Ms1Pin = 1; 						//enter Tx mode
    Delay10us(6); 						//delay 60us for tx settling time

    CdTxenPin = 1; 						//start modulation
   	while(!FpRdy);						//wait until FP_RDY=1
    delay();							//delay>4us
	CdTxenPin = 0;
    Ms1Pin = 0; 						//quit Tx mode, enter synthesizer mode
}

/*********************************************************************
** RxPacket
*********************************************************************/
void RxPacket(void)
{
    Uint8 i;
    Uint8 RecvData;
    Uint8 tmp;

    Err_BitCnt=0; //clear Err_BitCnt
	FIFORd_En(); //rx fifo enable
    for (i=0; i<64; i++)
    {
        RecvData = FIFORead();

        if((RecvData ^ PN9_Tab[i])!=0)
        {
            tmp = RecvData ^ PN9_Tab[i];
            Err_BitCnt += (BitCount_Tab[tmp>>4] + BitCount_Tab[tmp & 0x0F]);
        }
    }
	FIFORd_Di(); //rx fifo disable
}

/*********************************************************************
** WaitRx
*********************************************************************/
void WaitRx(void)
{
	SetFreq(50);						//freq 2450MHz
	SetTRC(0);							//set bit TRC=0
	Ms1Pin = 1; 						//enter Rx mdoe
	while(FpRdy==0);					//wait FP_RDY=1
	Ms1Pin = 0;							//exit Rxmode, enter synthesizer mode
}

/*********************************************************************
** WaitAck
*********************************************************************/
Uint8 WaitAck(void)
{
	SetFreq(50);						//freq 2450MHz
	SetTRC(0);							//set bit TRC=0
	Ms1Pin = 1; 						//enter Rx mdoe

	Rst_Timer0();						//reload timer0
	TR0=1;								//start timer0
	while(TimeoutFlag==0 && FpRdy==0);	//wait timerout or data ready
	if (TimeoutFlag==1)
	{
		Ms1Pin=0;						//exit rx mode
		return 0;						//return 0
	}

    TR0=0;								//stop timer0
    Ms1Pin = 0;							//exit rx mode
    return 1;							//return 1
}

/*********************************************************************
** MeasureRSSI
*********************************************************************/
void MeasureRSSI(void)
{
	Uint16 tmp;
	Uint8 rssiValue;

	Ms1Pin = 1;	//enter rx mode
	Delay10us(6); //delay 60us for pll settling time

	SpiWrite(0x0005, CALIBRATION_CONTROL2_REG);	//ERSS=1
	do
	{
		tmp = SpiRead(CALIBRATION_CONTROL2_REG);
		tmp &= 0x0004;
	}
	while (tmp!=0); //check bit ERSS ok?
	Ms1Pin = 0;	//exit rx mode
	rssiValue = (SpiRead(RSSI_REG) & 0xFF00)>>8;
	_nop_();
}