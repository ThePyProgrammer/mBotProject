## Initiation

```cpp
// imports
#include <MeMCore.h>

// initiate
MeIR ir;

// in setup():
ir.begin();
```

## Operation

### Check if Data is Available

```cpp
ir.decode() // -> boolean value
```

### Get Value and Cases

```cpp
unsigned long value = ir.value;
unsigned long checkable_value = value >> 16 & 0xff;

switch(checkable_value) {
	case IR_BUTTON_LEFT: break;// do stuff if you need to move left
	case IR_BUTTON_RIGHT: break;// do stuff if you need to move right
	case IR_BUTTON_DOWN: break;// do stuff if you need to move backward
	case IR_BUTTON_UP: break;// do stuff if you need to move forward
	case IR_BUTTON_SETTING: break;// just stop yourself
	case IR_BUTTON_A: break;// ihni what this is supposed to do :skull:
}
```
