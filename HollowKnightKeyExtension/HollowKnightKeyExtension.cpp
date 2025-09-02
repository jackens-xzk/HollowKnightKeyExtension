#include <windows.h>
#include <cstdio>

HWND hg_wnd;
HHOOK keyboard_hook;
WCHAR hollow_knight_text[256] = L"Hollow Knight";
constexpr int delay_time = 15;
bool hollow_knight_game_active = false;
DWORD last_window_check = 0;
constexpr DWORD window_check_interval = 100; // 100ms检查一次

// 更新当前窗口状态的函数
void update_window_status() {
    DWORD current_time = GetTickCount();
    // 避免检查过于频繁
    if (current_time - last_window_check > window_check_interval) {
        HWND foreground_window = GetForegroundWindow();
        if (foreground_window != nullptr) {
            WCHAR window_text_buff[256] = {};
            GetWindowText(foreground_window, window_text_buff, 255);
            hollow_knight_game_active = wcscmp(window_text_buff, hollow_knight_text) == 0;
        }
        else {
            hollow_knight_game_active = false;
        }
        last_window_check = current_time;
    }
}

LRESULT CALLBACK keyboard_proc(const int n_code, const WPARAM w_param, const LPARAM l_param) {
#pragma region 基本处理

    // 忽略系统消息等其他事件
    if (n_code < 0) {
        return CallNextHookEx(keyboard_hook, n_code, w_param, l_param);
    }

    // 更新窗口状态（但不会太频繁）
    update_window_status();

    // 比较窗口标题
    if (!hollow_knight_game_active) {
        return CallNextHookEx(keyboard_hook, n_code, w_param, l_param);
    }

#pragma endregion

    // ReSharper disable once CppFunctionalStyleCast
    const auto p = PKBDLLHOOKSTRUCT(l_param); // NOLINT(performance-no-int-to-ptr)

    if (w_param == WM_KEYDOWN || w_param == WM_SYSKEYDOWN) {
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
            Sleep(delay_time);
            keybd_event('X', 0, 0, 0);
            keybd_event('X', 0, KEYEVENTF_KEYUP, 0);
            keybd_event(VK_LEFT, 0, KEYEVENTF_KEYUP, 0);
            return 1;

        case 'E': // 向左移动时 向右回身斩
            keybd_event(VK_LEFT, 0, KEYEVENTF_KEYUP, 0);
            keybd_event(VK_RIGHT, 0, 0, 0);
            Sleep(delay_time);
            keybd_event('X', 0, 0, 0);
            keybd_event('X', 0, KEYEVENTF_KEYUP, 0);
            keybd_event(VK_RIGHT, 0, KEYEVENTF_KEYUP, 0);
            return 1;

        case VK_CAPITAL: // 向右移动时 向左回身冲刺 (Caps Lock)
            keybd_event(VK_RIGHT, 0, KEYEVENTF_KEYUP, 0);
            keybd_event(VK_LEFT, 0, 0, 0);
            Sleep(delay_time);
            keybd_event('C', 0, 0, 0);
            keybd_event('C', 0, KEYEVENTF_KEYUP, 0);
            keybd_event(VK_LEFT, 0, KEYEVENTF_KEYUP, 0);
            return 1;

        case 'F': // 向左移动时 向右回身冲刺
            keybd_event(VK_LEFT, 0, KEYEVENTF_KEYUP, 0);
            keybd_event(VK_RIGHT, 0, 0, 0);
            Sleep(delay_time);
            keybd_event('C', 0, 0, 0);
            keybd_event('C', 0, KEYEVENTF_KEYUP, 0);
            keybd_event(VK_RIGHT, 0, KEYEVENTF_KEYUP, 0);
            return 1;
        default: break;
        }
    }
    if (w_param == WM_KEYUP || w_param == WM_SYSKEYUP) {
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
            keybd_event('I', 0, KEYEVENTF_KEYUP, 0);
            return 1;
        default: break;
        }
    }

    return CallNextHookEx(keyboard_hook, n_code, w_param, l_param);
}

// 窗口过程处理
LRESULT CALLBACK wnd_proc(const HWND hwnd, const UINT msg, const WPARAM w_param, const LPARAM l_param) {
    // 创建自定义字体
    static HFONT hFont = nullptr;
    if (hFont == nullptr) {
        hFont = CreateFont(
            24,                        // 字体高度
            0,                         // 字体宽度
            0,                         // 文本倾斜角度
            0,                         // 字符基线倾斜角度
            FW_NORMAL,                 // 字体粗细
            FALSE,                     // 是否斜体
            FALSE,                     // 是否下划线
            FALSE,                     // 是否删除线
            DEFAULT_CHARSET,           // 字符集
            OUT_DEFAULT_PRECIS,        // 输出精度
            CLIP_DEFAULT_PRECIS,       // 裁剪精度
            CLEARTYPE_QUALITY,         // 输出质量 - 使用ClearType
            DEFAULT_PITCH,             // 字符间距
            L"微软雅黑"                // 字体名称
        );
        
        // 如果微软雅黑不可用，使用系统默认字体
        if (hFont == nullptr) {
            hFont = CreateFont(
                16, 0, 0, 0, FW_NORMAL, FALSE, FALSE, FALSE,
                DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS,
                CLEARTYPE_QUALITY, DEFAULT_PITCH, nullptr
            );
        }
    }

    // 绘制提示文本
    static auto hint_text =
        L"首次使用前请重置空洞骑士按键键位，此工具免费\n"
        L"并将ASDF键对应的操作依次改到JKLU键上，即：\n"
        L"聚集/施法改为J，超级冲刺改为K，梦之钉改为L，快速施法改为U\n"
        L"\n"
        L"AD左右移动 W跳跃(你没看错) 空格键 聚集/施法\n"
        L"横向移动添加覆盖功能,AD同时按住人物不会不动,会以后按的键的方向为准\n"
        L"\n"
        L"(以下数字均代指小键盘数字键,开启本程序时最好确保小键盘灯常量(数字模式))\n"
        L"4劈砍 7上劈 0下劈 (为此而生)\n"
        L"5快速施法/横波 8上吼 2下砸\n"
        L"以上劈砍和魔法均可自动连发（配合快劈，快冲效果更佳）\n"
        L"按1蓄力剑技启动自动蓄力模式，按4普攻取消自动蓄力模式\n"
        L"1为自动蓄力剑技，蓄力完成后按1、7、0可释放对应剑技，释放后无缝自动蓄力下一次\n"
        L"6冲刺 3下冲(需佩戴冲刺大师) +超冲 enter超冲\n"
        L"\n"
        L"9快速地图\n"
        L"end(方向键上面)物品栏 `(Esc下面)物品栏\n"
        L"PageDown梦之钉\n"
        L"(梦之钉的传送与放置请自行配合方向键，用↑↓←→而不是WASD)\n"
        L"\n"
        L"按住D连点Q 保持右移同时左劈\n"
        L"按住A连点E 保持左移同时右劈\n"
        L"右走时按Caps左冲，左走时按F右冲\n"
        L"\n"
        L"与椅子/人物交互可用S或7\n"
        L"交谈时，4下一句，W确认选择 \n"
        L"切换护符时用右手控制方向键（用↑↓←→而不是WASD）,左手W键拆装护符\n"
        L"\n"
        L"                                                                                             ------ made by jackens";

    switch (msg) {
    case WM_PAINT: {
        PAINTSTRUCT ps;
        RECT rect;
        HDC hdc = BeginPaint(hwnd, &ps);
        GetClientRect(hwnd, &rect);
        
        // 选择自定义字体
        HFONT hOldFont = static_cast<HFONT>(SelectObject(hdc, hFont));
        
        // 设置文本背景模式为透明
        SetBkMode(hdc, TRANSPARENT);
        
        // 绘制文本
        DrawText(hdc, hint_text, -1, &rect, DT_LEFT | DT_TOP);
        
        // 恢复旧字体
        SelectObject(hdc, hOldFont);
        
        EndPaint(hwnd, &ps);
        return 0;
    }
        
    case WM_CLOSE:
        DestroyWindow(hwnd);
        break;
        
    case WM_DESTROY:
        // 删除字体资源
        if (hFont != nullptr) {
            DeleteObject(hFont);
            hFont = nullptr;
        }
        
        if (keyboard_hook) {
            UnhookWindowsHookEx(keyboard_hook);
            keyboard_hook = nullptr;
        }
        PostQuitMessage(0);
        break;
        
    default:
        return DefWindowProc(hwnd, msg, w_param, l_param);
    }
    return 0;
}


int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {
    WNDCLASSEX wc;
    MSG msg;

    constexpr WCHAR sz_class_name[] = L"myWindowClass";

    // 1. 设置注册窗口结构体
    wc.cbSize = sizeof(WNDCLASSEX); // 注册窗口结构体的大小
    wc.style = CS_HREDRAW | CS_VREDRAW; // 窗口的样式
    wc.lpfnWndProc = wnd_proc; // 指向窗口处理过程的函数指针
    wc.cbClsExtra = 0; // 指定紧跟在窗口类结构后的附加字节数
    wc.cbWndExtra = 0; // 指定紧跟在窗口事例后的附加字节数
    wc.hInstance = hInstance; // 本模块的实例句柄
    wc.hIcon = LoadIcon(nullptr, IDI_APPLICATION); // 图标的句柄
    wc.hCursor = LoadCursor(nullptr, IDC_ARROW); // 光标的句柄
    // ReSharper disable once CppFunctionalStyleCast
    wc.hbrBackground = HBRUSH(COLOR_WINDOW + 1); // 背景画刷的句柄  // NOLINT(performance-no-int-to-ptr)
    wc.lpszMenuName = nullptr; // 指向菜单的指针
    wc.lpszClassName = sz_class_name; // 指向类名称的指针
    wc.hIconSm = LoadIcon(nullptr, IDI_APPLICATION); // 和窗口类关联的小图标

    // 2. 使用【窗口结构体】注册窗口
    if (!RegisterClassEx(&wc)) {
        MessageBox(nullptr, L"窗口注册失败！", L"错误", MB_ICONEXCLAMATION | MB_OK);
        return 0;
    }

    // 3. 创建窗口
    const HWND hwnd = CreateWindowEx(
        WS_EX_CLIENTEDGE, // 窗口的扩展风格
        sz_class_name, // 指向注册类名的指针
        L"空洞骑士按键扩展v0.1", // 指向窗口名称的指针
        WS_OVERLAPPEDWINDOW, // 窗口风格
        CW_USEDEFAULT, CW_USEDEFAULT, 720, 740, // 窗口的 x,y 坐标以及宽高
        nullptr, // 父窗口的句柄
        nullptr, // 菜单的句柄
        hInstance, // 应用程序实例的句柄
        nullptr // 指向窗口的创建数据
    );


    if (hwnd == nullptr) {
        MessageBox(nullptr, L"窗口创建失败", L"错误", MB_ICONEXCLAMATION | MB_OK);
        return 0;
    }

    // 4. 显示窗口
    ShowWindow(hwnd, nCmdShow);
    UpdateWindow(hwnd);

    // 设置键盘全局监听
    keyboard_hook = SetWindowsHookEx(
        WH_KEYBOARD_LL, // 监听类型【键盘】
        keyboard_proc, // 处理函数
        hInstance, // 当前实例句柄
        0 // 监听窗口句柄(NULL为全局监听)
    );

    //缩减键盘重复延时
    SystemParametersInfo(SPI_SETKEYBOARDDELAY, 0, 0, SPIF_SENDWININICHANGE);
    //加快按键重复速度
    SystemParametersInfo(SPI_SETKEYBOARDSPEED, 31, 0, SPIF_SENDWININICHANGE);

    if (keyboard_hook == nullptr) {
        WCHAR text[256];
        swprintf_s(text, L"键盘监听失败！error : %d \n", GetLastError()); // NOLINT(cert-err33-c)
        MessageBox(hwnd, text, L"错误", MB_OK);
        return 1;
    }

    // 5. 消息循环
    while (GetMessage(&msg, nullptr, 0, 0) > 0) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    // 清理资源
    if (keyboard_hook) {
        UnhookWindowsHookEx(keyboard_hook);
    }

    return static_cast<int>(msg.wParam);
}
