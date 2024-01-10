//Car alarm project with three sensor (button) inputs and four LED 
//outputs (one off board) and an alarm buzzer.

//=====[Libraries]=============================================================

#include "mbed.h"
#include "arm_book_lib.h"

//=====[Declaration and initialization of public global objects]===============

DigitalIn ignition(BUTTON1);  
DigitalIn driverSeat(D4);  
DigitalIn driverBelt(D5);  
DigitalIn passengerSeat(D6);
DigitalIn passengerBelt(D7);

DigitalOut alarm(LED3);  // Red
DigitalOut ignitionEnabled(LED1); // Green
DigitalOut engineStarted(LED2); // Blue
DigitalInOut sirenPin(PE_10); 

UnbufferedSerial uartUsb(USBTX, USBRX, 115200);

//=====[Declaration and initialization of public global variables]=============

bool seatsSecure;
bool alarmState;
bool errorPrinted;

//=====[Declarations (prototypes) of public functions]=========================

void inputsInit();
void outputsInit();

void checkSeats();
void checkIgnition();

void systemGreeting();
void checkErrors();
//void availableCommands();

//=====[Main function, the program entry point after power on or reset]========

int main()
{
    inputsInit();
    outputsInit();
    systemGreeting();
    while (true) {
        checkSeats(); //
        checkIgnition();  //
        checkErrors();
    }
}

//=====[Implementations of public functions]===================================

void inputsInit()
{
    driverSeat.mode(PullDown);
    driverBelt.mode(PullDown);
    passengerSeat.mode(PullDown);
    passengerBelt.mode(PullDown);
}

void outputsInit()
{
    alarm = OFF;
    ignitionEnabled = OFF;
    engineStarted = OFF;
    sirenPin = ON; //active low
    errorPrinted = false;
    alarmState = OFF;
    seatsSecure = false;
}

void checkSeats()
{
   if (!engineStarted) {

        if ( driverSeat && driverBelt && passengerSeat && passengerBelt ) {
            seatsSecure = true;
            alarmState = OFF;
        } else {
            seatsSecure = false;
        }
            ignitionEnabled = seatsSecure;
     }
}

void checkIgnition()
{
    if ( ignition && !engineStarted ) {
        if ( ignitionEnabled)  {
                    ignitionEnabled = OFF;
                    engineStarted = ON;
                    uartUsb.write( "Engine Started\r\n", 16);
        }else{
                    alarmState = ON;
        }
        }
}

void checkErrors()
{

    alarm = alarmState;
    sirenPin = !alarmState;
    if ( alarmState && !errorPrinted ) {
        errorPrinted = true;
        uartUsb.write( "Ignition Inhibited\r\n", 20);
        if ( !driverSeat) {
                uartUsb.write( "What? No Driver?\r\n", 18);
            } 
        if (!driverBelt) {
                uartUsb.write( "Driver seatbelt not fastened\r\n",30);
                    }     
        if (!passengerSeat) {
                uartUsb.write("No passenger seated\r\n",21); 
                        } 
        if (!passengerBelt) {
                uartUsb.write( "Passenger seatbelt not fastened\r\n",33);
                            }
         }
  //  alarmState = OFF;
}

void systemGreeting()
{
 uartUsb.write( "\n\nWelcome to enhanced alarm system model 218-W24\r\n", 50 );
}