# LL(1) AutomataScript grammar

## Variable declaration
NFAvar ::= lval `=` rhs
lval ::= id
rhs ::= exp `;`

## Printing and running
print ::= `PRINT` `:`  Id()* `;`
test   ::= `TEST` `:` lval `<<` `"` id* `"` `;`

## Define binary expression precedence from highest to lowest
   exp       ::= exp_p2 (binop exp_p2)*
exp_p2       ::= exp_p1 unop?
exp_p1       ::= `(` exp `)`
               | exp_ac
               | `(` exp_ac `-` exp_ac `)`
               | lval
exp_ac       ::= `"` id `"`

## Operands (highest to lowest precedence)
binop ::= `|` | `^` | 
unop ::- `*` | `+` | `?`
