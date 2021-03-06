/*
 * VirtualKeyCodes.h
 *
 *  Created on: Apr 25, 2010
 *      Author: arris
 */

#ifndef NOUS_INPUT_VIRTUALKEYCODES_H
#define NOUS_INPUT_VIRTUALKEYCODES_H

#if defined __linux
#include <linux/input.h>
#elif defined WIN32

#endif

namespace Nous {
namespace Input {

struct KeyCode
{
public:
	static uint32_t toScanCode()
	{

	}
}; // struct KeyCode

} // namespace Nous
} // namespace Input
#if 0
// Reference: http://classicteck.com/rbarticles/mackeyboard.php
Key 	Mac 	Windows 	Linux 	Notes
rbKeyUp 	126 	26 	103
rbKeyDown 	125 	28 	108
rbKeyLeft 	123 	25 	105
rbKeyRight 	124 	27 	106
rbKeyBackspace 	117 	8 	14
rbKeyEnter 	76 	* 	28
rbKeyHome 	115 	36 	102
rbKeyEnd 	119 	35 	107
rbKeyPageDown 	121 	34 	109
rbKeyPageUp 	116 	33 	104
rbKeyReturn 	36 	13 	*
rbKeyDelete 	51 	46 	111
rbKeyTab 	48 	9 	15
rbKeySpacebar 	49 	20 	57
rbKeyShift 	56 	10 	*
rbKeyControl 	59 	11 	*
rbKeyMenu 	58 	18 	139 	The Alt key
rbKeyPrintScreen 	* 	42 	210
rbKeyEscape 	53 	27 	1
rbKeyCapsLock 	57 	20 	58
rbKeyHelp 	114 	47 	138
rbKeyF1 	122 	112 	59
rbKeyF2 	120 	113 	60
rbKeyF3 	99 	114 	61
rbKeyF4 	118 	115 	62
rbKeyF5 	96 	116 	63
rbKeyF6 	97 	117 	64
rbKeyF7 	98 	118 	65
rbKeyF8 	100 	119 	66
rbKeyF9 	101 	120 	67
rbKeyF10 	109 	121 	68
rbKeyF11 	103 	122 	87
rbKeyF12 	111 	123 	88
rbKeyMacFn 	63 	* 	*
rbKeyMacOption 	58 	* 	*
rbKeyMacCommand 	55 	* 	*
rbKeyWinLeftWindow 	* 	91 	* 	On "Natural" keyboards
rbKeyWinRightWindow 	* 	92 	* 	On "Natural" keyboards
rbKeyWinApplication 	110 	93 	* 	On "Natural" keyboards
rbKeyQ 	12 	81 	16
rbKeyW 	13 	87 	17
rbKeyE 	14 	69 	18
rbKeyR 	15 	82 	19
rbKeyT 	17 	84 	20
rbKeyY 	16 	89 	21
rbKeyU 	32 	85 	22
rbKeyI 	34 	73 	23
rbKeyO 	31 	79 	24
rbKeyP 	35 	80 	25
rbKeyA 	* 	65 	30
rbKeyS 	1 	83 	31
rbKeyD 	2 	68 	32
rbKeyF 	3 	70 	33
rbKeyG 	5 	71 	34
rbKeyH 	4 	72 	35
rbKeyJ 	38 	74 	36
rbKeyK 	40 	75 	37
rbKeyL 	37 	76 	38
rbKeyZ 	6 	90 	44
rbKeyX 	7 	88 	45
rbKeyC 	8 	67 	46
rbKeyV 	9 	86 	47
rbKeyB 	11 	66 	48
rbKeyN 	45 	78 	49
rbKeyM 	46 	77 	50
rbKey0 	29 	48 	11
rbKey1 	18 	49 	2
rbKey2 	19 	50 	3
rbKey3 	20 	51 	4
rbKey4 	21 	52 	5
rbKey5 	23 	53 	6
rbKey6 	22 	54 	7
rbKey7 	26 	55 	8
rbKey8 	28 	56 	9
rbKey9 	25 	57 	10
rbKeyPeriod 	47 	190 	52
rbKeyComma 	43 	188 	51
rbKeySlash 	44 	191 	53 	The key with /? generally next to right shift key.
rbKeyNum0 	82 	96 	82 	On numeric keypad or with NumLock
rbKeyNum1 	83 	97 	79 	On numeric keypad or with NumLock
rbKeyNum2 	84 	98 	80 	On numeric keypad or with NumLock
rbKeyNum3 	85 	99 	81 	On numeric keypad or with NumLock
rbKeyNum4 	86 	100 	75 	On numeric keypad or with NumLock
rbKeyNum5 	87 	101 	76 	On numeric keypad or with NumLock
rbKeyNum6 	88 	102 	77 	On numeric keypad or with NumLock
rbKeyNum7 	89 	103 	71 	On numeric keypad or with NumLock
rbKeyNum8 	91 	104 	72 	On numeric keypad or with NumLock
rbKeyNum9 	92 	105 	73 	On numeric keypad or with NumLock
rbKeyMultiply 	67 	106 	55 	On numeric keypad or with NumLock
rbKeyAdd 	69 	107 	78 	On numeric keypad or with NumLock
rbKeySubtract 	78 	109 	74 	On numeric keypad or with NumLock
rbKeyDivide 	75 	111 	98 	On numeric keypad or with NumLock
rbKeyDecimal 	65 	110 	83 	On numeric keypad or with NumLock
rbKeyNumEqual 	81 	* 	117 	On numeric keypad or with NumLock
#endif

#endif // NOUS_INPUT_VIRTUALKEYCODES_H
