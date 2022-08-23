#ifndef __COLOR_H__
#define __COLOR_H__

/**
 * @file ansi-color.h
 * @author Mauro De Rosa (it.mauro.derosa@gmail.com)
 * @brief escape ansi codes
 * @version 1.0.0 alpha
 * @date 2022-08-23
 * 
 * @copyright Copyright (c) 2022
 * @license licensed under MIT license
 * 
 */

//Regular text
#define CR_BLK "\e[0;30m" ///< regular text color: black
#define CR_RED "\e[0;31m" ///< regular text color: red
#define CR_GRN "\e[0;32m" ///< regular text color: green
#define CR_YEL "\e[0;33m" ///< regular text color: yellow
#define CR_BLU "\e[0;34m" ///< regular text color: blue
#define CR_MAG "\e[0;35m" ///< regular text color: magenta
#define CR_CYN "\e[0;36m" ///< regular text color: cyan
#define CR_WHT "\e[0;37m" ///< regular text color: white

//High intensty text
#define CHR_RED "\e[0;91m" ///< high intensity text color: red
#define CHR_BLK "\e[0;90m" ///< high intensity text color: black
#define CHR_GRN "\e[0;92m" ///< high intensity text color: green
#define CHR_YEL "\e[0;93m" ///< high intensity text color: yellow
#define CHR_BLU "\e[0;94m" ///< high intensity text color: blue
#define CHR_MAG "\e[0;95m" ///< high intensity text color: magenta
#define CHR_CYN "\e[0;96m" ///< high intensity text color: cyan
#define CHR_WHT "\e[0;97m" ///< high intensity text color: white

//Regular bold text
#define CRB_BLK "\e[1;30m" ///< bold regular text color: black
#define CRB_RED "\e[1;31m" ///< bold regular text color: red
#define CRB_GRN "\e[1;32m" ///< bold regular text color: green
#define CRB_YEL "\e[1;33m" ///< bold regular text color: yellow
#define CRB_BLU "\e[1;34m" ///< bold regular text color: blue
#define CRB_MAG "\e[1;35m" ///< bold regular text color: magenta
#define CRB_CYN "\e[1;36m" ///< bold regular text color: cyan
#define CRB_WHT "\e[1;37m" ///< bold regular text color: white

//Bold high intensity text
#define CHB_BLK "\e[1;90m" ///< bold high intensity text color: black
#define CHB_RED "\e[1;91m" ///< bold high intensity text color: red
#define CHB_GRN "\e[1;92m" ///< bold high intensity text color: green
#define CHB_YEL "\e[1;93m" ///< bold high intensity text color: yellow
#define CHB_BLU "\e[1;94m" ///< bold high intensity text color: blue
#define CHB_MAG "\e[1;95m" ///< bold high intensity text color: magenta
#define CHB_CYN "\e[1;96m" ///< bold high intensity text color: cyan
#define CHB_WHT "\e[1;97m" ///< bold high intensity text color: white

//Regular underline text
#define CRU_BLK "\e[4;30m" ///< regular underlined text color: black
#define CRU_RED "\e[4;31m" ///< regular underlined text color: red
#define CRU_GRN "\e[4;32m" ///< regular underlined text color: green
#define CRU_YEL "\e[4;33m" ///< regular underlined text color: yellow
#define CRU_BLU "\e[4;34m" ///< regular underlined text color: blue
#define CRU_MAG "\e[4;35m" ///< regular underlined text color: magenta
#define CRU_CYN "\e[4;36m" ///< regular underlined text color: cyan
#define CRU_WHT "\e[4;37m" ///< regular underlined text color: white

//Regular background
#define BGR_BLK "\e[40m" ///< regular background color: black
#define BGR_RED "\e[41m" ///< regular background color: red
#define BGR_GRN "\e[42m" ///< regular background color: green
#define BGR_YEL "\e[43m" ///< regular background color: yellow
#define BGR_BLU "\e[44m" ///< regular background color: blue
#define BGR_MAG "\e[45m" ///< regular background color: magenta
#define BGR_CYN "\e[46m" ///< regular background color: cyan
#define BGR_WHT "\e[47m" ///< regular background color: white

//High intensty background 
#define BGH_BLK "\e[0;100m" ///< high intensity background color: black
#define BGH_RED "\e[0;101m" ///< high intensity background color: red
#define BGH_GRN "\e[0;102m" ///< high intensity background color: green
#define BGH_YEL "\e[0;103m" ///< high intensity background color: yellow
#define BGH_BLU "\e[0;104m" ///< high intensity background color: blue
#define BGH_MAG "\e[0;105m" ///< high intensity background color: magenta
#define BGH_CYN "\e[0;106m" ///< high intensity background color: cyan
#define BGH_WHT "\e[0;107m" ///< high intensity background color: white

//Reset
#define C_RESET "\e[0m" ///< reset color

#endif /* __COLOR_H__ */
