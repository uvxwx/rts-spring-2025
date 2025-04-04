volatile unsigned long long prev_time = 0;
volatile unsigned long long seg_sum = 0;
volatile unsigned long long seg_square_sum = 0;
volatile unsigned long long seg_cnt = 0;
const unsigned long long MAX_SEG_CNT = 1000;

void setup()
{
	Serial.begin(9600);
	pinMode(3, INPUT);
	attachInterrupt(digitalPinToInterrupt(3), handle_seg, HIGH);
}

void handle_seg()
{
	unsigned long long cur = micros();
	unsigned long long seg = cur - prev_time;
	
	seg_sum += seg;
	seg_square_sum += seg * seg;
	seg_cnt++;
	
	prev_time = cur;
}

void loop()
{
	if (seg_cnt >= MAX_SEG_CNT) {
		noInterrupts();
		
		float average = (float) seg_sum / seg_cnt;
		float stddev = sqrt(((float) seg_square_sum / seg_cnt) - (average * average));
		
		Serial.print("Average: ");
		Serial.print(average);
		Serial.print(" us, stddev: ");
		Serial.print(stddev);
		Serial.println(" us");
		seg_sum = 0;
		seg_square_sum = 0;
		seg_cnt = 0;
		interrupts();
	}
}
