FUNC(   SIN,    "sin",     1,  true,    false,    "")
FUNC(   COS,    "cos",     2,  true,    false,    "")
FUNC(   TAN,    "tan",     3,  true,    false,    "")
FUNC(   EXP,    "exp",     4,  true,    false,    "")
FUNC(   LN,     "ln",      5,  true,    false,    "")
FUNC(   POW,    "pow",     6,  true,    false,    "")

FUNC(   INC,    "inc",     7,  false,   false,    "push %cx\npush 1\nadd\npop %cx\n")
FUNC(   DEC,    "dec",     8,  false,   false,    "push %cx\npush 1\nsub\npop %cx\n")

FUNC(   PRINT,  "print",   9,  false,   true,     "out\n")
FUNC(   SCAN,   "scan",    10, false,   false,    "in\npop %cx\n")

FUNC(   A_DIFF, "Da",      11, false,   false,    "")
FUNC(   B_DIFF, "Db",      12, false,   false,    "")
FUNC(   C_DIFF, "Dc",      13, false,   false,    "")
FUNC(   D_DIFF, "Dd",      14, false,   false,    "")