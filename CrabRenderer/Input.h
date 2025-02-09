#pragma once

namespace crab
{

// clang-format off
enum class eKey
{
    Unknown = 0,
    A = 4,
    B,C,D,E,F,G,H,I,J,K,L,M,N,O,P,Q,R,S,T,U,V,W,X,Y,Z,
    Num1, Num2, Num3, Num4, Num5, Num6, Num7, Num8, Num9, Num0,

    Enter, Escape, Backspace, Tab, Space, Minus, Equals, LeftBracket, RightBracket,
    Backslash, NonUSHash, Semicolon, Apostrophe, Grave, Comma, Period, Slash, CapsLock,

    F1, F2, F3, F4, F5, F6, F7, F8, F9, F10, F11, F12,

    PrintScreen, ScrollLock, Pause, Insert, Home, PageUp, Delete, End, PageDown, Right, Left, Down, Up,

    NumLockClear, KP_Divide, KP_Multiply, KP_Minus, KP_Plus, KP_Enter, KP_1, KP_2, KP_3, KP_4, KP_5, KP_6, KP_7, KP_8, KP_9, KP_0, KP_Period,

    NonUSBackslash, Application, Power, KP_Equals, F13, F14, F15, F16, F17, F18, F19, F20, F21, F22, F23, F24, 
    Execute, Help, Menu, Select, Stop, Again, Undo, Cut, Copy, Paste, Find, Mute, VolumeUp, VolumeDown, LockingCapsLock, LockingNumLock,
    LockingScrollLock, KP_Comma, KP_EqualsAS400,
    International1, International2, International3, International4, International5, International6, International7, International8, International9,
    Lang1, Lang2, Lang3, Lang4, Lang5, Lang6, Lang7, Lang8, Lang9, AltErase, SysReq, Cancel, Clear, Prior, Return2, Separator, Out, Oper, ClearAgain,
    CrSel, ExSel, KP_00 = 176, KP_000, ThousandsSeparator, DecimalSeparator, CurrencyUnit, CurrencySubUnit, KP_LeftParen, KP_RightParen, KP_LeftBrace,
    KP_RightBrace, KP_Tab, KP_Backspace, KP_A, KP_B, KP_C, KP_D, KP_E, KP_F, KP_XOR, KP_Power, KP_Percent, KP_Less, KP_Greater, KP_Ampersand,
    KP_DBLAmpersand, KP_VerticalBar, KP_DBLVerticalBar, KP_Colon, KP_Hash, KP_Space, KP_At, KP_Exclam, KP_MemStore, KP_MemRecall, KP_MemClear, 
    KP_MemAdd, KP_MemSubtract, KP_MemMultiply, KP_MemDivide, KP_PlusMinus, KP_Clear, KP_ClearEntry, KP_Binary, KP_Octal, KP_Decimal, KP_Hexadecimal,
    LeftCtrl = 224, LeftShift, LeftAlt, LeftGUI, RightCtrl, RightShift, RightAlt, RightGUI, Mode = 257, AudioNext, AudioPrev, AudioStop, AudioPlay,
    AudioMute, MediaSelect, WWW, Mail, Calculator, Computer, AC_Search, AC_Home, AC_Back, AC_Forward, AC_Stop, AC_Refresh, AC_Bookmarks, BrightnessDown,
    BrightnessUp, DisplaySwitch, KBDIllumToggle, KBDIllumDown, KBDIllumUp, Eject, Sleep, App1, App2, AudioRewind, AudioFastForward,

    Count = 512, 
};

enum
{
    eKey_Count = static_cast<uint64>(eKey::Count)
};

enum class eMouse
{
    Left = 1, Middle, Right
};

// clang-format on

struct CrabIO
{
    // - Keyboard
    std::array<bool, eKey_Count> isKeyDown;
    std::array<bool, eKey_Count> prevKeyDown;

    // - Mouse
    std::array<bool, 3> isMouseDown;
    std::array<bool, 3> prevMouseDown;

    int32 mouseDeltaScrollX = 0;
    int32 mouseDeltaScrollY = 0;

    int32 mousePosX     = 0;
    int32 mousePosY     = 0;

    int32 prevMousePosX = 0;
    int32 prevMousePosY = 0;

    int32 relativeMouseDeltaX = 0;
    int32 relativeMouseDeltaY = 0;
};

//===================================================
// Input (Core)
//===================================================

struct CrabEvent;

class Input
{
public:
    // - Core
    static void Init();
    static void Update();

    // - Keyboard
    static bool IsKeyDown(eKey in_key);
    static bool IsKeyHold(eKey in_key);   // IsKeyDown, but not IsKeyPressed
    static bool IsKeyPressed(eKey in_key);

    static bool IsKeyUp(eKey in_key);
    static bool IsKeyReleased(eKey in_key);
    static bool IsKeyAway(eKey in_key);   // IsKeyUp, but not IsKeyReleased

    // - Mouse
    static bool IsMouseDown(eMouse in_mouse);
    static bool IsMousePressed(eMouse in_mouse);
    static bool IsMouseHold(eMouse in_mouse);   // IsMouseDown, but not IsMousePressed

    static bool IsMouseUp(eMouse in_mouse);
    static bool IsMouseReleased(eMouse in_mouse);
    static bool IsMouseAway(eMouse in_mouse);   // IsMouseUp, but not IsMouseReleased

    static auto GetMousePos();
    static auto GetPrevMousePos();
    static auto GetMouseDeltaPos();
    static auto GetMouseRelativeDeltaPos();
    static void SetMousePos(uint32 in_x, uint32 in_y);
    static void SetMousePosToCenter();

    // - Event
    static void OnEvent(CrabEvent& in_event);

private:
    static Scope<CrabIO> m_io;
};

inline auto Input::GetMousePos()
{
    return std::make_pair(m_io->mousePosX, m_io->mousePosY);
}

inline auto Input::GetMouseDeltaPos()
{
    int dx, dy;
    dx = m_io->mousePosX - m_io->prevMousePosX;
    dy = m_io->mousePosY - m_io->prevMousePosY;
    return std::make_pair(dx, dy);
}

inline auto Input::GetPrevMousePos()
{
    return std::make_pair(m_io->prevMousePosX, m_io->prevMousePosY);
}

inline auto Input::GetMouseRelativeDeltaPos()
{
    return std::make_pair(m_io->relativeMouseDeltaX, m_io->relativeMouseDeltaY);
}


}   // namespace crab