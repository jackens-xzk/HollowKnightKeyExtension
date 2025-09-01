#include <windows.h>
#include <stdio.h>

#define KeyDown(vk_code) ((GetAsyncKeyState(vk_code) & 0x8000) ? 1 : 0)
#define KeyUp(vk_code) ((GetAsyncKeyState(vk_code) & 0x8000) ? 0 : 1)

HWND hgWnd;
HHOOK myhook;
LPCSTR infoText;
char hollowText[256] = { 0 };

const int delayTime = 15;

LRESULT CALLBACK KeyboardProc(int nCode, WPARAM wParam, LPARAM lParam)
{
#pragma region 判断程序是否为knight
	//判断程序是否为knight;(自己想出来的,也不确定这方法是否高效)
    //拿到当前操作窗口的句柄
	HWND hwnd = ::GetActiveWindow();//当前活动窗口  ::为全局符号
	if (hwnd == nullptr) {
		//没有当前活动窗口
		hwnd = ::GetForegroundWindow();//最前面的窗口
		if (hwnd == nullptr) {
			//无顶层窗口 无窗口
			//不作处理
			return CallNextHookEx(myhook, nCode, wParam, lParam);
		}
	}
	//从句柄拿到标题
	char windowTextBuff[256] = { 0 };
	GetWindowTextA(hwnd, windowTextBuff, 255);
	//比较
	if (strcmp(windowTextBuff, hollowText) != 0)
		return CallNextHookEx(myhook, nCode, wParam, lParam);
#pragma endregion

	PKBDLLHOOKSTRUCT p = (PKBDLLHOOKSTRUCT)lParam;

	if (wParam == WM_KEYDOWN) { // 按下
		switch (p->vkCode) {

		case 'W': // Z 跳跃
			keybd_event('Z', 0, 0, 0); // 跳跃
			return 1;
			break;
		case 'S': // 下  下看、交互 
			keybd_event(VK_DOWN, 0, 0, 0);
			return 1;
			break;
        case 'A': // 左移动
            keybd_event(VK_LEFT, 0, 0, 0);
            return 1;
            break;
        case 'D':  // 右移动
            keybd_event(VK_RIGHT, 0, 0, 0);
            return 1;
            break;
		case ' ':  // 聚气回血、技能施法
			keybd_event('J', 0, 0, 0); // 专注 施法
            // keybd_event('Z', 0, 0, 0); // 跳跃
			return 1;
			break;
		
		case VK_NUMPAD7: // 上劈
		    keybd_event(VK_UP, 0, 0, 0);
		    keybd_event('X', 0, 0, 0);
		    return 1;
            break;
        case VK_NUMPAD4: // 横批
            keybd_event('X', 0, 0, 0);
		    return 1;
            break;
        case VK_NUMPAD1: // 下劈
		case VK_NUMPAD0: // 下劈
		    keybd_event(VK_DOWN, 0, 0, 0);
		    keybd_event('X', 0, 0, 0);
		    return 1;
            break;
          
        case VK_NUMPAD8: // 上吼
            keybd_event(VK_UP, 0, 0, 0);
            keybd_event('U', 0, 0, 0);
            return 1;
            break;
        case VK_NUMPAD5: // 横波
            keybd_event('U', 0, 0, 0);
            return 1;
            break;
        case VK_NUMPAD2: // 下砸
            keybd_event(VK_DOWN, 0, 0, 0);
            keybd_event('U', 0, 0, 0);
            return 1;
            break;
        
        case VK_NUMPAD6: // 冲刺
            keybd_event('C', 0, 0, 0);
            return 1;
            break;
            
		case VK_NUMPAD9: //小地图
			keybd_event(VK_TAB, 0, 0, 0);
			return 1;
			break;

		case VK_DECIMAL: //小地图
			keybd_event(VK_TAB, 0, 0, 0);
			return 1;
			break;

		case VK_ADD:  //小键盘加号对应 超级冲刺
		case VK_RETURN: //小键盘回车对应  超级冲刺
// 		case VK_SHIFT: shift键不行
			keybd_event('K', 0, 0, 0);
			return 1;
			break;
		case VK_NEXT:  //pageDown 对应 梦之钉 
			keybd_event('L', 0, 0, 0);
			return 1;
			break;

		case VK_END: //end对应 道具栏
		case VK_OEM_3:
			keybd_event('I', 0, 0, 0);
			keybd_event('I', 0, KEYEVENTF_KEYUP, 0);
			return 1;
			break;
			
		case 'Q': // 向右移动时 向左回身斩
			keybd_event(VK_RIGHT, 0, KEYEVENTF_KEYUP, 0);
			keybd_event(VK_LEFT, 0, 0, 0);
			Sleep(delayTime);
			keybd_event('X', 0, 0, 0);
			keybd_event('X', 0, KEYEVENTF_KEYUP, 0);
			keybd_event(VK_LEFT, 0, KEYEVENTF_KEYUP, 0);
			return 1;
			break;
		case 'E': // 向左移动时 向右回身斩
			keybd_event(VK_LEFT, 0, KEYEVENTF_KEYUP, 0);
			keybd_event(VK_RIGHT, 0, 0, 0);
			Sleep(delayTime);
			keybd_event('X', 0, 0, 0);
			keybd_event('X', 0, KEYEVENTF_KEYUP, 0);
			keybd_event(VK_RIGHT, 0, KEYEVENTF_KEYUP, 0);
			return 1;
			break;
			
		case VK_CAPITAL: // 向右移动时 向左回身冲刺
			keybd_event(VK_RIGHT, 0, KEYEVENTF_KEYUP, 0);
			keybd_event(VK_LEFT, 0, 0, 0);
			Sleep(delayTime);
			keybd_event('C', 0, 0, 0);
			keybd_event('C', 0, KEYEVENTF_KEYUP, 0);
			keybd_event(VK_LEFT, 0, KEYEVENTF_KEYUP, 0);
			return 1;
			break;
		case 'F': // 向左移动时 向右回身冲刺
			keybd_event(VK_LEFT, 0, KEYEVENTF_KEYUP, 0);
			keybd_event(VK_RIGHT, 0, 0, 0);
			Sleep(delayTime);
			keybd_event('C', 0, 0, 0);
			keybd_event('C', 0, KEYEVENTF_KEYUP, 0);
			keybd_event(VK_RIGHT, 0, KEYEVENTF_KEYUP, 0);
			return 1;
			break;
		}
	}
	if (wParam == WM_KEYUP) { // 处理按键抬起事件
		switch (p->vkCode) {
		
		case 'W':
			keybd_event('Z', 0, KEYEVENTF_KEYUP, 0);
			return 1;
			break;
        case 'S':
            keybd_event(VK_DOWN, 0, KEYEVENTF_KEYUP, 0);
            // keybd_event('J', 0, KEYEVENTF_KEYUP, 0); // 回血
            return 1;
            break;
        case 'A':
            keybd_event(VK_LEFT, 0, KEYEVENTF_KEYUP, 0);
            return 1;
            break;
        case 'D':
            keybd_event(VK_RIGHT, 0, KEYEVENTF_KEYUP, 0);
            return 1;
            break;
        
		case ' ':
			keybd_event('J', 0, KEYEVENTF_KEYUP, 0); //专注 施法
            // keybd_event('Z', 0, KEYEVENTF_KEYUP, 0); // 跳跃
			return 1;
			break;
			
        case VK_NUMPAD7:
            keybd_event(VK_UP, 0, KEYEVENTF_KEYUP, 0);
            keybd_event('X', 0, KEYEVENTF_KEYUP, 0);
		    return 1;
            break;
        case VK_NUMPAD4:
            keybd_event('X', 0, KEYEVENTF_KEYUP, 0);
		    return 1;
            break;
        case VK_NUMPAD1:
        case VK_NUMPAD0:
		    keybd_event(VK_DOWN, 0, KEYEVENTF_KEYUP, 0);
            keybd_event('X', 0, KEYEVENTF_KEYUP, 0);
		    return 1;
            break;
            
        case VK_NUMPAD8:
            keybd_event(VK_UP, 0, KEYEVENTF_KEYUP, 0);
            keybd_event('U', 0, KEYEVENTF_KEYUP, 0);
            return 1;
            break;
        case VK_NUMPAD5:
            keybd_event('U', 0, KEYEVENTF_KEYUP, 0);
            return 1;
            break;
        case VK_NUMPAD2:
            keybd_event(VK_DOWN, 0, KEYEVENTF_KEYUP, 0);
            keybd_event('U', 0, KEYEVENTF_KEYUP, 0);
            return 1;
            break;
            
        case VK_NUMPAD6:
            keybd_event('C', 0, KEYEVENTF_KEYUP, 0);
            return 1;
            break;
            
		case VK_NUMPAD9://小地图
			keybd_event(VK_TAB, 0, KEYEVENTF_KEYUP, 0);
			return 1;
			break;
        case VK_DECIMAL://小地图
            keybd_event(VK_TAB, 0, KEYEVENTF_KEYUP, 0);
            return 1;
            break;
		case VK_ADD://超级冲刺
		case VK_RETURN:
// 		case VK_SHIFT: // shift键不行
			keybd_event('K', 0, KEYEVENTF_KEYUP, 0);
			return 1;
			break;
		case VK_NEXT://梦之钉 pageDown
			keybd_event('L', 0, KEYEVENTF_KEYUP, 0);
			return 1;
			break;
		}

	}

	return CallNextHookEx(myhook, nCode, wParam, lParam);
}

// 5. 窗口过程处理
LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	hgWnd = hwnd;

	HDC hdc;  //设备环境句柄
	PAINTSTRUCT ps;
	RECT rect;
	infoText = TEXT(
		"\n"
		"游戏外使用请先关闭本程序,会对打字等功能产生影响\n"
		"\n"
		"首次使用前请重置空洞骑士按键键位\n"
		"并将ASDF键对应的操作依次改到JKLU键上\n"
		"\n"
		"AD左右移动\n"
		"W跳跃(你没看错)\n"
		"空格键 聚集/施法\n"
		"(以下数字均代指小键盘数字键,开启本程序时请确保小键盘灯常量(数字模式))\n"
		"4劈砍/剑技 7上劈 1下劈 0下劈\n"
		"5快速施法/横波 8上吼 2下砸\n"
		"6冲刺 3下冲(需佩戴冲刺大师)\n"
		"+超冲 enter超冲\n"
		"\n"
		"9快速地图 *快速地图\n"
		"end(方向键上面)物品栏 `(Esc下面)物品栏\n"
		"PageDown梦之钉 NumLock梦之钉\n"
		"(梦之钉的传送与放置请自行配合方向键)\n"
		"(有椅子mod谁还用梦之钉传送)\n"
		"\n"
		"按住D连点Q 保持右移同时左劈\n"
		"按住A连点E 保持左移同时右劈\n"
		"(回身劈功能不是很稳定,有概率砍错方向)\n"
		"\n"
		"与椅子/人物交互可用上劈\n"
		"交谈时W确认 4取消/跳过\n"
		"切换护符时用右手控制方向键,左手W键拆装护符\n"
		"\n"
		"横向移动添加覆盖功能,AD同时按住人物不会不动,以后按的键的方向为准\n"
		"(BUG:上下劈按住会保持连劈,但快速点按会吞键,导致攻速下降,冲刺同理)\n"
		"\n"
		"\n"
		"                                                                                                 ------ made by jackens"
	);
	switch (msg)
	{
	case WM_PAINT:
		hdc = BeginPaint(hwnd, &ps);
		GetClientRect(hwnd, &rect);
		DrawText(
			hdc,
			infoText,
			-1,
			&rect,
			DT_CENTER | DT_VCENTER
		);
		EndPaint(hwnd, &ps);
		return 0;
	case WM_CLOSE:
		DestroyWindow(hwnd);
		break;
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	}
	return DefWindowProc(hwnd, msg, wParam, lParam);
}

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance,
	LPSTR lpCmdLine, int nCmdShow)
{
	WNDCLASSEX wc;  // http://baike.baidu.com/view/1750396.htm
	HWND hwnd;
	MSG Msg;
	char text[30];

	const char szClassName[] = "myWindowClass";

	// 1. 设置注册窗口结构体
	wc.cbSize = sizeof(WNDCLASSEX);              // 注册窗口结构体的大小
	wc.style = 0;                               // 窗口的样式
	wc.lpfnWndProc = WndProc;                         // 指向窗口处理过程的函数指针
	wc.cbClsExtra = 0;                               // 指定紧跟在窗口类结构后的附加字节数
	wc.cbWndExtra = 0;                               // 指定紧跟在窗口事例后的附加字节数
	wc.hInstance = hInstance;                       // 本模块的实例句柄
	wc.hIcon = LoadIcon(NULL, IDI_APPLICATION); // 图标的句柄
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);     // 光标的句柄
	wc.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);        // 背景画刷的句柄
	wc.lpszMenuName = NULL;                            // 指向菜单的指针
	wc.lpszClassName = szClassName;                     // 指向类名称的指针
	wc.hIconSm = LoadIcon(NULL, IDI_APPLICATION); // 和窗口类关联的小图标

	// 2. 使用【窗口结构体】注册窗口
	if (!RegisterClassEx(&wc))
	{
		MessageBox(NULL, TEXT("窗口注册失败！"), TEXT("错误"), MB_ICONEXCLAMATION | MB_OK);
		return 0;
	}

	// 3. 创建窗口
	hwnd = CreateWindowEx(
		WS_EX_CLIENTEDGE,       // 窗口的扩展风格
		szClassName,            // 指向注册类名的指针
		TEXT("空洞骑士按键扩展v0.1"),       // 指向窗口名称的指针
		WS_OVERLAPPEDWINDOW,    // 窗口风格
		CW_USEDEFAULT, CW_USEDEFAULT, 600, 600, // 窗口的 x,y 坐标以及宽高
		NULL,                   // 父窗口的句柄
		NULL,                   // 菜单的句柄
		hInstance,              // 应用程序实例的句柄
		NULL                    // 指向窗口的创建数据
	);



	if (hwnd == NULL)
	{
		MessageBox(NULL, TEXT("窗口创建失败"), TEXT("错误"), MB_ICONEXCLAMATION | MB_OK);
		return 0;
	}

	// 4. 显示窗口
	ShowWindow(hwnd, nCmdShow);
	UpdateWindow(hwnd);

	// 设置键盘全局监听
	myhook = SetWindowsHookEx(
		WH_KEYBOARD_LL, // 监听类型【键盘】
		KeyboardProc,   // 处理函数
		hInstance,      // 当前实例句柄
		0               // 监听窗口句柄(NULL为全局监听)
	);

	//缩减键盘重复延时
	SystemParametersInfo(SPI_SETKEYBOARDDELAY, 0, 0, SPIF_SENDWININICHANGE);
	//加快按键重复速度
	SystemParametersInfo(SPI_SETKEYBOARDSPEED, 31, 0, SPIF_SENDWININICHANGE);

	sprintf(hollowText, "Hollow Knight");

	if (myhook == NULL)
	{
		wsprintf(text, "键盘监听失败！error : %d \n", GetLastError());
		MessageBox(hwnd, text, TEXT("错误"), MB_OK);
	}

	// 5. 消息循环
	while (GetMessage(&Msg, NULL, 0, 0) > 0)
	{
		TranslateMessage(&Msg);
		DispatchMessage(&Msg);
	}
	return Msg.wParam;
}