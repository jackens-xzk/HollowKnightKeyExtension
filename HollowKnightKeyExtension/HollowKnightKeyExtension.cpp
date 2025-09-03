#include <windows.h>
#include <cstdio>

HWND hg_wnd;
HHOOK keyboard_hook;
WCHAR hollow_knight_text[256] = L"Hollow Knight";

// 回身劈，回身冲刺的延迟，单位ms。 根据不同设备的反应速度，此值需要自行调整
// 延迟过低会导致游戏读取不到回身指令导致无法回身，延迟过高会导致回身劈反应迟缓
// 回身劈，回身冲只建议点按，长按会累积延迟导致松开后还会继续劈砍或冲刺一段时间
constexpr int back_delay_time = 20;

// 回血冲刺，在玩家回血时遇到紧急情况按下冲刺会自动停止回血然后冲刺
// 因为回血中断后存在后摇，所以这个需要的延迟比较大
constexpr int heal_delay_time = 250;

bool hollow_knight_game_active = false;
DWORD last_window_check = 0;

// 前台窗口检测间隔，单位ms，用于检查前台窗口是否为空洞骑士，可自行调整
// 间隔过低会频繁检测前台窗口，造成部分性能占用，高配机可忽略
// 间隔过高导致切出游戏后按键映射还继续存在，打字等操作可能会对其他软件造成干扰，如果你肯在切出后等待超过这个间隔时间再做其他事就不影响。
constexpr DWORD window_check_interval = 200;

bool charge_mode; // 蓄力模式
bool moving_left; // 正在向左移动
bool moving_right; // 正在向右移动

//按键状态记录
bool pad7;
bool pad4;
bool pad0;
bool pad8;
bool pad5;
bool pad2;
bool pad6;
bool pad3;

bool a;
bool d;

bool space;

// 前台窗口检测
void foreground_window_check() {
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

// 键盘按键的连续处理函数 达成持续按压转按键连发 按键优先级覆盖等功能
void keyboard_continue_proc() {
    // 前台窗口检测
    foreground_window_check();
    if (!hollow_knight_game_active) return;

    // 移动控制
    // 左走
    if (a && !d) {
        moving_left = true;
        moving_right = false;
        keybd_event(VK_RIGHT, 0, KEYEVENTF_KEYUP, 0);
        keybd_event(VK_LEFT, 0, 0, 0);
    }
    // 右走
    if (!a && d) {
        moving_right = true;
        moving_left = false;
        keybd_event(VK_LEFT, 0, KEYEVENTF_KEYUP, 0);
        keybd_event(VK_RIGHT, 0, 0, 0);
    }
    // ad都按下了，以后按下的方向为准
    if (a && d) {
        if (moving_left) {
            keybd_event(VK_LEFT, 0, KEYEVENTF_KEYUP, 0);
            keybd_event(VK_RIGHT, 0, 0, 0);
        }
        if (moving_right) {
            keybd_event(VK_RIGHT, 0, KEYEVENTF_KEYUP, 0);
            keybd_event(VK_LEFT, 0, 0, 0);
        }
    }
    // 不动，停止
    if (!a && !d) {
        // 仅做一次抬起处理，否则静止时干预正常的左右方向键←→功能
        if (moving_left || moving_right) {
            moving_left = false;
            moving_right = false;
            keybd_event(VK_RIGHT, 0, KEYEVENTF_KEYUP, 0);
            keybd_event(VK_LEFT, 0, KEYEVENTF_KEYUP, 0);
        }
    }

    if (pad7) {
        // 上劈
        if (charge_mode) {
            //蓄力模式
            keybd_event(VK_UP, 0, 0, 0);
            keybd_event('X', 0, KEYEVENTF_KEYUP, 0); // 松开触发剑技
            Sleep(1); // 延时供游戏检测松开剑技
            keybd_event('X', 0, 0, 0); // 不断连发延长旋风斩时间
            Sleep(1); // 延时供游戏检测上劈状态
            keybd_event(VK_UP, 0, KEYEVENTF_KEYUP, 0);
            // keybd_event('X', 0, KEYEVENTF_KEYUP, 0); // 取消松开保持连续蓄力
        }
        else {
            // 普通模式
            keybd_event(VK_UP, 0, 0, 0);
            keybd_event('X', 0, 0, 0);
            Sleep(1); // 延时供游戏检测上劈状态
            keybd_event(VK_UP, 0, KEYEVENTF_KEYUP, 0);
            keybd_event('X', 0, KEYEVENTF_KEYUP, 0);
        }
    }

    if (pad4) {
        // 横劈
        keybd_event('X', 0, 0, 0);
        Sleep(1);
        keybd_event('X', 0, KEYEVENTF_KEYUP, 0);
    }


    if (pad0) {
        // 下劈
        if (charge_mode) {
            // 蓄力模式
            keybd_event(VK_DOWN, 0, 0, 0);
            keybd_event('X', 0, KEYEVENTF_KEYUP, 0); // 松开触发剑技
            Sleep(1); // 松开触发剑技（延时供游戏检测）
            keybd_event('X', 0, 0, 0); // 不断连发延长旋风斩时间
            Sleep(1); // 延时供游戏检测下劈状态
            keybd_event(VK_DOWN, 0, KEYEVENTF_KEYUP, 0);
            // keybd_event('X', 0, KEYEVENTF_KEYUP, 0); // 取消松开保持连续蓄力
        }
        else {
            // 普通模式
            keybd_event(VK_DOWN, 0, 0, 0);
            keybd_event('X', 0, 0, 0);
            Sleep(1); // 延时供游戏检测下劈状态
            keybd_event(VK_DOWN, 0, KEYEVENTF_KEYUP, 0);
            keybd_event('X', 0, KEYEVENTF_KEYUP, 0);
        }
    }

    if (pad8) {
        // 上吼
        keybd_event(VK_UP, 0, 0, 0);
        keybd_event('U', 0, 0, 0);
        Sleep(1);
        keybd_event(VK_UP, 0, KEYEVENTF_KEYUP, 0);
        keybd_event('U', 0, KEYEVENTF_KEYUP, 0);
    }

    if (pad5) {
        // 横波
        keybd_event('U', 0, 0, 0);
        Sleep(1);
        keybd_event('U', 0, KEYEVENTF_KEYUP, 0);
    }

    if (pad2) {
        // 下砸
        keybd_event(VK_DOWN, 0, 0, 0);
        keybd_event('U', 0, 0, 0);
        Sleep(1);
        keybd_event(VK_DOWN, 0, KEYEVENTF_KEYUP, 0);
        keybd_event('U', 0, KEYEVENTF_KEYUP, 0);
    }

    if (pad6) {
        // 冲刺
        if (space) {
            // 如果正在回血，则取消回血再冲刺
            keybd_event('J', 0, KEYEVENTF_KEYUP, 0);
            Sleep(heal_delay_time);
            keybd_event('C', 0, 0, 0);
            Sleep(1);
            keybd_event('C', 0, KEYEVENTF_KEYUP, 0);
        }
        else {
            keybd_event('C', 0, 0, 0);
            Sleep(1);
            keybd_event('C', 0, KEYEVENTF_KEYUP, 0);
        }
    }

    if (pad3) {
        // 下冲
        keybd_event(VK_DOWN, 0, 0, 0);
        keybd_event('C', 0, 0, 0);
        Sleep(1);
        keybd_event('C', 0, KEYEVENTF_KEYUP, 0);
        keybd_event(VK_DOWN, 0, KEYEVENTF_KEYUP, 0);
    }
}


// 键盘钩子函数 仅对 按下、持续按压、抬起 做基本的按键映射
LRESULT CALLBACK keyboard_proc(const int n_code, const WPARAM w_param, const LPARAM l_param) {
    // 忽略系统消息等其他事件
    if (n_code < 0) {
        return CallNextHookEx(keyboard_hook, n_code, w_param, l_param);
    }

    // 前台窗口检测
    foreground_window_check();

    // 比较窗口标题
    if (!hollow_knight_game_active) {
        return CallNextHookEx(keyboard_hook, n_code, w_param, l_param);
    }

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
            a = true;
            return 1;

        case 'D': // 右移动
            d = true;
            return 1;

        case ' ': // 聚气回血、技能施法
            keybd_event('J', 0, 0, 0);
            space = true;
            return 1;

        case VK_NUMPAD7: // 上劈
            pad7 = true;
            return 1;

        case VK_NUMPAD4: // 横批
            pad4 = true;
            charge_mode = false;
            return 1;

        case VK_NUMPAD1: // 剑技蓄力
            keybd_event('X', 0, KEYEVENTF_KEYUP, 0);
            Sleep(1);
            keybd_event('X', 0, 0, 0);
            charge_mode = true;
            return 1;
        case VK_NUMPAD0: // 下劈
            pad0 = true;
            return 1;

        case VK_NUMPAD8: // 上吼
            pad8 = true;
            return 1;

        case VK_NUMPAD5: // 横波
            pad5 = true;
            return 1;

        case VK_NUMPAD2: // 下砸
            pad2 = true;
            return 1;

        case VK_NUMPAD6: // 冲刺
            pad6 = true;
            return 1;

        case VK_NUMPAD3: // 下冲
            pad3 = true;
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
            Sleep(back_delay_time);
            keybd_event('X', 0, 0, 0);
            Sleep(1); // 延迟1ms，确保能触发
            keybd_event('X', 0, KEYEVENTF_KEYUP, 0);
            keybd_event(VK_LEFT, 0, KEYEVENTF_KEYUP, 0);
            return 1;

        case 'E': // 向左移动时 向右回身斩
            keybd_event(VK_LEFT, 0, KEYEVENTF_KEYUP, 0);
            keybd_event(VK_RIGHT, 0, 0, 0);
            Sleep(back_delay_time);
            keybd_event('X', 0, 0, 0);
            Sleep(1); // 延迟1ms，确保能触发
            keybd_event('X', 0, KEYEVENTF_KEYUP, 0);
            keybd_event(VK_RIGHT, 0, KEYEVENTF_KEYUP, 0);
            return 1;

        case VK_CAPITAL: // 向右移动时 向左回身冲刺 (Caps Lock)
            keybd_event(VK_RIGHT, 0, KEYEVENTF_KEYUP, 0);
            keybd_event(VK_LEFT, 0, 0, 0);
            Sleep(back_delay_time);
            keybd_event('C', 0, 0, 0);
            Sleep(1); // 延迟1ms，确保能触发
            keybd_event('C', 0, KEYEVENTF_KEYUP, 0);
            keybd_event(VK_LEFT, 0, KEYEVENTF_KEYUP, 0);
            return 1;

        case 'F': // 向左移动时 向右回身冲刺
            keybd_event(VK_LEFT, 0, KEYEVENTF_KEYUP, 0);
            keybd_event(VK_RIGHT, 0, 0, 0);
            Sleep(back_delay_time);
            keybd_event('C', 0, 0, 0);
            Sleep(1); // 延迟1ms，确保能触发
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
            a = false;
            return 1;

        case 'D':
            d = false;
            return 1;

        case ' ':
            keybd_event('J', 0, KEYEVENTF_KEYUP, 0);
            space = false;
            return 1;

        case VK_NUMPAD7: // 上劈
            pad7 = false;
            return 1;

        case VK_NUMPAD4: // 横批
            pad4 = false;
            return 1;

        case VK_NUMPAD1: // 剑技蓄力
            return 1;
        case VK_NUMPAD0: // 下劈
            pad0 = false;
            return 1;

        case VK_NUMPAD8: // 上吼
            pad8 = false;
            return 1;

        case VK_NUMPAD5: // 横波
            pad5 = false;
            return 1;

        case VK_NUMPAD2: // 下砸
            pad2 = false;
            return 1;

        case VK_NUMPAD6: // 冲刺
            pad6 = false;
            return 1;

        case VK_NUMPAD3: // 下冲
            pad3 = false;
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
            24, // 字体高度
            0, // 字体宽度
            0, // 文本倾斜角度
            0, // 字符基线倾斜角度
            FW_NORMAL, // 字体粗细
            FALSE, // 是否斜体
            FALSE, // 是否下划线
            FALSE, // 是否删除线
            DEFAULT_CHARSET, // 字符集
            OUT_DEFAULT_PRECIS, // 输出精度
            CLIP_DEFAULT_PRECIS, // 裁剪精度
            CLEARTYPE_QUALITY, // 输出质量 - 使用ClearType
            DEFAULT_PITCH, // 字符间距
            L"微软雅黑" // 字体名称
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
        L"6冲刺 3下冲(需佩戴冲刺大师) +超冲 enter超冲 w中断超冲\n"
        L"\n"
        L"9快速地图\n"
        L"end(方向键上面)物品栏 `(Esc下面)物品栏\n"
        L"PageDown梦之钉\n"
        L"(梦之钉的传送与放置请自行配合方向键，用↑↓←→而不是WASD)\n"
        L"向上看也需要按住方向键↑，向下看可以选择按住S\n"
        L"\n"
        L"按住D连点Q 保持右移同时左劈\n"
        L"按住A连点E 保持左移同时右劈\n"
        L"右走时按Caps左冲，左走时按F右冲\n"
        L"回身劈，回身冲只建议点按，长按会累积延迟导致松开后还会继续劈砍或冲刺一段时间\n"
        L"\n"
        L"回血冲刺，如果正在按住空格回血时遇到紧急情况，按下了小键盘6冲刺\n"
        L"会帮助中断回血并在中断回血的后摇结束后冲刺，因为中断回血存在后摇所以反应稍慢\n"
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
        CW_USEDEFAULT, CW_USEDEFAULT, 720, 850, // 窗口的 x,y 坐标以及宽高
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
    // while (GetMessage(&msg, nullptr, 0, 0) > 0) {
    //     TranslateMessage(&msg);
    //     DispatchMessage(&msg);
    // }

    // 兼容消息循环和按键事件处理
    while (true) {
        if (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE)) {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
        else {
            // 消息循环外处理按键事件，防止事件阻塞
            keyboard_continue_proc();
            Sleep(1); // 防止CPU占用过高
        }
    }

    // 清理资源
    if (keyboard_hook) {
        UnhookWindowsHookEx(keyboard_hook);
    }

    return static_cast<int>(msg.wParam);
}
