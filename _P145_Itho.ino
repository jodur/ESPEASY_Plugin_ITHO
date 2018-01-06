//#######################################################################################################
//############################## Plugin 145: Itho ventilation unit 868Mhz remote ########################
//#######################################################################################################

// List of commands:
// JOIN to join ESP8266 with Itho ventilation unit
// LOW - set Itho ventilation unit to lowest speed
// MEDIUM - set Itho ventilation unit to medium speed
// HIGH - set Itho ventilation unit to max speed
// TIMER1 - set itho to max speed with hardware timer (10 min)
// TIMER2 - set itho to max speed with hardware timer (20 min)
// TIMER3 - set itho to max speed with hardware timer (30 min)

//List of States:
// 1 - set Itho ventilation unit to lowest speed
// 2 - set Itho ventilation unit to medium speed
// 3 - set Itho ventilation unit to max speed
// 10 - set itho to max speed with hardware timer (10 min)
// 20 - set itho to max speed with hardware timer (20 min)
// 30 - set itho to max speed with hardware timer (30 min)

// Usage (not case sensitive):
// http://ip/control?cmd=ITHOSEND,join
// http://ip/control?cmd=ITHOSEND,low
// http://ip/control?cmd=ITHOSEND,medium
// http://ip/control?cmd=ITHOSEND,high


// This code needs the library made by 'supersjimmie': https://github.com/supersjimmie/IthoEcoFanRFT/tree/master/Master/Itho
// A CC1101 868Mhz transmitter is needed
// See https://gathering.tweakers.net/forum/list_messages/1690945 for more information

#include <SPI.h>
#include "IthoCC1101.h"
#include "IthoPacket.h"
#include <Ticker.h>

IthoCC1101 PLUGIN_145_rf;


// extra for interrupt handling
bool PLUGIN_145_ITHOhasPacket = false;
Ticker PLUGIN_145_ITHOticker;
int PLUGIN_145_State=1; // after startup it is assumed that the fan is running low
int PLUGIN_145_OldState=1;
int PLUGIN_145_Timer=0;

#define PLUGIN_145
#define PLUGIN_ID_145         145
#define PLUGIN_NAME_145       "Itho ventilation remote"
#define PLUGIN_VALUENAME1_145 "State"
#define PLUGIN_VALUENAME2_145 "Timer"

// Timer values for hardware timer in Fan
#define PLUGIN_145_Time1      10*60
#define PLUGIN_145_Time2      20*60
#define PLUGIN_145_Time3      30*60

#define PLUGIN_145_IRQ_PIN D1

boolean Plugin_145(byte function, struct EventStruct *event, String& string)
{
	boolean success = false;

	switch (function)
	{

		case PLUGIN_DEVICE_ADD:
		{
			Device[++deviceCount].Number = PLUGIN_ID_145;
      Device[deviceCount].Type = DEVICE_TYPE_DUMMY;
      Device[deviceCount].VType = SENSOR_TYPE_DUAL;
      Device[deviceCount].Ports = 0;
      Device[deviceCount].PullUpOption = false;
      Device[deviceCount].InverseLogicOption = false;
      Device[deviceCount].FormulaOption = false;
      Device[deviceCount].ValueCount = 2;
      Device[deviceCount].SendDataOption = true;
      Device[deviceCount].TimerOption = true;
      Device[deviceCount].GlobalSyncOption = true;
      break;
		}

		case PLUGIN_GET_DEVICENAME:
		{
			string = F(PLUGIN_NAME_145);
			break;
		}

		case PLUGIN_GET_DEVICEVALUENAMES:
		{
			strcpy_P(ExtraTaskSettings.TaskDeviceValueNames[0], PSTR(PLUGIN_VALUENAME1_145));
      strcpy_P(ExtraTaskSettings.TaskDeviceValueNames[1], PSTR(PLUGIN_VALUENAME2_145));
			break;
		}
  
  		
		case PLUGIN_INIT:
		{
			PLUGIN_145_rf.init();
      pinMode(PLUGIN_145_IRQ_PIN, INPUT);
      attachInterrupt(PLUGIN_145_IRQ_PIN, PLUGIN_145_ITHOinterrupt, RISING);
			addLog(LOG_LEVEL_INFO, F("CC1101 868Mhz transmitter initialized"));
      PLUGIN_145_rf.initReceive();
			success = true;
			break;
		}



    case PLUGIN_ONCE_A_SECOND:
    {
      //decrement timer when timermode is running
      if (PLUGIN_145_State>=10) PLUGIN_145_Timer--;
      
      //if timer has elapsed set Fan state to low
      if ((PLUGIN_145_State>=10) && (PLUGIN_145_Timer<=0)) 
       { PLUGIN_145_State=1;
         PLUGIN_145_Timer=0;
       } 
      
    }


    case PLUGIN_READ: {    
      
              UserVar[event->BaseVarIndex]=PLUGIN_145_State;
              UserVar[event->BaseVarIndex+1]=PLUGIN_145_Timer;
              String log = F("State: ");
              log += UserVar[event->BaseVarIndex];
              addLog(LOG_LEVEL_INFO, log);
              log = F("Timer: ");
              log += UserVar[event->BaseVarIndex+1];
              addLog(LOG_LEVEL_INFO, log);
              
              success = true;
              break;
    }  
    
		case PLUGIN_WRITE: {

			String tmpString = string;
		  String cmd = parseString(tmpString, 1);
			String param1 = parseString(tmpString, 2);


			if (cmd.equalsIgnoreCase(F("ITHOSEND")))
			{

				if (param1.equalsIgnoreCase(F("join")))
				{
					PLUGIN_145_rf.sendCommand(IthoJoin);
					addLog(LOG_LEVEL_INFO, F("Sent command for 'join' to Itho unit"));
					printWebString += F("Sent command for 'join' to Itho unit");
					success = true;
				}

				if (param1.equalsIgnoreCase(F("leave")))
				{
					PLUGIN_145_rf.sendCommand(IthoLeave);
					addLog(LOG_LEVEL_INFO, F("Sent command for 'leave' to Itho unit"));
					printWebString += F("Sent command for 'leave' to Itho unit");
					success = true;
				}

				if (param1.equalsIgnoreCase(F("low")))
				{
					PLUGIN_145_rf.sendCommand(IthoLow);
					addLog(LOG_LEVEL_INFO, F("Sent command for 'low speed' to Itho unit"));
					printWebString += F("Sent command for 'low speed' to Itho unit");
          PLUGIN_145_State=1;
          PLUGIN_145_Timer=0;
					success = true;
				}

				if (param1.equalsIgnoreCase(F("medium")))
				{
					PLUGIN_145_rf.sendCommand(IthoMedium);
					addLog(LOG_LEVEL_INFO, F("Sent command for 'medium speed' to Itho unit"));
					printWebString += F("Sent command for 'medium speed' to Itho unit");
          PLUGIN_145_State=2;
          PLUGIN_145_Timer=0;
					success = true;
				}

				if (param1.equalsIgnoreCase(F("high")))
				{
					PLUGIN_145_rf.sendCommand(IthoHigh);
					addLog(LOG_LEVEL_INFO, F("Sent command for 'full speed' to Itho unit"));
					printWebString += F("Sent command for 'full speed' to Itho unit");
          PLUGIN_145_State=3;
          PLUGIN_145_Timer=0;
					success = true;
				}

				if (param1.equalsIgnoreCase(F("timer1")))
				{
					PLUGIN_145_rf.sendCommand(IthoTimer1);
					addLog(LOG_LEVEL_INFO, F("Sent command for 'timer 1' to Itho unit"));
					printWebString += F("Sent command for 'timer 1' to Itho unit");
          PLUGIN_145_State=10;
          PLUGIN_145_Timer=PLUGIN_145_Time1;
					success = true;
				}				

				if (param1.equalsIgnoreCase(F("timer2")))
				{
					PLUGIN_145_rf.sendCommand(IthoTimer2);
					addLog(LOG_LEVEL_INFO, F("Sent command for 'timer 2' to Itho unit"));
					printWebString += F("Sent command for 'timer 2' to Itho unit");
          PLUGIN_145_State=20;
          PLUGIN_145_Timer=PLUGIN_145_Time2;
					success = true;
				}		

				if (param1.equalsIgnoreCase(F("timer3")))
				{
					PLUGIN_145_rf.sendCommand(IthoTimer3);
					addLog(LOG_LEVEL_INFO, F("Sent command for 'timer 3' to Itho unit"));
					printWebString += F("Sent command for 'timer 3' to Itho unit");
          PLUGIN_145_State=30;
					PLUGIN_145_Timer=PLUGIN_145_Time3;
					success = true;
				}	
			} 
      break; }
	}
return success;
} 


void PLUGIN_145_ITHOinterrupt() {PLUGIN_145_ITHOticker.once_ms(10, PLUGIN_145_ITHOcheck);}

void PLUGIN_145_ITHOcheck()
  {Serial.print("RF signal received");
  if (PLUGIN_145_rf.checkForNewPacket()) {
    IthoCommand cmd = PLUGIN_145_rf.getLastCommand();     
   String log = F("device-ID remote: "); 
   log+=PLUGIN_145_rf.getLastIDstr();
   log+=F(" ,Command received=" );
   switch (cmd) {
        case IthoUnknown:
          log+=F("unknown\n");
          break;
        case IthoLow:
          log+=F("low\n");
          PLUGIN_145_State=1;
          PLUGIN_145_Timer=0;
          break;
        case IthoMedium:
          log+=F("medium\n");
          PLUGIN_145_State=2;
          PLUGIN_145_Timer=0;
          break;
        case IthoHigh:
          log+=F("high\n");
          PLUGIN_145_State=3;
          PLUGIN_145_Timer=0;
          break;
        case IthoFull:
          log+=F("full\n");
          PLUGIN_145_State=4;
          PLUGIN_145_Timer=0;
          break;
        case IthoTimer1:
          log=+F("timer1\n");
          PLUGIN_145_State=10;
          PLUGIN_145_Timer=PLUGIN_145_Time1;
          break;
        case IthoTimer2:
          log+=F("timer2\n");
          PLUGIN_145_State=20;
          PLUGIN_145_Timer=PLUGIN_145_Time2;
          break;
        case IthoTimer3:
          log+=F("timer3\n");
          PLUGIN_145_State=30;
          PLUGIN_145_Timer=PLUGIN_145_Time3;
          break;
        case IthoJoin:
          log+=F("join\n");
          break;
        case IthoLeave:
          log+=F("leave\n");
          break;
      }
    addLog(LOG_LEVEL_DEBUG, log);
    }    
  }
  

  

