/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   colors.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: umosse <umosse@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/09/14 14:19:41 by kmendes-          #+#    #+#             */
/*   Updated: 2024/08/23 15:06:48 by umosse           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef COLORS_H
# define COLORS_H

# define ERR "\033[0;31mError - \e[0m"
# define ERRM "\033[0;48;1mError ( MALLOC )\n\e[0m"

# define ERR0 "\033[38;5;121mError (0)- \e[0m"
# define ERR1 "\033[38;5;31mError (1)- \e[0m"
# define ERR2 "\033[38;5;45mError (2)- \e[0m"
# define ERR3 "\033[38;5;167mError (3)- \e[0m"
# define ERR4 "\033[38;5;176mError (4)- \e[0m"
# define ERR5 "\033[38;5;73mError (5)- \e[0m"
# define ERR6 "\033[38;5;100mError (6)- \e[0m"
# define ERR7 "\033[38;5;73mError (7)- \e[0m"
# define ERR8 "\033[38;5;92mError (8)- \e[0m"
# define ERR9 "\033[38;5;82mError (9)- \e[0m"

# define CLEAR "\e[2J\e[H"
# define CLS "\e[2J\e[H"
# define REVERSE "\e[7m"
# define BLINK "\e[5m"
# define R_BLINK "\e[25m"

# define RESET "\e[0m"
# define END "\e[0m\n"

# define BLACK "\033[0;30m"
# define RED "\033[0;31m"
# define GREEN "\033[0;32m"
# define YELLOW "\033[0;33m"
# define BLUE "\033[0;34m"
# define PURPLE "\033[0;35m"
# define CYAN "\033[0;36m"
# define WHITE "\033[0;37m"

# define C_000 "\033[38;5;16m"
# define C_001 "\033[38;5;17m"
# define C_002 "\033[38;5;18m"
# define C_003 "\033[38;5;19m"
# define C_004 "\033[38;5;20m"
# define C_005 "\033[38;5;21m"
# define C_010 "\033[38;5;22m"
# define C_011 "\033[38;5;23m"
# define C_012 "\033[38;5;24m"
# define C_013 "\033[38;5;25m"
# define C_014 "\033[38;5;26m"
# define C_015 "\033[38;5;27m"
# define C_020 "\033[38;5;28m"
# define C_021 "\033[38;5;29m"
# define C_022 "\033[38;5;30m"
# define C_023 "\033[38;5;31m"
# define C_024 "\033[38;5;32m"
# define C_025 "\033[38;5;33m"
# define C_030 "\033[38;5;34m"
# define C_031 "\033[38;5;35m"
# define C_032 "\033[38;5;36m"
# define C_033 "\033[38;5;37m"
# define C_034 "\033[38;5;38m"
# define C_035 "\033[38;5;39m"
# define C_040 "\033[38;5;40m"
# define C_041 "\033[38;5;41m"
# define C_042 "\033[38;5;42m"
# define C_043 "\033[38;5;43m"
# define C_044 "\033[38;5;44m"
# define C_045 "\033[38;5;45m"
# define C_050 "\033[38;5;46m"
# define C_051 "\033[38;5;47m"
# define C_052 "\033[38;5;48m"
# define C_053 "\033[38;5;49m"
# define C_054 "\033[38;5;50m"
# define C_055 "\033[38;5;51m"
# define C_100 "\033[38;5;52m"
# define C_101 "\033[38;5;53m"
# define C_102 "\033[38;5;54m"
# define C_103 "\033[38;5;55m"
# define C_104 "\033[38;5;56m"
# define C_105 "\033[38;5;57m"
# define C_110 "\033[38;5;58m"
# define C_111 "\033[38;5;59m"
# define C_112 "\033[38;5;60m"
# define C_113 "\033[38;5;61m"
# define C_114 "\033[38;5;62m"
# define C_115 "\033[38;5;63m"
# define C_120 "\033[38;5;64m"
# define C_121 "\033[38;5;65m"
# define C_122 "\033[38;5;66m"
# define C_123 "\033[38;5;67m"
# define C_124 "\033[38;5;68m"
# define C_125 "\033[38;5;69m"
# define C_130 "\033[38;5;70m"
# define C_131 "\033[38;5;71m"
# define C_132 "\033[38;5;72m"
# define C_133 "\033[38;5;73m"
# define C_134 "\033[38;5;74m"
# define C_135 "\033[38;5;75m"
# define C_140 "\033[38;5;76m"
# define C_141 "\033[38;5;77m"
# define C_142 "\033[38;5;78m"
# define C_143 "\033[38;5;79m"
# define C_144 "\033[38;5;80m"
# define C_145 "\033[38;5;81m"
# define C_150 "\033[38;5;82m"
# define C_151 "\033[38;5;83m"
# define C_152 "\033[38;5;84m"
# define C_153 "\033[38;5;85m"
# define C_154 "\033[38;5;86m"
# define C_155 "\033[38;5;87m"
# define C_200 "\033[38;5;88m"
# define C_201 "\033[38;5;89m"
# define C_202 "\033[38;5;90m"
# define C_203 "\033[38;5;91m"
# define C_204 "\033[38;5;92m"
# define C_205 "\033[38;5;93m"
# define C_210 "\033[38;5;94m"
# define C_211 "\033[38;5;95m"
# define C_212 "\033[38;5;96m"
# define C_213 "\033[38;5;97m"
# define C_214 "\033[38;5;98m"
# define C_215 "\033[38;5;99m"
# define C_220 "\033[38;5;100m"
# define C_221 "\033[38;5;101m"
# define C_222 "\033[38;5;102m"
# define C_223 "\033[38;5;103m"
# define C_224 "\033[38;5;104m"
# define C_225 "\033[38;5;105m"
# define C_230 "\033[38;5;106m"
# define C_231 "\033[38;5;107m"
# define C_232 "\033[38;5;108m"
# define C_233 "\033[38;5;109m"
# define C_234 "\033[38;5;110m"
# define C_235 "\033[38;5;111m"
# define C_240 "\033[38;5;112m"
# define C_241 "\033[38;5;113m"
# define C_242 "\033[38;5;114m"
# define C_243 "\033[38;5;115m"
# define C_244 "\033[38;5;116m"
# define C_245 "\033[38;5;117m"
# define C_250 "\033[38;5;118m"
# define C_251 "\033[38;5;119m"
# define C_252 "\033[38;5;120m"
# define C_253 "\033[38;5;121m"
# define C_254 "\033[38;5;122m"
# define C_255 "\033[38;5;123m"
# define C_300 "\033[38;5;124m"
# define C_301 "\033[38;5;125m"
# define C_302 "\033[38;5;126m"
# define C_303 "\033[38;5;127m"
# define C_304 "\033[38;5;128m"
# define C_305 "\033[38;5;129m"
# define C_310 "\033[38;5;130m"
# define C_311 "\033[38;5;131m"
# define C_312 "\033[38;5;132m"
# define C_313 "\033[38;5;133m"
# define C_314 "\033[38;5;134m"
# define C_315 "\033[38;5;135m"
# define C_320 "\033[38;5;136m"
# define C_321 "\033[38;5;137m"
# define C_322 "\033[38;5;138m"
# define C_323 "\033[38;5;139m"
# define C_324 "\033[38;5;140m"
# define C_325 "\033[38;5;141m"
# define C_330 "\033[38;5;142m"
# define C_331 "\033[38;5;143m"
# define C_332 "\033[38;5;144m"
# define C_333 "\033[38;5;145m"
# define C_334 "\033[38;5;146m"
# define C_335 "\033[38;5;147m"
# define C_340 "\033[38;5;148m"
# define C_341 "\033[38;5;149m"
# define C_342 "\033[38;5;150m"
# define C_343 "\033[38;5;151m"
# define C_344 "\033[38;5;152m"
# define C_345 "\033[38;5;153m"
# define C_350 "\033[38;5;154m"
# define C_351 "\033[38;5;155m"
# define C_352 "\033[38;5;156m"
# define C_353 "\033[38;5;157m"
# define C_354 "\033[38;5;158m"
# define C_355 "\033[38;5;159m"
# define C_400 "\033[38;5;160m"
# define C_401 "\033[38;5;161m"
# define C_402 "\033[38;5;162m"
# define C_403 "\033[38;5;163m"
# define C_404 "\033[38;5;164m"
# define C_405 "\033[38;5;165m"
# define C_410 "\033[38;5;166m"
# define C_411 "\033[38;5;167m"
# define C_412 "\033[38;5;168m"
# define C_413 "\033[38;5;169m"
# define C_414 "\033[38;5;170m"
# define C_415 "\033[38;5;171m"
# define C_420 "\033[38;5;172m"
# define C_421 "\033[38;5;173m"
# define C_422 "\033[38;5;174m"
# define C_423 "\033[38;5;175m"
# define C_424 "\033[38;5;176m"
# define C_425 "\033[38;5;177m"
# define C_430 "\033[38;5;178m"
# define C_431 "\033[38;5;179m"
# define C_432 "\033[38;5;180m"
# define C_433 "\033[38;5;181m"
# define C_434 "\033[38;5;182m"
# define C_435 "\033[38;5;183m"
# define C_440 "\033[38;5;184m"
# define C_441 "\033[38;5;185m"
# define C_442 "\033[38;5;186m"
# define C_443 "\033[38;5;187m"
# define C_444 "\033[38;5;188m"
# define C_445 "\033[38;5;189m"
# define C_450 "\033[38;5;190m"
# define C_451 "\033[38;5;191m"
# define C_452 "\033[38;5;192m"
# define C_453 "\033[38;5;193m"
# define C_454 "\033[38;5;194m"
# define C_455 "\033[38;5;195m"
# define C_500 "\033[38;5;196m"
# define C_501 "\033[38;5;197m"
# define C_502 "\033[38;5;198m"
# define C_503 "\033[38;5;199m"
# define C_504 "\033[38;5;200m"
# define C_505 "\033[38;5;201m"
# define C_510 "\033[38;5;202m"
# define C_511 "\033[38;5;203m"
# define C_512 "\033[38;5;204m"
# define C_513 "\033[38;5;205m"
# define C_514 "\033[38;5;206m"
# define C_515 "\033[38;5;207m"
# define C_520 "\033[38;5;208m"
# define C_521 "\033[38;5;209m"
# define C_522 "\033[38;5;210m"
# define C_523 "\033[38;5;211m"
# define C_524 "\033[38;5;212m"
# define C_525 "\033[38;5;213m"
# define C_530 "\033[38;5;214m"
# define C_531 "\033[38;5;215m"
# define C_532 "\033[38;5;216m"
# define C_533 "\033[38;5;217m"
# define C_534 "\033[38;5;218m"
# define C_535 "\033[38;5;219m"
# define C_540 "\033[38;5;220m"
# define C_541 "\033[38;5;221m"
# define C_542 "\033[38;5;222m"
# define C_543 "\033[38;5;223m"
# define C_544 "\033[38;5;224m"
# define C_545 "\033[38;5;225m"
# define C_550 "\033[38;5;226m"
# define C_551 "\033[38;5;227m"
# define C_552 "\033[38;5;228m"
# define C_553 "\033[38;5;229m"
# define C_554 "\033[38;5;230m"
# define C_555 "\033[38;5;231m"

/*
\e = \x1B = \033 = ^[[ = ascii 27

\x[00-ff] write caracter hexa
\[000-377] OCTAL

\e[arg1;arg2;arg3m
COMBINAISONS: BOLD_BLACK "\033[30;1m"

    [nA - Move cursor up n line.
    [nB - Move cursor down n line.
    [nC - Move cursor forward (right) n column.
    [nD - Move cursor backward (left) n column.

    [E - Move cursor to the beginning of the next line.
    [F - Move cursor to the beginning of the previous line.
    [nG - Move cursor to specified column in the current row.
    [n;mH - Move cursor to the position (row and column). [H moves to home
    [J - Clear part of the screen.
        [0J clears from the cursor position to the end of the screen.
        [1J clears from the beginning of the screen to the cursor position.
        [2J clears the entire screen.
    [K - Clear part of the current line.
        [0K clears from the cursor position to the end of the line.
        [1K clears from the beginning of the line to the cursor position.
        [2K clears the entire line.

    [0m: Reset all text formatting (color, style, etc.) to default.
        [1m: Bold or increased intensity.
        [2m: Faint or decreased intensity.
        [3m: Italicized text (not widely supported).
        [4m: Underlined text.
        [5m: Blinking text (not widely supported).
        [7m: Inverted colors (swap foreground and background).
            [8m: Hidden text (not displayed, but still occupies space).
            [9m: Crossed-out or strikethrough text.
                [21m: Reset double underline.
                [22m: Reset bold/bright and dim text to default intensity.
                [24m: Reset underlined text.
                [25m: Reset blinking text.

[0] black [1] red [2] green [3] yellow [4] blue [5] magenta [6] cyan
[7] white) [8] default

"\033[38;5;{foreground_color_code};48;5;{background_color_code}m
    [3(0-8)m: Foreground color [0;3(0-8)m (0; is  the  reset 0)
    [4(0-8)m: Background color [0;4(0-8)m

    [0;38;<n>m: ANSI color codes (0-7) foreground      = [0;38;0;<n>m
    [0;48;<n>m:         Background
    [38;2;<r>;<g>;<b>m: Truecolor foreground (RGB).
    [48;2;<r>;<g>;<b>m:
    [38;5;<n>m: 256-color foreground (0-255).
    [48;5;<n>m:

///////////////////////////////////////////////////////////////////////////////]
    [6n > \e[<row>;<column>R
        used to request the cursor position information. When the terminal
			receives this sequence, it typically responds with another
			sequence containing the current cursor position
    \e[s: Save current cursor position.
        \e[u: Restore cursor position saved with \e[s
    [?25l Hides the cursor.
        [?25h Shows the cursor again.
    [?1049h": Save cursor and screen state (alternative buffer).
        [?1049l": Restore cursor and screen state (main buffer).
    [?2004h": Enable bracketed paste mode (allows the terminal to distinguish
			pasted text from typed text).
        [?2004l": Disable bracketed paste mode.

*/

#endif //COLORS_H

///////////////////////////////////////////////////////////////////////////////]
/*
The color range of a 256 color terminal consists of 4 parts, often 5, in which
	 case you actually get 258 colors:
Color numbers 0 to 7 are the default terminal colors, the actual RGB value of
	which is not standardized and can often be configured.
Color numbers 8 to 15 are the "bright" colors. Most of the time these are a
	lighter shade of the color with index - 8. They are also not standardized
	and can often be configured. Depending on terminal and shell, they are
		often used instead of or in conjunction with bold font faces.
Color numbers 16 to 231 are RGB colors. These 216 colors are defined by 6
	values on each of the three RGB axes. That is, instead of values 0 -
	255, each color only ranges from 0 - 5.
The color number is then calculated like this:
number = 16 + 36 * r + 6 * g + b
with r, g and b in the range 0 - 5.
The color numbers 232 to 255 are grayscale with 24 shades of gray from
	dark to light.
The default colors for foreground and background. In many terminals they can
	be configured independently from the 256 indexed colors, giving an
	additional two configurable colors . You get them when not setting any
	other color or disabling other colors (i.e. print '\e[m').
*/
