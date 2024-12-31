# LL(1) AutomataScript grammar

# Variable declaration
NFAvar ::= lval `=` rhs
lval ::= id
rhs ::= exp `;`

# Printing and running
print ::= `PRINT` `:`  Id()* `;`
test   ::= `TEST` `:` lval `<<` Id()* `;`

# Define binary expression precedence from highest to lowest
   exp       ::= exp_p2 (binop_p2 exp_p2)*
exp_p2       ::= exp_p1 binop_p1?
exp_p1       ::= `(` exp `)`
               | exp_ac
               | `(` id[0] `-` id[0] `)`
exp_ac       ::= `"` id `"`

# Operands (highest to lowest precedence)
binop_p2 ::= `|` | `^` | 
binop_p1 ::- `*` | `+` | `?`
-- ---

# TODO: Add checks to make sure a-z is in range, etc
