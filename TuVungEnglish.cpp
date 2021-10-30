#pragma warning(disable : 4996)

#include <iostream>
#include <io.h> //_setmode()
#include <fcntl.h> ///_O_WTEXT
#include <fstream>
#include <string> //getline
#include <locale>
#include <codecvt>
#include <cstdlib> // rand
#include <ctime> // time
#include <conio.h> // getch
#include <Windows.h> //Sleep
#include <iomanip>

using namespace std;

void SetWindowSize(SHORT width, SHORT height); // kích thước màn hình
void DisableResizeWindow();	// ngăn không cho thay đổi kích thước
void DisableCtrButton(bool Close, bool Min, bool Max); // tắt các nút điều khiển (tắt, phóng to, thu nhỏ)
void ShowScrollbar(BOOL Show); // Tắt thanh cuộn

void gotoxy(int x, int y);
void veVien();
void keNgang();
wstring getRandomLine();
wstring getEnglish(wstring line);
wstring getVietnamse(wstring line);

void inCauHoi(wstring dapAnCauHoi, wstring dapAn[]);
void intro1();
void mainMenu();
void playWindows();
void inHighScore(int);
void nameSoftware();
void deleteLine(int);
void huongDan();
void thongTin();

int score = 0;

int main()
{
	// Thiết lập cửa sổ hiển thị
	system("color f0"); // nền trắng chữ đen
	SetWindowSize(101, 30); // kích thước màn hình
	DisableResizeWindow(); // ngắt chỉnh độ rộng window
	DisableCtrButton(0, 1, 1);
	ShowScrollbar(0);


	// Thiết lập ban đầu
	_setmode(_fileno(stdout), _O_WTEXT); //needed for output tiếng việt
	_setmode(_fileno(stdin), _O_WTEXT); //needed for input tiếng việt
	srand(time(NULL)); // hạt giống random

	// Run
	intro1();
	nameSoftware();
	mainMenu();

	return 0;
}

wstring getRandomLine()
{
	// Lấy một số ngẫu nhiên
	int num_line = rand() % 1496 - 0 + 1;
	wstring line;

	// File đầu vào
	locale loc(std::locale(), new std::codecvt_utf8<wchar_t>);
	wifstream file(L"lib.canh");
	file.imbue(loc);

	// Gán cho line 1 chuỗi ở dòng thứ num_line
	for (int i = 0; i < num_line; i++)
	{
		getline(file, line);
	}

	file.close();
	return line;
}

wstring getEnglish(wstring line)
{
	wstring english;

	for (int i = 0; i < line.find(':') ; i++)
		english += line[i];

	// viết hoa đầu dòng
	if (english[0] >= 'a' && english[0] <= 'z') english[0] -= 32;
	return english;
}

wstring getVietnamse(wstring line)
{
	wstring vietnamse;

	for (int i = line.find(':') + 2; i < line.length(); i++)
		vietnamse += line[i];
	// viết hoa đầu dòng, nhưng chưa bao gồm tiếng việt
	if (vietnamse[0] >= 'a' && vietnamse[0] <= 'z') vietnamse[0] -= 32;
	return vietnamse;
}

void keNgang()
{
	wcout << L"---------------------------------------------------------------------------------------------------";
}

void playWindows()
{
	system("cls");
	// Reset score
	score = 0;
	float total_time = 0;

	// sinh mạng
	bool life = true;

	// Lặp
	while (life == true)
	{
		veVien();

		// khởi tạo đáp án
		wstring dapAn[4];
		for (int i = 0; i < 4; i++)
		{
			dapAn[i] = getRandomLine();
		}

		// chọn ngẫu nhiên một đáp án để hỏi
		wstring dapAnCauHoi = dapAn[rand() % 4];

		// in giao diện câu hỏi
		inCauHoi(dapAnCauHoi, dapAn);

		// Nhận đáp án từ người dùng
		gotoxy(40, 16); wcout << L"Đáp án của bạn là :";
		char reply = 'o';		
		char key;
		int run = 1;
		// time
		gotoxy(79, 1); wcout << L"Thời gian : ";
		int time = 5; gotoxy(91, 1);  wcout << time;

		// không nhập sẽ thua
		life = false;

		// nhận đáp án từ người dùng mỗi 0.2s, mỗi một giây in lại time
		while (time > 0)
		{
			Sleep(100);
			total_time += 0.1;
			if (kbhit())
			{
				reply = _getch();
				life = true;
				break;
			}
			run++;
			if (run % 10 == 0)
			{
				time--;
				gotoxy(91, 1);  wcout << time;
			}
		}
		

		// Xu li ket qua
		int indexReply;
		gotoxy(60, 16);
		switch (reply)
		{
		case '1':
		case 'a':
		case 'A':
			indexReply = 0;
			wcout << L"A";
			break;
		case '2':
		case 'b':
		case 'B':
			indexReply = 1;
			wcout << L"B";
			break;
		case '3':
		case 'c':
		case 'C':
			indexReply = 2;
			wcout << L"C";
			break;
		case '4':
		case 'd':
		case 'D':
			indexReply = 3;
			wcout << L"D";
			break;
		default:
			deleteLine(16);
			if (reply == 'o') { gotoxy(46, 16); wcout << L"Hết giờ!!"; }
			else { gotoxy(6, 16); wcout << reply << L" là đáp án nào ?! Cho game over luôn cho biết ha"; }
			indexReply = 0;
			break;
		}
		gotoxy(2, 18); keNgang();

		// kiểm tra đúng sai
		if (dapAn[indexReply] == dapAnCauHoi) {
			gotoxy(28, 20); wcout << L"Bạn đã trả lời đúng, bạn được cộng 100 điểm !!\a";
			gotoxy(2, 22); keNgang();
			gotoxy(17 + score / 1000, 1); wcout << L"+ 100";
			score += 100;
			Sleep(550);
			system("cls");
		}
		else 
		{ // end game
			wstring dap_an = L"Đáp án đúng phải là \"" + getVietnamse(dapAnCauHoi) + L"\"";
			gotoxy(50 - dap_an.length()/2, 20); wcout << dap_an;
			gotoxy(2, 22); keNgang();

			if (score / 100 >= 5)
			{
				gotoxy(27, 24); wcout << L"Chúc mừng bạn đã trả lời đúng " << score / 100 << L" câu liên tiếp !!!";
			}
			bool inTop = false;
			life = false;
			if (score != 0)
			{
				struct NameAndScore
				{
					wstring name;
					long long score;
					float time;
				};

				locale loc(std::locale(), new std::codecvt_utf8<wchar_t>);
				wifstream high_score(L"hs.canh");
				high_score.imbue(loc);

				NameAndScore Top[10];
				for (int i = 0; i < 10; i++)
				{
					high_score >> Top[i].name >> Top[i].score >> Top[i].time;
				}
				high_score.close();

				for (int i = 0; i < 10; i++)
				{
					bool check = false;
					if (score > Top[i].score) check = true;
					if (score == Top[i].score && total_time < Top[i].time) check = true;

					// check xem có lọt top không
					if (check)
					{
						inTop = true;

						// đẩy bảng xếp hạng xuống để lấy chỗ
						for (int j = 9; j > i; j--)
						{
							Top[j] = Top[j - 1];
						}


						// in ra thông báo để lấy thông tin người chơi
						deleteLine(24);
						gotoxy(35, 24); wcout << L"Bạn vừa đánh bại kỷ lục của " << Top[i].name << " !!";
						gotoxy(35, 26); wcout << L"Xin hãy cho biết quý danh : ";
						wstring player_name;
						wcin >> player_name;


						Top[i].name = player_name;
						Top[i].score = score;
						Top[i].time = total_time;

						// in ra file high_score
						wofstream fout(L"hs.canh");
						fout.imbue(loc);
						for (int i = 0; i < 10; i++)
						{
							fout << Top[i].name << " " << Top[i].score << " " << setprecision(2) << Top[i].time << endl;
						}

						fout.close();
						inHighScore(i);
						break;
					}
				}
			
			}
			if (!inTop) {
				gotoxy(45, 25); wcout << L"GAME OVER";
				gotoxy(35, 26); wcout << L"Bạn có muốn thử lại không? (Y/N)";

				reply = _getch();
				wcout << L"\a";
				if (reply == 'Y' or reply == 'y' or reply == 13) playWindows();
				else mainMenu();
			}
				
		}
	}
}

void inCauHoi(wstring dapAnCauHoi, wstring dapAn[])
{

	// in câu hỏi tiếng anh
	wstring english = getEnglish(dapAnCauHoi);
	gotoxy(6, 1); wcout << L"Score: " << score;
	gotoxy(2, 2); keNgang();
	gotoxy(51 - english.length() / 2, 4); wcout << english;
	gotoxy(2, 6); keNgang();

	// in 4 đáp án
	wstring vietnam;
	for (int i = 0, y = 8; i < 4; i += 2, y += 4)
	{
		vietnam = getVietnamse(dapAn[i]);
		gotoxy(29 - vietnam.length() / 2, y); wcout << vietnam;

		vietnam = getVietnamse(dapAn[i + 1]);
		gotoxy(74 - vietnam.length() / 2, y); wcout << vietnam;
	}
	// A
	gotoxy(3, 7); wcout << "  __";
	gotoxy(3, 8); wcout << " / /\\";
	gotoxy(3, 9); wcout << "/_/--\\";
	// B
	gotoxy(95, 7); wcout << " ___";
	gotoxy(95, 8); wcout << "| |_)";
	gotoxy(95, 9); wcout << "|_|_)";
	// C
	gotoxy(3, 11); wcout << " ___";
	gotoxy(3, 12); wcout << "/ /``";
	gotoxy(3, 13); wcout << "\\_\\_,";
	// D
	gotoxy(95, 11); wcout << " ___";
	gotoxy(95, 12); wcout << "| | \\";
	gotoxy(95, 13); wcout << "|_|_/";


	// kẻ đường cho đẹp
	gotoxy(2, 10); keNgang();
	for (int i = 7; i < 14; i++)
	{
		gotoxy(50, i); wcout << "|";
	}
	gotoxy(2, 14); keNgang();
}

void gotoxy(int x, int y)
{
	static HANDLE h = NULL;
	if (!h)
		h = GetStdHandle(STD_OUTPUT_HANDLE);
	COORD c = { x, y };
	SetConsoleCursorPosition(h, c);
}

void SetWindowSize(SHORT width, SHORT height)
{
	HANDLE hStdout = GetStdHandle(STD_OUTPUT_HANDLE);

	SMALL_RECT WindowSize;
	WindowSize.Top = 0;
	WindowSize.Left = 0;
	WindowSize.Right = width;
	WindowSize.Bottom = height;

	SetConsoleWindowInfo(hStdout, 1, &WindowSize);
}

void veVien()
{
	for (int i = 0; i < 30; i++)
	{
		gotoxy(1, i);
		wcout << L"|";
		gotoxy(101, i);
		wcout << L"|";
	}
	gotoxy(2, 0);
	for (int i = 0; i < 99; i++)
	{
		wcout << "-";
	}
	gotoxy(2, 29);
	for (int i = 0; i < 99; i++)
	{
		wcout << "-";
	}

}

void DisableResizeWindow()
{
	HWND hWnd = GetConsoleWindow();
	SetWindowLong(hWnd, GWL_STYLE, GetWindowLong(hWnd, GWL_STYLE) & ~WS_SIZEBOX);
}

void DisableCtrButton(bool Close, bool Min, bool Max)
{
	HWND hWnd = GetConsoleWindow();
	HMENU hMenu = GetSystemMenu(hWnd, false);

	if (Close == 1)
	{
		DeleteMenu(hMenu, SC_CLOSE, MF_BYCOMMAND);
	}
	if (Min == 1)
	{
		DeleteMenu(hMenu, SC_MINIMIZE, MF_BYCOMMAND);
	}
	if (Max == 1)
	{
		DeleteMenu(hMenu, SC_MAXIMIZE, MF_BYCOMMAND);
	}
}

void ShowScrollbar(BOOL Show)
{
	HWND hWnd = GetConsoleWindow();
	ShowScrollBar(hWnd, SB_BOTH, Show);
}

void mainMenu()
{
	system("cls");
	veVien();

	// doc file
	locale loc(std::locale(), new std::codecvt_utf8<wchar_t>);
	wifstream menu(L"mn.canh");
	wstring line;
	menu.imbue(loc);
	int num_line = 2;
	while (getline(menu, line))
	{
		gotoxy(30, num_line);  wcout << line;
		num_line++;
	}
	menu.close();

	gotoxy(2, 14); keNgang();
	gotoxy(45, 15);  wcout << L"1 - Bắt đầu";
	gotoxy(2, 16); keNgang();
	gotoxy(45, 17); wcout << L"2 - Điểm cao";
	gotoxy(2, 18); keNgang();
	gotoxy(45, 19); wcout  << L"3 - Hướng dẫn";
	gotoxy(2, 20); keNgang();
	gotoxy(45, 21); wcout << L"4 - Tác giả";
	gotoxy(2, 22); keNgang();
	gotoxy(45, 23); wcout << L"5 - Thoát";
	gotoxy(2, 24); keNgang();
	char choose;
	choose = getch();
	wcout << "\a";
	system("cls");
	switch (choose)
	{
	case '1':
	case 13 :
		//demNguoc();
		playWindows();
		break;
	case '2':
		inHighScore(-1);
		break;
	case '3':
		huongDan();
		break;
	case '4':
		thongTin();
		break;
	case '5':
	case 27 : 
		exit;
		break;
	}
}

void inHighScore(int current = -1)
{
	system("cls");
	veVien();
	gotoxy(25, 1); wcout << L"  __    __       __ ";
	gotoxy(25, 2); wcout << L" |  |--|__.-----|  |--.  .-----.----.-----.----.-----.";
	gotoxy(25, 3); wcout << L" |     |  |  _  |     |  |__ --|  __|  _  |   _|  -__|";
	gotoxy(25, 4); wcout << L" |__|__|__|___  |__|__|  |_____|____|_____|__| |_____|";
	gotoxy(25, 5); wcout << L"          |_____|";

	struct NameAndScore
	{
		wstring name;
		long long score;
		float time;
	};


	locale loc(std::locale(), new std::codecvt_utf8<wchar_t>);
	wifstream highScore("hs.canh");

	highScore.imbue(loc);
	NameAndScore nameAndScore[10];

	

	for (int i = 0, j = 8; i < 10; i++, j += 2)
	{
		highScore >> nameAndScore[i].name >> nameAndScore[i].score >> nameAndScore[i].time;
		gotoxy(38, j); wcout << L"Top " << i + 1 << L"\t";
		gotoxy(48, j); wcout << nameAndScore[i].name << L" " << nameAndScore[i].score << L" (" << nameAndScore[i].time << L"s)";
	}
	highScore.close();

	if (current != -1)
	{
		while (true)
		{	
			int hang = current * 2 + 8;

			// nháy điểm hiện tại
			Sleep(400);
			deleteLine(hang);
			Sleep(400);
			gotoxy(38, hang); wcout << L"Top " << current + 1 << L"\t";
			gotoxy(48, hang); wcout << nameAndScore[current].name << L" " << nameAndScore[current].score << L" (" << nameAndScore[current].time << L"s)";


			// rời khỏi đó
			if (kbhit())
				break;
		}
	}
	getch();
	wcout << "\a";
	mainMenu();
	
}

void intro1()
{
	system("cls");
	veVien();

	// doc file
	locale loc(std::locale(), new std::codecvt_utf8<wchar_t>);
	wifstream intro(L"it.canh");
	wstring line;
	intro.imbue(loc);	
	int num_line = 12;
	while (getline(intro, line))
	{
		gotoxy(20, num_line);  wcout << line;
		num_line++;
	}
	intro.close();
	Sleep(800);
}

void nameSoftware()
{
	system("cls");
	veVien();
	gotoxy(2, 4); keNgang();
	gotoxy(2, 25); keNgang();
	// doc file
	locale loc(std::locale(), new std::codecvt_utf8<wchar_t>);
	wifstream name(L"nS.canh");
	wstring line;
	name.imbue(loc);
	int num_line = 6;
	int col = 8;
	while (getline(name, line))
	{
		if (num_line > 14) col = 30;
		gotoxy(col, num_line); wcout << line;
		num_line++;
	}
	name.close();
	Sleep(1200);
	system("cls");
}

void deleteLine(int line)
{
	gotoxy(2, line);
	for (int i = 1; i < 100; i++)
		wcout << L" ";
}

void huongDan()
{
	system("cls");
	veVien();
	// doc file
	locale loc(std::locale(), new std::codecvt_utf8<wchar_t>);
	wifstream huong_dan(L"hd.canh");
	wstring line;
	huong_dan.imbue(loc);

	int num_line = 2;
	while (getline(huong_dan, line))
	{
		gotoxy(4, num_line);
		wcout << line;
		num_line++;
	}
	huong_dan.close();
	_getch();
	mainMenu();
}

void thongTin()
{
	system("cls");
	veVien();
	// doc file
	locale loc(std::locale(), new std::codecvt_utf8<wchar_t>);
	wifstream infor(L"if.canh");
	wstring line;
	infor.imbue(loc);

	int num_line = 2;
	while (getline(infor, line))
	{
		gotoxy(4, num_line);
		wcout << line;
		num_line++;
	}
	infor.close();
	_getch();
	mainMenu();
}