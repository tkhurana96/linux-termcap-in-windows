#include "move_cursor.h"

bool move_cursor(HANDLE consoleHandle, int dx, int dy){
// TODO: Handle the case where cursor needs to be moved to previous or next lines given enough dx
CONSOLE_SCREEN_BUFFER_INFO bufferInfo;

if (!GetConsoleScreenBufferInfo(consoleHandle, &bufferInfo)) {
	// Error in fetching screen buffer info
	return false;
}

COORD dest = { bufferInfo.dwCursorPosition.X + dx,  bufferInfo.dwCursorPosition.Y + dy };
if (!SetConsoleCursorPosition(consoleHandle, dest)) {
	return false;
}
return true;
}
