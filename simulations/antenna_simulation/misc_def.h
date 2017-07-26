#ifndef MISC_DEF_H_
#define MISC_DEF_H_

//Macros
#define BIT_TOGGLE(REG, BIT)    (REG ^= BIT)        /**< macro that toggles a bit in a register */
#define BIT_SET(REG, BIT)       (REG |= BIT)        /**< macro that set a bit in a register */
#define BIT_CLEAR(REG, BIT)     (REG &= ~BIT)       /**< macro that clear a bit in a register */
#define BIT_READ(REG, BIT)      ((REG & BIT) != 0)   /**< macro that read a bit in a register */

#endif
