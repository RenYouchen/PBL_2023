#include <Pixy2.h>
#include <SPI.h>

Pixy2 pixy;

void initPixy() {
	pixy.init();
}

int getBlocks() {
	pixy.ccc.getBlocks();
	if (pixy.ccc.numBlocks) {
		return 1;
	} else {
		return 0;
	}
}

int xDiff() {
	getBlocks();
	int x = pixy.ccc.blocks[0].m_x;
	return x - 160;
}

int averageX() {
	int sum = 0;
	for (int i = 0; i < 2; i++) {
		sum += pixy.ccc.blocks[0].m_x;
	}
	return sum / 2;
}
