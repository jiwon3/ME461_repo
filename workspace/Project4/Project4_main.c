//#############################################################################
// FILE:   LABstarter_main.c
//
// TITLE:  Lab Starter
//#############################################################################

// Included Files
#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <string.h>
#include <math.h>
#include <limits.h>
#include "F28x_Project.h"
#include "driverlib.h"
#include "device.h"
#include "F28379dSerial.h"
#include "LEDPatterns.h"
#include "song.h"
#include "dsp.h"
#include "fpu32/fpu_rfft.h"

#define PI          3.1415926535897932384626433832795
#define TWOPI       6.283185307179586476925286766559
#define HALFPI      1.5707963267948966192313216916398
// The Launchpad's CPU Frequency set to 200 you should not change this value
#define LAUNCHPAD_CPU_FREQUENCY 200


// Interrupt Service Routines predefinition
__interrupt void cpu_timer0_isr(void);
__interrupt void cpu_timer1_isr(void);
__interrupt void cpu_timer2_isr(void);
__interrupt void SWI_isr(void);

// Count variables
uint32_t numTimer0calls = 0;
uint32_t numSWIcalls = 0;
extern uint32_t numRXA;
uint16_t UARTPrint = 0;
uint16_t LEDdisplaynum = 0;

uint16_t I2C_count = 0;
uint16_t I2C_Print = 0;
uint8_t servo_up = 1;
int8_t servo_count = 0;
uint16_t adc1 =0;
uint16_t adc2 =0;
float voltage1 =0;
float voltage2 =0;

uint16_t second_read =0;
uint16_t minute_read =0;
uint16_t hour_read =0;
uint16_t day_read =0;
uint16_t date_read =0;
uint16_t month_read =0;
uint16_t year_read =0;

uint16_t * second_write = 0;
uint16_t * minute_write = 0;
uint16_t * hour_write = 0;
uint16_t * day_write = 0;
uint16_t * date_write = 0;
uint16_t * month_write = 0;
uint16_t * year_write = 0;


uint16_t WriteDAN777RCServo(uint16_t RC1, uint16_t RC2);
uint16_t ReadDAN777ADC(uint16_t *ADC1, uint16_t *ADC2);
uint16_t WriteDS1388Z(uint16_t second,uint16_t minute,uint16_t hour,uint16_t day,uint16_t date,uint16_t month,uint16_t year);
uint16_t ReadDS1388Z(uint16_t *second,uint16_t *minute,uint16_t *hour,uint16_t *day,uint16_t *date,uint16_t *month,uint16_t *year);

void main(void)
{
    // PLL, WatchDog, enable Peripheral Clocks
    // This example function is found in the F2837xD_SysCtrl.c file.
    InitSysCtrl();

    InitGpio();

    // Blue LED on LaunchPad
    GPIO_SetupPinMux(31, GPIO_MUX_CPU1, 0);
    GPIO_SetupPinOptions(31, GPIO_OUTPUT, GPIO_PUSHPULL);
    GpioDataRegs.GPASET.bit.GPIO31 = 1;

    // Red LED on LaunchPad
    GPIO_SetupPinMux(34, GPIO_MUX_CPU1, 0);
    GPIO_SetupPinOptions(34, GPIO_OUTPUT, GPIO_PUSHPULL);
    GpioDataRegs.GPBSET.bit.GPIO34 = 1;

    // LED1 and PWM Pin
    GPIO_SetupPinMux(22, GPIO_MUX_CPU1, 0);
    GPIO_SetupPinOptions(22, GPIO_OUTPUT, GPIO_PUSHPULL);
    GpioDataRegs.GPACLEAR.bit.GPIO22 = 1;

    // LED2
    GPIO_SetupPinMux(94, GPIO_MUX_CPU1, 0);
    GPIO_SetupPinOptions(94, GPIO_OUTPUT, GPIO_PUSHPULL);
    GpioDataRegs.GPCCLEAR.bit.GPIO94 = 1;

    // LED3
    GPIO_SetupPinMux(95, GPIO_MUX_CPU1, 0);
    GPIO_SetupPinOptions(95, GPIO_OUTPUT, GPIO_PUSHPULL);
    GpioDataRegs.GPCCLEAR.bit.GPIO95 = 1;

    // LED4
    GPIO_SetupPinMux(97, GPIO_MUX_CPU1, 0);
    GPIO_SetupPinOptions(97, GPIO_OUTPUT, GPIO_PUSHPULL);
    GpioDataRegs.GPDCLEAR.bit.GPIO97 = 1;

    // LED5
    GPIO_SetupPinMux(111, GPIO_MUX_CPU1, 0);
    GPIO_SetupPinOptions(111, GPIO_OUTPUT, GPIO_PUSHPULL);
    GpioDataRegs.GPDCLEAR.bit.GPIO111 = 1;

    // LED6
    GPIO_SetupPinMux(130, GPIO_MUX_CPU1, 0);
    GPIO_SetupPinOptions(130, GPIO_OUTPUT, GPIO_PUSHPULL);
    GpioDataRegs.GPECLEAR.bit.GPIO130 = 1;

    // LED7
    GPIO_SetupPinMux(131, GPIO_MUX_CPU1, 0);
    GPIO_SetupPinOptions(131, GPIO_OUTPUT, GPIO_PUSHPULL);
    GpioDataRegs.GPECLEAR.bit.GPIO131 = 1;

    // LED8
    GPIO_SetupPinMux(25, GPIO_MUX_CPU1, 0);
    GPIO_SetupPinOptions(25, GPIO_OUTPUT, GPIO_PUSHPULL);
    GpioDataRegs.GPACLEAR.bit.GPIO25 = 1;

    // LED9
    GPIO_SetupPinMux(26, GPIO_MUX_CPU1, 0);
    GPIO_SetupPinOptions(26, GPIO_OUTPUT, GPIO_PUSHPULL);
    GpioDataRegs.GPACLEAR.bit.GPIO26 = 1;

    // LED10
    GPIO_SetupPinMux(27, GPIO_MUX_CPU1, 0);
    GPIO_SetupPinOptions(27, GPIO_OUTPUT, GPIO_PUSHPULL);
    GpioDataRegs.GPACLEAR.bit.GPIO27 = 1;

    // LED11
    GPIO_SetupPinMux(60, GPIO_MUX_CPU1, 0);
    GPIO_SetupPinOptions(60, GPIO_OUTPUT, GPIO_PUSHPULL);
    GpioDataRegs.GPBCLEAR.bit.GPIO60 = 1;

    // LED12
    GPIO_SetupPinMux(61, GPIO_MUX_CPU1, 0);
    GPIO_SetupPinOptions(61, GPIO_OUTPUT, GPIO_PUSHPULL);
    GpioDataRegs.GPBCLEAR.bit.GPIO61 = 1;

    // LED13
    GPIO_SetupPinMux(157, GPIO_MUX_CPU1, 0);
    GPIO_SetupPinOptions(157, GPIO_OUTPUT, GPIO_PUSHPULL);
    GpioDataRegs.GPECLEAR.bit.GPIO157 = 1;

    // LED14
    GPIO_SetupPinMux(158, GPIO_MUX_CPU1, 0);
    GPIO_SetupPinOptions(158, GPIO_OUTPUT, GPIO_PUSHPULL);
    GpioDataRegs.GPECLEAR.bit.GPIO158 = 1;

    // LED15
    GPIO_SetupPinMux(159, GPIO_MUX_CPU1, 0);
    GPIO_SetupPinOptions(159, GPIO_OUTPUT, GPIO_PUSHPULL);
    GpioDataRegs.GPECLEAR.bit.GPIO159 = 1;

    // LED16
    GPIO_SetupPinMux(160, GPIO_MUX_CPU1, 0);
    GPIO_SetupPinOptions(160, GPIO_OUTPUT, GPIO_PUSHPULL);
    GpioDataRegs.GPFCLEAR.bit.GPIO160 = 1;

    //WIZNET Reset
    GPIO_SetupPinMux(0, GPIO_MUX_CPU1, 0);
    GPIO_SetupPinOptions(0, GPIO_OUTPUT, GPIO_PUSHPULL);
    GpioDataRegs.GPASET.bit.GPIO0 = 1;

    //ESP8266 Reset
    GPIO_SetupPinMux(1, GPIO_MUX_CPU1, 0);
    GPIO_SetupPinOptions(1, GPIO_OUTPUT, GPIO_PUSHPULL);
    GpioDataRegs.GPASET.bit.GPIO1 = 1;

    //SPIRAM  CS  Chip Select
    GPIO_SetupPinMux(19, GPIO_MUX_CPU1, 0);
    GPIO_SetupPinOptions(19, GPIO_OUTPUT, GPIO_PUSHPULL);
    GpioDataRegs.GPASET.bit.GPIO19 = 1;

    //DRV8874 #1 DIR  Direction
    GPIO_SetupPinMux(29, GPIO_MUX_CPU1, 0);
    GPIO_SetupPinOptions(29, GPIO_OUTPUT, GPIO_PUSHPULL);
    GpioDataRegs.GPASET.bit.GPIO29 = 1;

    //DRV8874 #2 DIR  Direction
    GPIO_SetupPinMux(32, GPIO_MUX_CPU1, 0);
    GPIO_SetupPinOptions(32, GPIO_OUTPUT, GPIO_PUSHPULL);
    GpioDataRegs.GPBSET.bit.GPIO32 = 1;

    //DAN28027  CS  Chip Select
    GPIO_SetupPinMux(9, GPIO_MUX_CPU1, 0);
    GPIO_SetupPinOptions(9, GPIO_OUTPUT, GPIO_PUSHPULL);
    GpioDataRegs.GPASET.bit.GPIO9 = 1;

    //MPU9250  CS  Chip Select
    GPIO_SetupPinMux(66, GPIO_MUX_CPU1, 0);
    GPIO_SetupPinOptions(66, GPIO_OUTPUT, GPIO_PUSHPULL);
    GpioDataRegs.GPCSET.bit.GPIO66 = 1;

    //WIZNET  CS  Chip Select
    GPIO_SetupPinMux(125, GPIO_MUX_CPU1, 0);
    GPIO_SetupPinOptions(125, GPIO_OUTPUT, GPIO_PUSHPULL);
    GpioDataRegs.GPDSET.bit.GPIO125 = 1;

    //PushButton 1
    GPIO_SetupPinMux(4, GPIO_MUX_CPU1, 0);
    GPIO_SetupPinOptions(4, GPIO_INPUT, GPIO_PULLUP);

    //PushButton 2
    GPIO_SetupPinMux(5, GPIO_MUX_CPU1, 0);
    GPIO_SetupPinOptions(5, GPIO_INPUT, GPIO_PULLUP);

    //PushButton 3
    GPIO_SetupPinMux(6, GPIO_MUX_CPU1, 0);
    GPIO_SetupPinOptions(6, GPIO_INPUT, GPIO_PULLUP);

    //PushButton 4
    GPIO_SetupPinMux(7, GPIO_MUX_CPU1, 0);
    GPIO_SetupPinOptions(7, GPIO_INPUT, GPIO_PULLUP);

    //Joy Stick Pushbutton
    GPIO_SetupPinMux(8, GPIO_MUX_CPU1, 0);
    GPIO_SetupPinOptions(8, GPIO_INPUT, GPIO_PULLUP);

    // Clear all interrupts and initialize PIE vector table:
    // Disable CPU interrupts
    DINT;

    // Initialize the PIE control registers to their default state.
    // The default state is all PIE interrupts disabled and flags
    // are cleared.
    // This function is found in the F2837xD_PieCtrl.c file.
    InitPieCtrl();

    // Disable CPU interrupts and clear all CPU interrupt flags:
    IER = 0x0000;
    IFR = 0x0000;

    // Initialize the PIE vector table with pointers to the shell Interrupt
    // Service Routines (ISR).
    // This will populate the entire table, even if the interrupt
    // is not used in this example.  This is useful for debug purposes.
    // The shell ISR routines are found in F2837xD_DefaultIsr.c.
    // This function is found in F2837xD_PieVect.c.
    InitPieVectTable();

    // Interrupts that are used in this example are re-mapped to
    // ISR functions found within this project
    EALLOW;  // This is needed to write to EALLOW protected registers
    PieVectTable.TIMER0_INT = &cpu_timer0_isr;
    PieVectTable.TIMER1_INT = &cpu_timer1_isr;
    PieVectTable.TIMER2_INT = &cpu_timer2_isr;
    PieVectTable.SCIA_RX_INT = &RXAINT_recv_ready;
    PieVectTable.SCIB_RX_INT = &RXBINT_recv_ready;
    PieVectTable.SCIC_RX_INT = &RXCINT_recv_ready;
    PieVectTable.SCID_RX_INT = &RXDINT_recv_ready;
    PieVectTable.SCIA_TX_INT = &TXAINT_data_sent;
    PieVectTable.SCIB_TX_INT = &TXBINT_data_sent;
    PieVectTable.SCIC_TX_INT = &TXCINT_data_sent;
    PieVectTable.SCID_TX_INT = &TXDINT_data_sent;

    PieVectTable.EMIF_ERROR_INT = &SWI_isr;
    EDIS;    // This is needed to disable write to EALLOW protected registers


    // Initialize the CpuTimers Device Peripheral. This function can be
    // found in F2837xD_CpuTimers.c
    InitCpuTimers();

    // Configure CPU-Timer 0, 1, and 2 to interrupt every given period:
    // 200MHz CPU Freq,                       Period (in uSeconds)
    ConfigCpuTimer(&CpuTimer0, LAUNCHPAD_CPU_FREQUENCY, 10000);
    ConfigCpuTimer(&CpuTimer1, LAUNCHPAD_CPU_FREQUENCY, 20000);
    ConfigCpuTimer(&CpuTimer2, LAUNCHPAD_CPU_FREQUENCY, 40000);

    // Enable CpuTimer Interrupt bit TIE
    CpuTimer0Regs.TCR.all = 0x4000;
    CpuTimer1Regs.TCR.all = 0x4000;
    CpuTimer2Regs.TCR.all = 0x4000;

    init_serialSCIA(&SerialA,115200);
    //    init_serialSCIB(&SerialB,115200);
    //    init_serialSCIC(&SerialC,115200);
    //    init_serialSCID(&SerialD,115200);


    // Enable CPU int1 which is connected to CPU-Timer 0, CPU int13
    // which is connected to CPU-Timer 1, and CPU int 14, which is connected
    // to CPU-Timer 2:  int 12 is for the SWI.  
    IER |= M_INT1;
    IER |= M_INT8;  // SCIC SCID
    IER |= M_INT9;  // SCIA
    IER |= M_INT12;
    IER |= M_INT13;
    IER |= M_INT14;

    // Enable TINT0 in the PIE: Group 1 interrupt 7
    PieCtrlRegs.PIEIER1.bit.INTx7 = 1;
    // Enable SWI in the PIE: Group 12 interrupt 9
    PieCtrlRegs.PIEIER12.bit.INTx9 = 1;

    // Enable global Interrupts and higher priority real-time debug events
    EINT;  // Enable Global interrupt INTM
    ERTM;  // Enable Global realtime interrupt DBGM


    uint16_t check_write_DS = WriteDS1388(uint16_t second,uint16_t minute,uint16_t hour,uint16_t day,uint16_t date,uint16_t month,uint16_t year)
    // IDLE loop. Just sit and loop forever (optional):
    while(1)
    {
        if (UARTPrint == 1 ) {
            serial_printf(&SerialA,"%c %d/%d/%d %d:%d:%d /r/n ", day_print, month_read, date_read, year_read, hour_read, minute_read, second_read);
            UARTPrint = 0;
            // can you print strings with %c ?
        }

    }
}


// SWI_isr,  Using this interrupt as a Software started interrupt
__interrupt void SWI_isr(void) {

    // These three lines of code allow SWI_isr, to be interrupted by other interrupt functions
    // making it lower priority than all other Hardware interrupts.
    PieCtrlRegs.PIEACK.all = PIEACK_GROUP12;
    asm("       NOP");                    // Wait one cycle
    EINT;                                 // Clear INTM to enable interrupts



    // Insert SWI ISR Code here.......


    numSWIcalls++;

    DINT;

}

// cpu_timer0_isr - CPU Timer0 ISR
__interrupt void cpu_timer0_isr(void)
{
    CpuTimer0.InterruptCount++;

    numTimer0calls++;

    //    if ((numTimer0calls%50) == 0) {
    //        PieCtrlRegs.PIEIFR12.bit.INTx9 = 1;  // Manually cause the interrupt for the SWI
    //    }

    if ((numTimer0calls%250) == 0) {
        displayLEDletter(LEDdisplaynum);
        LEDdisplaynum++;
        if (LEDdisplaynum == 0xFFFF) {  // prevent roll over exception
            LEDdisplaynum = 0;
        }
    }

    // Blink LaunchPad Red LED
    GpioDataRegs.GPBTOGGLE.bit.GPIO34 = 1;

    // Acknowledge this interrupt to receive more interrupts from group 1
    PieCtrlRegs.PIEACK.all = PIEACK_GROUP1;



}

// cpu_timer1_isr - CPU Timer1 ISR
__interrupt void cpu_timer1_isr(void)
{
    CpuTimer1.InterruptCount++;

    if ((CpuTimer1.InterruptCount++ %5) == 0){
        UARTPrint =1;
    }

    if (servo_up == 1){
        servo_count ++ 5;
    }
    else{
        servo_count -- 5;
    }

    if (servo_count >= 90){
        servo_up = 0;
    }

    if (servo_count <= -90){
        servo_up = 1;
    }

    uint16_t servo_1 = 3200 + servo_count*2000/90;
    uint16_t servo_2 = 3200 + servo_count*2000/90;

    // Question: Do I have to convert to float?
    // Question: How can I convert back to int?

    uint16_t check_write_DAN = WriteDAN777RCServo(servo_1, servo_2);
    uint16_t check_read_DAN = ReadDAN777ADC(adc1, adc2);
    uint16_t check_read_DS = ReadDS1388Z(*second_read, *minute_read, *hour_read, *day_read, *date_read, *month_read, *year_read);

    voltage1 = adc1*3.3/1023.0;
    voltage2 = adc1*3.3/1023.0;

    //Question: Does float have enough space to store the 16-bit integer converted? What if the MSB is shown in stead of LSB?

    const char *day[] = {'Monday','Tuesday','Wednesday','Thursday','Friday','Saturday','Sunday'};
    char day_print = *day[*day_read]
    //Question: Is this the right way of printing strings?

}

// cpu_timer2_isr CPU Timer2 ISR
__interrupt void cpu_timer2_isr(void)
{


    // Blink LaunchPad Blue LED
    GpioDataRegs.GPATOGGLE.bit.GPIO31 = 1;

    CpuTimer2.InterruptCount++;

    if ((CpuTimer2.InterruptCount % 50) == 0) {
        // UARTPrint = 1;
    }
}

uint16_t WriteDAN777RCServo(uint16_t Cmd16bit_1, uint16_t Cmd16bit_2) {
    uint16_t Cmd1LSB = 0;
    uint16_t Cmd1MSB = 0;
    uint16_t Cmd2LSB = 0;
    uint16_t Cmd2MSB = 0;
    Cmd1LSB = Cmd16bit_1 & 0xFF; //Bottom 8 bits of command
    Cmd1MSB = (Cmd16bit_1 >> 8) & 0xFF; //Top 8 bits of command
    Cmd2LSB = Cmd16bit_2 & 0xFF; //Bottom 8 bits of command
    Cmd2MSB = (Cmd16bit_2 >> 8) & 0xFF; //Top 8 bits of command
    DELAY_US(200); // Allow time for I2C to finish up previous commands. It pains me to have this
    // delay here but I have not had time to figure out what status bit to poll on to
    // to check if the I2C peripheral is ready of the next command. I have tried the busy
    // bit but that gave me some issues especially at startup. This would be a great
    // choice for a part of your final project if you would like to learn more about I2C.
    if (I2cbRegs.I2CSTR.bit.BB == 1) { // Check if I2C busy, if it is better
        return 2; // to Exit and try again next sample
    } // This should not happen too often
    while(!I2cbRegs.I2CSTR.bit.XRDY); //Poll until I2C ready to Transmit
    I2cbRegs.I2CSAR.all = 0x25; // I2C address of DAN777
    I2cbRegs.I2CCNT = 5; //Num Values plus Start Register 4 + 1
    I2cbRegs.I2CDXR.all = 4; // First need to transfer the Register value to start writing data
    // I2C in master mode (MST), I2C is in transmit mode (TRX) with start and stop
    I2cbRegs.I2CMDR.all = 0x6E20;

    while(!I2cbRegs.I2CSTR.bit.XRDY); //Poll until I2C ready to Transmit
    I2cbRegs.I2CDXR.all = Cmd1LSB; // Write Command 1 LSB
    if (I2cbRegs.I2CSTR.bit.NACK == 1){ // Check for No Acknowledgment
        return 3; // This should not happen
    }
    while(!I2cbRegs.I2CSTR.bit.XRDY); //Poll until I2C ready to Transmit
    I2cbRegs.I2CDXR.all = Cmd1MSB; // Write Command 1 MSB
    if (I2cbRegs.I2CSTR.bit.NACK == 1){ // Check for No Acknowledgment
        return 3; // This should not happen
    }
    while(!I2cbRegs.I2CSTR.bit.XRDY); //Poll until I2C ready to Transmit
    I2cbRegs.I2CDXR.all = Cmd2LSB; // Write Command 2 LSB
    if (I2cbRegs.I2CSTR.bit.NACK == 1){ // Check for No Acknowledgment
        return 3; // This should not happen
    }
    while(!I2cbRegs.I2CSTR.bit.XRDY); //Poll until I2C ready to Transmit
    I2cbRegs.I2CDXR.all = Cmd2MSB; // Write Command 2 MSB
    // After this write since I2CCNT = 0
    // A Stop condition will be issued
    if (I2cbRegs.I2CSTR.bit.NACK == 1){
        return 3;
    }
    return 0;
}


//Read Two 16 Bit values from I2C Slave CHIPXYZ starting at CHIPXYZ's register 10
//Notice the Rvalue1 and Rvalue2 passed as pointers (passed by reference)
//So pass address of uint16_t variable when using this function for example:
// uint16_t Rval1 = 0;
// uint16_t Rval2 = 0;
// err = ReadTwo16BitValuesFromCHIPXYZ(&Rval1,&Rval2);
// This allows Rval1 and Rval2 to be changed inside the function and return
// the values read inside the function.
uint16_t ReadDAN777ADC(uint16_t *Rvalue1,uint16_t *Rvalue2) {
    uint16_t Val1LSB = 0;
    uint16_t Val1MSB = 0;
    uint16_t Val2LSB = 0;
    uint16_t Val2MSB = 0;
    DELAY_US(200); // Allow time for I2C to finish up previous commands. It pains me to have this
    // delay here but I have not had time to figure out what status bit to poll on to
    // to check if the I2C peripheral is ready of the next command. I have tried the busy
    // bit but that gave me some issues especially at startup. This would be a great
    // choice for a part of your final project if you would like to learn more about I2C.
    if (I2cbRegs.I2CSTR.bit.BB == 1) {
        return 2;
    }
    while(!I2cbRegs.I2CSTR.bit.XRDY); //Poll until I2C ready to Transmit
    I2cbRegs.I2CSAR.all = 0x25; // I2C address of DAN777
    I2cbRegs.I2CCNT = 1; // Just Sending Address to start reading from
    I2cbRegs.I2CDXR.all = 0; // Start Reading at this Register location
    // I2C in master mode (MST), I2C is in transmit mode (TRX) with start
    I2cbRegs.I2CMDR.all = 0x6620;
    if (I2cbRegs.I2CSTR.bit.NACK == 1) {
        return 3;
    }
    while(!I2cbRegs.I2CSTR.bit.XRDY); //Poll until I2C ready to Transmit
    I2cbRegs.I2CSAR.all = 0x25; // I2C address of ChipXYZ
    I2cbRegs.I2CCNT = 4;
    // I2C in master mode (MST), TRX=0, receive mode start stop
    I2cbRegs.I2CMDR.all = 0x6C20; // Reissuing another Start with Read
    if (I2cbRegs.I2CSTR.bit.NACK == 1) {
        return 3;
    }
    while(!I2cbRegs.I2CSTR.bit.RRDY); //Poll until I2C has Recieved 8 bit value
    Val1LSB = I2cbRegs.I2CDRR.all; // Read CHIPXYZ
    if (I2cbRegs.I2CSTR.bit.NACK == 1) {
        return 3;
    }
    while(!I2cbRegs.I2CSTR.bit.RRDY); //Poll until I2C has Recieved 8 bit value
    Val1MSB = I2cbRegs.I2CDRR.all; // Read CHIPXYZ
    if (I2cbRegs.I2CSTR.bit.NACK == 1) {
        return 3;
    }
    while(!I2cbRegs.I2CSTR.bit.RRDY); //Poll until I2C has Recieved 8 bit value
    Val2LSB = I2cbRegs.I2CDRR.all; // Read CHIPXYZ
    if (I2cbRegs.I2CSTR.bit.NACK == 1) {
        return 3;
    }
    while(!I2cbRegs.I2CSTR.bit.RRDY); //Poll until I2C has Recieved 8 bit value
    Val2MSB = I2cbRegs.I2CDRR.all; // Read CHIPXYZ
    if (I2cbRegs.I2CSTR.bit.NACK == 1) {
        return 3;
    }
    // After this read since I2CCNT = 0
    // A Stop condition will be issued
    *Rvalue1 = (Val1MSB << 8) | (Val1LSB & 0xFF);
    *Rvalue2 = (Val2MSB << 8) | (Val2LSB & 0xFF);
    return 0;
}

uint16_t WriteDS1388(uint16_t second,uint16_t minute,uint16_t hour,uint16_t day,uint16_t date,uint16_t month,uint16_t year) {

    uint16_t second_send = ((second/10)<<4) | (second%10);
    uint16_t minute_send = ((minute/10)<<4) | (minute%10);
    uint16_t hour_send = hour = ((hour/10)<<4) | (hour%10);
    uint16_t day_send = day;
    uint16_t date_send = ((date/10)<<4) | (date%10);
    uint16_t month_send = ((month/10)<<4) | (month%10);
    uint16_t year_send = ((year/10)<<4) | (year%10);

    DELAY_US(200); // Allow time for I2C to finish up previous commands. It pains me to have this
    // delay here but I have not had time to figure out what status bit to poll on to
    // to check if the I2C peripheral is ready of the next command. I have tried the busy
    // bit but that gave me some issues especially at startup. This would be a great
    // choice for a part of your final project if you would like to learn more about I2C.
    if (I2cbRegs.I2CSTR.bit.BB == 1) { // Check if I2C busy, if it is better
        return 2; // to Exit and try again next sample
    } // This should not happen too often
    while(!I2cbRegs.I2CSTR.bit.XRDY); //Poll until I2C ready to Transmit
    I2cbRegs.I2CSAR.all = 0x68; // I2C address of DS1388
    I2cbRegs.I2CCNT = 8; //Num Values plus Start Register 7 + 1
    I2cbRegs.I2CDXR.all = 1; // First need to transfer the Register value
    // to start writing data
    // I2C in master mode (MST), I2C is in transmit mode (TRX) with start and stop
    I2cbRegs.I2CMDR.all = 0x6E20;

    while(!I2cbRegs.I2CSTR.bit.XRDY); //Poll until I2C ready to Transmit
    I2cbRegs.I2CDXR.all = second_send; // Write Command 1 LSB
    if (I2cbRegs.I2CSTR.bit.NACK == 1){ // Check for No Acknowledgment
        return 3; // This should not happen
    }
    while(!I2cbRegs.I2CSTR.bit.XRDY); //Poll until I2C ready to Transmit
    I2cbRegs.I2CDXR.all = minute_send; // Write Command 1 MSB
    if (I2cbRegs.I2CSTR.bit.NACK == 1){ // Check for No Acknowledgment
        return 3; // This should not happen
    }
    while(!I2cbRegs.I2CSTR.bit.XRDY); //Poll until I2C ready to Transmit
    I2cbRegs.I2CDXR.all = hour_send; // Write Command 2 LSB
    if (I2cbRegs.I2CSTR.bit.NACK == 1){ // Check for No Acknowledgment
        return 3; // This should not happen
    }
    while(!I2cbRegs.I2CSTR.bit.XRDY); //Poll until I2C ready to Transmit
    I2cbRegs.I2CDXR.all = day_send; // Write Command 2 MSB
    // After this write since I2CCNT = 0
    // A Stop condition will be issued
    if (I2cbRegs.I2CSTR.bit.NACK == 1){
        return 3;
    }
    while(!I2cbRegs.I2CSTR.bit.XRDY); //Poll until I2C ready to Transmit
    I2cbRegs.I2CDXR.all = date_send; // Write Command 1 LSB
    if (I2cbRegs.I2CSTR.bit.NACK == 1){ // Check for No Acknowledgment
        return 3; // This should not happen
    }
    while(!I2cbRegs.I2CSTR.bit.XRDY); //Poll until I2C ready to Transmit
    I2cbRegs.I2CDXR.all = month_send; // Write Command 1 LSB
    if (I2cbRegs.I2CSTR.bit.NACK == 1){ // Check for No Acknowledgment
        return 3; // This should not happen
    }
    while(!I2cbRegs.I2CSTR.bit.XRDY); //Poll until I2C ready to Transmit
    I2cbRegs.I2CDXR.all = year_send; // Write Command 1 LSB
    if (I2cbRegs.I2CSTR.bit.NACK == 1){ // Check for No Acknowledgment
        return 3; // This should not happen
    }

    return 0;
}

//Read Two 16 Bit values from I2C Slave CHIPXYZ starting at CHIPXYZ's register 10
//Notice the Rvalue1 and Rvalue2 passed as pointers (passed by reference)
//So pass address of uint16_t variable when using this function for example:
// uint16_t Rval1 = 0;
// uint16_t Rval2 = 0;
// err = ReadTwo16BitValuesFromCHIPXYZ(&Rval1,&Rval2);
// This allows Rval1 and Rval2 to be changed inside the function and return
// the values read inside the function.

uint16_t ReadDS1388Z(uint16_t *second,uint16_t *minute,uint16_t *hour,uint16_t *day,uint16_t *date,uint16_t *month,uint16_t *year) {


    DELAY_US(200); // Allow time for I2C to finish up previous commands. It pains me to have this
    // delay here but I have not had time to figure out what status bit to poll on to
    // to check if the I2C peripheral is ready of the next command. I have tried the busy
    // bit but that gave me some issues especially at startup. This would be a great
    // choice for a part of your final project if you would like to learn more about I2C.

    if (I2cbRegs.I2CSTR.bit.BB == 1) {
        return 2;
    }
    while(!I2cbRegs.I2CSTR.bit.XRDY); //Poll until I2C ready to Transmit
    I2cbRegs.I2CSAR.all = 0x68; // I2C address of DS1388
    I2cbRegs.I2CCNT = 1;
    I2cbRegs.I2CDXR.all = 1; // Start Reading at this Register location
    // I2C in master mode (MST), I2C is in transmit mode (TRX) with start
    I2cbRegs.I2CMDR.all = 0x6620;
    if (I2cbRegs.I2CSTR.bit.NACK == 1) {
        return 3;
    }
    while(!I2cbRegs.I2CSTR.bit.XRDY); //Poll until I2C ready to Transmit
    I2cbRegs.I2CSAR.all = 0x68; // I2C address of DS1388
    I2cbRegs.I2CCNT = 7;
    // I2C in master mode (MST), TRX=0, receive mode start stop
    I2cbRegs.I2CMDR.all = 0x6C20; // Reissuing another Start with Read
    if (I2cbRegs.I2CSTR.bit.NACK == 1) {
        return 3;
    }
    while(!I2cbRegs.I2CSTR.bit.RRDY); //Poll until I2C has Received 8 bit value
    *second = I2cbRegs.I2CDRR.all; // Read CHIPXYZ
    if (I2cbRegs.I2CSTR.bit.NACK == 1) {
        return 3;
    }
    while(!I2cbRegs.I2CSTR.bit.RRDY); //Poll until I2C has Received 8 bit value
    *minute = I2cbRegs.I2CDRR.all; // Read CHIPXYZ
    if (I2cbRegs.I2CSTR.bit.NACK == 1) {
        return 3;
    }
    while(!I2cbRegs.I2CSTR.bit.RRDY); //Poll until I2C has Received 8 bit value
    *hour= I2cbRegs.I2CDRR.all; // Read CHIPXYZ
    if (I2cbRegs.I2CSTR.bit.NACK == 1) {
        return 3;
    }
    while(!I2cbRegs.I2CSTR.bit.RRDY); //Poll until I2C has Received 8 bit value
    *day = I2cbRegs.I2CDRR.all; // Read CHIPXYZ
    if (I2cbRegs.I2CSTR.bit.NACK == 1) {
        return 3;
    }
    while(!I2cbRegs.I2CSTR.bit.RRDY); //Poll until I2C has Received 8 bit value
    *date = I2cbRegs.I2CDRR.all; // Read CHIPXYZ
    if (I2cbRegs.I2CSTR.bit.NACK == 1) {
        return 3;
    }
    while(!I2cbRegs.I2CSTR.bit.RRDY); //Poll until I2C has Received 8 bit value
    *month = I2cbRegs.I2CDRR.all; // Read CHIPXYZ
    if (I2cbRegs.I2CSTR.bit.NACK == 1) {
        return 3;
    }
    while(!I2cbRegs.I2CSTR.bit.RRDY); //Poll until I2C has Received 8 bit value
    *year = I2cbRegs.I2CDRR.all; // Read CHIPXYZ
    if (I2cbRegs.I2CSTR.bit.NACK == 1) {
        return 3;
    }
    // After this read since I2CCNT = 0
    // A Stop condition will be issued

    *second = ((*second>>4)&(0xFF))*10 + (*second)&(0xFF);
    *minute = ((*minute>>4)&(0xFF))*10 + (*minute)&(0xFF);
    *hour = ((*hour>>4)&(0x03))*10 + (*hour)&(0xFF);
    *day = (*day)&(0xFF);
    *date = ((*date>>4)&(0xFF))*10 + (*date)&(0xFF);
    *month = ((*month>>4)&(0xFF))*10 + (*month)&(0xFF);
    *year = ((*year>>4)&(0xFF))*10 + (*year)&(0xFF);


    return 0

}



