#pragma once
#include "iostream"
#include "InterfaceOGL.h"

using namespace std;

int main() {
	InterfaceOGL objOpenGl;
	
	while (!objOpenGl.needCloseWindow()) {
		objOpenGl.run(0,0,0,1);
	}

	objOpenGl.terminate();
}