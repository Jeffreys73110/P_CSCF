// AnsiColor.h

#ifndef	_ANSI_COLOR_H_
#define	_ANSI_COLOR_H_

// |------------+----------+---------+-------+------------------+------------------------------+--------------------------------------|
// | color-mode | octal    | hex     | bash  | description      | example (= in octal)         | NOTE                                 |
// |------------+----------+---------+-------+------------------+------------------------------+--------------------------------------|
// |          0 | \033[0m  | \x1b[0m | \e[0m | reset any affect | echo -e "\033[0m"            | 0m equals to m                       |
// |          1 | \033[1m  |         |       | light (= bright) | echo -e "\033[1m####\033[m"  | -                                    |
// |          2 | \033[2m  |         |       | dark (= fade)    | echo -e "\033[2m####\033[m"  | -                                    |
// |------------+----------+---------+-------+------------------+------------------------------+--------------------------------------|
// |  text-mode | ~        |         |       | ~                | ~                            | ~                                    |
// |------------+----------+---------+-------+------------------+------------------------------+--------------------------------------|
// |          3 | \033[3m  |         |       | italic           | echo -e "\033[3m####\033[m"  |                                      |
// |          4 | \033[4m  |         |       | underline        | echo -e "\033[4m####\033[m"  |                                      |
// |          5 | \033[5m  |         |       | blink (slow)     | echo -e "\033[3m####\033[m"  |                                      |
// |          6 | \033[6m  |         |       | blink (fast)     | ?                            | not wildly support                   |
// |          7 | \003[7m  |         |       | reverse          | echo -e "\033[7m####\033[m"  | it affects the background/foreground |
// |          8 | \033[8m  |         |       | hide             | echo -e "\033[8m####\033[m"  | it affects the background/foreground |
// |          9 | \033[9m  |         |       | cross            | echo -e "\033[9m####\033[m"  |                                      |
// |------------+----------+---------+-------+------------------+------------------------------+--------------------------------------|
// | foreground | ~        |         |       | ~                | ~                            | ~                                    |
// |------------+----------+---------+-------+------------------+------------------------------+--------------------------------------|
// |         30 | \033[30m |         |       | black            | echo -e "\033[30m####\033[m" |                                      |
// |         31 | \033[31m |         |       | red              | echo -e "\033[31m####\033[m" |                                      |
// |         32 | \033[32m |         |       | green            | echo -e "\033[32m####\033[m" |                                      |
// |         33 | \033[33m |         |       | yellow           | echo -e "\033[33m####\033[m" |                                      |
// |         34 | \033[34m |         |       | blue             | echo -e "\033[34m####\033[m" |                                      |
// |         35 | \033[35m |         |       | purple           | echo -e "\033[35m####\033[m" | real name: magenta = reddish-purple  |
// |         36 | \033[36m |         |       | cyan             | echo -e "\033[36m####\033[m" |                                      |
// |         37 | \033[37m |         |       | white            | echo -e "\033[37m####\033[m" |                                      |
// |------------+----------+---------+-------+------------------+------------------------------+--------------------------------------|
// |         38 | 8/24     |                    This is for special use of 8-bit or 24-bit                                            |
// |------------+----------+---------+-------+------------------+------------------------------+--------------------------------------|
// | background | ~        |         |       | ~                | ~                            | ~                                    |
// |------------+----------+---------+-------+------------------+------------------------------+--------------------------------------|
// |         40 | \033[40m |         |       | black            | echo -e "\033[40m####\033[m" |                                      |
// |         41 | \033[41m |         |       | red              | echo -e "\033[41m####\033[m" |                                      |
// |         42 | \033[42m |         |       | green            | echo -e "\033[42m####\033[m" |                                      |
// |         43 | \033[43m |         |       | yellow           | echo -e "\033[43m####\033[m" |                                      |
// |         44 | \033[44m |         |       | blue             | echo -e "\033[44m####\033[m" |                                      |
// |         45 | \033[45m |         |       | purple           | echo -e "\033[45m####\033[m" | real name: magenta = reddish-purple  |
// |         46 | \033[46m |         |       | cyan             | echo -e "\033[46m####\033[m" |                                      |
// |         47 | \033[47m |         |       | white            | echo -e "\033[47m####\033[m" |                                      |
// |------------+----------+---------+-------+------------------+------------------------------+--------------------------------------|

#include <cstdarg>

// Ansi Color Table
enum ACT_ColorMode { CM_Reset, CM_Light, CM_Dark, CM_Size };
const char szColorMode[CM_Size][2][16]={
	{"0", "reset"},		// reset any affect
	{"1", "light"},		// light (= bright)
	{"2", "dark"}		// dark (= fade)
};

enum ACT_TextMode { TM_Dft, TM_Italic, TM_Underline, TM_BlinkSlow, TM_BlinkFast, TM_Reverse, TM_Hide, TM_Cross, TM_Size };
const char szTextMode[TM_Size][2][16]={
	{"", ""},
	{"3", "italic"},		// italic
	{"4", "underline"},		// underline
	{"5", "blink-slow"},	// blink (slow)
	{"6", "blink-fast"},	// blink (fast)
	{"7", "reverse"},		// reverse
	{"8", "hide"},			// hide
	{"9", "cross"}			// cross
};

enum ACT_FgColor { FG_Dft, FG_Black, FG_Red, FG_Green, FG_Yellow, FG_Blue, FG_Purple, FG_Cyan, FG_White, FG_Size };
const char szForeground[FG_Size][2][16]={
	{"", ""},
	{"30", "black"},	// black
	{"31", "red"},		// red
	{"32", "green"},	// green
	{"33", "yellow"},	// yellow
	{"34", "blue"},		// blue
	{"35", "purple"},	// purple
	{"36", "cyan"},		// cyan
	{"37", "white"}		// white
};

enum ACT_BgColor { BG_Dft, BG_Black, BG_Red, BG_Green, BG_Yellow, BG_Blue, BG_Purple, BG_Cyan, BG_White, BG_Size };
const char szBackground[BG_Size][2][16]={
	{"", ""},
	{"40", "black"},	// black
	{"41", "red"},		// red
	{"42", "green"},	// green
	{"43", "yellow"},	// yellow
	{"44", "blue"},		// blue
	{"45", "purple"},	// purple
	{"46", "cyan"},		// cyan
	{"47", "white"}		// white
};

struct ColorParam_t
{
	ACT_ColorMode color_mode;
	ACT_TextMode text_mode;
	ACT_FgColor fg_color;
	ACT_BgColor bg_color;
};


#define COLOR_Header	"\033["
#define COLOR_Off		"\033[0m"	// reset
#define COLOR_STR(color_param, ...)	ColorString(color_param, __VA_ARGS__).c_str()

#define COLOR_FG(fg, ...)	COLOR_STR({CM_Light, TM_Dft, fg, BG_Dft}, __VA_ARGS__)	


static std::string ColorString(ColorParam_t color_param, const char *format, ...)
{
	va_list arglist;
    int		Len;
	char	buf[2048];
	std::string str;

	va_start(arglist, format);
	if (format!=NULL)
	{	// Combine String
		Len = vsnprintf(buf, sizeof(buf),format, arglist);
        buf[sizeof(buf)-1]=0;
		va_end(arglist);
		str = buf;
	}
	else	va_end(arglist);
	
	// set color
	str.append(COLOR_Off);
	str.insert(0, 1, 'm');
	if (color_param.bg_color!=BG_Dft)
	{
		str.insert(0, szBackground[color_param.bg_color][0]);
		str.insert(0, 1, ';');
	}
	if (color_param.fg_color!=FG_Dft)
	{
		str.insert(0, szForeground[color_param.fg_color][0]);
		str.insert(0, 1, ';');
	}
	if (color_param.text_mode!=TM_Dft)
	{
		str.insert(0, szTextMode[color_param.text_mode][0]);
		str.insert(0, 1, ';');
	}
	str.insert(0, szColorMode[color_param.color_mode][0]);
	// printf("[TestMsg] %s\n", str.c_str());

	str.insert(0, COLOR_Header);
	
	// printf("[TestMsg] %s\n", str.c_str());
	// printf("[TestMsg] color string end\n");

	return str;
}

static int AnsiColorTable()
{
	// printf("%s\n", ColorString({CM_Reset, TM_Dft, FG_Dft, BG_Dft}, "[Important] %s", "abc").c_str());
	printf("\n\nANSI Color Table:\n");

	for (int i=0; i<CM_Size; i++)
	{
		for (int j=1; j<TM_Size; j++)
		{
			for (int k=1; k<FG_Size; k++)
			{
				for (int l=1; l<BG_Size; l++)
				{
					printf("%s ===> \"\\033[%s;%s;%s;%sm%s %s %s %s\\033[0m\"\n", 
						COLOR_STR({(ACT_ColorMode)i, (ACT_TextMode)j, (ACT_FgColor)k, (ACT_BgColor)l}, 
							"%s %s %s %s",
							szColorMode[i][1], szTextMode[j][1], szForeground[k][1], szBackground[l][1]),
						szColorMode[i][0], szTextMode[j][0], szForeground[k][0], szBackground[l][0],
						szColorMode[i][1], szTextMode[j][1], szForeground[k][1], szBackground[l][1]
					);
				}
			}
		}
	}
	printf("\n\n");
	return 0;
}


#endif	// _ANSI_COLOR_H_