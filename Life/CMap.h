#include <Windows.h>
#include <windowsx.h>
#include <vector>
#include <fstream>
#include <string>
using std::vector;
using std::ifstream;
using std::string;
class CMap {
public:
	CMap(int w, int h) :
		width(w),
		height(h),
		field(height, vector<bool>(width, 0)),
		temp_field(field),
		paused(false),
		generation(0),
		cells_counter(0)
	{};
	void DrawGrid(HDC, RECT); 
	void DrawCells(HDC, RECT);
	void ReadPosition(ifstream&, int&, int&);
	int LoadPattern(string);
	void SetCell(RECT, int, int, bool);
	void Update();
	bool paused;
	int generation;
	int cells_counter;
private:
	int height;
	int width;
	vector<vector<bool>> field;
	vector<vector<bool>> temp_field;
};