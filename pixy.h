#include <Pixy2.h>
#include <SPI.h>

Pixy2 pixy;

void initPixy() {
	pixy.init();
}

void getBlocks() {
	pixy.ccc.getBlocks();
}

int xDiff() {
	getBlocks();
	int x = pixy.ccc.blocks[0].m_x;
	return x - 160;
}
