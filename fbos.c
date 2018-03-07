#define SCREEN_W 80
#define SCREEN_H 25
#define VMEM_MAX SCREEN_W * SCREEN_H

unsigned short *VMEM  = (unsigned short *) 0xb8000;

struct {
	int x, y;
} CURSOR;

void clrscr(void)
{
	int i;

	for (i = 0; i < VMEM_MAX; i++) {
		VMEM[i] = 0;
	}
}

void scrollup(void)
{
	int x, y;

	for (x = 0; x < SCREEN_W; x++) {
		for (y = 0; y < SCREEN_H - 1; y++) {
			VMEM[x + y * SCREEN_W] = VMEM[x + (y + 1) * SCREEN_W];
		}
	}
	CURSOR.x = 0;
	CURSOR.y = SCREEN_H - 1;
	for (x = 0; x < SCREEN_W; x++) VMEM[x + CURSOR.y * SCREEN_W] = 0;
}

void writec(char c)
{
	if (CURSOR.x > SCREEN_W - 1) {
		CURSOR.x = 0;
		++CURSOR.y;
	}

	if (CURSOR.y > SCREEN_H - 1) {
		scrollup();
	}

	VMEM[CURSOR.x + CURSOR.y * SCREEN_W] = c | 7 << 8;

	++CURSOR.x;
}

void writes(char *s)
{
	while (*s) writec(*s++);
}

int revi(int i)
{
	int r = 0;

	while (i > 0) {
		r = r * 10 + i % 10;
		i /= 10;
	}

	return r;
}

void writei(int i)
{
	if (i < 0) {
		writec('-');
		i = -i;
	}

	i = revi(i);

	do {
		writec(i % 10 + '0');
		i /= 10;
	} while (i != 0);
}

void newln(void)
{
	while (CURSOR.x < SCREEN_W) {
		writec(0);
	}
}

void writesln(char *s)
{
	writes(s);
	newln();
}

void writeiln(int i)
{
	writei(i);
	newln();
}

void delay(void)
{
	int i;
	for (i = 0; i < 10000000; i++);
}

void fizzbuzz(void)
{
	int i;

	for (i = 1; i <= 100; i++) {
		if (i % 15 == 0) {
			writesln("Fizzbuzz");
		} else if (i % 3 == 0) {
			writesln("Fizz");
		} else if (i % 5 == 0) {
			writesln("Buzz");
		} else {
			writeiln(i);
		}
		delay();
	}
}

int fbosmain(void)
{
	CURSOR.x = 0;
	CURSOR.y = 0;

	clrscr();
	fizzbuzz();

	return 0;
}
