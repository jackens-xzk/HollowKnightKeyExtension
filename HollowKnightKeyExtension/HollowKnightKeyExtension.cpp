#include <windows.h>
#include <stdio.h>

#define KeyDown(vk_code) ((GetAsyncKeyState(vk_code) & 0x8000) ? 1 : 0)
#define KeyUp(vk_code) ((GetAsyncKeyState(vk_code) & 0x8000) ? 0 : 1)

HWND hgWnd;
HHOOK myhook;
LPCWSTR infoText;
WCHAR hollowText[256] = {0};

const int delayTime = 15;

LRESULT CALLBACK KeyboardProc(int nCode, WPARAM wParam, LPARAM lParam) {
    // 只处理正常代码的按键事件
    if (nCode < 0) {
        return CallNextHookEx(myhook, nCode, wParam, lParam);
    }

#pragma region 判断程序是否为knight
    // 判断程序是否为Hollow Knight
    HWND hwnd = ::GetForegroundWindow();
    if (hwnd == nullptr) {
        return CallNextHookEx(myhook, nCode, wParam, lParam);
    }

    WCHAR windowTextBuff[256] = {0};
    GetWindowText(hwnd, windowTextBuff, 255);

    // 比较窗口标题
    if (wcscmp(windowTextBuff, hollowText) != 0) {
        return CallNextHookEx(myhook, nCode, wParam, lParam);
    }

#pragma endregion

    PKBDLLHOOKSTRUCT p = (PKBDLLHOOKSTRUCT)lParam;

    if (wParam == WM_KEYDOWN || wParam == WM_SYSKEYDOWN) {
        // 处理按键按下事件
        switch (p->vkCode) {
        case 'W': // Z 跳跃
            keybd_event('Z', 0, 0, 0);
            return 1;

        case 'S': // 下  下看、交互 
            keybd_event(VK_DOWN, 0, 0, 0);
            return 1;

        case 'A': // 左移动
            keybd_event(VK_LEFT, 0, 0, 0);
            return 1;

        case 'D': // 右移动
            keybd_event(VK_RIGHT, 0, 0, 0);
            return 1;

        case ' ': // 聚气回血、技能施法
            keybd_event('J', 0, 0, 0);
            return 1;

        case VK_NUMPAD7: // 上劈
            keybd_event(VK_UP, 0, 0, 0);
            keybd_event('X', 0, 0, 0);
            return 1;

        case VK_NUMPAD4: // 横批
            keybd_event('X', 0, 0, 0);
            return 1;

        case VK_NUMPAD1: // 下劈
        case VK_NUMPAD0: // 下劈
            keybd_event(VK_DOWN, 0, 0, 0);
            keybd_event('X', 0, 0, 0);
            return 1;

        case VK_NUMPAD8: // 上吼
            keybd_event(VK_UP, 0, 0, 0);
            keybd_event('U', 0, 0, 0);
            return 1;

        case VK_NUMPAD5: // 横波
            keybd_event('U', 0, 0, 0);
            return 1;

        case VK_NUMPAD2: // 下砸
            keybd_event(VK_DOWN, 0, 0, 0);
            keybd_event('U', 0, 0, 0);
            return 1;

        case VK_NUMPAD6: // 冲刺
            keybd_event('C', 0, 0, 0);
            return 1;

        case VK_NUMPAD9: // 小地图
        case VK_DECIMAL: // 小地图
            keybd_event(VK_TAB, 0, 0, 0);
            return 1;

        case VK_ADD: // 小键盘加号对应 超级冲刺
        case VK_RETURN: // 小键盘回车对应 超级冲刺
            keybd_event('K', 0, 0, 0);
            return 1;

        case VK_NEXT: // pageDown 对应 梦之钉 
            keybd_event('L', 0, 0, 0);
            return 1;

        case VK_END: // end对应 道具栏
        case VK_OEM_3: // `键对应 道具栏
            keybd_event('I', 0, 0, 0);
            return 1;

        case 'Q': // 向右移动时 向左回身斩
            keybd_event(VK_RIGHT, 0, KEYEVENTF_KEYUP, 0);
            keybd_event(VK_LEFT, 0, 0, 0);
            Sleep(delayTime); 
            keybd_event('X', 0, 0, 0);
            keybd_event('X', 0, KEYEVENTF_KEYUP, 0);
            keybd_event(VK_LEFT, 0, KEYEVENTF_KEYUP, 0);
            return 1;

        case 'E': // 向左移动时 向右回身斩
            keybd_event(VK_LEFT, 0, KEYEVENTF_KEYUP, 0);
            keybd_event(VK_RIGHT, 0, 0, 0);
            Sleep(delayTime);
            keybd_event('X', 0, 0, 0);
            keybd_event('X', 0, KEYEVENTF_KEYUP, 0);
            keybd_event(VK_RIGHT, 0, KEYEVENTF_KEYUP, 0);
            return 1;

        case VK_CAPITAL: // 向右移动时 向左回身冲刺 (Caps Lock)
            keybd_event(VK_RIGHT, 0, KEYEVENTF_KEYUP, 0);
            keybd_event(VK_LEFT, 0, 0, 0);
            Sleep(delayTime);
            keybd_event('C', 0, 0, 0);
            keybd_event('C', 0, KEYEVENTF_KEYUP, 0);
            keybd_event(VK_LEFT, 0, KEYEVENTF_KEYUP, 0);
            return 1;

        case 'F': // 向左移动时 向右回身冲刺
            keybd_event(VK_LEFT, 0, KEYEVENTF_KEYUP, 0);
            keybd_event(VK_RIGHT, 0, 0, 0);
            Sleep(delayTime);
            keybd_event('C', 0, 0, 0);
            keybd_event('C', 0, KEYEVENTF_KEYUP, 0);
            keybd_event(VK_RIGHT, 0, KEYEVENTF_KEYUP, 0);
            return 1;
        }
    }
    if (wParam == WM_KEYUP || wParam == WM_SYSKEYUP) {
        // 处理按键抬起事件
        switch (p->vkCode) {
        case 'W':
            keybd_event('Z', 0, KEYEVENTF_KEYUP, 0);
            return 1;

        case 'S':
            keybd_event(VK_DOWN, 0, KEYEVENTF_KEYUP, 0);
            return 1;

        case 'A':
            keybd_event(VK_LEFT, 0, KEYEVENTF_KEYUP, 0);
            return 1;

        case 'D':
            keybd_event(VK_RIGHT, 0, KEYEVENTF_KEYUP, 0);
            return 1;

        case ' ':
            keybd_event('J', 0, KEYEVENTF_KEYUP, 0);
            return 1;

        case VK_NUMPAD7:
            keybd_event(VK_UP, 0, KEYEVENTF_KEYUP, 0);
            keybd_event('X', 0, KEYEVENTF_KEYUP, 0);
            return 1;

        case VK_NUMPAD4:
            keybd_event('X', 0, KEYEVENTF_KEYUP, 0);
            return 1;

        case VK_NUMPAD1:
        case VK_NUMPAD0:
            keybd_event(VK_DOWN, 0, KEYEVENTF_KEYUP, 0);
            keybd_event('X', 0, KEYEVENTF_KEYUP, 0);
            return 1;

        case VK_NUMPAD8:
            keybd_event(VK_UP, 0, KEYEVENTF_KEYUP, 0);
            keybd_event('U', 0, KEYEVENTF_KEYUP, 0);
            return 1;

        case VK_NUMPAD5:
            keybd_event('U', 0, KEYEVENTF_KEYUP, 0);
            return 1;

        case VK_NUMPAD2:
            keybd_event(VK_DOWN, 0, KEYEVENTF_KEYUP, 0);
            keybd_event('U', 0, KEYEVENTF_KEYUP, 0);
            return 1;

        case VK_NUMPAD6:
            keybd_event('C', 0, KEYEVENTF_KEYUP, 0);
            return 1;

        case VK_NUMPAD9:
        case VK_DECIMAL:
            keybd_event(VK_TAB, 0, KEYEVENTF_KEYUP, 0);
            return 1;

        case VK_ADD:
        case VK_RETURN:
            keybd_event('K', 0, KEYEVENTF_KEYUP, 0);
            return 1;

        case VK_NEXT:
            keybd_event('L', 0, KEYEVENTF_KEYUP, 0);
            return 1;

        case VK_END:
        case VK_OEM_3:
            // 物品栏按键抬起不需要特殊处理
            return 1;
        }
    }

    return CallNextHookEx(myhook, nCode, wParam, lParam);
}

// 窗口过程处理
LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) {
    static HWND hgWnd;
    hgWnd = hwnd;

    HDC hdc;
    PAINTSTRUCT ps;
    RECT rect;

    static LPCWSTR infoText = nullptr;
    
    if (infoText == nullptr) {
        infoText = 
            L"\n"
            L"游戏外使用请先关闭本程序,会对打字等功能产生影响\n"
            L"\n"
            L"首次使用前请重置空洞骑士按键键位\n"
            L"并将ASDF键对应的操作依次改到JKLU键上\n"
            L"\n"
            L"AD左右移动\n"
            L"W跳跃(你没看错)\n"
            L"空格键 聚集/施法\n"
            L"(以下数字均代指小键盘数字键,开启本程序时请确保小键盘灯常量(数字模式))\n"
            L"4劈砍/剑技 7上劈 1下劈 0下劈\n"
            L"5快速施法/横波 8上吼 2下砸\n"
            L"6冲刺 3下冲(需佩戴冲刺大师)\n"
            L"+超冲 enter超冲\n"
            L"\n"
            L"9快速地图 *快速地图\n"
            L"end(方向键上面)物品栏 `(Esc下面)物品栏\n"
            L"PageDown梦之钉 NumLock梦之钉\n"
            L"(梦之钉的传送与放置请自行配合方向键)\n"
            L"(有椅子mod谁还用梦之钉传送)\n"
            L"\n"
            L"按住D连点Q 保持右移同时左劈\n"
            L"按住A连点E 保持左移同时右劈\n"
            L"(回身劈功能不是很稳定,有概率砍错方向)\n"
            L"\n"
            L"与椅子/人物交互可用上劈\n"
            L"交谈时W确认 4取消/跳过\n"
            L"切换护符时用右手控制方向键,左手W键拆装护符\n"
            L"\n"
            L"横向移动添加覆盖功能,AD同时按住人物不会不动,以后按的键的方向为准\n"
            L"(BUG:上下劈按住会保持连劈,但快速点按会吞键,导致攻速下降,冲刺同理)\n"
            L"\n"
            L"\n"
            L"                                                                                                 ------ made by jackens";
    }

    switch (msg) {
    case WM_PAINT:
        hdc = BeginPaint(hwnd, &ps);
        GetClientRect(hwnd, &rect);
        DrawText(hdc, infoText, -1, &rect, DT_LEFT | DT_TOP);  // Unicode版本
        EndPaint(hwnd, &ps);
        return 0;
    case WM_CLOSE:
        DestroyWindow(hwnd);
        break;
    case WM_DESTROY:
        if (myhook) {
            UnhookWindowsHookEx(myhook);
            myhook = NULL;
        }
        PostQuitMessage(0);
        break;
    default:
        return DefWindowProc(hwnd, msg, wParam, lParam);
    }
    return 0;
}

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {
    WNDCLASSEX wc = {0};
    HWND hwnd;
    MSG Msg;
    WCHAR text[256];  // 使用宽字符数组

    const WCHAR szClassName[] = L"myWindowClass";

    // 1. 设置注册窗口结构体
    wc.cbSize = sizeof(WNDCLASSEX); // 注册窗口结构体的大小
    wc.style = CS_HREDRAW | CS_VREDRAW; // 窗口的样式
    wc.lpfnWndProc = WndProc; // 指向窗口处理过程的函数指针
    wc.cbClsExtra = 0; // 指定紧跟在窗口类结构后的附加字节数
    wc.cbWndExtra = 0; // 指定紧跟在窗口事例后的附加字节数
    wc.hInstance = hInstance; // 本模块的实例句柄
    wc.hIcon = LoadIcon(NULL, IDI_APPLICATION); // 图标的句柄
    wc.hCursor = LoadCursor(NULL, IDC_ARROW); // 光标的句柄
    wc.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1); // 背景画刷的句柄
    wc.lpszMenuName = NULL; // 指向菜单的指针
    wc.lpszClassName = szClassName; // 指向类名称的指针
    wc.hIconSm = LoadIcon(NULL, IDI_APPLICATION); // 和窗口类关联的小图标

    // 2. 使用【窗口结构体】注册窗口
    if (!RegisterClassEx(&wc)) {
        MessageBox(NULL, L"窗口注册失败！", L"错误", MB_ICONEXCLAMATION | MB_OK);
        return 0;
    }

    // 3. 创建窗口
    hwnd = CreateWindowEx(
        WS_EX_CLIENTEDGE, // 窗口的扩展风格
        szClassName, // 指向注册类名的指针
        L"空洞骑士按键扩展v0.1", // 指向窗口名称的指针
        WS_OVERLAPPEDWINDOW, // 窗口风格
        CW_USEDEFAULT, CW_USEDEFAULT, 600, 600, // 窗口的 x,y 坐标以及宽高
        NULL, // 父窗口的句柄
        NULL, // 菜单的句柄
        hInstance, // 应用程序实例的句柄
        NULL // 指向窗口的创建数据
    );


    if (hwnd == NULL) {
        MessageBox(NULL, L"窗口创建失败", L"错误", MB_ICONEXCLAMATION | MB_OK);
        return 0;
    }

    // 4. 显示窗口
    ShowWindow(hwnd, nCmdShow);
    UpdateWindow(hwnd);

    // 设置键盘全局监听
    myhook = SetWindowsHookEx(
        WH_KEYBOARD_LL, // 监听类型【键盘】
        KeyboardProc, // 处理函数
        hInstance, // 当前实例句柄
        0 // 监听窗口句柄(NULL为全局监听)
    );

    //缩减键盘重复延时
    SystemParametersInfo(SPI_SETKEYBOARDDELAY, 0, 0, SPIF_SENDWININICHANGE);
    //加快按键重复速度
    SystemParametersInfo(SPI_SETKEYBOARDSPEED, 31, 0, SPIF_SENDWININICHANGE);

    wcscpy_s(hollowText, L"Hollow Knight");

    if (myhook == NULL) {
        swprintf_s(text, L"键盘监听失败！error : %d \n", GetLastError());
        MessageBox(hwnd, text, L"错误", MB_OK);
        return 1;
    }

    // 5. 消息循环
    while (GetMessage(&Msg, NULL, 0, 0) > 0) {
        TranslateMessage(&Msg);
        DispatchMessage(&Msg);
    }

    // 清理资源
    if (myhook) {
        UnhookWindowsHookEx(myhook);
    }

    return (int)Msg.wParam;
}
