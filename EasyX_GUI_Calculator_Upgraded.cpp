#include <graphics.h>
#include <conio.h>
#include <windows.h>
#include <string>
#include <vector>
#include <sstream>
#include <iomanip>
#include <cmath>
#include <algorithm>

using namespace std;

// -------------------- Helpers --------------------
wstring toW(const string& s) {
    return wstring(s.begin(), s.end());
}

string trimNumber(double value) {
    if (isnan(value) || isinf(value)) return "Error";

    stringstream ss;
    ss << fixed << setprecision(8) << value;
    string s = ss.str();

    while (!s.empty() && s.back() == '0') s.pop_back();
    if (!s.empty() && s.back() == '.') s.pop_back();
    if (s == "-0") s = "0";
    return s;
}

double calculate(double a, double b, char op, bool& ok) {
    ok = true;

    switch (op) {
    case '+': return a + b;
    case '-': return a - b;
    case '*': return a * b;
    case '/':
        if (fabs(b) < 1e-12) {
            ok = false;
            return 0;
        }
        return a / b;
    default:
        ok = false;
        return 0;
    }
}

bool inside(int mx, int my, int x1, int y1, int x2, int y2) {
    return mx >= x1 && mx <= x2 && my >= y1 && my <= y2;
}

struct Button {
    int x1, y1, x2, y2;
    string text;
};

// -------------------- Drawing --------------------
void drawButton(const Button& b, bool hover, bool pressed) {
    COLORREF fillColor = hover ? RGB(215, 230, 255) : RGB(245, 247, 250);
    COLORREF borderColor = hover ? RGB(40, 110, 220) : RGB(160, 170, 180);

    if (pressed) fillColor = RGB(180, 210, 255);

    setfillcolor(fillColor);
    setlinecolor(borderColor);
    solidroundrect(b.x1, b.y1, b.x2, b.y2, 12, 12);

    setbkmode(TRANSPARENT);
    settextcolor(RGB(30, 35, 45));
    settextstyle(24, 0, _T("Consolas"));

    wstring wt = toW(b.text);
    int tw = textwidth(wt.c_str());
    int th = textheight(wt.c_str());
    outtextxy((b.x1 + b.x2 - tw) / 2, (b.y1 + b.y2 - th) / 2, wt.c_str());
}

void drawDisplay(const string& expression, const string& display, const string& message) {
    setfillcolor(RGB(255, 255, 255));
    setlinecolor(RGB(130, 145, 165));
    solidroundrect(35, 25, 445, 115, 16, 16);

    setbkmode(TRANSPARENT);

    settextstyle(16, 0, _T("Consolas"));
    settextcolor(RGB(100, 105, 115));
    outtextxy(50, 38, toW(expression).c_str());

    settextstyle(30, 0, _T("Consolas"));
    settextcolor(message == "Error" ? RGB(210, 50, 50) : RGB(20, 25, 35));

    string shown = message.empty() ? display : message;
    if (shown.empty()) shown = "0";
    if (shown.length() > 18) shown = shown.substr(shown.length() - 18);

    wstring ws = toW(shown);
    int tw = textwidth(ws.c_str());
    outtextxy(430 - tw, 70, ws.c_str());
}

void drawHistory(const vector<string>& history) {
    settextstyle(17, 0, _T("Consolas"));
    settextcolor(RGB(60, 70, 80));
    outtextxy(35, 475, L"History (latest 5):");

    int y = 505;
    int start = max(0, (int)history.size() - 5);
    for (int i = start; i < (int)history.size(); ++i) {
        string item = history[i];
        if (item.length() > 38) item = item.substr(0, 38);
        outtextxy(35, y, toW(item).c_str());
        y += 23;
    }
}

// -------------------- App Logic --------------------
int main() {
    initgraph(480, 640);
    setbkcolor(RGB(235, 239, 245));

    vector<Button> buttons;
    vector<vector<string>> layout = {
        {"C", "DEL", "+/-", "/"},
        {"7", "8", "9", "*"},
        {"4", "5", "6", "-"},
        {"1", "2", "3", "+"},
        {"0", ".", "=", "Exit"}
    };

    int startX = 35, startY = 145;
    int w = 95, h = 55, gap = 10;

    for (int i = 0; i < (int)layout.size(); ++i) {
        for (int j = 0; j < (int)layout[i].size(); ++j) {
            int x1 = startX + j * (w + gap);
            int y1 = startY + i * (h + gap);
            buttons.push_back({ x1, y1, x1 + w, y1 + h, layout[i][j] });
        }
    }

    string display = "";
    string expression = "";
    string message = "";
    vector<string> history;

    double num1 = 0;
    char op = 0;
    bool newNumber = false;
    bool running = true;

    auto clearAll = [&]() {
        display.clear();
        expression.clear();
        message.clear();
        num1 = 0;
        op = 0;
        newNumber = false;
    };

    auto pressButton = [&](const string& key) {
        message.clear();

        if (key == "Exit") {
            running = false;
            return;
        }

        if (key == "C") {
            clearAll();
            return;
        }

        if (key == "DEL") {
            if (!display.empty() && !newNumber) display.pop_back();
            return;
        }

        if (key == "+/-") {
            if (!display.empty() && display != "0") {
                if (display[0] == '-') display.erase(display.begin());
                else display = "-" + display;
            }
            return;
        }

        if ((key >= "0" && key <= "9") || key == ".") {
            if (newNumber) {
                display.clear();
                newNumber = false;
            }
            if (key == "." && display.find('.') != string::npos) return;
            if (key == "." && display.empty()) display = "0";
            if (display == "0" && key != ".") display = key;
            else display += key;
            return;
        }

        if (key == "+" || key == "-" || key == "*" || key == "/") {
            if (!display.empty()) {
                num1 = atof(display.c_str());
                op = key[0];
                expression = trimNumber(num1) + " " + op;
                newNumber = true;
            }
            return;
        }

        if (key == "=") {
            if (!display.empty() && op != 0) {
                double num2 = atof(display.c_str());
                bool ok = true;
                double result = calculate(num1, num2, op, ok);

                if (!ok) {
                    message = "Error";
                    display.clear();
                    expression.clear();
                    op = 0;
                    newNumber = true;
                    return;
                }

                string resultText = trimNumber(result);
                string record = trimNumber(num1) + " " + op + " " + trimNumber(num2) + " = " + resultText;
                history.push_back(record);

                display = resultText;
                expression = record;
                op = 0;
                newNumber = true;
            }
        }
    };

    BeginBatchDraw();

    while (running) {
        ExMessage msg;
        int mx = -1, my = -1;
        bool hasMouse = peekmessage(&msg, EX_MOUSE | EX_KEY);

        if (hasMouse) {
            if (msg.message == WM_MOUSEMOVE || msg.message == WM_LBUTTONDOWN) {
                mx = msg.x;
                my = msg.y;
            }

            if (msg.message == WM_LBUTTONDOWN) {
                for (const auto& b : buttons) {
                    if (inside(msg.x, msg.y, b.x1, b.y1, b.x2, b.y2)) {
                        drawButton(b, true, true);
                        FlushBatchDraw();
                        Sleep(70);
                        pressButton(b.text);
                    }
                }
            }

            if (msg.message == WM_KEYDOWN) {
                char ch = (char)msg.vkcode;
                if (ch >= '0' && ch <= '9') pressButton(string(1, ch));
                else if (ch == VK_ESCAPE) pressButton("Exit");
                else if (ch == VK_BACK) pressButton("DEL");
                else if (ch == VK_RETURN) pressButton("=");
                else if (ch == VK_DELETE) pressButton("C");
                else if (ch == '+' || ch == '-' || ch == '*' || ch == '/' || ch == '.') pressButton(string(1, ch));
            }
        }

        cleardevice();

        settextstyle(22, 0, _T("Consolas"));
        settextcolor(RGB(40, 45, 55));
        outtextxy(35, 5, L"EasyX GUI Calculator - Upgraded");

        drawDisplay(expression, display, message);

        POINT p;
        GetCursorPos(&p);
        ScreenToClient(GetHWnd(), &p);
        mx = p.x;
        my = p.y;

        for (const auto& b : buttons) {
            bool hover = inside(mx, my, b.x1, b.y1, b.x2, b.y2);
            drawButton(b, hover, false);
        }

        drawHistory(history);

        settextstyle(14, 0, _T("Consolas"));
        settextcolor(RGB(95, 105, 115));
        outtextxy(35, 615, L"Keyboard: 0-9  + - * /  .  Enter  Backspace  Delete  Esc");

        FlushBatchDraw();
        Sleep(10);
    }

    EndBatchDraw();
    closegraph();
    return 0;
}
