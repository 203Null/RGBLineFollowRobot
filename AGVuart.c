#pragma config(Sensor, in1, M, sensorLineFollower)
#pragma config(Sensor, in2, L, sensorLineFollower)
#pragma config(Sensor, in3, R, sensorLineFollower)
#pragma config(Sensor, dgtl1, Sonar, sensorSONAR_mm)
#pragma config(Sensor, dgtl3, Bump, sensorTouch)
#pragma config(Sensor, dgtl6, Rl, sensorLEDtoVCC)
#pragma config(Sensor, dgtl7, Ml, sensorLEDtoVCC)
#pragma config(Sensor, dgtl8, Ll, sensorLEDtoVCC)
#pragma config(Sensor, dgtl12, LED, sensorLEDtoVCC)
#pragma config(Motor, port1, Rm, tmotorVex393_HBridge, openLoop, reversed, driveRight)
#pragma config(Motor, port10, Lm, tmotorVex393_HBridge, openLoop, driveLeft)
#pragma config(UART_Usage, UART1, uartUserControl, baudRate9600, IOPins, None, None)
//*!!Code automatically generated by 'ROBOTC' configuration wizard							 !!*//

int LFthreshold = 1800;
int timer = 0;
float speed = 1;
bool stopcomplete = false;
bool lastheading = false; // f for L | t for R
bool lasthdelay = false;
char lastchar;

bool Lv;
bool Mv;
bool Rv;

bool ReadL();
bool ReadM();
bool ReadR();
void updateSensor();
void stopAll();
void startAll();
void startAll(int spd);
void shiftR();
void shiftL();
void ledUpdate();
void ledUpdateUART();
void nonblockingdelay(int ms);
void pause();
void findline();

task main()
{
	turnLEDOff(LED);
	repeat(forever) //loop
	{
		updateSensor();
		ledUpdateUART();
		if (SensorValue[Bump] == 1) //Press to start
		{
			sendChar(uartOne, 'O');
			turnLEDOn(LED);
			nonblockingdelay(1000);
			sendChar(uartOne, 'F');
			startAll();
			while (SensorValue[Bump] == 0) // Press to exit loop
			{
				updateSensor();
				//Event Handle
				if (Mv && !Lv && !Rv) { //Only M
					startAll();
					if(lastchar != 'G'){
						sendChar(uartOne, 'G');
						lastchar = 'G'
					}
				}
				else if (Lv && Rv) //Both L&R
				{
					pause();
				}
				else if (Lv) //Left
				{
					shiftL();
				}
				else if (Rv) //Right
				{
					shiftR();
				}

				if (!Lv && !Rv && !Mv) //No line
				{
					if (timer == 0)
					{
						timer = time1[T1];
					}
					else if (timer + 2000 < time1(T1))
					{
						findline();
					}
				}
					else
					{
						timer = 0;

					}
					/*
					timer = 0;
					if(lastheading)
					{
					shiftL();
					}
					else
					{
					shiftR();
					}
					}
					}
					*/
				}
				sendChar(uartOne, '0');
				stopcomplete = false;
				stopAll();
				nonblockingdelay(500);
				turnLEDOff(LED);
			}
		}
	}



	bool ReadL()
	{
		return SensorValue[L] > LFthreshold;
	}

	bool ReadM()
	{
		return SensorValue[M] > LFthreshold;
	}

	bool ReadR()
	{
		return SensorValue[R] > LFthreshold;
	}

	void stopAll()
	{
		sendChar(uartOne, 'F');
		stopMotor(Rm);
		stopMotor(Lm);
	}

	void startAll()
	{
		startMotor(Rm, 127 * speed);
		startMotor(Lm, 127 * speed);
	}

	void startAll(int spd)
	{
		startMotor(Rm, spd);
		startMotor(Lm, spd);
	}

	void shiftR()
	{
		if(lastchar != '>'){
			sendChar(uartOne, '>');
			lastchar = '>'
		}
		lasthdelay = true;
		if(lasthdelay)
			lastheading = true;
		startMotor(Rm, -127);
		while (!Mv) {
			updateSensor();
			if (Lv && Rv) {
				pause();
				return;
			}
		}
		if (Lv && Rv) {
			pause();
			return;
		}
		startMotor(Rm, 127 * speed);
		sendChar(uartOne, 'F');
	}

	void shiftL()
	{
		if(lastchar != '<'){
			sendChar(uartOne, '<');
			lastchar = '<'
		}
		lasthdelay = false;
		if(!lasthdelay)
			lastheading = false;
		startMotor(Lm, -127);
		while (!Mv) {
			updateSensor();
			if (Lv && Rv) {
				pause();
				return;
			}
		}
		if (Lv && Rv) {
			pause();
			return;
		}
		sendChar(uartOne, 'F');
		startMotor(Lm, 127 * speed);
	}

	void ledUpdate()
	{
		if (Lv) {
			turnLEDOn(Ll);
		}
		else {
			turnLEDOff(Ll);
		}

		if (Mv) {
			turnLEDOn(Ml);
		}
		else {
			turnLEDOff(Ml);
		}

		if (Rv) {
			turnLEDOn(Rl);
		}
		else {
			turnLEDOff(Rl);
		}
	}

	void ledUpdateUART()
	{
		if (Lv) {
			turnLEDOn(Ll);
			sendChar(uartOne, 'L');
		}
		else {
			turnLEDOff(Ll);
			sendChar(uartOne, 'l');
		}

		if (Mv) {
			turnLEDOn(Ml);
			sendChar(uartOne, 'M');
		}
		else {
			turnLEDOff(Ml);
			sendChar(uartOne, 'm');
		}

		if (Rv) {
			turnLEDOn(Rl);
			sendChar(uartOne, 'R');
		}
		else {
			turnLEDOff(Rl);
			sendChar(uartOne, 'r');
		}
	}

	void nonblockingdelay(int ms)
	{
		int delaytimer = time1[T1];
		while (delaytimer + ms >= time1[T1]) {
			ledUpdate();
		}
		return;
	}

	void pause()
	{
		sendChar(uartOne, 'P');
		stopAll();
		nonblockingdelay(6000);
		startAll();
		nonblockingdelay(200);
		stopAll();
		sendChar(uartOne, 'F');
		findline();
		return;
	}

	void updateSensor()
	{
		Lv = ReadL();
		Mv = ReadM();
		Rv = ReadR();
		ledUpdate();
	}

	void findline()
	{
		stopAll();
		int scantime = 40;

		if (Mv && !Rv && !Lv || !Rv && Lv || Rv && !Lv){
			return;
		}

		if (lastheading) {
			sendChar(uartOne, '>');
			for (int R = 0; R < scantime; R++) {
				startMotor(Lm, 100);
				startMotor(Rm, -100);
				delay(20);
				//stopAll();
				updateSensor();
				if (Mv) {
					stopAll();
					return;
				}
				//delay(150);
			}
			sendChar(uartOne, '<');
			for (int L = 0; L < scantime * 2; L++) {
				startMotor(Rm, 100);
				startMotor(Lm, -100);
				delay(20);
				//stopAll();
				updateSensor();
				if (Mv) {
					stopAll();
					return;
				}
				//delay(150);
			}
		}
		else
		{
			sendChar(uartOne, '<');
			for (int L = 0; L < scantime; L++) {
				startMotor(Rm, 100);
				startMotor(Lm, -100);
				delay(20);
				//stopAll();
				updateSensor();
				if (Mv) {
					stopAll();
					return;
				}
				//delay(150);
			}
			sendChar(uartOne, '>');
			for (int R = 0; R < scantime * 2; R++) {
				startMotor(Lm, 100);
				startMotor(Rm, -100);
				delay(20);
				//stopAll();
				updateSensor();
				if (Mv) {
					stopAll();
					return;
				}
				//delay(150);
			}
		}
	}
