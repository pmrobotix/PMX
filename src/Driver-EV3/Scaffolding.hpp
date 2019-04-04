/*
 * Scaffolding.hpp
 *
 *  Created on: 3 juin 2018
 *      Author: pmx
 */

#ifndef DRIVER_EV3_SCAFFOLDING_HPP_
#define DRIVER_EV3_SCAFFOLDING_HPP_


/*
     ____ __     ____   ___    ____ __         (((((()
    | |_  \ \  /   ) ) | |  ) | |_  \ \  /  \(@)- /
    |_|__  \_\/  __)_) |_|_/  |_|__  \_\/   /(@)- \
                                               ((())))
 *//**
 *  \file   scaffolding.h
 *  \brief  ARM-BBR scaffolding function prototypes
 *  \author  See AUTHORS for a full list of the developers
 *  \copyright  See the LICENSE file.
 */

/*
 *  \References
 *  https://github.com/ev3dev/ev3dev/issues/1103
 *  https://github.com/tcwan/ev3dev-arm-bbr/blob/master/common/include/scaffolding.h
 *  list of fonts
 *  https://ofalcao.pt/blog/2014/ev3dev-usando-o-lcd-do-ev3
 */


//#include "ev3dev-arm-ctypes.h"
#include "ev3dev.h"

/** @addtogroup common */
/*@{*/

/** @defgroup scaffolding Program Scaffolding Routines
 *
 * The Scaffolding library perform miscellaneous startup and shutdown tasks,
 * as well as send display to the console with basic terminal handling.
 */
/*@{*/

/**
 * Text Terminal Screen Dimensions
 *
 * Assumes use of Lat15-Terminus12x6.psf.gz font
 * for the EV3 178x128 Monochrome LCD Display
 *
 */
#define __FBCON_EV3__

#ifdef  __FBCON_EV3__
#define TERM_COL_MIN  1                           ///< Minimum Console Column value
#define TERM_COL_MAX 29                           ///< Maximum Console Column value
#define TERM_ROW_MIN  1                           ///< Minimum Console Row value
#define TERM_ROW_MAX 10                           ///< Maximum Console Row value
#endif

/**
 * Common Startup/Shutdown Routines for ARM-BBR programs
 */

/**
 * Initialize ARM-BBR subsystems
 * Clears LCD screen and generate startup tone
 *    @param None
 *    @return None
 */
void prog_init(void);                /* Program Initialization Routine */

/**
 * Exit ARM-BBR subsystems
 * Generate exit tone
 *    @param None
 *    @return None
 */
void prog_exit(void);                   /* Program Exit Routine */

/**
 * Display Title string on LCD (Row 1)
 *    @param string: Null-terminated string
 *    @return None
 */
void prog_title(char *string);       /* Display Title string on LCD Screen */

/**
 * Display Content string on LCD (Row 3)
 *    @param string: Null-terminated string
 *    @return None
 */
void prog_content1(char *string);     /* Display Content string on LCD Screen */

/**
 * Display Content string on LCD (Row 5)
 *    @param string: Null-terminated string
 *    @return None
 */
void prog_content2(char *string);     /* Display Content string on LCD Screen */

/**
 * Display Content string on LCD (Row row)
 *    @param string: Null-terminated string
 *    @param row int: Row Index [1..MAX_ROW]
 *    @return None
 */
void prog_contentX(char *string, int row);     /* Display Content string on LCD Screen */

//
// Generic Output routines
//
/**
 * Clear the LCD Text Display. Cursor will be placed at (1,1) (Top Left)
 *    @param None
 *    @return None
 */
void prog_clearscreen(void);                /* LCD Text Screen Init Routine */


/**
 * Set the cursor on the LCD to new cursor position
 * Note: This only works on the LCD screen.
 * It does not work in the Debugger output window.
 *    @param col: integer value
 *    @param row: integer value
 *    @return success: bool value
 */
bool prog_set_cursorpos(int col, int row);

/**
 * Display string on LCD at current cursor position
 *    @param string: Null-terminated string
 *    @return None
 */
void prog_display_string(char *string);

/**
 * Display signed integer on LCD at current cursor position
 * The negative sign is displayed if it is negative,
 * otherwise no signed is displayed.
 *    @param value: signed long value
 *    @return None
 */
void prog_display_integer(int value);

/**
 * Display signed integer on LCD at current cursor position
 * The sign is always displayed, regardless of the value
 * being positive or negative.
 *    @param value: signed long value
 *    @return None
 */
void prog_display_signed_int(int value);

/**
 * Display unsigned integer on LCD at current cursor position
 *    @param value: unsigned long value
 *    @return None
 */
void prog_display_unsigned_int(uint value);

/**
 * Display 8-bit binary on LCD at current cursor position
 *    @param value: 8-bit binary value
 *    @return None
 */
void prog_display_bin8(unsigned char binvalue);

/**
 * Display 32-bit hexadecimal on LCD at current cursor position
 *    @param value: 32-bit hexadecimal value
 *    @return None
 */
void prog_display_hex32(uint hexvalue);

/*@}*/
/*@}*/


#endif /* DRIVER_EV3_SCAFFOLDING_HPP_ */
