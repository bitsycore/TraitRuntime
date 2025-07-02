#ifndef FLAG_H
#define FLAG_H

#define FLAG_GET(flags_var, flag_mask) ((flags_var) & (flag_mask))
#define FLAG_SET(flags_var, flag_to_set) ((flags_var) |= (flag_to_set))
#define FLAG_CLEAR(flags_var, flag_to_clear) ((flags_var) &= ~(flag_to_clear))
#define FLAG_TOGGLE(flags_var, flag_to_toggle) ((flags_var) ^= (flag_to_toggle))

#endif //FLAG_H