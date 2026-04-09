#include <graphics.h>
#include <string>
#include <vector>
#include <sstream>

using namespace std;

// Convert string → wstring
wstring toW(const string& s) {
    return wstring(s.begin(), s.end());
}

// Logic
double calc(double a, double b, char op) {
    if (op == '+') return a + b;
    if (op == '-') return a - b;
    if (op == '*') return a * b;
    if (op == '/') return (b != 0) ? a / b : 0;
    return 0;
}

// Draw button
void drawBtn(int x1, int y1, int x2, int y2, const wchar_t* t) {
    rectangle(x1, y1, x2, y2);
    outtextxy(x1 + 20, y1 + 15, t);
}

// Click check
bool click(int mx, int my, int x1, int y1, int x2, int y2) {
    return mx > x1 && mx<x2 && my>y1 && my < y2;
}

int main() {
    initgraph(420, 600);

    string display = "";
    vector<string> history;

    double num1 = 0;
    char op = 0;

    while (true) {
        cleardevice();

        // Display
        rectangle(50, 30, 370, 80);
        outtextxy(60, 50, toW(display).c_str());

        // Buttons
        const wchar_t* btn[4][4] = {
            {L"7",L"8",L"9",L"/"},
            {L"4",L"5",L"6",L"*"},
            {L"1",L"2",L"3",L"-"},
            {L"0",L"C",L"=",L"+"}
        };

        int x = 50, y = 120;

        for (int i = 0;i < 4;i++) {
            x = 50;
            for (int j = 0;j < 4;j++) {
                drawBtn(x, y, x + 70, y + 50, btn[i][j]);
                x += 80;
            }
            y += 70;
        }

        // History
        outtextxy(50, 420, L"History:");
        int hy = 450;

        for (int i = max(0, (int)history.size() - 5); i < history.size(); i++) {
            outtextxy(50, hy, toW(history[i]).c_str());
            hy += 20;
        }

        // Mouse
        MOUSEMSG m = GetMouseMsg();

        if (m.uMsg == WM_LBUTTONDOWN) {
            x = 50; y = 120;

            for (int i = 0;i < 4;i++) {
                x = 50;
                for (int j = 0;j < 4;j++) {

                    if (click(m.x, m.y, x, y, x + 70, y + 50)) {
                        string b(1, (char)btn[i][j][0]);

                        if (b >= "0" && b <= "9") display += b;

                        else if (b == "C") {
                            display = "";
                            op = 0;
                        }

                        else if (b == "+" || b == "-" || b == "*" || b == "/") {
                            if (!display.empty()) {
                                num1 = atof(display.c_str());
                                op = b[0];
                                display = "";
                            }
                        }

                        else if (b == "=") {
                            if (!display.empty() && op != 0) {
                                double num2 = atof(display.c_str());
                                double r = calc(num1, num2, op);

                                stringstream ss;
                                ss << num1 << " " << op << " " << num2 << " = " << r;
                                history.push_back(ss.str());

                                display = to_string(r);
                                op = 0;
                            }
                        }
                    }

                    x += 80;
                }
                y += 70;
            }
        }
    }

    closegraph();
    return 0;
}