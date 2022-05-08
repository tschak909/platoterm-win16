#include <stdbool.h>
#include <stdio.h>
#include "keyboard.h"
#include "protocol.h"
#include "io.h"
#include "key.h"

bool control_pressed=false;
bool shift_pressed=false;
extern bool keyboardActive;

char debugMsg[128];

void keyboard_out(unsigned char platoKey)
{
  if (platoKey==0xff)
    return;

  if (platoKey>0x7F)
    {
      Key(ACCESS);
      Key(ACCESS_KEYS[platoKey-0x80]);
      return;
    }
  Key(platoKey);
  return;
}

LRESULT keyboard_main(UINT wParam)
{
    unsigned char k=0xFF;

    switch(wParam)
    {
        case VK_SHIFT:
            shift_pressed=true;
            break;
        case VK_CONTROL:
            control_pressed=true;
            break;
        case VK_ESCAPE:
            k=PKEY_ASSIGN;
            break;
        case VK_SPACE:
            k=(shift_pressed ? PKEY_BACKSPACE : PKEY_SPACE);
            break;
        case VK_BACK:
            k=(shift_pressed ? PKEY_ERASE1 : PKEY_ERASE);
            break;
        case VK_TAB:
            k=(shift_pressed ? PKEY_CR : PKEY_TAB);
            break;
        case VK_RETURN:
            k=(shift_pressed ? PKEY_NEXT1 : PKEY_NEXT);
            break;
        case VK_PRIOR:
            k=PKEY_DIVIDE;
            break;
        case VK_NEXT:
            k=PKEY_MULTIPLY;
            break;
        case VK_LEFT:
            k=PKEY_a;
            break;
        case VK_RIGHT:
            k=PKEY_d;
            break;
        case VK_UP:
            k=PKEY_w;
            break;
        case VK_DOWN:
            k=PKEY_x;
            break;
        case VK_HELP:
            k=(shift_pressed ? PKEY_HELP1 : PKEY_HELP);
            break;
        case '0':
            k=(shift_pressed ? PKEY_PARENTHESIS_RIGHT : PKEY_0);
            break;
        case '1':
            k=(shift_pressed ? PKEY_EXCLAMATION : PKEY_1);
            break;
        case '2':
            k=(shift_pressed ? PKEY_AT : PKEY_2);
            break;
        case '3':
            k=(shift_pressed ? PKEY_POUND : PKEY_3);
            break;
        case '4':
            k=(shift_pressed ? PKEY_DOLLAR : PKEY_4);
            break;
        case '5':
            k=(shift_pressed ? PKEY_PERCENT : PKEY_5);
            break;
        case '6':
            k=(shift_pressed ? PKEY_CIRCUMFLEX : PKEY_6);
            break;
        case '7':
            k=(shift_pressed ? PKEY_AMPERSAND : PKEY_7);
            break;
        case '8':
            k=(shift_pressed ? PKEY_ASTERISK : PKEY_8);
            break;
        case '9':
            k=(shift_pressed ? PKEY_PARENTHESIS_LEFT : PKEY_9);
            break;
        case 'A':
            if (control_pressed && shift_pressed)
                k=PKEY_TERM;
            else if (control_pressed)
                k=PKEY_ANS;
            else if (shift_pressed)
                k=PKEY_A;
            else
                k=PKEY_a;
            break;
        case 'B':
            if (control_pressed && shift_pressed)
                k=PKEY_BACK1;
            else if (control_pressed)
                k=PKEY_BACK;
            else if (shift_pressed)
                k=PKEY_B;
            else
                k=PKEY_b;
            break;
        case 'C':
            if (control_pressed && shift_pressed)
                k=PKEY_COPY1;
            else if (control_pressed)
                k=PKEY_COPY;
            else if (shift_pressed)
                k=PKEY_C;
            else
                k=PKEY_c;
            break;
        case 'D':
            if (control_pressed && shift_pressed)
                k=PKEY_DATA1;
            else if (control_pressed)
                k=PKEY_DATA;
            else if (shift_pressed)
                k=PKEY_D;
            else
                k=PKEY_d;
            break;
        case 'E':
            if (control_pressed && shift_pressed)
                k=PKEY_EDIT1;
            else if (control_pressed)
                k=PKEY_EDIT;
            else if (shift_pressed)
                k=PKEY_E;
            else
                k=PKEY_e;
            break;
        case 'F':
            if (control_pressed && shift_pressed)
                k=PKEY_MICRO;
            else if (control_pressed)
                k=PKEY_FONT;
            else if (shift_pressed)
                k=PKEY_F;
            else
                k=PKEY_f;
            break;
        case 'G':
            if (control_pressed && shift_pressed)
                k=PKEY_INTERSECT;
            else if (control_pressed)
                k=PKEY_DIVIDE;
            else if (shift_pressed)
                k=PKEY_G;
            else
                k=PKEY_g;
            break;
        case 'H':
            if (control_pressed && shift_pressed)
                k=PKEY_HELP1;
            else if (control_pressed)
                k=PKEY_HELP;
            else if (shift_pressed)
                k=PKEY_H;
            else
                k=PKEY_h;
            break;
        case 'I':
            if (shift_pressed)
                k=PKEY_I;
            else
                k=PKEY_i;
            break;
        case 'J':
            if (shift_pressed)
                k=PKEY_J;
            else
                k=PKEY_j;
            break;
        case 'K':
            if (shift_pressed)
                k=PKEY_K;
            else
                k=PKEY_k;
            break;
        case 'L':
            if (control_pressed && shift_pressed)
                k=PKEY_LAB1;
            else if (control_pressed)
                k=PKEY_LAB;
            else if (shift_pressed)
                k=PKEY_L;
            else
                k=PKEY_l;
            break;
        case 'M':
            if (control_pressed && shift_pressed)
                k=PKEY_MICRO;
            else if (control_pressed)
                k=PKEY_MICRO;
            else if (shift_pressed)
                k=PKEY_M;
            else
                k=PKEY_m;
            break;
        case 'N':
            if (control_pressed && shift_pressed)
                k=PKEY_NEXT1;
            else if (control_pressed)
                k=PKEY_NEXT;
            else if (shift_pressed)
                k=PKEY_N;
            else
                k=PKEY_n;
            break;
        case 'O':
            if (shift_pressed)
                k=PKEY_O;
            else
                k=PKEY_o;
            break;
        case 'P':
            if (control_pressed && shift_pressed)
                k=PKEY_SUPER1;
            else if (control_pressed)
                k=PKEY_SUPER;
            else if (shift_pressed)
                k=PKEY_P;
            else
                k=PKEY_p;
            break;
        case 'Q':
            if (control_pressed)
                k=PKEY_SQUARE;
            else if (shift_pressed)
                k=PKEY_Q;
            else
                k=PKEY_q;
            break;
        case 'R':
            if (control_pressed && shift_pressed)
                k=PKEY_ERASE1;
            else if (control_pressed)
                k=PKEY_ERASE;
            else if (shift_pressed)
                k=PKEY_R;
            else
                k=PKEY_r;
            break;
        case 'S':
            if (control_pressed && shift_pressed)
                k=PKEY_STOP1;
            else if (control_pressed)
                k=PKEY_STOP;
            else if (shift_pressed)
                k=PKEY_S;
            else
                k=PKEY_s;
            break;
        case 'T':
            if (control_pressed && shift_pressed)
                k=PKEY_TERM;
            else if (control_pressed)
                k=PKEY_TERM;
            else if (shift_pressed)
                k=PKEY_T;
            else
                k=PKEY_t;
            break;
        case 'U':
            if (shift_pressed)
                k=PKEY_U;
            else
                k=PKEY_u;
            break;
        case 'V':
            if (shift_pressed)
                k=PKEY_V;
            else
                k=PKEY_v;
            break;
        case 'W':
            if (shift_pressed)
                k=PKEY_W;
            else
                k=PKEY_w;
            break;
        case 'X':
            if (control_pressed && shift_pressed)
                k=PKEY_UNION;
            else if (control_pressed)
                k=PKEY_MULTIPLY;
            else if (shift_pressed)
                k=PKEY_X;
            else
                k=PKEY_x;
            break;
        case 'Y':
            if (control_pressed && shift_pressed)
                k=PKEY_SUB1;
            else if (control_pressed)
                k=PKEY_SUB;
            else if (shift_pressed)
                k=PKEY_Y;
            else
                k=PKEY_y;
            break;
        case 'Z':
            if (shift_pressed)
                k=PKEY_Z;
            else
                k=PKEY_z;
            break;
    }

    keyboard_out(k);

    return 0;
}

LRESULT keyboard_main_up(UINT wParam)
{
    switch(wParam)
    {
        case VK_SHIFT:
            shift_pressed=false;
            break;
        case VK_CONTROL:
            control_pressed=false;
            break;
    }
    return 0;
}

void keyboard_clear(void)
{
}

void keyboard_out_tty(char ch)
{
  io_send_byte(ch);
}
